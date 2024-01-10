
#pragma warning(push, 3)
#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include <emmintrin.h>
#include <xaudio2.h>
#include <stdint.h>
#include "stb_vorbis.h"
#pragma warning(pop)


#include "Main.h"
#include "CharacterName.h"
#include "ExitYesNoScreen.h"
#include "OpeningSplashScreen.h"
#include "OptionsScreen.h"
#include "TitleScreen.h"
#include "OverWorld.h"
#include "miniz.h"


#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "XAudio2.lib")


CRITICAL_SECTION gLogCritSec;                 ////used to sync logging to logfile between muli-threads

// post-build commandline was $(SolutionDir)CopyAssets.bat

// Map any char value to an offset dictated by the g6x7Font ordering.
int gFontCharacterPixelOffset[] = {
    //  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
        93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,
    //     !  "  #  $  %  &  '  (  )  *  +  ,  -  .  /  0  1  2  3  4  5  6  7  8  9  :  ;  <  =  >  ?
        94,64,87,66,67,68,70,85,72,73,71,77,88,74,91,92,52,53,54,55,56,57,58,59,60,61,86,84,89,75,90,93,
    //  @  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z  [  \  ]  ^  _
        65,0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,80,78,81,69,76,
    //  `  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z  {  |  }  ~  ..
        62,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,82,79,83,63,93,
    //  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
        93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,
    //  .. .. .. .. .. .. .. .. .. .. .. «  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. »  .. .. .. ..
        93,93,93,93,93,93,93,93,93,93,93,96,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,95,93,93,93,93,
    //  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..
        93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,
    //  .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. F2 .. .. .. .. .. .. .. .. .. .. .. .. ..
        93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,97,93,93,93,93,93,93,93,93,93,93,93,93,93
};

BOOL gWindowHasFocus;

IXAudio2* gXAudio;

IXAudio2MasteringVoice* gXAudioMasteringVoice;

uint8_t gSFXSourceVoiceSelector;


int WinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PreviousInstance, _In_ PSTR CommandLine, _In_ int CmdShow)
{   
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(PreviousInstance);
    UNREFERENCED_PARAMETER(CommandLine);
    UNREFERENCED_PARAMETER(CmdShow);



    MSG Message = { 0 };

    int64_t FrameStart = 0;
    int64_t FrameEnd = 0;
    int64_t ElapsedMicroseconds = 0;
    int64_t ElapsedMicrosecondsPerFrameAccumulatorRaw = 0;
    int64_t ElapsedMicrosecondsPerFrameAccumulatorCooked = 0;

    FILETIME ProcessCreationTime = { 0 };
    FILETIME ProcessExitTime = { 0 };
    int64_t CurrentUserCPUTime = 0;
    int64_t CurrentKernelCPUTime = 0;
    int64_t PreviousUserCPUTime = 0;
    int64_t PreviousKernelCPUTime = 0;

    HANDLE ProcessHandle = GetCurrentProcess();
    HMODULE NtDllModuleHandle = NULL;

    gPassableTiles[0] = TILE_GRASS_01;

    //this crit section is used to sync access to log file with LogMessageA when used by multiple threads
#pragma warning(suppress: 6031)
    InitializeCriticalSectionAndSpinCount(&gLogCritSec, 0x400);

    gEssentialAssetsLoadedEvent = CreateEventA(NULL, TRUE, FALSE, "gEssentialAssetsLoadedEvent");

    if (LoadRegistryParameters() != ERROR_SUCCESS)
    {
        goto Exit;
    }

    LogMessageA(LL_INFO, "[%s] Starting %s version %s", __FUNCTION__, GAME_NAME, GAME_VERSION);


    if (GameIsAlreadyRunning() == TRUE)
    {
        LogMessageA(LL_WARNING, "[%s] Another instance is already running!", __FUNCTION__);
        MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if ((NtDllModuleHandle = GetModuleHandleA("ntdll.dll")) == NULL)
    {
        LogMessageA(LL_ERROR, "[%s] Couldn't load ntdll.dll! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Couldn't load ntdll.dll!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if ((NtQueryTimerResolution = (_NtQueryTimerResolution)GetProcAddress(NtDllModuleHandle, "NtQueryTimerResolution")) == NULL)
    {
        LogMessageA(LL_ERROR, "[%s] Couldn't find function NtQueryTimerResolution in ntdll.dll! GetProcAddress Failed! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Couldn't find function NtQueryTimerResolution in ntdll.dll!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    NtQueryTimerResolution(&gGamePerformanceData.MinimumTimerResolution, &gGamePerformanceData.MaximumTimerResolution, &gGamePerformanceData.CurrentTimerResolution);

    GetSystemInfo(&gGamePerformanceData.SystemInfo);

    LogMessageA(LL_INFO, "[%s] Number of CPUs (Logical Processors): %d", __FUNCTION__, gGamePerformanceData.SystemInfo.dwNumberOfProcessors);

    switch (gGamePerformanceData.SystemInfo.wProcessorArchitecture)
    {
        case PROCESSOR_ARCHITECTURE_INTEL:
        {
            LogMessageA(LL_INFO, "[%s] CPU Architecture: x86", __FUNCTION__);
            break;
        }
        case PROCESSOR_ARCHITECTURE_IA64:
        {
            LogMessageA(LL_INFO, "[%s] CPU Architecture: Itanium", __FUNCTION__);
            break;
        }
        case PROCESSOR_ARCHITECTURE_ARM64:
        {
            LogMessageA(LL_INFO, "[%s] CPU Architecture: ARM64", __FUNCTION__);
            break;
        }
        case PROCESSOR_ARCHITECTURE_ARM:
        {
            LogMessageA(LL_INFO, "[%s] CPU Architecture: ARM", __FUNCTION__);
            break;
        }
        case PROCESSOR_ARCHITECTURE_AMD64:
        {
            LogMessageA(LL_INFO, "[%s] CPU Architecture: x64", __FUNCTION__);
            break;
        }
        default:
        {
            LogMessageA(LL_INFO, "[%s] CPU Architecture: Unknown", __FUNCTION__);
        }
    }

    GetSystemTimeAsFileTime((FILETIME*)&gGamePerformanceData.PreviousSystemTime);


    if (timeBeginPeriod(1) == TIMERR_NOCANDO)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to set global timer resolution!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to set timer resolution!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (SetPriorityClass(ProcessHandle, HIGH_PRIORITY_CLASS) == 0)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to set process priority! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed to set process priority!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST) == 0)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to set thread priority! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed to set thread priority!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (CreateMainGameWindow() != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] CreateMainGameWindow failed!", __FUNCTION__);
        goto Exit;
    }

    ////////////////////////////// create new thread for loading assets

    if ((gAssetLoadingThreadHandle = CreateThread(NULL, 0, AssetLoadingThreadProc, NULL, 0, NULL)) == NULL)
    {
        MessageBoxA(NULL, "CreateThread failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (InitializeSoundEngine() != S_OK)
    {
        MessageBoxA(NULL, "InitializeSoundEngine failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    QueryPerformanceFrequency((LARGE_INTEGER*)&gGamePerformanceData.PerfFrequency);

    gBackBuffer.BitmapInfo.bmiHeader.biSize = sizeof(gBackBuffer.BitmapInfo.bmiHeader);
    gBackBuffer.BitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;
    gBackBuffer.BitmapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;
    gBackBuffer.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;
    gBackBuffer.BitmapInfo.bmiHeader.biCompression = BI_RGB;
    gBackBuffer.BitmapInfo.bmiHeader.biPlanes = 1;
    if ((gBackBuffer.Memory = VirtualAlloc(NULL, GAME_DRAWING_AREA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to allocate memory for backbuffer! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed to allocate memory for drawing surface!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    if (InitializePlayer() != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to initialize player sprite!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to Initialize Player Sprite!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    gGameIsRunning = TRUE;

    while (gGameIsRunning == TRUE) //basic message loop
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&FrameStart);

        while (PeekMessageA(&Message, gGameWindow, 0, 0, PM_REMOVE))
        {
            DispatchMessageA(&Message);
        }
        ProcessPlayerInput();
        RenderFrameGraphics();

        QueryPerformanceCounter((LARGE_INTEGER*)&FrameEnd);
            ElapsedMicroseconds = FrameEnd - FrameStart;  //calc elapsed microseconds during frame draw and player input
            ElapsedMicroseconds *= 1000000;
            ElapsedMicroseconds /= gGamePerformanceData.PerfFrequency;
            gGamePerformanceData.TotalFramesRendered++;               //increment count every frame
            ElapsedMicrosecondsPerFrameAccumulatorRaw += ElapsedMicroseconds;

            while (ElapsedMicroseconds < GOAL_MICROSECONDS_PER_FRAME)      //loop when overshooting framerate
            {
                ElapsedMicroseconds = FrameEnd - FrameStart;  //recalculate
                ElapsedMicroseconds *= 1000000;
                ElapsedMicroseconds /= gGamePerformanceData.PerfFrequency;
                QueryPerformanceCounter((LARGE_INTEGER*)&FrameEnd);

                if (ElapsedMicroseconds < (GOAL_MICROSECONDS_PER_FRAME * 0.75f ))
                {
                    Sleep(1);       //can be between 1ms and one system tick (~15.625ms)
                }
            }
            ElapsedMicrosecondsPerFrameAccumulatorCooked += ElapsedMicroseconds;

        if ((gGamePerformanceData.TotalFramesRendered % CALCULATE_AVG_FPS_EVERY_X_FRAMES) == 0)
        {
            GetSystemTimeAsFileTime((FILETIME*) &gGamePerformanceData.CurrentSystemTime);
            GetProcessTimes(ProcessHandle, &ProcessCreationTime, &ProcessExitTime, (FILETIME*)&CurrentKernelCPUTime, (FILETIME*)&CurrentUserCPUTime);

            gGamePerformanceData.CPUPercent = (double)(CurrentKernelCPUTime - PreviousKernelCPUTime) + (CurrentUserCPUTime - PreviousUserCPUTime);
            gGamePerformanceData.CPUPercent /= (gGamePerformanceData.CurrentSystemTime - gGamePerformanceData.PreviousSystemTime);
            gGamePerformanceData.CPUPercent /= gGamePerformanceData.SystemInfo.dwNumberOfProcessors; //kept returning 0 processors and then dividing by 0
            gGamePerformanceData.CPUPercent *= 100;

            GetProcessHandleCount(ProcessHandle, &gGamePerformanceData.HandleCount);
            K32GetProcessMemoryInfo(ProcessHandle, (PROCESS_MEMORY_COUNTERS*)&gGamePerformanceData.MemInfo, sizeof(gGamePerformanceData.MemInfo));

            gGamePerformanceData.RawFPSAverage = 1.0f / ((ElapsedMicrosecondsPerFrameAccumulatorRaw / CALCULATE_AVG_FPS_EVERY_X_FRAMES) * 0.000001f);
            gGamePerformanceData.CookedFPSAverage = 1.0f / ((ElapsedMicrosecondsPerFrameAccumulatorCooked / CALCULATE_AVG_FPS_EVERY_X_FRAMES) * 0.000001f);
            ElapsedMicrosecondsPerFrameAccumulatorRaw = 0; 
            ElapsedMicrosecondsPerFrameAccumulatorCooked = 0;

            PreviousKernelCPUTime = CurrentKernelCPUTime;
            PreviousUserCPUTime = CurrentUserCPUTime;
            gGamePerformanceData.PreviousSystemTime = gGamePerformanceData.CurrentSystemTime;
        }
    }

Exit:
    return (0);
}


LRESULT CALLBACK MainWindowProc(
    _In_ HWND WindowHandle,        // handle to window
    _In_ UINT Message,        // message identifier
    _In_ WPARAM WParam,    // first message parameter
    _In_ LPARAM LParam)    // second message parameter
{
    LRESULT Result = 0; 

    switch (Message)
    {
        case WM_CLOSE:      //close game stops EXE
        {   gGameIsRunning = FALSE;
            PostQuitMessage(0);
            break;
        }
        case WM_ACTIVATE:
        {
            if (WParam == 0)
            {
                gWindowHasFocus = FALSE;        //lost window focus (tabbed out)
            }
            else
            {
                //ShowCursor(FALSE);          //remove mouse cursor when mouse is over game surface
                gWindowHasFocus = TRUE;         //gained focus (tabbed in)
                break;
            }
        }
        default:
        {   Result = DefWindowProcA(WindowHandle, Message, WParam, LParam);
        }
    }
    return (Result);
}

DWORD CreateMainGameWindow(void)
{
    DWORD Result = ERROR_SUCCESS;

    WNDCLASSEXA WindowClass = { 0 };

    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.style = CS_VREDRAW || CS_HREDRAW;
    WindowClass.lpfnWndProc = MainWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = GetModuleHandleA(NULL);
    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
#ifdef _DEBUG
    WindowClass.hbrBackground = CreateSolidBrush(RGB(255, 0, 255));
#else
    WindowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
#endif
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = GAME_NAME "_WINDOWCLASS";

    if (RegisterClassExA(&WindowClass) == 0)
    {
        Result = GetLastError();
        LogMessageA(LL_ERROR, "[%s] Window Registration Failed! RegisterClassExa Failed! Error 0x%08lx!", __FUNCTION__, Result);
            MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            goto Exit;
    }

    gGameWindow = CreateWindowExA(0, WindowClass.lpszClassName, "Window Title Name", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, GetModuleHandleA(NULL), NULL);

    if (gGameWindow == NULL)
    {
        Result = GetLastError();
        LogMessageA(LL_ERROR, "[%s] CreateWindowExA failed! Error 0x%08lx!", __FUNCTION__, Result);
            MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            goto Exit;
    }

    gGamePerformanceData.MonitorInfo.cbSize = sizeof(MONITORINFO);

    if (GetMonitorInfoA(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gGamePerformanceData.MonitorInfo) == 0)
    {
        Result = ERROR_MONITOR_NO_DESCRIPTOR;       // GetMonitorInfoA cannot Result = GetLastError
        LogMessageA(LL_ERROR, "[%s] GetMonitorInfoA(MonitorFromWindow()) failed! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    for (uint8_t Counter = 1; Counter < 12; Counter++)
    {
        if (GAME_RES_WIDTH * Counter > (gGamePerformanceData.MonitorInfo.rcMonitor.right - gGamePerformanceData.MonitorInfo.rcMonitor.left) ||
            GAME_RES_HEIGHT * Counter > (gGamePerformanceData.MonitorInfo.rcMonitor.bottom - gGamePerformanceData.MonitorInfo.rcMonitor.top))
        {
            gGamePerformanceData.MaxScaleFactor = Counter - 1;
            break;
        }
    }

    if (gRegistryParams.ScaleFactor == 0)
    {
        gGamePerformanceData.CurrentScaleFactor = gGamePerformanceData.MaxScaleFactor;
    }
    else
    {
        gGamePerformanceData.CurrentScaleFactor = (uint8_t)gRegistryParams.ScaleFactor;
    }


    LogMessageA(LL_INFO, "[%s] Current screen scale factor is %d. Max scale factor is %d.", __FUNCTION__, gGamePerformanceData.CurrentScaleFactor, gGamePerformanceData.MaxScaleFactor);
    LogMessageA(LL_INFO, "[%s] Will draw at %dx%d", __FUNCTION__, gGamePerformanceData.CurrentScaleFactor * GAME_RES_WIDTH, gGamePerformanceData.CurrentScaleFactor * GAME_RES_HEIGHT);

    if (SetWindowLongPtrA(gGameWindow, GWL_STYLE, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_OVERLAPPEDWINDOW) == 0)
    {
        Result = GetLastError();
        LogMessageA(LL_ERROR, "[%s] SetWindowLongPtrA failed! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    if (gRegistryParams.FullScreen == TRUE)     //fullscreen
    {
        if (SetWindowPos(gGameWindow,
            HWND_TOP,
            gGamePerformanceData.MonitorInfo.rcMonitor.left,
            gGamePerformanceData.MonitorInfo.rcMonitor.top,
            gGamePerformanceData.MonitorInfo.rcMonitor.right - gGamePerformanceData.MonitorInfo.rcMonitor.left,
            gGamePerformanceData.MonitorInfo.rcMonitor.bottom - gGamePerformanceData.MonitorInfo.rcMonitor.top,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
        {
            Result = GetLastError();
            LogMessageA(LL_ERROR, "[%s] SetWindowPos failed! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }
    else                    //Windowed
    {
        if (SetWindowPos(gGameWindow,
            HWND_TOP,
            ((gGamePerformanceData.MonitorInfo.rcMonitor.right - gGamePerformanceData.MonitorInfo.rcMonitor.left) / 2) - (GAME_RES_WIDTH * gGamePerformanceData.CurrentScaleFactor / 2),
            ((gGamePerformanceData.MonitorInfo.rcMonitor.bottom - gGamePerformanceData.MonitorInfo.rcMonitor.top) / 2) - (GAME_RES_HEIGHT * gGamePerformanceData.CurrentScaleFactor / 2),
            //gGamePerformanceData.MonitorInfo.rcMonitor.left,
            //gGamePerformanceData.MonitorInfo.rcMonitor.top,
            GAME_RES_WIDTH * gGamePerformanceData.CurrentScaleFactor,
            GAME_RES_HEIGHT * gGamePerformanceData.CurrentScaleFactor,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
        {
            Result = GetLastError();
            LogMessageA(LL_ERROR, "[%s] SetWindowPos failed! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }

Exit:
    return(Result);
}

BOOL GameIsAlreadyRunning(void)
{ 
    HANDLE Mutex = NULL;

    Mutex = CreateMutexA(NULL, FALSE, GAME_NAME "_GameMutex");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

void ProcessPlayerInput(void)
{
    if (gWindowHasFocus == FALSE)
    {
        return;
    }

    gGameInput.EscapeKeyPressed = GetAsyncKeyState(VK_ESCAPE);
    gGameInput.DebugKeyPressed = GetAsyncKeyState(VK_F1);                                   //F1 default debug key
    gGameInput.TabKeyPressed = GetAsyncKeyState(VK_TAB);
    gGameInput.SDownKeyPressed = GetAsyncKeyState('S') | GetAsyncKeyState(VK_DOWN);
    gGameInput.ALeftKeyPressed = GetAsyncKeyState('A') | GetAsyncKeyState(VK_LEFT);       // WASD and ArrowKey movement
    gGameInput.DRightKeyPressed = GetAsyncKeyState('D') | GetAsyncKeyState(VK_RIGHT);
    gGameInput.WUpKeyPressed = GetAsyncKeyState('W') | GetAsyncKeyState(VK_UP);
    gGameInput.ChooseKeyPressed = GetAsyncKeyState('E') | GetAsyncKeyState(VK_RETURN);

    if (gGameInput.DebugKeyPressed && !gGameInput.DebugKeyAlreadyPressed)
    {
        gGamePerformanceData.DisplayDebugInfo = !gGamePerformanceData.DisplayDebugInfo;
    }
    
    switch (gCurrentGameState)
    {
        case GAMESTATE_OPENINGSPLASH:
        {
            PPI_OpeningSplashScreen();
            break;
        }
        case GAMESTATE_TITLESCREEN:
        {
            PPI_TitleScreen();
            break;
        }
        case GAMESTATE_OPTIONS:
        {
            PPI_OptionsScreen();
            break;
        }
        case GAMESTATE_EXITYESNO:
        {
            PPI_ExitYesNoScreen();
            break;
        }
        case GAMESTATE_CHARACTERNAME:
        {
            PPI_CharacterName();
            break;
        }
        case GAMESTATE_OVERWORLD:
        {
            PPI_Overworld();
            break;
        }
        default:
        {
            ASSERT(FALSE, "Unknown GameState for player input!")
            break;
        }
    }

    gGameInput.EscapeKeyAlreadyPressed = gGameInput.EscapeKeyPressed;
    gGameInput.DebugKeyAlreadyPressed = gGameInput.DebugKeyPressed;
    gGameInput.ALeftKeyAlreadyPressed = gGameInput.ALeftKeyPressed;
    gGameInput.DRightKeyAlreadyPressed = gGameInput.DRightKeyPressed;
    gGameInput.WUpKeyAlreadyPressed = gGameInput.WUpKeyPressed;
    gGameInput.SDownKeyAlreadyPressed = gGameInput.SDownKeyPressed;
    gGameInput.ChooseKeyAlreadyPressed = gGameInput.ChooseKeyPressed;
    gGameInput.TabKeyAlreadyPressed = gGameInput.TabKeyPressed;

}

DWORD Load32BppBitmapFromFile(_In_ char* FileName, _Inout_ GAMEBITMAP* GameBitmap)
{
    DWORD Error = ERROR_SUCCESS;

    HANDLE FileHandle = INVALID_HANDLE_VALUE;

    WORD BitmapHeader = 0;

    DWORD PixelDataOffset = 0;

    DWORD NumberOfBytesRead = 2;

    if ((FileHandle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] CreateFileA failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, &BitmapHeader, 2, &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (BitmapHeader != 0x4d42)     //0x4d42 is "BM" backwards
    {
        Error = ERROR_FILE_INVALID;
        LogMessageA(LL_ERROR, "[%s] First two bytes are not 'BM'! Error 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (SetFilePointer(FileHandle, 0xA, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] SetFilePointer failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, &PixelDataOffset, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (SetFilePointer(FileHandle, 0xE, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] SetFilePointer failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, &GameBitmap->BitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if ((GameBitmap->Memory = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, GameBitmap->BitmapInfo.bmiHeader.biSizeImage)) == NULL)
    {
        Error = ERROR_NOT_ENOUGH_MEMORY;
        LogMessageA(LL_ERROR, "[%s] HeapAlloc couldn't allocate memory! Error 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (SetFilePointer(FileHandle, PixelDataOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] SetFilePointer failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, GameBitmap->Memory, GameBitmap->BitmapInfo.bmiHeader.biSizeImage, &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

Exit:
    if (FileHandle && (FileHandle != INVALID_HANDLE_VALUE))
    {
        CloseHandle(FileHandle);
    }

    if (Error == ERROR_SUCCESS)
    {
        LogMessageA(LL_INFO, "[%s] Loading Successful: %s", __FUNCTION__, FileName);
    }
    else
    {
        LogMessageA(LL_ERROR, "[%s] Loading failed: %s! Error 0x%08lx!", __FUNCTION__, FileName, Error);
    }

    return(Error);
}

DWORD InitializePlayer(void)
{
    DWORD Error = ERROR_SUCCESS;

    gPlayer.ScreenPos.x = 192;
    gPlayer.ScreenPos.y = 32;
    gPlayer.WorldPos.x = 192;
    gPlayer.WorldPos.y = 32;
    gPlayer.CurrentSuit = SUIT_0;
    gPlayer.Direction = DOWN;



    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingDown0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingDown0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingDown0.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_DOWN_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingDown0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingDown0!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingDown1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingDown1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingDown1.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_DOWN_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingDown1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingDown1!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingDown2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingDown2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingDown2.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_DOWN_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingDown2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingDown2!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingLeft0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingLeft0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingLeft0.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_LEFT_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingLeft0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingLeft0!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingLeft1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingLeft1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingLeft1.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_LEFT_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingLeft1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingLeft1!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingLeft2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingLeft2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingLeft2.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_LEFT_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingLeft2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingLeft2!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingRight0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingRight0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingRight0.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_RIGHT_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingRight0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingRight0!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingRight1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingRight1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingRight1.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_RIGHT_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingRight1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingRight1!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingRight2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingRight2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingRight2.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_RIGHT_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingRight2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingRight2!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingUp0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingUp0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingUp0.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_UP_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingUp0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingUp0!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingUp1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingUp1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingUp1.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_UP_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingUp1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingUp1!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingUp2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingUp2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/

    /*if ((Error = Load32BppBitmapFromMem(ASSET_FILE, "Suit0FacingUp2.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_UP_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingUp2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromMem failed on Suit0FacingUp2!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }*/


Exit:
    return(Error);
}

void BlitStringToBuffer(_In_ char* String, _In_ GAMEBITMAP* FontSheet, _In_ PIXEL32* Color, _In_ uint16_t x, _In_ uint16_t y)
{
    uint16_t CharWidth = (uint16_t)FontSheet->BitmapInfo.bmiHeader.biWidth / FONT_SHEET_CHARACTERS_PER_ROW;
    uint16_t CharHeight = (uint16_t)FontSheet->BitmapInfo.bmiHeader.biHeight;      // only one row
    uint16_t BytesPerCharacter = (CharWidth * CharHeight * (FontSheet->BitmapInfo.bmiHeader.biBitCount / 8));
    uint16_t StringLength = (uint16_t)strlen(String);

    GAMEBITMAP StringBitmap = { 0 };

    StringBitmap.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;
    StringBitmap.BitmapInfo.bmiHeader.biHeight = CharHeight;
    StringBitmap.BitmapInfo.bmiHeader.biWidth = CharWidth * StringLength;
    StringBitmap.BitmapInfo.bmiHeader.biPlanes = 1;
    StringBitmap.BitmapInfo.bmiHeader.biCompression = BI_RGB;
    StringBitmap.Memory = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ((size_t)BytesPerCharacter * (size_t)StringLength));

    for (int Character = 0; Character < StringLength; Character++)
    {
        int StartingFontSheetPixel = 0;
        int FontSheetOffset = 0;
        int StringBitmapOffset = 0;

        PIXEL32 FontSheetPixel = { 0 };

        StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * gFontCharacterPixelOffset[(uint8_t)String[Character]]);

        for (int YPixel = 0; YPixel < CharHeight; YPixel++)
        {
            for (int XPixel = 0; XPixel < CharWidth; XPixel++)
            {
                FontSheetOffset = StartingFontSheetPixel + XPixel - (FontSheet->BitmapInfo.bmiHeader.biWidth * YPixel);

                StringBitmapOffset = (Character * CharWidth) + ((StringBitmap.BitmapInfo.bmiHeader.biWidth * StringBitmap.BitmapInfo.bmiHeader.biHeight) - StringBitmap.BitmapInfo.bmiHeader.biWidth) + XPixel - (StringBitmap.BitmapInfo.bmiHeader.biWidth * YPixel);

                memcpy_s(&FontSheetPixel, sizeof(PIXEL32), (PIXEL32*)FontSheet->Memory + FontSheetOffset, sizeof(PIXEL32));

                FontSheetPixel.Red = Color->Red;
                FontSheetPixel.Green = Color->Green;
                FontSheetPixel.Blue = Color->Blue;

                memcpy_s((PIXEL32*)StringBitmap.Memory + StringBitmapOffset, sizeof(PIXEL32*), &FontSheetPixel, sizeof(PIXEL32));
            }

        }

    }

    Blit32BppBitmapToBuffer(&StringBitmap, x, y);

    if (StringBitmap.Memory)        //remember to free memory
    {
        HeapFree(GetProcessHeap(), 0, StringBitmap.Memory);
    }
}

void RenderFrameGraphics(void)
{

    switch (gCurrentGameState)
    {   
        case GAMESTATE_OPENINGSPLASH:
        {
            DrawOpeningSplashScreen();
            break;
        }
        case GAMESTATE_TITLESCREEN:
        {
            DrawTitleScreen();
            break;
        }
        case GAMESTATE_EXITYESNO:
        {
            DrawExitYesNoScreen();
            break;
        }
        case GAMESTATE_OPTIONS:
        {
            DrawOptionsScreen();
            break;
        }
        case GAMESTATE_CHARACTERNAME:
        {
            DrawCharacterNaming();
            break;
        }
        case GAMESTATE_OVERWORLD:
        {
            DrawOverworldScreen();
            break;
        }
        default:
        {
            ASSERT(FALSE, "GAMESTATE not implemented!")
        }
    }

    if (gGamePerformanceData.DisplayDebugInfo == TRUE)
    {
        DrawDebugInfo();
    }

    if (gRegistryParams.FullScreen == TRUE)
    {
        HDC DeviceContext = GetDC(gGameWindow);
        StretchDIBits(DeviceContext,
            ((gGamePerformanceData.MonitorInfo.rcMonitor.right - gGamePerformanceData.MonitorInfo.rcMonitor.left) / 2) - (GAME_RES_WIDTH * gGamePerformanceData.CurrentScaleFactor / 2),
            ((gGamePerformanceData.MonitorInfo.rcMonitor.bottom - gGamePerformanceData.MonitorInfo.rcMonitor.top) / 2) - (GAME_RES_HEIGHT * gGamePerformanceData.CurrentScaleFactor / 2),
            GAME_RES_WIDTH * gGamePerformanceData.CurrentScaleFactor,
            GAME_RES_HEIGHT * gGamePerformanceData.CurrentScaleFactor,
            0,
            0,
            GAME_RES_WIDTH,
            GAME_RES_HEIGHT,
            gBackBuffer.Memory,
            &gBackBuffer.BitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY);

        ReleaseDC(gGameWindow, DeviceContext);
    }
    else
    {
        HDC DeviceContext = GetDC(gGameWindow);
        StretchDIBits(DeviceContext,
            0,
            0,
            GAME_RES_WIDTH * gGamePerformanceData.CurrentScaleFactor,
            GAME_RES_HEIGHT * gGamePerformanceData.CurrentScaleFactor,
            0,
            0,
            GAME_RES_WIDTH,
            GAME_RES_HEIGHT,
            gBackBuffer.Memory,
            &gBackBuffer.BitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY);

        ReleaseDC(gGameWindow, DeviceContext);
    }
}


void Blit32BppBitmapToBuffer(_In_ GAMEBITMAP* GameBitmap, _In_ uint16_t x, _In_ uint16_t y)
{
    int32_t StartingScreenPixel = ((GAME_RES_HEIGHT * GAME_RES_WIDTH) - GAME_RES_WIDTH) - (GAME_RES_WIDTH * y) + x;
    int32_t StartingBitmapPixel = ((GameBitmap->BitmapInfo.bmiHeader.biWidth * GameBitmap->BitmapInfo.bmiHeader.biHeight) - GameBitmap->BitmapInfo.bmiHeader.biWidth);
    int32_t MemoryOffset = 0;
    int32_t BitmapOffset = 0;
    PIXEL32 BitmapPixel = { 0 };

    for (int16_t YPixel = 0; YPixel < GameBitmap->BitmapInfo.bmiHeader.biHeight; YPixel++)
    {
        for (int16_t XPixel = 0; XPixel < GameBitmap->BitmapInfo.bmiHeader.biWidth; XPixel++)
        {
            MemoryOffset = StartingScreenPixel + XPixel - (GAME_RES_WIDTH * YPixel);
            
            BitmapOffset = StartingBitmapPixel + XPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            memcpy_s(&BitmapPixel, sizeof(PIXEL32), (PIXEL32*)GameBitmap->Memory + BitmapOffset, sizeof(PIXEL32));     //copy contents of bitmap pixel

            if (BitmapPixel.Alpha > 0)
            {
                memcpy_s((PIXEL32*)gBackBuffer.Memory + MemoryOffset, sizeof(PIXEL32), &BitmapPixel, sizeof(PIXEL32));     //place contents of bitmap pixel onto backbuffer
            }
        }
    }
}   

void BlitBackgroundToBuffer(_In_ GAMEBITMAP* GameBitmap)
{
    int32_t StartingScreenPixel = ((GAME_RES_HEIGHT * GAME_RES_WIDTH) - GAME_RES_WIDTH);
    int32_t StartingBitmapPixel = ((GameBitmap->BitmapInfo.bmiHeader.biWidth * GameBitmap->BitmapInfo.bmiHeader.biHeight) - GameBitmap->BitmapInfo.bmiHeader.biWidth) +gCamera.x - (GameBitmap->BitmapInfo.bmiHeader.biWidth * gCamera.y);
    int32_t MemoryOffset = 0;
    int32_t BitmapOffset = 0;

#ifdef AVX
    __m256i BitmapOctoPixel;

    for (int16_t YPixel = 0; YPixel < GAME_RES_HEIGHT; YPixel++)
    {
        for (int16_t XPixel = 0; XPixel < GAME_RES_WIDTH; XPixel += 8)
        {
            MemoryOffset = StartingScreenPixel + XPixel - (GAME_RES_WIDTH * YPixel);

            BitmapOffset = StartingBitmapPixel + XPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            BitmapOctoPixel = _mm256_loadu_si256((PIXEL32*)GameBitmap->Memory + BitmapOffset);

            _mm256_store_si256((PIXEL32*)gBackBuffer.Memory + MemoryOffset, BitmapOctoPixel);
        }
    }

#elif defined SSE2    
    __m128i BitmapQuadPixel;

    for (int16_t YPixel = 0; YPixel < GAME_RES_HEIGHT; YPixel++)
    {
        for (int16_t XPixel = 0; XPixel < GAME_RES_WIDTH; XPixel += 4)
        {
            MemoryOffset = StartingScreenPixel + XPixel - (GAME_RES_WIDTH * YPixel);

            BitmapOffset = StartingBitmapPixel + XPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            BitmapQuadPixel = _mm_load_si128((PIXEL32*)GameBitmap->Memory + BitmapOffset);

            _mm_store_si128((PIXEL32*)gBackBuffer.Memory + MemoryOffset, BitmapQuadPixel);
        }
    }


#else
    PIXEL32 BitmapPixel = { 0 };
    for (int16_t YPixel = 0; YPixel < GAME_RES_HEIGHT; YPixel++)
    {
        for (int16_t XPixel = 0; XPixel < GAME_RES_WIDTH; XPixel++)
        {
            MemoryOffset = StartingScreenPixel + XPixel - (GAME_RES_WIDTH * YPixel);

            BitmapOffset = StartingBitmapPixel + XPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            memcpy_s(&BitmapPixel, sizeof(PIXEL32), (PIXEL32*)GameBitmap->Memory + BitmapOffset, sizeof(PIXEL32));     //copy contents of bitmap pixel

            memcpy_s((PIXEL32*)gBackBuffer.Memory + MemoryOffset, sizeof(PIXEL32), &BitmapPixel, sizeof(PIXEL32));     //place contents of bitmap pixel onto backbuffer
        }
    }
#endif

}

DWORD LoadRegistryParameters(void) 
{
    DWORD Result = ERROR_SUCCESS;

    HKEY RegKey = NULL;

    DWORD RegDisposition = 0;

    DWORD RegBytesRead = sizeof(DWORD);

    Result = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\" GAME_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &RegKey, &RegDisposition);

    if (Result != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] RegCreateKey failed with error code 0x%08lx!", __FUNCTION__, Result);

        goto Exit;
    }

    if (RegDisposition == REG_CREATED_NEW_KEY)
    {
        LogMessageA(LL_INFO, "[%s] Registry key did not exist; created new key HKCU\\SOFTWARE\\%s.", __FUNCTION__, GAME_NAME);
    }
    else
    {
        LogMessageA(LL_INFO, "[%s] Opened existing registry key HCKU\\SOFTWARE\\%s", __FUNCTION__, GAME_NAME);
    }



    Result = RegGetValueA(RegKey, NULL, "LogLevel", RRF_RT_DWORD, NULL, (BYTE*)&gRegistryParams.LogLevel, &RegBytesRead);

    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LL_INFO, "[%s] Registry value 'LogLevel' not found. Using default of 0. (LOG_LEVEL_NONE)", __FUNCTION__);
            gRegistryParams.LogLevel = LL_NONE;
        }
        else
        {
            LogMessageA(LL_ERROR, "[%s] Failed to read the 'LogLevel' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }

    LogMessageA(LL_INFO, "[%s] LogLevel is %d", __FUNCTION__, gRegistryParams.LogLevel);



    Result = RegGetValueA(RegKey, NULL, "ScaleFactor", RRF_RT_DWORD, NULL, (BYTE*)&gRegistryParams.ScaleFactor, &RegBytesRead);

    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LL_INFO, "[%s] Registry value 'ScaleFactor' not found. Using default of 0.", __FUNCTION__);
            gRegistryParams.ScaleFactor = 0;
        }
        else
        {
            LogMessageA(LL_ERROR, "[%s] Failed to read the 'ScaleFactor' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }
    LogMessageA(LL_INFO, "[%s] ScaleFactor is %d", __FUNCTION__, gRegistryParams.ScaleFactor);



    Result = RegGetValueA(RegKey, NULL, "SFXVolume", RRF_RT_DWORD, NULL, &gRegistryParams.SFXVolume, &RegBytesRead);

    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LL_INFO, "[%s] Registry value 'SFXVolume' not found. Using default of 0.5.", __FUNCTION__);
            gRegistryParams.SFXVolume = 50;
        }
        else
        {
            LogMessageA(LL_ERROR, "[%s] Failed to read the 'SFXVolume' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }

    gSFXVolume = gRegistryParams.SFXVolume;
    LogMessageA(LL_INFO, "[%s] SFXVolume is %d", __FUNCTION__, gSFXVolume);



    Result = RegGetValueA(RegKey, NULL, "MusicVolume", RRF_RT_DWORD, NULL, &gRegistryParams.MusicVolume, &RegBytesRead);

    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LL_INFO, "[%s] Registry value 'MusicVolume' not found. Using default of 0.5.", __FUNCTION__);
            gRegistryParams.MusicVolume = 50;
        }
        else
        {
            LogMessageA(LL_ERROR, "[%s] Failed to read the 'MusicVolume' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }

    gMusicVolume = gRegistryParams.MusicVolume;
    LogMessageA(LL_INFO, "[%s] MusicVolume is %d", __FUNCTION__, gMusicVolume);



    Result = RegGetValueA(RegKey, NULL, "FullScreen", RRF_RT_DWORD, NULL, (BYTE*)&gRegistryParams.FullScreen, &RegBytesRead);

    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LL_INFO, "[%s] Registry value 'FullScreen' not found. Using default of FALSE.", __FUNCTION__);
            gRegistryParams.FullScreen = 0;
        }
        else
        {
            LogMessageA(LL_ERROR, "[%s] Failed to read the 'FullScreen' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }
    LogMessageA(LL_INFO, "[%s] FullScreen is %d", __FUNCTION__, gRegistryParams.FullScreen);



    //...

Exit:
    if (RegKey)
    {
        RegCloseKey(RegKey);
    }
    return(Result);
}

DWORD SaveRegistryParameters(void)
{

    DWORD Result = ERROR_SUCCESS;

    HKEY RegKey = NULL;

    DWORD RegDisposition = 0;

    //DWORD SFXVolume = (DWORD)gSFXVolume * 100.0f;

    //DWORD MusicVolume = (DWORD)gMusicVolume * 100.0f;

    Result = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\" GAME_NAME, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &RegKey, &RegDisposition);

    if (Result != ERROR_SUCCESS)
    {
        //LogMessageA(LL_ERROR, "[%s] RegCreateKey failed with error code 0x%08lx!", __FUNCTION__, Result);

        goto Exit;
    }



    Result = RegSetValueExA(RegKey, "SFXVolume", 0, REG_DWORD, (const BYTE*)&gSFXVolume, sizeof(DWORD));

    if (Result != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to set 'SFXVolume' in registry! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }
    LogMessageA(LL_INFO, "[%s] Saved 'SFXVolume' in registry: %d. ", __FUNCTION__, gSFXVolume);



    Result = RegSetValueExA(RegKey, "MusicVolume", 0, REG_DWORD, (const BYTE*)&gMusicVolume, sizeof(DWORD));

    if (Result != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to set 'MusicVolume' in registry! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }
    LogMessageA(LL_INFO, "[%s] Saved 'MusicVolume' in registry: %d. ", __FUNCTION__, gMusicVolume);



    Result = RegSetValueExA(RegKey, "ScaleFactor", 0, REG_DWORD, (const BYTE*)&gGamePerformanceData.CurrentScaleFactor, sizeof(DWORD));

    if (Result != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to set 'ScaleFactor' in registry! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }
    LogMessageA(LL_INFO, "[%s] Saved 'ScaleFactor' in registry: %d. ", __FUNCTION__, gGamePerformanceData.CurrentScaleFactor);



    Result = RegSetValueExA(RegKey, "FullScreen", 0, REG_DWORD, (const BYTE*)&gRegistryParams.FullScreen, sizeof(BOOL));

    if (Result != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to set 'FullScreen' in registry! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }
    LogMessageA(LL_INFO, "[%s] Saved 'FullScreen' in registry: %d. ", __FUNCTION__, gRegistryParams.FullScreen);
    

Exit:
    if (RegKey)
    {
        RegCloseKey(RegKey);
    }
    return(Result);
}

void LogMessageA(_In_ LOGLEVEL LogLevel, _In_ char* Message, _In_ ...)
{
    size_t MessageLength = strlen(Message);

    SYSTEMTIME Time = { 0 };

    HANDLE LogFileHandle = INVALID_HANDLE_VALUE;

    DWORD EndOfFile = 0;

    DWORD NumberOfBytesWritten = 0;

    char DateTimeString[96] = { 0 };

    char SeverityString[8] = { 0 };

    char FormattedString[4096] = { 0 };

    int Error = 0;

    if (gRegistryParams.LogLevel < (DWORD)LogLevel)
    {
        return;
    }
    if (MessageLength < 1 || MessageLength > 4096)
    {
        ASSERT(FALSE, "Message was either too short or too long!")
    }
    switch (LogLevel)
    {   
        case LL_NONE:
        {
            return;
        }
        case LL_INFO:
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[INFO]");
            break;
        }
        case LL_WARNING:
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[WARN]");
            break;
        }
        case LL_ERROR:
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[ERROR]");
            break;
        }
        case LL_DEBUG:
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[DEBUG]");
            break;
        }
        default:
        {
            ASSERT(FALSE, "Unrecognized LogLevel!");
        }
    }
    GetLocalTime(&Time);

    va_list ArgPointer = NULL;
    va_start(ArgPointer, Message);

    _vsnprintf_s(FormattedString, sizeof(FormattedString), _TRUNCATE, Message, ArgPointer);

    va_end(ArgPointer);

    Error = _snprintf_s(DateTimeString, sizeof(DateTimeString), _TRUNCATE, "\r\n[%02u/%02u/%u %02u:%02u:%02u.%03u]", Time.wMonth, Time.wDay, Time.wYear, Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);

    EnterCriticalSection(&gLogCritSec);

    if ((LogFileHandle = CreateFileA(LOG_FILE_NAME, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        ASSERT(FALSE, "Failed to access log file!");
    }

    EndOfFile = SetFilePointer(LogFileHandle, 0, NULL, FILE_END);

    WriteFile(LogFileHandle, DateTimeString, (DWORD)strlen(DateTimeString), &NumberOfBytesWritten, NULL);
    WriteFile(LogFileHandle, SeverityString, (DWORD)strlen(SeverityString), &NumberOfBytesWritten, NULL);
    WriteFile(LogFileHandle, FormattedString, (DWORD)strlen(FormattedString), &NumberOfBytesWritten, NULL);
    if (LogFileHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(LogFileHandle);
    }

    LeaveCriticalSection(&gLogCritSec);         ////////remember to leave at every available exit from this function, this function only has one
}

void DrawDebugInfo(void)
{
        char DebugTextBuffer[64] = { 0 };
        PIXEL32 White = { 0xFF, 0xFF, 0xFF, 0xFF };
        PIXEL32 LimeGreen = { 0x00, 0xFF, 0x17, 0xFF };
        PIXEL32 SkyBlue = { 0xFF, 0x0F, 0x00, 0xFF };

        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FPS Raw: %.01f ", gGamePerformanceData.RawFPSAverage);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 0);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FPS Cook : %.01f ", gGamePerformanceData.CookedFPSAverage);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &LimeGreen, 0, 8);
        /*sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Min Timer: %.03f", gGamePerformanceData.MinimumTimerResolution / 10000.0f);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 16);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Max Timer:  %.03f", gGamePerformanceData.MaximumTimerResolution / 10000.0f);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 24);*/
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "C.Time Res:%.02f", gGamePerformanceData.CurrentTimerResolution / 10000.0f);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &LimeGreen, 0, 16);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Memory: %llu KB ", (gGamePerformanceData.MemInfo.PrivateUsage / 1024));
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &LimeGreen, 0, 24);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "CPU: %.03f%%", gGamePerformanceData.CPUPercent);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &LimeGreen, 0, 32);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Handles  :  %lu ", gGamePerformanceData.HandleCount);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 40);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Frames   :%llu", gGamePerformanceData.TotalFramesRendered);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 48);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Screen: (%d,%d) ", gPlayer.ScreenPos.x, gPlayer.ScreenPos.y);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &SkyBlue, 0, 56);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "World: (%d,%d) ", gPlayer.WorldPos.x, gPlayer.WorldPos.y);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &SkyBlue, 0, 64);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Camera: (%d,%d) ", gCamera.x, gCamera.y);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &SkyBlue, 0, 72);
}




HRESULT InitializeSoundEngine(void)
{
    HRESULT Result = S_OK;

    WAVEFORMATEX SfxWaveFormat = { 0 };
    WAVEFORMATEX MusicWaveFormat = { 0 };

    float SFXVolume = (DWORD)gSFXVolume / 100.0f;
    float MusicVolume = (DWORD)gMusicVolume / 100.0f;

    Result = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    if (Result != S_OK)
    {
        LogMessageA(LL_ERROR, "[%s] CoInitializeEx failed with 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    Result = XAudio2Create(&gXAudio, 0, XAUDIO2_ANY_PROCESSOR);

    if (FAILED(Result))
    {
        LogMessageA(LL_ERROR, "[%s] XAudio2Create failed with 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    Result = gXAudio->lpVtbl->CreateMasteringVoice(gXAudio, &gXAudioMasteringVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL, 0);

    if (FAILED(Result))
    {
        LogMessageA(LL_ERROR, "[%s] CreateMasteringVoice failed with 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    SfxWaveFormat.wFormatTag = WAVE_FORMAT_PCM;

    SfxWaveFormat.nChannels = 1;    //mono

    SfxWaveFormat.nSamplesPerSec = 44100;   //hz of .wav file

    SfxWaveFormat.nAvgBytesPerSec = SfxWaveFormat.nSamplesPerSec * SfxWaveFormat.nChannels * 2;

    SfxWaveFormat.nBlockAlign = SfxWaveFormat.nChannels * 2;

    SfxWaveFormat.wBitsPerSample = 16;

    SfxWaveFormat.cbSize = 0x6164;

    for (uint8_t Counter = 0; Counter < NUMBER_OF_SFX_SOURCE_VOICES; Counter++)
    {
        Result = gXAudio->lpVtbl->CreateSourceVoice(gXAudio, &gXAudioSFXSourceVoice[Counter], &SfxWaveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);

        if (Result != S_OK)
        {
            LogMessageA(LL_ERROR, "[%s] CreateSourceVoice for SFX failed with 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }

        gXAudioSFXSourceVoice[Counter]->lpVtbl->SetVolume(gXAudioSFXSourceVoice[Counter], SFXVolume, XAUDIO2_COMMIT_NOW);
    }

    MusicWaveFormat.wFormatTag = WAVE_FORMAT_PCM;

    MusicWaveFormat.nChannels = 2;      //stereo

    MusicWaveFormat.nSamplesPerSec = 44100;

    MusicWaveFormat.nAvgBytesPerSec = MusicWaveFormat.nSamplesPerSec * MusicWaveFormat.nChannels * 2;

    MusicWaveFormat.nBlockAlign = MusicWaveFormat.nChannels * 2;

    MusicWaveFormat.wBitsPerSample = 16;

    MusicWaveFormat.cbSize = 0;

    Result = gXAudio->lpVtbl->CreateSourceVoice(gXAudio, &gXAudioMusicSourceVoice, &MusicWaveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);

    if (Result != S_OK)
    {

        LogMessageA(LL_ERROR, "[%s] CreateSourceVoice for MUSIC failed with 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    gXAudioMusicSourceVoice->lpVtbl->SetVolume(gXAudioMusicSourceVoice, MusicVolume, XAUDIO2_COMMIT_NOW);

Exit:
    return(Result);
}

DWORD LoadWaveFromFile(_In_ char* FileName, _Inout_ GAMESOUND* GameSound)
{
    DWORD Error = ERROR_SUCCESS;

    DWORD NumberOfBytesRead = 0;

    DWORD RIFF = 0;

    uint16_t DataChunkOffset = 0;

    DWORD DataChunkSearcher = 0;

    BOOL DataChunkFound = FALSE;

    DWORD DataChunkSize = 0;

    HANDLE FileHandle = INVALID_HANDLE_VALUE;

    void* AudioData = NULL;



    if ((FileHandle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] CreateFileA failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, &RIFF, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile1 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (RIFF != 0x46464952)     //0x46464952 is "RIFF" backwards
    {
        Error = ERROR_FILE_INVALID;
        LogMessageA(LL_ERROR, "[%s] First four bytes of this file are not 'RIFF'! Error 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (SetFilePointer(FileHandle, 20, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] SetFilePointer1 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, &GameSound->WaveFormat, sizeof(WAVEFORMATEX), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile2 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (GameSound->WaveFormat.nBlockAlign != (GameSound->WaveFormat.nChannels * GameSound->WaveFormat.wBitsPerSample / 8) || 
        GameSound->WaveFormat.wFormatTag != WAVE_FORMAT_PCM ||
        GameSound->WaveFormat.wBitsPerSample != 16)
    {
        Error = ERROR_DATATYPE_MISMATCH;
        LogMessageA(LL_ERROR, "[%s] This wav file did not meet format requirements! Only PCM format, 44.1kHz, 16bits per sample wav files are supported. Error 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    while (DataChunkFound == FALSE)
    {
        if (SetFilePointer(FileHandle, DataChunkOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            Error = GetLastError();
            LogMessageA(LL_ERROR, "[%s] SetFilePointer2 failed with 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }

        if (ReadFile(FileHandle, &DataChunkSearcher, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
        {
            Error = GetLastError();
            LogMessageA(LL_ERROR, "[%s] ReadFile3 failed with 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }

        if (DataChunkSearcher == 0x61746164)    ////'data' backwords
        {
            DataChunkFound = TRUE;
            break;
        }
        else
        {
            DataChunkOffset += 4;
        }

        if (DataChunkOffset > 256)
        {
            Error = ERROR_DATATYPE_MISMATCH;
            LogMessageA(LL_ERROR, "[%s] Datachunk not found in first 256 bytes of this file! Error 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }
    }

    if (SetFilePointer(FileHandle, DataChunkOffset + 4, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] SetFilePointer3 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }
    
    if (ReadFile(FileHandle, &DataChunkSize, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile4 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    AudioData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, DataChunkSize);

    if (AudioData == NULL)
    {
        Error = ERROR_NOT_ENOUGH_MEMORY;
        LogMessageA(LL_ERROR, "[%s] HeapAlloc failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    GameSound->Buffer.Flags = XAUDIO2_END_OF_STREAM;
    GameSound->Buffer.AudioBytes = DataChunkSize;

    if (SetFilePointer(FileHandle, DataChunkOffset + 8, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] SetFilePointer4 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, AudioData, DataChunkSize, &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile5 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    GameSound->Buffer.pAudioData = AudioData;

Exit:

    if (Error == ERROR_SUCCESS)
    {
        LogMessageA(LL_INFO, "[%s] Loading successful: %s", __FUNCTION__, FileName);
    }
    else
    {
        LogMessageA(LL_ERROR, "[%s] Failed to load %s! Error 0x%08lx!", __FUNCTION__, FileName, Error);
    }

    if (FileHandle && (FileHandle != INVALID_HANDLE_VALUE))
    {
        CloseHandle(FileHandle);
    }

    return(Error);
}


DWORD LoadWaveFromMem(_In_ void* Buffer, _Inout_ GAMESOUND* GameSound)
{

    DWORD Error = ERROR_SUCCESS;

    //DWORD NumberOfBytesRead = 0;

    DWORD RIFF = 0;

    uint16_t DataChunkOffset = 0;

    DWORD DataChunkSearcher = 0;

    BOOL DataChunkFound = FALSE;

    DWORD DataChunkSize = 0;

    //HANDLE FileHandle = INVALID_HANDLE_VALUE;

    //void* AudioData = NULL;



    //if ((FileHandle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    //{
    //    Error = GetLastError();
    //    LogMessageA(LL_ERROR, "[%s] CreateFileA failed with 0x%08lx!", __FUNCTION__, Error);
    //    goto Exit;
    //}

    memcpy(&RIFF, Buffer, sizeof(DWORD));

    //if (ReadFile(FileHandle, &RIFF, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
    //{
    //    Error = GetLastError();
    //    LogMessageA(LL_ERROR, "[%s] ReadFile1 failed with 0x%08lx!", __FUNCTION__, Error);
    //    goto Exit;
    //}

    if (RIFF != 0x46464952)     //0x46464952 is "RIFF" backwards
    {
        Error = ERROR_FILE_INVALID;
        LogMessageA(LL_ERROR, "[%s] First four bytes of memory buffer are not 'RIFF'! Error 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    //if (SetFilePointer(FileHandle, 20, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    //{
    //    Error = GetLastError();
    //    LogMessageA(LL_ERROR, "[%s] SetFilePointer1 failed with 0x%08lx!", __FUNCTION__, Error);
    //    goto Exit;
    //}

    memcpy(&GameSound->WaveFormat, (BYTE*)Buffer + 20, sizeof(WAVEFORMATEX));

    /*if (ReadFile(FileHandle, &GameSound->WaveFormat, sizeof(WAVEFORMATEX), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile2 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }*/

    if (GameSound->WaveFormat.nBlockAlign != (GameSound->WaveFormat.nChannels * GameSound->WaveFormat.wBitsPerSample / 8) ||
        GameSound->WaveFormat.wFormatTag != WAVE_FORMAT_PCM ||
        GameSound->WaveFormat.wBitsPerSample != 16)
    {
        Error = ERROR_DATATYPE_MISMATCH;
        LogMessageA(LL_ERROR, "[%s] This wav data in the memory buffer did not meet format requirements! Only PCM format, 44.1kHz, 16bits per sample wav files are supported. Error 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    while (DataChunkFound == FALSE)
    {
        /*if (SetFilePointer(FileHandle, DataChunkOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {
            Error = GetLastError();
            LogMessageA(LL_ERROR, "[%s] SetFilePointer2 failed with 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }

        if (ReadFile(FileHandle, &DataChunkSearcher, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
        {
            Error = GetLastError();
            LogMessageA(LL_ERROR, "[%s] ReadFile3 failed with 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }*/

        memcpy(&DataChunkSearcher, (BYTE*)Buffer + DataChunkOffset, sizeof(DWORD));

        if (DataChunkSearcher == 0x61746164)    ////'data' backwords
        {
            DataChunkFound = TRUE;
            break;
        }
        else
        {
            DataChunkOffset += 4;
        }

        if (DataChunkOffset > 256)
        {
            Error = ERROR_DATATYPE_MISMATCH;
            LogMessageA(LL_ERROR, "[%s] Datachunk not found in first 256 bytes of the memory buffer! Error 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }
    }

    //if (SetFilePointer(FileHandle, DataChunkOffset + 4, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    //{
    //    Error = GetLastError();
    //    LogMessageA(LL_ERROR, "[%s] SetFilePointer3 failed with 0x%08lx!", __FUNCTION__, Error);
    //    goto Exit;
    //}

    memcpy(&DataChunkSize, (BYTE*)Buffer + DataChunkOffset + 4, sizeof(DWORD));

    /*if (ReadFile(FileHandle, &DataChunkSize, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile4 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }*/

    /*AudioData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, DataChunkSize);

    if (AudioData == NULL)
    {
        Error = ERROR_NOT_ENOUGH_MEMORY;
        LogMessageA(LL_ERROR, "[%s] HeapAlloc failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }*/

    GameSound->Buffer.Flags = XAUDIO2_END_OF_STREAM;
    GameSound->Buffer.AudioBytes = DataChunkSize;

    /*if (SetFilePointer(FileHandle, DataChunkOffset + 8, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] SetFilePointer4 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, AudioData, DataChunkSize, &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile5 failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }*/

    GameSound->Buffer.pAudioData = (BYTE*)Buffer + DataChunkOffset + 8;

Exit:

    if (Error == ERROR_SUCCESS)
    {
        LogMessageA(LL_INFO, "[%s] Successfully loaded wav from memory!", __FUNCTION__);
    }
    else
    {
        LogMessageA(LL_ERROR, "[%s] Failed to load wav from memory! Error 0x%08lx!", __FUNCTION__, Error);
    }

    /*if (FileHandle && (FileHandle != INVALID_HANDLE_VALUE))
    {
        CloseHandle(FileHandle);
    }*/

    return(Error);
}


DWORD LoadOggFromMem(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ GAMESOUND* GameSound)
{
    DWORD Error = ERROR_SUCCESS;

    //HANDLE FileHandle = INVALID_HANDLE_VALUE;

    //LARGE_INTEGER FileSize = { 0 };

    //DWORD BytesRead = 0;

    //void* FileBuffer = NULL;

    int SamplesDecoded = 0;

    int Channels = 0;

    int SampleRate = 0;

    short* DecodedAudio = NULL;


   /* if ((FileHandle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] CreateFileA failed with 0x%08lx on %s!", __FUNCTION__, Error, FileName);
        goto Exit;
    }*/

    /*if (GetFileSizeEx(FileHandle, &FileSize) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] GetFileSizeEx failed with 0x%08lx on %s!", __FUNCTION__, Error, FileName);
        goto Exit;
    }*/

    LogMessageA(LL_INFO, "[%s] Size of Ogg file: %lu.", __FUNCTION__, BufferSize);


    /*FileBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, FileSize.QuadPart);

    if (FileBuffer == NULL)
    {
        Error = ERROR_OUTOFMEMORY;
        LogMessageA(LL_ERROR, "[%s] HeapAllox failed with 0x%08lx on %s!", __FUNCTION__, Error, FileName);
        goto Exit;
    }*/

    /*if (ReadFile(FileHandle, FileBuffer, FileSize.QuadPart, &BytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx on %s!", __FUNCTION__, Error, FileName);
        goto Exit;
    }*/

    SamplesDecoded = stb_vorbis_decode_memory(Buffer, (int)BufferSize, &Channels, &SampleRate, &DecodedAudio);

    if (SamplesDecoded < 1)
    {
        Error = ERROR_BAD_COMPRESSION_BUFFER;
        LogMessageA(LL_ERROR, "[%s] stb_vorbis_decode_memory failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    GameSound->WaveFormat.wFormatTag = WAVE_FORMAT_PCM;

    GameSound->WaveFormat.nChannels = Channels;

    GameSound->WaveFormat.nSamplesPerSec = SampleRate;

    GameSound->WaveFormat.nAvgBytesPerSec = GameSound->WaveFormat.nSamplesPerSec * GameSound->WaveFormat.nChannels * 2;

    GameSound->WaveFormat.nBlockAlign = GameSound->WaveFormat.nChannels * 2;

    GameSound->WaveFormat.wBitsPerSample = 16;

    GameSound->Buffer.Flags = XAUDIO2_END_OF_STREAM;

    GameSound->Buffer.AudioBytes = SamplesDecoded * GameSound->WaveFormat.nChannels * 2;

    GameSound->Buffer.pAudioData = DecodedAudio;


Exit:

    return(Error);
}



DWORD LoadTileMapFromMem(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ TILEMAP* TileMap)
{
    DWORD Error = ERROR_SUCCESS;

    DWORD BytesRead = 0;

    char* Cursor = NULL;

    char TempBuffer[16] = { 0 };

    uint16_t Rows = 0;

    uint16_t Columns = 0;


    if (BufferSize < 300)
    {
        Error = ERROR_FILE_INVALID;

        LogMessageA(LL_ERROR, "[%s] Buffer is too small to be a valid tile map! 0x08%lx!", __FUNCTION__, Error);

        goto Exit;
    }



    ///////////width

    if ((Cursor = strstr(Buffer, "width=")) == NULL)
    {
        Error = ERROR_INVALID_DATA;
        LogMessageA(LL_ERROR, "[%s] Could not locate Width attribute! 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    BytesRead = 0;      //reset

    for (;;)
    {
        if (BytesRead > 8)
        {
            ////should have found opening quotation mark ("width"=)
            Error = ERROR_INVALID_DATA;
            LogMessageA(LL_ERROR, "[%s] Could not locate opening quotation mark before Width attribute! 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }
        if (*Cursor == '\"')
        {
            Cursor++;
            break;
        }
        else
        {
            Cursor++;
        }
        BytesRead++;
    }

    BytesRead = 0;      //reset

    for (uint8_t Counter = 0; Counter < 6; Counter++)
    {
        if (*Cursor == '\"')
        {
            Cursor++;
            break;
        }
        else
        {
            TempBuffer[Counter] = *Cursor;
            Cursor++;
        }
    }

    TileMap->Width = atoi(TempBuffer);
    if (TileMap->Width == 0)
    {
        Error = ERROR_INVALID_DATA;
        LogMessageA(LL_ERROR, "[%s] Width attribute was 0! 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    memset(TempBuffer, 0, sizeof(TempBuffer));

    //////////height

    if ((Cursor = strstr(Buffer, "height=")) == NULL)
    {
        Error = ERROR_INVALID_DATA;
        LogMessageA(LL_ERROR, "[%s] Could not locate height attribute! 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    BytesRead = 0;      //reset

    for (;;)
    {
        if (BytesRead > 8)
        {
            ////should have found opening quotation mark ("height"=)
            Error = ERROR_INVALID_DATA;
            LogMessageA(LL_ERROR, "[%s] Could not locate opening quotation mark before height attribute! 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }
        if (*Cursor == '\"')
        {
            Cursor++;
            break;
        }
        else
        {
            Cursor++;
        }
        BytesRead++;
    }

    BytesRead = 0;      //reset

    for (uint8_t Counter = 0; Counter < 6; Counter++)
    {
        if (*Cursor == '\"')
        {
            Cursor++;
            break;
        }
        else
        {
            TempBuffer[Counter] = *Cursor;
            Cursor++;
        }
    }

    TileMap->Height = atoi(TempBuffer);
    if (TileMap->Height == 0)
    {
        Error = ERROR_INVALID_DATA;
        LogMessageA(LL_ERROR, "[%s] Height attribute was 0! 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    LogMessageA(LL_INFO, "[%s] TileMap dimensions: %dx%d.", __FUNCTION__, TileMap->Width, TileMap->Height);

    Rows = TileMap->Height;

    Columns = TileMap->Width;

    TileMap->Map = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Rows * sizeof(void*));

    if (TileMap->Map == NULL)
    {
        Error = ERROR_OUTOFMEMORY;
        LogMessageA(LL_ERROR, "[%s] HeapAlloc of height Failed with error 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    for (uint16_t Counter = 0; Counter < TileMap->Height; Counter++)
    {
        TileMap->Map[Counter] = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Columns * sizeof(void*));

        if (TileMap->Map[Counter] == NULL)
        {
            Error = ERROR_OUTOFMEMORY;
            LogMessageA(LL_ERROR, "[%s] HeapAlloc of width Failed with error 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }
    }

    BytesRead = 0;

    memset(TempBuffer, 0, sizeof(TempBuffer));

    if ((Cursor = strstr(Buffer, ",")) == NULL)
    {
        Error = ERROR_INVALID_DATA;

        LogMessageA(LL_ERROR, "[%s] Could not find a comma character in the Buffer! 0x%08lx!", __FUNCTION__, Error);

        goto Exit;
    }

    while (*Cursor != '\r' && *Cursor != '\n')
    {
        if (BytesRead > 4)
        {
            Error = ERROR_INVALID_DATA;

            LogMessageA(LL_ERROR, "[%s] Could not find a new line character at the beginning of the tile map data in the Buffer! 0x%08lx!", __FUNCTION__, Error);

            goto Exit;
        }

        BytesRead++;

        Cursor--;
    }

    Cursor++;

    for (uint16_t Row = 0; Row < Rows; Row++)
    {
        for (uint16_t Column = 0; Column < Columns; Column++)
        {
            memset(TempBuffer, 0, sizeof(TempBuffer));

        Skip:

            if (*Cursor == '\r' || *Cursor == '\n')
            {
                Cursor++;

                goto Skip;
            }

            for (uint8_t Counter = 0; Counter <= 10; Counter++)
            {
                if (*Cursor == ',' || *Cursor == '<')
                {
                    if (((TileMap->Map[Row][Column]) = (uint8_t)atoi(TempBuffer)) == 0)
                    {
                        Error = ERROR_INVALID_DATA;

                        LogMessageA(LL_ERROR, "[%s] atoi failed while converting tile map data in the Buffer! 0x%08lx!", __FUNCTION__, Error);

                        goto Exit;
                    }

                    Cursor++;

                    break;
                }

                TempBuffer[Counter] = *Cursor;

                Cursor++;
            }
        }
    }



Exit:
    
        ////TODO: free Buffer somehow, using an mz_ function probably
    /*if (Buffer)
    {
        HeapFree(GetProcessHeap(), 0, Buffer);
    }*/

    return(Error);
}



DWORD Load32BppBitmapFromMem(_In_ void* Buffer, _Inout_ GAMEBITMAP* GameBitmap)
{
    DWORD Error = ERROR_SUCCESS;

    WORD BitmapHeader = 0;

    DWORD PixelDataOffset = 0;

    //DWORD NumberOfBytesRead = 2;

    memcpy(&BitmapHeader, Buffer, 2);

    if (BitmapHeader != 0x4d42)     //0x4d42 is "BM" backwards
    {
        Error = ERROR_INVALID_DATA;
        LogMessageA(LL_ERROR, "[%s] First two bytes are not 'BM'! Error 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    memcpy(&PixelDataOffset, (BYTE*)Buffer + 0xA, sizeof(DWORD));

    memcpy(&GameBitmap->BitmapInfo.bmiHeader, (BYTE*)Buffer + 0xE, sizeof(BITMAPINFOHEADER));

    GameBitmap->Memory = (BYTE*)Buffer + PixelDataOffset;

    

Exit:

    if (Error == ERROR_SUCCESS)
    {
        LogMessageA(LL_INFO, "[%s] Loading Successful", __FUNCTION__);
    }
    else
    {
        LogMessageA(LL_ERROR, "[%s] Loading failed! Error 0x%08lx!", __FUNCTION__, Error);
    }

    return(Error);
}


void PlayGameSound(_In_ GAMESOUND* GameSound)
{
    gXAudioSFXSourceVoice[gSFXSourceVoiceSelector]->lpVtbl->SubmitSourceBuffer(gXAudioSFXSourceVoice[gSFXSourceVoiceSelector], &GameSound->Buffer, NULL);

    gXAudioSFXSourceVoice[gSFXSourceVoiceSelector]->lpVtbl->Start(gXAudioSFXSourceVoice[gSFXSourceVoiceSelector], 0, XAUDIO2_COMMIT_NOW);

    gSFXSourceVoiceSelector++;

    if (gSFXSourceVoiceSelector > (NUMBER_OF_SFX_SOURCE_VOICES - 1))
    {
        gSFXSourceVoiceSelector = 0;
    }
}

void PlayGameMusic(_In_ GAMESOUND* GameSound)
{
    gXAudioMusicSourceVoice->lpVtbl->Stop(gXAudioMusicSourceVoice, 0, 0);

    gXAudioMusicSourceVoice->lpVtbl->FlushSourceBuffers(gXAudioMusicSourceVoice);

    GameSound->Buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

    gXAudioMusicSourceVoice->lpVtbl->SubmitSourceBuffer(gXAudioMusicSourceVoice, &GameSound->Buffer, NULL);

    gXAudioMusicSourceVoice->lpVtbl->Start(gXAudioMusicSourceVoice, 0, XAUDIO2_COMMIT_NOW);
}


DWORD LoadTileMapFromFile(_In_ char* FileName, _Inout_ TILEMAP* TileMap)
{
    DWORD Error = ERROR_SUCCESS;

    HANDLE FileHandle = INVALID_HANDLE_VALUE;

    LARGE_INTEGER FileSize = { 0 };

    DWORD BytesRead = 0;

    void* FileBuffer = NULL;

    char* Cursor = NULL;

    char TempBuffer[16] = { 0 };

    uint16_t Rows = 0;

    uint16_t Columns = 0;


    if ((FileHandle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] CreateFileA failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (GetFileSizeEx(FileHandle, &FileSize) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] GetFileSizeEx failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    LogMessageA(LL_INFO, "[%s] Size of file %s: %lu.", __FUNCTION__, FileName, FileSize.QuadPart);

    FileBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, FileSize.QuadPart);

    if (FileBuffer == NULL)
    {
        Error = ERROR_OUTOFMEMORY;
        LogMessageA(LL_ERROR, "[%s] HeapAllox failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    if (ReadFile(FileHandle, FileBuffer, FileSize.QuadPart, &BytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    ///////////width

    if ((Cursor = strstr(FileBuffer, "width=")) == NULL)
    {
        Error = ERROR_INVALID_DATA;
        LogMessageA(LL_ERROR, "[%s] Could not locate Width attribute! 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    BytesRead = 0;      //reset

    for (;;)
    {
        if (BytesRead > 8)
        {
            ////should have found opening quotation mark ("width"=)
            Error = ERROR_INVALID_DATA;
            LogMessageA(LL_ERROR, "[%s] Could not locate opening quotation mark before Width attribute! 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }
        if (*Cursor == '\"')
        {
            Cursor++;
            break;
        }
        else
        {
            Cursor++;
        }
        BytesRead++;
    }

    BytesRead = 0;      //reset

    for (uint8_t Counter = 0; Counter < 6; Counter++)
    {
        if (*Cursor == '\"')
        {
            Cursor++;
            break;
        }
        else
        {
            TempBuffer[Counter] = *Cursor;
            Cursor++;
        }
    }

    TileMap->Width = atoi(TempBuffer);
    if (TileMap->Width == 0)
    {
        Error = ERROR_INVALID_DATA;
        LogMessageA(LL_ERROR, "[%s] Width attribute was 0! 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }
            
    memset(TempBuffer, 0, sizeof(TempBuffer));

    //////////height

    if ((Cursor = strstr(FileBuffer, "height=")) == NULL)
    {
        Error = ERROR_INVALID_DATA;
        LogMessageA(LL_ERROR, "[%s] Could not locate height attribute! 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    BytesRead = 0;      //reset

    for (;;)
    {
        if (BytesRead > 8)
        {
            ////should have found opening quotation mark ("height"=)
            Error = ERROR_INVALID_DATA;
            LogMessageA(LL_ERROR, "[%s] Could not locate opening quotation mark before height attribute! 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }
        if (*Cursor == '\"')
        {
            Cursor++;
            break;
        }
        else
        {
            Cursor++;
        }
        BytesRead++;
    }

    BytesRead = 0;      //reset

    for (uint8_t Counter = 0; Counter < 6; Counter++)
    {
        if (*Cursor == '\"')
        {
            Cursor++;
            break;
        }
        else
        {
            TempBuffer[Counter] = *Cursor;
            Cursor++;
        }
    }

    TileMap->Height = atoi(TempBuffer);
    if (TileMap->Height == 0)
    {
        Error = ERROR_INVALID_DATA;
        LogMessageA(LL_ERROR, "[%s] Height attribute was 0! 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    LogMessageA(LL_INFO, "[%s] %s TileMap dimensions: %dx%d.", __FUNCTION__, FileName, TileMap->Width, TileMap->Height);

    Rows = TileMap->Height;

    Columns = TileMap->Width;

    TileMap->Map = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Rows * sizeof(void*));
    if (TileMap->Map == NULL)
    {
        Error = ERROR_OUTOFMEMORY;
        LogMessageA(LL_ERROR, "[%s] HeapAlloc of height Failed with error 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    for (uint16_t Counter = 0; Counter < TileMap->Height; Counter++)
    {
        TileMap->Map[Counter] = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Columns * sizeof(void*));

        if (TileMap->Map[Counter] == NULL)
        {
            Error = ERROR_OUTOFMEMORY;
            LogMessageA(LL_ERROR, "[%s] HeapAlloc of width Failed with error 0x%08lx!", __FUNCTION__, Error);
            goto Exit;
        }
    }

    BytesRead = 0;

    memset(TempBuffer, 0, sizeof(TempBuffer));

    if ((Cursor = strstr(FileBuffer, ",")) == NULL)
    {
        Error = ERROR_INVALID_DATA;

        LogMessageA(LL_ERROR, "[%s] Could not find a comma character in the file %s! 0x%08lx!", __FUNCTION__, FileName, Error);

        goto Exit;
    }

    while (*Cursor != '\r' && *Cursor != '\n')
    {
        if (BytesRead > 4)
        {
            Error = ERROR_INVALID_DATA;

            LogMessageA(LL_ERROR, "[%s] Could not find a new line character at the beginning of the tile map data in the file %s! 0x%08lx!", __FUNCTION__, FileName, Error);

            goto Exit;
        }

        BytesRead++;

        Cursor--;
    }

    Cursor++;

    for (uint16_t Row = 0; Row < Rows; Row++)
    {
        for (uint16_t Column = 0; Column < Columns; Column++)
        {
            memset(TempBuffer, 0, sizeof(TempBuffer));

            Skip:

            if (*Cursor == '\r' || *Cursor == '\n')
            {
                Cursor++;

                goto Skip;
            }

            for (uint8_t Counter = 0; Counter <= 10; Counter++)
            {
                if (*Cursor == ',' || *Cursor == '<')
                {
                    if (((TileMap->Map[Row][Column]) = (uint8_t)atoi(TempBuffer)) == 0)
                    {
                        Error = ERROR_INVALID_DATA;

                        LogMessageA(LL_ERROR, "[%s] atoi failed while converting tile map data in the file %s! 0x%08lx!", __FUNCTION__, FileName, Error);

                        goto Exit;
                    }

                    Cursor++;

                    break;
                }

                TempBuffer[Counter] = *Cursor;

                Cursor++;
            }
        }
    }



Exit:
    if (FileHandle && (FileHandle != INVALID_HANDLE_VALUE))
    {
        CloseHandle(FileHandle);
    }

    if (FileBuffer)
    {

        HeapFree(GetProcessHeap(), 0, FileBuffer);
    }

    return(Error);
}


DWORD LoadOggFromFile(_In_ char* FileName, _Inout_ GAMESOUND* GameSound)
{
    DWORD Error = ERROR_SUCCESS;

    HANDLE FileHandle = INVALID_HANDLE_VALUE;

    LARGE_INTEGER FileSize = { 0 };

    DWORD BytesRead = 0;

    void* FileBuffer = NULL;

    int SamplesDecoded = 0;

    int Channels = 0;

    int SampleRate = 0;

    short* DecodedAudio = NULL;


    if ((FileHandle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] CreateFileA failed with 0x%08lx on %s!", __FUNCTION__, Error, FileName);
        goto Exit;
    }

    if (GetFileSizeEx(FileHandle, &FileSize) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] GetFileSizeEx failed with 0x%08lx on %s!", __FUNCTION__, Error, FileName);
        goto Exit;
    }

    LogMessageA(LL_INFO, "[%s] Size of file %s: %lu.", __FUNCTION__, FileName, FileSize.QuadPart);


    FileBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, FileSize.QuadPart);

    if (FileBuffer == NULL)
    {
        Error = ERROR_OUTOFMEMORY;
        LogMessageA(LL_ERROR, "[%s] HeapAllox failed with 0x%08lx on %s!", __FUNCTION__, Error, FileName);
        goto Exit;
    }

    if (ReadFile(FileHandle, FileBuffer, FileSize.QuadPart, &BytesRead, NULL) == 0)
    {
        Error = GetLastError();
        LogMessageA(LL_ERROR, "[%s] ReadFile failed with 0x%08lx on %s!", __FUNCTION__, Error, FileName);
        goto Exit;
    }

    SamplesDecoded = stb_vorbis_decode_memory(FileBuffer, FileSize.QuadPart, &Channels, &SampleRate, &DecodedAudio);

    if (SamplesDecoded < 1)
    {
        Error = ERROR_BAD_COMPRESSION_BUFFER;
        LogMessageA(LL_ERROR, "[%s] stb_vorbis_decode_memory failed with 0x%08lx on %s!", __FUNCTION__, Error, FileName);
        goto Exit;
    }

    GameSound->WaveFormat.wFormatTag = WAVE_FORMAT_PCM;

    GameSound->WaveFormat.nChannels = Channels;

    GameSound->WaveFormat.nSamplesPerSec = SampleRate;

    GameSound->WaveFormat.nAvgBytesPerSec = GameSound->WaveFormat.nSamplesPerSec * GameSound->WaveFormat.nChannels * 2;

    GameSound->WaveFormat.nBlockAlign = GameSound->WaveFormat.nChannels * 2;

    GameSound->WaveFormat.wBitsPerSample = 16;

    GameSound->Buffer.Flags = XAUDIO2_END_OF_STREAM;

    GameSound->Buffer.AudioBytes = SamplesDecoded * GameSound->WaveFormat.nChannels * 2;

    GameSound->Buffer.pAudioData = DecodedAudio;


Exit:

    if (FileBuffer)
    {
        HeapFree(GetProcessHeap(), 0, FileBuffer);
    }

    return(Error);
}

DWORD LoadAssetFromArchive(_In_ char* ArchiveName, _In_ char* AssetFileName, _In_ RESOURCE_TYPE ResourceType, _Inout_ void* Resource)
{
    DWORD Error = ERROR_SUCCESS;

    mz_zip_archive Archive = { 0 };

    BYTE* DecompressedBuffer = NULL;

    size_t DecompressedSize = 0;

    BOOL FileFoundInArchive = FALSE;

    if ((mz_zip_reader_init_file(&Archive, ArchiveName, 0)) == FALSE)       
    {                                                                       
        Error = mz_zip_get_last_error(&Archive);                           

        char* ErrorMessage = mz_zip_get_error_string(Error);

        LogMessageA(LL_ERROR, "[%s] mz_zip_reader_init_file failed with error 0x%08lx on archive file %s! Error: %s", __FUNCTION__, Error, ArchiveName, ErrorMessage);

        goto Exit;
    }

    //      iterate through each file until we find the correct one

    for (int FileIndex = 0; FileIndex < (int)mz_zip_reader_get_num_files(&Archive); FileIndex++)
    {
        mz_zip_archive_file_stat CompressedFileStatistics = { 0 };

        if (mz_zip_reader_file_stat(&Archive, FileIndex, &CompressedFileStatistics) == MZ_FALSE)
        {
            Error = mz_zip_get_last_error(&Archive);

            char* ErrorMessage = mz_zip_get_error_string(Error);

            LogMessageA(LL_ERROR, "[%s] mz_zip_reader_init_file failed with error 0x%08lx! Archive: %s File: %s Error: %s", __FUNCTION__, Error, ArchiveName, AssetFileName, ErrorMessage);

            goto Exit;

        }

        if (_stricmp(CompressedFileStatistics.m_filename, AssetFileName) == 0)      //changed FileIndex to AssetFileName and program works.
        {
            FileFoundInArchive = TRUE;

            if ((DecompressedBuffer = mz_zip_reader_extract_file_to_heap(&Archive, AssetFileName, &DecompressedSize, 0)) == NULL)   ////need both parentheticals around DecompressedBuffer otherwise it is NULL
            {
                Error = mz_zip_get_last_error(&Archive);

                char* ErrorMessage = mz_zip_get_error_string(Error);

                LogMessageA(LL_ERROR, "[%s] mz_zip_reader_extract_file_to_heap failed with error 0x%08lx! Archive: %s File: %s Error %s", __FUNCTION__, Error, ArchiveName, AssetFileName, ErrorMessage);

                goto Exit;
            }

            break;
        }
    }

    if (FileFoundInArchive == FALSE)
    {
        Error = ERROR_FILE_NOT_FOUND;

        LogMessageA(LL_ERROR, "[%s] File %s was not found in archive %s! 0x%08lx!", __FUNCTION__, AssetFileName, ArchiveName, Error);

        goto Exit;
    }

    switch (ResourceType)
    {
        case (RESOURCE_TYPE_WAV):
        {
            Error = LoadWaveFromMem(DecompressedBuffer, Resource);

            break;
        }
        case (RESOURCE_TYPE_OGG):
        {
            Error = LoadOggFromMem(DecompressedBuffer, (uint32_t)DecompressedSize, Resource);

            break;
        }
        case (RESOURCE_TYPE_TILEMAP):
        {
            Error = LoadTileMapFromMem(DecompressedBuffer, (uint32_t)DecompressedSize, Resource);

            break;
        }
        case (RESOURCE_TYPE_BMPX):
        {
            Error = Load32BppBitmapFromMem(DecompressedBuffer, Resource);

            break;
        }
        default:
        {
            ASSERT(FALSE, "Unknown resource type in LoadAssetFromArchive!")
        }
    }


Exit:

    mz_zip_reader_end(&Archive);

    return(Error);
}


DWORD AssetLoadingThreadProc(_In_ LPVOID lpParam)
{
    UNREFERENCED_PARAMETER(lpParam);

    DWORD Error = ERROR_SUCCESS;


    if ((Error = LoadAssetFromArchive(ASSET_FILE, "PixelFont(6x7).bmpx", RESOURCE_TYPE_BMPX, &g6x7Font)) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "LoadAssetFromArchive PixelFont(6x7).bmpx failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if ((Error = LoadAssetFromArchive(ASSET_FILE, "SplashNoise.wav", RESOURCE_TYPE_WAV, &gSoundSplashScreen)) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "LoadAssetFromArchive failed with SplashNoise.wav!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    SetEvent(gEssentialAssetsLoadedEvent);

    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Overworld01.bmpx", RESOURCE_TYPE_BMPX, &gOverWorld01.GameBitmap)) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Overworld01.bmpx failed!", __FUNCTION__);
        goto Exit;
    }

    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Overworld01.tmx", RESOURCE_TYPE_TILEMAP, &gOverWorld01.TileMap)) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Overworld01.tmx failed!", __FUNCTION__);
        goto Exit;
    }

    if ((Error = LoadAssetFromArchive(ASSET_FILE, "menu.wav", RESOURCE_TYPE_WAV, &gSoundMenuNavigate)) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive menu.wav failed!", __FUNCTION__);
        goto Exit;
    }

    if ((Error = LoadAssetFromArchive(ASSET_FILE, "item.wav", RESOURCE_TYPE_WAV, &gSoundMenuChoose)) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive item.wav failed!", __FUNCTION__);
        goto Exit;
    }

    if ((Error = LoadAssetFromArchive(ASSET_FILE, "NewAdventures.ogg", RESOURCE_TYPE_OGG, &gMusicOverWorld01)) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive NewAdventures.ogg failed!", __FUNCTION__);
        goto Exit;
    }

    ////////////////////////////////////////////////////////// loading sprites

    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingDown0.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_DOWN_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingDown0.bmpx failed!", __FUNCTION__);
        goto Exit;
    }
    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingDown1.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_DOWN_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingDown1.bmpx failed!", __FUNCTION__);
        goto Exit;
    }
    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingDown2.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_DOWN_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingDown2.bmpx failed!", __FUNCTION__);
        goto Exit;
    }

    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingLeft0.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_LEFT_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingLeft0.bmpx failed!", __FUNCTION__);
        goto Exit;
    }
    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingLeft1.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_LEFT_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingLeft1.bmpx failed!", __FUNCTION__);
        goto Exit;
    }
    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingLeft2.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_LEFT_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingLeft2.bmpx failed!", __FUNCTION__);
        goto Exit;
    }

    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingRight0.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_RIGHT_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingRight0.bmpx failed!", __FUNCTION__);
        goto Exit;
    }
    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingRight1.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_RIGHT_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingRight1.bmpx failed!", __FUNCTION__);
        goto Exit;
    }
    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingRight2.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_RIGHT_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingRight2.bmpx failed!", __FUNCTION__);
        goto Exit;
    }

    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingUp0.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_UP_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingUp0.bmpx failed!", __FUNCTION__);
        goto Exit;
    }
    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingUp1.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_UP_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingUp1.bmpx failed!", __FUNCTION__);
        goto Exit;
    }
    if ((Error = LoadAssetFromArchive(ASSET_FILE, "Suit0FacingUp2.bmpx", RESOURCE_TYPE_BMPX, &gPlayer.Sprite[SUIT_0][FACING_UP_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] LoadAssetFromArchive Suit0FacingUp2.bmpx failed!", __FUNCTION__);
        goto Exit;
    }


Exit:
    return(Error);
}