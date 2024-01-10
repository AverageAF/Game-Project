
#pragma warning(push, 3)
#include <stdio.h>
#include <windows.h>

#include <psapi.h>

#include <emmintrin.h>
#pragma warning(pop)


#include <xaudio2.h>
#include <stdint.h>
#include "Main.h"
#include "Menus.h"

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "XAudio2.lib")

HWND gGameWindow;

BOOL gGameIsRunning;                //when set to FALSE ends the game, controls main game loop in winmain

GAMEBITMAP gBackBuffer;

GAMEBITMAP g6x7Font;

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


GAME_PERFORMANCE_DATA gGamePerformanceData;

PLAYER gPlayer;

BOOL gWindowHasFocus;

REGISTRYPARAMS gRegistryParams;

GAMESTATE gCurrentGameState = GAMESTATE_OPENINGSPLASH;
GAMESTATE gPreviousGameState = GAMESTATE_TITLESCREEN;
GAMESTATE gDesiredGameState;

GAMEINPUT gGameInput;

IXAudio2* gXAudio;

IXAudio2MasteringVoice* gXAudioMasteringVoice;

IXAudio2SourceVoice* gXAudioMusicSourceVoice;

IXAudio2SourceVoice* gXAudioSFXSourceVoice[NUMBER_OF_SFX_SOURCE_VOICES];

uint8_t gSFXSourceVoiceSelector;

uint8_t gSFXVolume = 50;
uint8_t gMusicVolume = 50;
GAMESOUND gSoundMenuNavigate;
GAMESOUND gSoundMenuChoose;
GAMESOUND gSoundSplashScreen;

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

    if ((Load32BppBitmapFromFile("..\\Assets\\PixelFont(6x7).bmpx", &g6x7Font)) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (InitializeSoundEngine() != S_OK)
    {
        MessageBoxA(NULL, "InitializeSoundEngine failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (LoadWaveFromFile("..\\Assets\\menu.wav", &gSoundMenuNavigate) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "LoadWaveFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if (LoadWaveFromFile("..\\Assets\\item.wav", &gSoundMenuChoose) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "LoadWaveFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if (LoadWaveFromFile("..\\Assets\\SplashNoise.wav", &gSoundSplashScreen) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "LoadWaveFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
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

    gPlayer.ScreenPosX = 32;
    gPlayer.ScreenPosY = 32;
    gPlayer.CurrentSuit = SUIT_0;
    gPlayer.Direction = DOWN;
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingDown0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingDown0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingDown1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingDown1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingDown2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingDown2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingLeft0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingLeft0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingLeft1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingLeft1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingLeft2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingLeft2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingRight0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingRight0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingRight1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingRight1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingRight2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingRight2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingUp0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingUp0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingUp1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingUp1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingUp2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Loading Suit0FacingUp2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }


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

#ifdef SIMD
__forceinline void ClearScreenColor(_In_ __m128i* Color)
{
    for (int x = 0; x < (GAME_RES_WIDTH * GAME_RES_HEIGHT / 4 ); x++)      //paint the screen 4 pixels at a time
    {
        _mm_store_si128((__m128i*)gBackBuffer.Memory + x, *Color);
    }
}
#else
__forceinline void ClearScreenColor(_In_ PIXEL32* Pixel)
{
    for (int x = 0; x < GAME_RES_WIDTH * GAME_RES_HEIGHT; x ++)      //paint the screen 1 pixel at a time
    {
        memcpy((PIXEL32*)gBackBuffer.Memory + x, Pixel, sizeof(PIXEL32));
    }
}
#endif

void Blit32BppBitmapToBuffer(_In_ GAMEBITMAP* GameBitmap, _In_ uint16_t x, _In_ uint16_t y)
{
    int32_t StartingScreenPixel = ((GAME_RES_HEIGHT * GAME_RES_WIDTH) - GAME_RES_WIDTH) - (GAME_RES_WIDTH * y) + x;
    int32_t StartingBitmapPixel = ((GameBitmap->BitmapInfo.bmiHeader.biWidth * GameBitmap->BitmapInfo.bmiHeader.biHeight) - GameBitmap->BitmapInfo.bmiHeader.biWidth);
    int32_t MemoryOffset = 0;
    int32_t BitmapOffset = 0;
    PIXEL32 BitmapPixel = { 0 };
    //PIXEL32 BackgroundPixel = { 0 };

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
}

void DrawDebugInfo(void)
{
        char DebugTextBuffer[64] = { 0 };
        PIXEL32 White = { 0xFF, 0xFF, 0xFF, 0xFF };
        PIXEL32 LimeGreen = { 0x00, 0xFF, 0x17, 0xFF };
        PIXEL32 SkyBlue = { 0xFF, 0xAF, 0x00, 0xFF };

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
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Coords: (%d,%d) ", gPlayer.ScreenPosX, gPlayer.ScreenPosY);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &SkyBlue, 0, 56);
}


void MenuItem_TitleScreen_Resume(void)
{

}
void MenuItem_TitleScreen_StartNew(void)
{
    //if a save exists ask "are you sure you want to start a new game?"
    //otherwise just go to the character naming screen

    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_CHARACTERNAME;


}

void MenuItem_CharacterName_Add(void) 
{

}

void MenuItem_CharacterName_Okay(void)
{

}

void MenuItem_CharacterName_Back(void)
{
    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_TITLESCREEN;
    gMenu_CharacterName.SelectedItem = 53;
}

void MenuItem_TitleScreen_Options(void)
{
    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_OPTIONS;
}
void MenuItem_TitleScreen_Exit(void)
{
    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_EXITYESNO;
    gMenu_ExitYesNoScreen.SelectedItem = 0;
}


void MenuItem_ExitYesNo_Yes(void)
{
    SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
}
void MenuItem_ExitYesNo_No(void)
{
    gCurrentGameState = gPreviousGameState;
    gPreviousGameState = GAMESTATE_EXITYESNO;
    
}

void MenuItem_OptionsScreen_SfxVolume(void)
{
    if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        if (gSFXVolume < 10)
        {
            gSFXVolume = 100;
        }
        else
        {
            gSFXVolume -= 10;
        }
    }
    else
    {
        if (gSFXVolume >= 100)
        {
            gSFXVolume = 0;
        }
        else
        {
            gSFXVolume += 10;
        }
    }

    float SFXVolume = gSFXVolume / 100.0f;

    for (uint8_t Counter = 0; Counter < NUMBER_OF_SFX_SOURCE_VOICES; Counter++)
    {
        gXAudioSFXSourceVoice[Counter]->lpVtbl->SetVolume(gXAudioSFXSourceVoice[Counter], SFXVolume, XAUDIO2_COMMIT_NOW);
    }
}
void MenuItem_OptionsScreen_MusicVolume(void)
{

    if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        if (gMusicVolume < 10)
        {
            gMusicVolume = 100;
        }
        else
        {
            gMusicVolume -= 10;
        }
    }
    else
    {
        if (gMusicVolume >= 100)
        {
            gMusicVolume = 0;
        }
        else
        {
            gMusicVolume += 10;
        }
    }

    float MusicVolume = gMusicVolume / 100.0f;

    gXAudioMusicSourceVoice->lpVtbl->SetVolume(gXAudioMusicSourceVoice, MusicVolume, XAUDIO2_COMMIT_NOW);
}
void MenuItem_OptionsScreen_ScreenSize(void)
{
    if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        if (gGamePerformanceData.CurrentScaleFactor > 1)
        {
            gGamePerformanceData.CurrentScaleFactor--;
        }
        else
        {
            gGamePerformanceData.CurrentScaleFactor = gGamePerformanceData.MaxScaleFactor;
        }
    }
    else
    {
        if (gGamePerformanceData.CurrentScaleFactor < gGamePerformanceData.MaxScaleFactor)
        {
            gGamePerformanceData.CurrentScaleFactor++;
        }
        else
        {
            gGamePerformanceData.CurrentScaleFactor = 1;
        }
    }

    InvalidateRect(gGameWindow, NULL, TRUE);
}

void MenuItem_OptionsScreen_FullScreen(void)
{
    if (gRegistryParams.FullScreen == FALSE)
    {
        gRegistryParams.FullScreen++;
    }
    else
    {
        gRegistryParams.FullScreen = FALSE;
    }
}

void MenuItem_OptionsScreen_Back(void)
{
    gPreviousGameState = gCurrentGameState;
    gCurrentGameState = GAMESTATE_TITLESCREEN;

    if (SaveRegistryParameters() != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Save Registry Parameters failed from Options>Back!", __FUNCTION__);
    }
}

void DrawOpeningSplashScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;
    }

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    if (LocalFrameCounter >= 60)
    {
        if (LocalFrameCounter == 60)
        {
            TextColor.Red = 255;
            TextColor.Blue = 255;
            TextColor.Green = 255;
            PlayGameSound(&gSoundSplashScreen);
        }

        if ((LocalFrameCounter >= 120 && LocalFrameCounter < 150) && (LocalFrameCounter % 10 == 0))
        {
            TextColor.Red -= 63;
            TextColor.Blue -= 63;
            TextColor.Green -= 63;
        }

        if (LocalFrameCounter == 150)
        {
            TextColor.Red = 0;
            TextColor.Blue = 0;
            TextColor.Green = 0;
        }

        if (LocalFrameCounter > 160)
        {
            gPreviousGameState = gCurrentGameState;
            gCurrentGameState = GAMESTATE_TITLESCREEN;
        }

        BlitStringToBuffer("A game by me :)", &g6x7Font, &TextColor, (GAME_RES_WIDTH / 2) - (6 * 16 / 2), 112);

        BlitStringToBuffer("Matthew Warriner", &g6x7Font, &TextColor, (GAME_RES_WIDTH / 2) - (6 * 17 / 2), 128);
    }



    LocalFrameCounter++;

    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}
void DrawTitleScreen(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 Red = { 0x00, 0x00, 0xFF, 0xFF };

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;

        if (gPlayer.Active) 
        {
            gMenu_TitleScreen.SelectedItem = 0;
        }
        else
        {
            gMenu_TitleScreen.SelectedItem = 1;
        }
    }

    if (LocalFrameCounter <= 8)
    {
        TextColor.Red = 64;
        TextColor.Blue = 64;
        TextColor.Green = 64;
    }
    if (LocalFrameCounter == 16)
    {
        TextColor.Red = 128;
        TextColor.Blue = 128;
        TextColor.Green = 128;
    }
    if (LocalFrameCounter == 24)
    {
        TextColor.Red = 192;
        TextColor.Blue = 192;
        TextColor.Green = 192;
    }
    if (LocalFrameCounter == 32)
    {
        TextColor.Red = 255;
        TextColor.Blue = 255;
        TextColor.Green = 255;
    }

    //memset(gBackBuffer.Memory, 0, GAME_DRAWING_AREA_MEMORY_SIZE);
                            //    AARRGGBB
    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    BlitStringToBuffer(GAME_NAME, &g6x7Font, &Red, (GAME_RES_WIDTH / 2) - (strlen(GAME_NAME) * 6 / 2), 60);

    for (uint8_t MenuItem = 0; MenuItem < gMenu_TitleScreen.ItemCount; MenuItem++)
    {
        if (gMenu_TitleScreen.Items[MenuItem]->Enabled == TRUE)
        {
            BlitStringToBuffer(gMenu_TitleScreen.Items[MenuItem]->Name, &g6x7Font, &TextColor, gMenu_TitleScreen.Items[MenuItem]->x, gMenu_TitleScreen.Items[MenuItem]->y);
        }
    }
    BlitStringToBuffer("»", &g6x7Font, &TextColor, gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->x - 6, gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->y);

    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}


void DrawCharacterNaming(void)
{
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 Red = { 0x00, 0x00, 0xFF, 0xFF };

    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;
        gMenu_CharacterName.SelectedItem = 0;
        TextColor.Red = 0;
        TextColor.Blue = 0;
        TextColor.Green = 0;
    }

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    if (LocalFrameCounter <= 10)
    {
        TextColor.Red = 64;
        TextColor.Blue = 64;
        TextColor.Green = 64;
    }
    if (LocalFrameCounter == 20)
    {
        TextColor.Red = 128;
        TextColor.Blue = 128;
        TextColor.Green = 128;
    }
    if (LocalFrameCounter == 30)
    {
        TextColor.Red = 192;
        TextColor.Blue = 192;
        TextColor.Green = 192;
    }
    if (LocalFrameCounter == 40)
    {
        TextColor.Red = 255;
        TextColor.Blue = 255;
        TextColor.Green = 255;
    }

    BlitStringToBuffer(gMenu_CharacterName.Name, &g6x7Font, &TextColor, (GAME_RES_WIDTH / 2) - (strlen(gMenu_CharacterName.Name) * 6 / 2), 16);

    for (uint8_t Counter = 0; Counter < gMenu_CharacterName.ItemCount; Counter++)
    {
        BlitStringToBuffer(gMenu_CharacterName.Items[Counter]->Name, &g6x7Font, &TextColor, gMenu_CharacterName.Items[Counter]->x, gMenu_CharacterName.Items[Counter]->y);
    }

    BlitStringToBuffer("»", &g6x7Font, &TextColor, gMenu_CharacterName.Items[gMenu_CharacterName.SelectedItem]->x - 6, gMenu_CharacterName.Items[gMenu_CharacterName.SelectedItem]->y);

    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}

void DrawOverworldScreen(void)
{

}
void DrawBattleScreen(void)
{

}
void DrawOptionsScreen(void)
{
    
    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    static PIXEL32 Red = { 0x00, 0x00, 0xFF, 0xFF };
    static PIXEL32 Gray = { 0x50, 0x50, 0x50, 0x50 };
    static PIXEL32 TextColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    char ScreenSizeString[64] = { 0 };

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;

        gMenu_OptionsScreen.SelectedItem = 0;
    }

    if (LocalFrameCounter <= 8)
    {
        TextColor.Red = 64;
        TextColor.Blue = 64;
        TextColor.Green = 64;

        Gray.Red = 20;
        Gray.Blue = 20;
        Gray.Green = 20;
    }
    if (LocalFrameCounter == 16)
    {
        TextColor.Red = 128;
        TextColor.Blue = 128;
        TextColor.Green = 128;

        Gray.Red = 40;
        Gray.Blue = 40;
        Gray.Green = 40;
    }
    if (LocalFrameCounter == 24)
    {
        TextColor.Red = 192;
        TextColor.Blue = 192;
        TextColor.Green = 192;

        Gray.Red = 60;
        Gray.Blue = 60;
        Gray.Green = 60;
    }
    if (LocalFrameCounter == 32)
    {
        TextColor.Red = 255;
        TextColor.Blue = 255;
        TextColor.Green = 255;

        Gray.Red = 80;
        Gray.Blue = 80;
        Gray.Green = 80;
    }

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    for (uint8_t MenuItem = 0; MenuItem < gMenu_OptionsScreen.ItemCount; MenuItem++)
    {
        if (gMenu_OptionsScreen.Items[MenuItem]->Enabled == TRUE)
        {
            BlitStringToBuffer(gMenu_OptionsScreen.Items[MenuItem]->Name, &g6x7Font, &TextColor, gMenu_OptionsScreen.Items[MenuItem]->x, gMenu_OptionsScreen.Items[MenuItem]->y);
        }
    }

    for (uint8_t Volume = 0; Volume < 10; Volume++)
    {
        if (Volume >= (gSFXVolume / 10))
        {
            BlitStringToBuffer("\xf2", &g6x7Font, &Gray, 240 + (Volume * 6), gMI_OptionsScreen_SfxVolume.y);
        }
        else
        { 
            BlitStringToBuffer("\xf2", &g6x7Font, &TextColor, 240 + (Volume * 6), gMI_OptionsScreen_SfxVolume.y);
        }
    }

    for (uint8_t Volume = 0; Volume < 10; Volume++)
    {
        if (Volume >= (gMusicVolume / 10))
        {
            BlitStringToBuffer("\xf2", &g6x7Font, &Gray, 240 + (Volume * 6), gMI_OptionsScreen_MusicVolume.y);
        }
        else
        {
            BlitStringToBuffer("\xf2", &g6x7Font, &TextColor, 240 + (Volume * 6), gMI_OptionsScreen_MusicVolume.y);
        }
    }

    if (gRegistryParams.FullScreen == TRUE)
    {
        BlitStringToBuffer("x", &g6x7Font, &TextColor, 269, gMI_OptionsScreen_FullScreen.y);
    }

    snprintf(ScreenSizeString, sizeof(ScreenSizeString), "%dx%d", GAME_RES_WIDTH * gGamePerformanceData.CurrentScaleFactor, GAME_RES_HEIGHT * gGamePerformanceData.CurrentScaleFactor);

    BlitStringToBuffer(ScreenSizeString, &g6x7Font, &TextColor, 240, gMI_OptionsScreen_ScreenSize.y);

    BlitStringToBuffer("»", &g6x7Font, &TextColor, gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->x - 6, gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->y);

    LocalFrameCounter++;
    LastFrameSeen = gGamePerformanceData.TotalFramesRendered;
}
void DrawExitYesNoScreen(void)
{
    static PIXEL32 White = { 0xFF, 0xFF, 0xFF, 0xFF };
    static PIXEL32 Red = { 0x00, 0x00, 0xFF, 0xFF };
    static PIXEL32 LimeGreen = { 0x00, 0xFF, 0x17, 0xFF };

    static uint64_t LocalFrameCounter;

    static uint64_t LastFrameSeen;

    if (gGamePerformanceData.TotalFramesRendered > LastFrameSeen + 1)
    {
        LocalFrameCounter = 0;
    }

    __stosd(gBackBuffer.Memory, 0xFF000000, GAME_DRAWING_AREA_MEMORY_SIZE / sizeof(DWORD));

    BlitStringToBuffer(gMenu_ExitYesNoScreen.Name, &g6x7Font, &White, (GAME_RES_WIDTH / 2) - (strlen(gMenu_ExitYesNoScreen.Name) * 6 / 2), 60);                                                             //Quit the game?

    BlitStringToBuffer(gMenu_ExitYesNoScreen.Items[0]->Name, &g6x7Font, &LimeGreen, (GAME_RES_WIDTH / 2) - (strlen(gMenu_ExitYesNoScreen.Items[0]->Name) * 6 / 2), gMenu_ExitYesNoScreen.Items[0]->y);          //yes
        
    BlitStringToBuffer(gMenu_ExitYesNoScreen.Items[1]->Name, &g6x7Font, &Red, (GAME_RES_WIDTH / 2) - (strlen(gMenu_ExitYesNoScreen.Items[1]->Name) * 6 / 2), gMenu_ExitYesNoScreen.Items[1]->y);                //no

    BlitStringToBuffer("»", &g6x7Font, &White, gMenu_ExitYesNoScreen.Items[gMenu_ExitYesNoScreen.SelectedItem]->x - 6, gMenu_ExitYesNoScreen.Items[gMenu_ExitYesNoScreen.SelectedItem]->y);                     //Cursor
}




void PPI_OpeningSplashScreen(void)      //skip splash screen
{
    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed ||
        gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed ||
        gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed ||
        gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed ||
        gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed ||
        gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = GAMESTATE_TITLESCREEN;
    }
}
void PPI_TitleScreen(void)
{
    if (gGameInput.TabKeyPressed && !gGameInput.TabKeyAlreadyPressed)
    {
        gDesiredGameState = gPreviousGameState;
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = gDesiredGameState;
        PlayGameSound(&gSoundMenuChoose);
    }

    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if (gMenu_TitleScreen.SelectedItem == gMenu_TitleScreen.ItemCount - 1)
        {
            gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->Action();
            gMenu_ExitYesNoScreen.SelectedItem = 0;
            PlayGameSound(&gSoundMenuChoose);
        }
        else
        {
            gMenu_TitleScreen.SelectedItem = gMenu_TitleScreen.ItemCount - 1;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {

        if (gMenu_TitleScreen.SelectedItem < gMenu_TitleScreen.ItemCount - 1)
        {
            gMenu_TitleScreen.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gPlayer.Active)                                //allow navigation to "resume" when save file present
        {
            if (gMenu_TitleScreen.SelectedItem > 0)
            {
                gMenu_TitleScreen.SelectedItem--;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }
        else                                                //prevent navigation to "Resume" with no save file
        {
            if (gMenu_TitleScreen.SelectedItem > 1)
            {
                gMenu_TitleScreen.SelectedItem--;
                PlayGameSound(&gSoundMenuNavigate);
            }
        }
    }

    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        gMenu_TitleScreen.Items[gMenu_TitleScreen.SelectedItem]->Action();
        PlayGameSound(&gSoundMenuChoose);
    }
}
void PPI_OptionsScreen(void)
{
    if (gGameInput.TabKeyPressed && !gGameInput.TabKeyAlreadyPressed)
    {
        gDesiredGameState = gPreviousGameState;
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = gDesiredGameState;
        PlayGameSound(&gSoundMenuChoose);
        if (SaveRegistryParameters() != ERROR_SUCCESS)
        {
            LogMessageA(LL_ERROR, "[%s] Save Registry Parameters failed from Options>Tabbed!", __FUNCTION__);
        }
    }

    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if (gMenu_OptionsScreen.SelectedItem == gMenu_OptionsScreen.ItemCount - 1)
        {
            gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
        else
        {
            gMenu_OptionsScreen.SelectedItem = gMenu_OptionsScreen.ItemCount - 1;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {

        if (gMenu_OptionsScreen.SelectedItem < gMenu_OptionsScreen.ItemCount - 1)
        {
            gMenu_OptionsScreen.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gMenu_OptionsScreen.SelectedItem > 0)
        {
            gMenu_OptionsScreen.SelectedItem--;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gMenu_OptionsScreen.SelectedItem >= gMenu_OptionsScreen.ItemCount - 2)              ////dont allow for sideways navigation on the 'back' button and 'FullScreen'
    {
        if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
        {
            gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
    }
    else
    {
        if ((gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed) ||
            (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed) ||
            (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed))
        {
            gMenu_OptionsScreen.Items[gMenu_OptionsScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
    }
}

void PPI_CharacterName(void)
{
    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        gMenu_CharacterName.Items[gMenu_CharacterName.SelectedItem]->Action();
        PlayGameSound(&gSoundMenuChoose);
    }


    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    { 
        if (gMenu_CharacterName.SelectedItem > 25 && gMenu_CharacterName.SelectedItem < 52)
        {
            gMenu_CharacterName.SelectedItem -= 26;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem < 25 && gMenu_CharacterName.SelectedItem > 0)
        {
            gMenu_CharacterName.SelectedItem += 26;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 0)         //akward movement to the "back" and "ok" buttons
        {
            gMenu_CharacterName.SelectedItem = 53;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 25)
        {
            gMenu_CharacterName.SelectedItem = 52;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 53)
        {
            gMenu_CharacterName.SelectedItem = 26;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 52)
        {
            gMenu_CharacterName.SelectedItem = 51;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {
        if (gMenu_CharacterName.SelectedItem < 26)
        {
            gMenu_CharacterName.SelectedItem += 26;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem > 27 && gMenu_CharacterName.SelectedItem < 51)
        {
            gMenu_CharacterName.SelectedItem -= 26;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 26)            //akward movement to the "back" and "ok" buttons
        {
            gMenu_CharacterName.SelectedItem = 53;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 51)
        {
            gMenu_CharacterName.SelectedItem = 52;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 53)
        {
            gMenu_CharacterName.SelectedItem = 0;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 52)
        {
            gMenu_CharacterName.SelectedItem = 25;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.ALeftKeyPressed && !gGameInput.ALeftKeyAlreadyPressed)
    {
        if (gMenu_CharacterName.SelectedItem == 0 || gMenu_CharacterName.SelectedItem == 26)
        {
            gMenu_CharacterName.SelectedItem += 25;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 52 )
        {
            gMenu_CharacterName.SelectedItem = 53;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 53)
        {
            gMenu_CharacterName.SelectedItem = 52;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_CharacterName.SelectedItem--;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.DRightKeyPressed && !gGameInput.DRightKeyAlreadyPressed)
    {
        if (gMenu_CharacterName.SelectedItem == 25 || gMenu_CharacterName.SelectedItem == 51)
        {
            gMenu_CharacterName.SelectedItem -= 25;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 52)
        {
            gMenu_CharacterName.SelectedItem = 53;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else if (gMenu_CharacterName.SelectedItem == 53)
        {
            gMenu_CharacterName.SelectedItem = 52;
            PlayGameSound(&gSoundMenuNavigate);
        }
        else
        {
            gMenu_CharacterName.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.TabKeyPressed && !gGameInput.TabKeyAlreadyPressed)
    {
        gDesiredGameState = gPreviousGameState;
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = gDesiredGameState;
        PlayGameSound(&gSoundMenuChoose);
    }

    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if (gMenu_CharacterName.SelectedItem == gMenu_CharacterName.ItemCount - 1)
        {
            gMenu_CharacterName.Items[gMenu_CharacterName.SelectedItem]->Action();
            gMenu_CharacterName.SelectedItem = 53;
            PlayGameSound(&gSoundMenuChoose);
        }
        else
        {
            gMenu_CharacterName.SelectedItem = gMenu_CharacterName.ItemCount - 1;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
}

void PPI_Overworld(void)            
{

    if (gGameInput.EscapeKeyPressed)
    {
        //SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
    }

    if (!gPlayer.MovementRemaining)
    {
        if (gGameInput.SDownKeyPressed)
        {
            if (gPlayer.ScreenPosY < GAME_RES_HEIGHT - 16)
            {
                gPlayer.Direction = DOWN;
                gPlayer.MovementRemaining = 16;
            }
        }
        else if (gGameInput.ALeftKeyPressed)
        {
            if (gPlayer.ScreenPosX > 0)
            {
                gPlayer.Direction = LEFT;
                gPlayer.MovementRemaining = 16;
            }
        }
        else if (gGameInput.DRightKeyPressed)
        {
            if (gPlayer.ScreenPosX < GAME_RES_WIDTH - 16)
            {
                gPlayer.Direction = RIGHT;
                gPlayer.MovementRemaining = 16;
            }
        }
        else if (gGameInput.WUpKeyPressed)
        {
            if (gPlayer.ScreenPosY > 0)
            {
                gPlayer.Direction = UP;
                gPlayer.MovementRemaining = 16;
            }
        }
    }
    else
    {
        gPlayer.MovementRemaining--;

        if (gPlayer.Direction == DOWN)
        {
            gPlayer.ScreenPosY++;
        }
        else if (gPlayer.Direction == LEFT)
        {
            gPlayer.ScreenPosX--;
        }
        else if (gPlayer.Direction == RIGHT)
        {
            gPlayer.ScreenPosX++;
        }
        else if (gPlayer.Direction == UP)
        {
            gPlayer.ScreenPosY--;
        }

        switch (gPlayer.MovementRemaining)
        {
            case 16:
            {
                gPlayer.SpriteIndex = 1;
                break;
            }
            case 12:
            {
                gPlayer.SpriteIndex = 1;
                break;
            }
            case 8:
            {
                gPlayer.SpriteIndex = 2;
                break;
            }
            case 4:
            {
                gPlayer.SpriteIndex = 2;
                break;
            }
            case 0:
            {
                gPlayer.SpriteIndex = 0;
                break;
            }
            default:
            {

            }
        }
    }
}                                                   /////////////////////////////////////////////////character movement
void PPI_ExitYesNoScreen(void)
{
    if (gGameInput.TabKeyPressed && !gGameInput.TabKeyAlreadyPressed)
    {
        gDesiredGameState = gPreviousGameState;
        gPreviousGameState = gCurrentGameState;
        gCurrentGameState = gDesiredGameState;
        PlayGameSound(&gSoundMenuChoose);
    }

    if (gGameInput.EscapeKeyPressed && !gGameInput.EscapeKeyAlreadyPressed)
    {
        if (gMenu_ExitYesNoScreen.SelectedItem == gMenu_ExitYesNoScreen.ItemCount - 1)
        {
            gMenu_ExitYesNoScreen.Items[gMenu_ExitYesNoScreen.SelectedItem]->Action();
            PlayGameSound(&gSoundMenuChoose);
        }
        else
        {
            gMenu_ExitYesNoScreen.SelectedItem = gMenu_ExitYesNoScreen.ItemCount - 1;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.SDownKeyPressed && !gGameInput.SDownKeyAlreadyPressed)
    {

        if (gMenu_ExitYesNoScreen.SelectedItem < gMenu_ExitYesNoScreen.ItemCount - 1)
        {
            gMenu_ExitYesNoScreen.SelectedItem++;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }
    if (gGameInput.WUpKeyPressed && !gGameInput.WUpKeyAlreadyPressed)
    {
        if (gMenu_ExitYesNoScreen.SelectedItem > 0)
        {
            gMenu_ExitYesNoScreen.SelectedItem--;
            PlayGameSound(&gSoundMenuNavigate);
        }
    }

    if (gGameInput.ChooseKeyPressed && !gGameInput.ChooseKeyAlreadyPressed)
    {
        gMenu_ExitYesNoScreen.Items[gMenu_ExitYesNoScreen.SelectedItem]->Action();
        PlayGameSound(&gSoundMenuChoose);
    }
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