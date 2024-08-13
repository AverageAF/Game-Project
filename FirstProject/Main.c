
#pragma warning(push, 3)
#include <stdio.h>
//#include <stdarg.h>
#include <windows.h>
#include <psapi.h>
#include <emmintrin.h>
#include <xaudio2.h>
#include <stdint.h>
#include "stb_vorbis.h"
#include <string.h>
#pragma warning(pop)


#include "Main.h"
#include "CharacterName.h"
#include "ExitYesNoScreen.h"
#include "OpeningSplashScreen.h"
#include "OptionsScreen.h"
#include "TitleScreen.h"
#include "OverWorld.h"
#include "miniz.h"
#include "BattleScreen.h"
#include "LoadGameSave.h"
#include "SaveGameSave.h"
#include "SimpleConstants.h"
#include "Inventory.h"
#include "InventoryItems.h"
#include "MonsterData.h"
#include "MonsterStatsScreen.h"
#include "MonsterStorage.h"
#include "StoreScreen.h"
#include "DriveStorageMenu.h"

#include "variables.h"
#include "flags.h"


#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "XAudio2.lib")


CRITICAL_SECTION gLogCritSec;                 ////used to sync logging to logfile between muli-threads

// post-build commandline was $(SolutionDir)CopyAssets.bat


//lookup table for fade in/out animations. Maps frame count to brightness adjustment

const int16_t gFadeBrightnessGradient[] = {
    -255, -255, -255, -255, -255, //-255, -255, -255, -255, -255,
    -128, -128, -128, -128, -128, //-128, -128, -128, -128,-128,
    -64, -64, -64, -64, -64, //-64, -64, -64, -64, -64,
    -32, -32, -32, -32, -32 //-32, -32, -32, -32, -32
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

    InitializeGlobals();

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

    if (InitializeSprites() != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to initialize NPC sprites!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to Initialize NPC sprites!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (InitializePlayer() != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to initialize player sprite!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to Initialize Player Sprite!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (InitializeTriggers() != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to initialize Script Triggers!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to Initialize Script Triggers!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

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

    gGameInput.EscapeKeyPressed = GetAsyncKeyState(VK_ESCAPE);
    gGameInput.DebugKeyPressed = GetAsyncKeyState(VK_F1);                                   //F1 default debug key
    gGameInput.TabKeyPressed = GetAsyncKeyState(VK_TAB);
    gGameInput.SDownKeyPressed = GetAsyncKeyState('S') | GetAsyncKeyState(VK_DOWN);
    gGameInput.ALeftKeyPressed = GetAsyncKeyState('A') | GetAsyncKeyState(VK_LEFT);       // WASD and ArrowKey movement
    gGameInput.DRightKeyPressed = GetAsyncKeyState('D') | GetAsyncKeyState(VK_RIGHT);
    gGameInput.WUpKeyPressed = GetAsyncKeyState('W') | GetAsyncKeyState(VK_UP);
    gGameInput.ChooseKeyPressed = GetAsyncKeyState('E') | GetAsyncKeyState(VK_RETURN);
    gGameInput.CtrlKeyPressed = GetAsyncKeyState(VK_CONTROL);
    gGameInput.QKeyPressed = GetAsyncKeyState('Q');

    if (gGameInput.DebugKeyPressed && !gGameInput.DebugKeyAlreadyPressed)
    {
        gGamePerformanceData.DisplayDebugInfo = !gGamePerformanceData.DisplayDebugInfo;
    }
    

    if ((gInputEnabled == FALSE) || (gWindowHasFocus == FALSE))
    {
        goto InputDisabled;
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
        case GAMESTATE_BATTLE_MONSTER:
        {
            PPI_BattleScreen();
            break; 
        }
        case GAMESTATE_BATTLE_TRAINER:
        {
            PPI_BattleScreen();
            break;
        }
        case GAMESTATE_LOADGAMESAVE:
        {
            PPI_LoadGameSave();
            break;
        }
        case GAMESTATE_SAVEGAMESAVE:
        {
            PPI_SaveGameSave();
            break;
        }
        case GAMESTATE_MONSTERSTATS:
        {
            PPI_MonsterStatsScreen();
            break;
        }
        case GAMESTATE_DELETESAVEYESNO:
        {
            break;
        }
        case GAMESTATE_INVENTORYSCREEN:
        {
            PPI_InventoryScreen();
            break;
        }
        case GAMESTATE_STORE:
        {
            PPI_StoreScreen();
            break;
        }
        case GAMESTATE_DRIVE_STORAGE:
        {
            PPI_StorageMenu();
            break;
        }
        default:
        {
            ASSERT(FALSE, "Unknown GameState for player input!")
            break;
        }
    }

InputDisabled:

    gGameInput.EscapeKeyAlreadyPressed = gGameInput.EscapeKeyPressed;
    gGameInput.DebugKeyAlreadyPressed = gGameInput.DebugKeyPressed;
    gGameInput.ALeftKeyAlreadyPressed = gGameInput.ALeftKeyPressed;
    gGameInput.DRightKeyAlreadyPressed = gGameInput.DRightKeyPressed;
    gGameInput.WUpKeyAlreadyPressed = gGameInput.WUpKeyPressed;
    gGameInput.SDownKeyAlreadyPressed = gGameInput.SDownKeyPressed;
    gGameInput.ChooseKeyAlreadyPressed = gGameInput.ChooseKeyPressed;
    gGameInput.TabKeyAlreadyPressed = gGameInput.TabKeyPressed;
    gGameInput.CtrlKeyAlreadyPressed = gGameInput.CtrlKeyPressed;
    gGameInput.QKeyAlreadyPressed = gGameInput.QKeyPressed;

}

DWORD InitializeSprites(void)
{
    /////////////////////////////////////////temporary init of character sprites/////////////////////

    gCharacterSprite[0].WorldPos.x = 352;
    gCharacterSprite[0].WorldPos.y = 4544;
    gCharacterSprite[0].ResetWorldPos.x = 352;
    gCharacterSprite[0].ResetWorldPos.y = 4544;
    gCharacterSprite[0].ResetOriginWorldPos.x = 352;
    gCharacterSprite[0].ResetOriginWorldPos.y = 4544;
    gCharacterSprite[0].Direction = LEFT;
    gCharacterSprite[0].ResetDirection = LEFT;
    gCharacterSprite[0].Event = EVENT_FLAG_MONSTER;
    gCharacterSprite[0].EventMonsterIndex = MONSTER_WOLF;
    gCharacterSprite[0].EventMonsterLevel = 6;
    gCharacterSprite[0].Movement = MOVEMENT_WALK_UP_DOWN;
    gCharacterSprite[0].MovementRange.y = 3;
    gCharacterSprite[0].MovementRange.x = 5;
    gCharacterSprite[0].Visible = FALSE;
    gCharacterSprite[0].Exists = TRUE;
    gCharacterSprite[0].Loaded = FALSE;
    gCharacterSprite[0].GameAreaIndex = 2;
    gCharacterSprite[0].Dialogue[DIALOGUE_FLAG_0] = "Here have a Monster!\nI don't really need one";
    gCharacterSprite[0].DialogueFlag = DIALOGUE_FLAG_0;
    gCharacterSprite[0].DialoguesBeforeLoop = DIALOGUE_FLAG_0;
    gCharacterSprite[0].DialogueLoopReturn = DIALOGUE_FLAG_0;

    ///////////////////////for right now only numeric sprites, TODO #define sprite names to be used in gCharacterSprite[name]

    gCharacterSprite[1].WorldPos.x = 4032;
    gCharacterSprite[1].WorldPos.y = 656;
    gCharacterSprite[1].ResetWorldPos.x = 4032;
    gCharacterSprite[1].ResetWorldPos.y = 656;
    gCharacterSprite[1].ResetOriginWorldPos.x = 4032;
    gCharacterSprite[1].ResetOriginWorldPos.y = 656;
    gCharacterSprite[1].Direction = DOWN;
    gCharacterSprite[1].ResetDirection = DOWN;
    gCharacterSprite[1].Movement = MOVEMENT_STILL;
    gCharacterSprite[1].Visible = FALSE;
    gCharacterSprite[1].Exists = TRUE;
    gCharacterSprite[1].Loaded = FALSE;
    gCharacterSprite[1].GameAreaIndex = 9;
    gCharacterSprite[1].Event = EVENT_FLAG_HEAL;
    gCharacterSprite[1].Dialogue[DIALOGUE_FLAG_0] = "You decided to rest for a while.";
    gCharacterSprite[1].Dialogue[DIALOGUE_FLAG_1] = "...";
    gCharacterSprite[1].Dialogue[DIALOGUE_FLAG_2] = "...";
    gCharacterSprite[1].Dialogue[DIALOGUE_FLAG_3] = "...";
    gCharacterSprite[1].Dialogue[DIALOGUE_FLAG_4] = "...";
    gCharacterSprite[1].Dialogue[DIALOGUE_FLAG_5] = "...";
    gCharacterSprite[1].Dialogue[DIALOGUE_FLAG_6] = "...";
    gCharacterSprite[1].Dialogue[DIALOGUE_FLAG_7] = "After sleeping a while \nyour party feels better!";
    gCharacterSprite[1].DialogueFlag = DIALOGUE_FLAG_0;
    gCharacterSprite[1].DialoguesBeforeLoop = DIALOGUE_FLAG_7;
    gCharacterSprite[1].DialogueLoopReturn = DIALOGUE_FLAG_0;

    //TODO: make this a function EmptyCharSprite();

    /*for (uint8_t spritevalue = 0; spritevalue < NUM_SPRITES_PALLET; spritevalue++)
    {
        gCharacterSprite[1].Sprite[spritevalue] = gEmptySprite;
    }*/


    //////////////////////////////////////////////////////////////

    gCharacterSprite[2].WorldPos.x = 1136;
    gCharacterSprite[2].WorldPos.y = 4496;
    gCharacterSprite[2].ResetWorldPos.x = 1136;
    gCharacterSprite[2].ResetWorldPos.y = 4496;
    gCharacterSprite[2].ResetOriginWorldPos.x = 1136;
    gCharacterSprite[2].ResetOriginWorldPos.y = 4496;
    gCharacterSprite[2].Direction = DOWN;
    gCharacterSprite[2].ResetDirection = DOWN;
    gCharacterSprite[2].SightRange = 3;
    gCharacterSprite[2].ResetSightRange = 3;
    gCharacterSprite[2].Event = EVENT_FLAG_BATLLE;
    gCharacterSprite[2].Movement = MOVEMENT_STILL;
    gCharacterSprite[2].Visible = FALSE;
    gCharacterSprite[2].Exists = TRUE;
    gCharacterSprite[2].Loaded = FALSE;
    gCharacterSprite[2].GameAreaIndex = 3;
    gCharacterSprite[2].Dialogue[DIALOGUE_FLAG_0] = "Lets have a battle!";
    gCharacterSprite[2].Dialogue[DIALOGUE_FLAG_1] = "Wow! You're strong!";
    gCharacterSprite[2].DialogueFlag = DIALOGUE_FLAG_0;
    gCharacterSprite[2].DialoguesBeforeLoop = DIALOGUE_FLAG_0;
    gCharacterSprite[2].DialogueLoopReturn = DIALOGUE_FLAG_1;
    gCharacterSprite[2].BattleAiFlag = FLAG_NPCAI_SWITCHDEFENSIVE;

    sprintf_s(gCharacterSprite[2].Name, sizeof(gCharacterSprite[2].Name), "Jerry");

    gCharacterSprite[2].MonsterParty[0] = GenerateMonsterForCharacterSpriteBattle(MONSTER_WOLF, 6, 0);
    gCharacterSprite[2].MonsterParty[1] = GenerateMonsterForCharacterSpriteBattle(MONSTER_AIRWOLF, 7, 0);
    gCharacterSprite[2].MonsterParty[2] = GenerateMonsterForCharacterSpriteBattle(MONSTER_EARTHWOLF, 7, 0);
    gCharacterSprite[2].MonsterParty[3] = GenerateMonsterForCharacterSpriteBattle(MONSTER_WATERWOLF, 8, 0);

    //////////////////////////////////////////////////////////////

    gCharacterSprite[3].WorldPos.x = 528;
    gCharacterSprite[3].WorldPos.y = 4624;
    gCharacterSprite[3].ResetWorldPos.x = 528;
    gCharacterSprite[3].ResetWorldPos.y = 4624;
    gCharacterSprite[3].ResetOriginWorldPos.x = 528;
    gCharacterSprite[3].ResetOriginWorldPos.y = 4624;
    gCharacterSprite[3].Direction = DOWN;
    gCharacterSprite[3].ResetDirection = DOWN;
    gCharacterSprite[3].Event = EVENT_FLAG_HEAL;
    gCharacterSprite[3].Movement = MOVEMENT_STILL;
    gCharacterSprite[3].Visible = FALSE;
    gCharacterSprite[3].Exists = FALSE;
    gCharacterSprite[3].Loaded = FALSE;
    gCharacterSprite[3].GameAreaIndex = 2;
    gCharacterSprite[3].Dialogue[DIALOGUE_FLAG_0] = "Let me heal your monsters!";
    gCharacterSprite[3].Dialogue[DIALOGUE_FLAG_1] = "...";
    gCharacterSprite[3].Dialogue[DIALOGUE_FLAG_2] = "...";
    gCharacterSprite[3].Dialogue[DIALOGUE_FLAG_3] = "...";
    gCharacterSprite[3].Dialogue[DIALOGUE_FLAG_4] = "There! They seem much better!";
    gCharacterSprite[3].DialogueFlag = DIALOGUE_FLAG_0;
    gCharacterSprite[3].DialoguesBeforeLoop = DIALOGUE_FLAG_4;
    gCharacterSprite[3].DialogueLoopReturn = DIALOGUE_FLAG_0;

    //////////////////////////////////////////////////////////////

    gCharacterSprite[4].WorldPos.x = 288;
    gCharacterSprite[4].WorldPos.y = 4480;
    gCharacterSprite[4].ResetWorldPos.x = 288;
    gCharacterSprite[4].ResetWorldPos.y = 4480;
    gCharacterSprite[4].ResetOriginWorldPos.x = 288;
    gCharacterSprite[4].ResetOriginWorldPos.y = 4480;
    gCharacterSprite[4].Direction = UP;
    gCharacterSprite[4].ResetDirection = UP;
    gCharacterSprite[4].Event = EVENT_FLAG_DRIVE_STORAGE;
    gCharacterSprite[4].Movement = MOVEMENT_STILL;
    gCharacterSprite[4].Visible = FALSE;
    gCharacterSprite[4].Exists = TRUE;
    gCharacterSprite[4].Loaded = FALSE;
    gCharacterSprite[4].GameAreaIndex = 2;
    gCharacterSprite[4].Dialogue[DIALOGUE_FLAG_0] = "You opened up the\nMonster Drive Storage System!";
    gCharacterSprite[4].DialogueFlag = DIALOGUE_FLAG_0;
    gCharacterSprite[4].DialoguesBeforeLoop = DIALOGUE_FLAG_0;
    gCharacterSprite[4].DialogueLoopReturn = DIALOGUE_FLAG_0;

    /*gCharacterSprite[4].Dialogue[DIALOGUE_FLAG_0] = "Do you want to buy something?";
    gCharacterSprite[4].Dialogue[DIALOGUE_FLAG_1] = "I have all sorts of wares!";
    gCharacterSprite[4].Dialogue[DIALOGUE_FLAG_2] = "Buy something!";
    gCharacterSprite[4].Dialogue[DIALOGUE_FLAG_9] = "Come again!";*/

    //gStoreType[4] = 4;
    //sprintf_s(gCharacterSprite[4].Name, sizeof(gCharacterSprite[4].Name), "Items");

    //////////////////////////////////////////////////////////////

    gCharacterSprite[5].WorldPos.x = 352 + 64;
    gCharacterSprite[5].WorldPos.y = 4544 + 64;
    gCharacterSprite[5].ResetWorldPos.x = 352 + 64;
    gCharacterSprite[5].ResetWorldPos.y = 4544 + 64;
    gCharacterSprite[5].ResetOriginWorldPos.x = 352 + 64;
    gCharacterSprite[5].ResetOriginWorldPos.y = 4544 + 64;
    gCharacterSprite[5].Direction = DOWN;
    gCharacterSprite[5].ResetDirection = DOWN;
    gCharacterSprite[5].Event = EVENT_FLAG_USEITEM_NOSPRITE;
    gCharacterSprite[5].Movement = MOVEMENT_SPARKLE;
    gCharacterSprite[5].Visible = FALSE;
    gCharacterSprite[5].Exists = TRUE;
    gCharacterSprite[5].Loaded = FALSE;
    gCharacterSprite[5].GameAreaIndex = 2;
    gCharacterSprite[5].Dialogue[DIALOGUE_FLAG_0] = "You found a hidden item!";
    gCharacterSprite[5].Dialogue[DIALOGUE_FLAG_1] = "You put it away \ninto your backpack.";
    gCharacterSprite[5].DialogueFlag = DIALOGUE_FLAG_0;
    gCharacterSprite[5].DialoguesBeforeLoop = DIALOGUE_FLAG_1;
    gCharacterSprite[5].DialogueLoopReturn = DIALOGUE_FLAG_0;
    gCharacterSprite[5].EventItemsIndex[0] = INV_USABLE_ITEM_0;
    gCharacterSprite[5].EventItemsCount[0] = 5;

    //////////////////////////////////////////////////////////////

    gCharacterSprite[6].WorldPos.x = 608;
    gCharacterSprite[6].WorldPos.y = 4464;
    gCharacterSprite[6].ResetWorldPos.x = 608;
    gCharacterSprite[6].ResetWorldPos.y = 4464;
    gCharacterSprite[6].ResetOriginWorldPos.x = 608;
    gCharacterSprite[6].ResetOriginWorldPos.y = 4464;
    gCharacterSprite[6].Direction = DOWN;
    gCharacterSprite[6].ResetDirection = DOWN;
    gCharacterSprite[6].Event = EVENT_FLAG_USEITEM_NOSPRITE;
    gCharacterSprite[6].Movement = MOVEMENT_ITEMPICKUP;
    gCharacterSprite[6].Visible = FALSE;
    gCharacterSprite[6].Exists = TRUE;
    gCharacterSprite[6].Loaded = FALSE;
    gCharacterSprite[6].GameAreaIndex = 3;
    gCharacterSprite[6].Dialogue[DIALOGUE_FLAG_0] = "You found an item!";
    gCharacterSprite[6].Dialogue[DIALOGUE_FLAG_1] = "You put it away \ninto your backpack.";
    gCharacterSprite[6].DialogueFlag = DIALOGUE_FLAG_0;
    gCharacterSprite[6].DialoguesBeforeLoop = DIALOGUE_FLAG_1;
    gCharacterSprite[6].DialogueLoopReturn = DIALOGUE_FLAG_0;
    gCharacterSprite[6].EventItemsIndex[0] = INV_USABLE_ITEM_0;
    gCharacterSprite[6].EventItemsCount[0] = 5;

    //////////////////////////////////////////////////////////////

    gCharacterSprite[7].WorldPos.x = 896;
    gCharacterSprite[7].WorldPos.y = 4464;
    gCharacterSprite[7].ResetWorldPos.x = 896;
    gCharacterSprite[7].ResetWorldPos.y = 4464;
    gCharacterSprite[7].ResetOriginWorldPos.x = 896;
    gCharacterSprite[7].ResetOriginWorldPos.y = 4464;
    gCharacterSprite[7].Direction = DOWN;
    gCharacterSprite[7].ResetDirection = DOWN;
    gCharacterSprite[7].SightRange = 3;
    gCharacterSprite[7].ResetSightRange = 3;
    gCharacterSprite[7].Event = EVENT_FLAG_BATLLE;
    gCharacterSprite[7].Movement = MOVEMENT_STILL;
    gCharacterSprite[7].Visible = FALSE;
    gCharacterSprite[7].Exists = TRUE;
    gCharacterSprite[7].Loaded = FALSE;
    gCharacterSprite[7].GameAreaIndex = 3;
    gCharacterSprite[7].Dialogue[DIALOGUE_FLAG_0] = "Are you going into the forest?";
    gCharacterSprite[7].Dialogue[DIALOGUE_FLAG_1] = "Battle me before you go in!";
    gCharacterSprite[7].Dialogue[DIALOGUE_FLAG_2] = "Maybe I should train more...";
    gCharacterSprite[7].DialogueFlag = DIALOGUE_FLAG_0;
    gCharacterSprite[7].DialoguesBeforeLoop = DIALOGUE_FLAG_1;
    gCharacterSprite[7].DialogueLoopReturn = DIALOGUE_FLAG_2;
    gCharacterSprite[7].BattleAiFlag = FLAG_NPCAI_SWITCHOFFENSIVE;

    sprintf_s(gCharacterSprite[7].Name, sizeof(gCharacterSprite[7].Name), "Jimmy");

    gCharacterSprite[7].MonsterParty[0] = GenerateMonsterForCharacterSpriteBattle(MONSTER_DEATHWOLF, 5, 0);
    gCharacterSprite[7].MonsterParty[1] = GenerateMonsterForCharacterSpriteBattle(MONSTER_LIFEWOLF, 6, 0);
    gCharacterSprite[7].MonsterParty[2] = GenerateMonsterForCharacterSpriteBattle(MONSTER_WATERWOLF, 7, 0);
    gCharacterSprite[7].MonsterParty[3] = GenerateMonsterForCharacterSpriteBattle(MONSTER_FIREWOLF, 8, 0);

    //////////////////////////////////////////////////////////////

    //trigger tile
    //TODO: make trigger tiles instead of using invisible NPC's as trigger tiles

    /*gCharacterSprite[8].WorldPos.x = 352;
    gCharacterSprite[8].WorldPos.y = 4544;
    gCharacterSprite[8].ResetWorldPos.x = 352;
    gCharacterSprite[8].ResetWorldPos.y = 4544;
    gCharacterSprite[8].ResetOriginWorldPos.x = 352;
    gCharacterSprite[8].ResetOriginWorldPos.y = 4544;
    gCharacterSprite[8].Direction = DOWN;
    gCharacterSprite[8].ResetDirection = DOWN;
    gCharacterSprite[8].Event = EVENT_FLAG_TRIGGER_ONCE;
    gCharacterSprite[8].Movement = MOVEMENT_TRIGGER;
    gCharacterSprite[8].Visible = FALSE;
    gCharacterSprite[8].Exists = TRUE;
    gCharacterSprite[8].Loaded = FALSE;
    gCharacterSprite[8].GameAreaIndex = 2;*/

    //////////////////////////////////////////////////////////////

    return (0);
}

DWORD InitializeTriggers(void)
{
    gTriggerTiles[0].WorldPos.x = 352;
    gTriggerTiles[0].WorldPos.y = 4544;
    gTriggerTiles[0].Flag = TRIGGER_FLAG_ONCE;
    gTriggerTiles[0].Exists = TRUE;
    gTriggerTiles[0].Loaded = FALSE;
    gTriggerTiles[0].Interactive = FALSE;
    gTriggerTiles[0].GameAreaIndex = 2;

    /////////////////////////////////////////////////



    return(0);
}

DWORD InitializePlayer(void)
{
    gPlayer.ScreenPos.x = 208;      //368 positions for top right corner            //192 for top left corner                   x192 y69
    gPlayer.ScreenPos.y = 128;       //0                                             //32
    gPlayer.WorldPos.x = 4064;       //3824                                          //192                                      x4048 y352 new spawn
    gPlayer.WorldPos.y = 640;        //0                                             //32
    gCamera.x = 3856;                  //3456                                          //0                                         x3856 y256 camera pos new spawn
    gCamera.y = 512;                  //0                                             //0
    gPlayer.CurrentSuit = SUIT_0;
    gPlayer.Direction = DOWN;
    gPlayer.RandomEncounterPercent = 200;   //50 == a 5% chance

    gPlayer.Seed = 777;

    //////leave this as a char[] so letters can be reassigned
    gPlayer.Name[0] = 'M';
    gPlayer.Name[1] = 'a';
    gPlayer.Name[2] = 'p';
    gPlayer.Name[3] = 'l';
    gPlayer.Name[4] = 'e';

    gSellingItems = FALSE;


    return(0);
}

void BlitStringToBuffer(_In_ char* String, _In_ GAMEBITMAP* FontSheet, _In_ PIXEL32* Color, _In_ uint16_t x, _In_ uint16_t y)
{

    // Map any char value to an offset dictated by the g6x7Font ordering.
    static int FontCharacterPixelOffset[] = {
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


    uint16_t CharWidth = (uint16_t)FontSheet->BitmapInfo.bmiHeader.biWidth / FONT_SHEET_CHARACTERS_PER_ROW;
    uint16_t CharHeight = (uint16_t)FontSheet->BitmapInfo.bmiHeader.biHeight;      // only one row, otherwise: biHeight / FONT_SHEET_CHARACHTERS_PER_COLUMN
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

        StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * FontCharacterPixelOffset[(uint8_t)String[Character]]);

        for (int YPixel = 0; YPixel < CharHeight; YPixel++)
        {
            for (int XPixel = 0; XPixel < CharWidth; XPixel++)
            {
                FontSheetOffset = StartingFontSheetPixel + XPixel - (FontSheet->BitmapInfo.bmiHeader.biWidth * YPixel);

                StringBitmapOffset = (Character * CharWidth) + ((StringBitmap.BitmapInfo.bmiHeader.biWidth * StringBitmap.BitmapInfo.bmiHeader.biHeight) - StringBitmap.BitmapInfo.bmiHeader.biWidth) + XPixel - (StringBitmap.BitmapInfo.bmiHeader.biWidth * YPixel);

                memcpy_s(&FontSheetPixel, sizeof(PIXEL32), (PIXEL32*)FontSheet->Memory + FontSheetOffset, sizeof(PIXEL32));

                FontSheetPixel.Colors.Red = Color->Colors.Red;
                FontSheetPixel.Colors.Green = Color->Colors.Green;
                FontSheetPixel.Colors.Blue = Color->Colors.Blue;

                memcpy_s((PIXEL32*)StringBitmap.Memory + StringBitmapOffset, sizeof(PIXEL32*), &FontSheetPixel, sizeof(PIXEL32));
            }

        }

    }

    Blit32BppBitmapToBuffer(&StringBitmap, x, y, 0 ); //TODO: make brightness usefull here

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
        case GAMESTATE_BATTLE_MONSTER:
        {
            DrawBattleScreen();
            break;
        }
        case GAMESTATE_BATTLE_TRAINER:
        {
            DrawBattleScreen();
            break;
        }
        case GAMESTATE_LOADGAMESAVE:
        {
            DrawLoadGameSave();
            break;
        }
        case GAMESTATE_SAVEGAMESAVE:
        {
            DrawSaveGameSave();
            break;
        }
        case GAMESTATE_MONSTERSTATS:
        {
            DrawMonsterStatsScreen();
            break;
        }
        case GAMESTATE_DELETESAVEYESNO:
        {
            break;
        }
        case GAMESTATE_INVENTORYSCREEN:
        {
            DrawInventoryScreen();
            break;
        }
        case GAMESTATE_STORE:
        {
            DrawStoreScreen();
            break;
        }
        case GAMESTATE_DRIVE_STORAGE:
        {
            DrawStorageMenuScreen();
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


void Blit32BppBitmapToBuffer(_In_ GAMEBITMAP* GameBitmap, _In_ int16_t x, _In_ int16_t y, _In_ int16_t BrightnessAdjustment)
{
    int32_t StartingScreenPixel = ((GAME_RES_HEIGHT * GAME_RES_WIDTH) - GAME_RES_WIDTH) - (GAME_RES_WIDTH * y) + x;
    int32_t StartingBitmapPixel = ((GameBitmap->BitmapInfo.bmiHeader.biWidth * GameBitmap->BitmapInfo.bmiHeader.biHeight) - GameBitmap->BitmapInfo.bmiHeader.biWidth);
    int32_t MemoryOffset = 0;
    int32_t BitmapOffset = 0;
    PIXEL32 BitmapPixel = { 0 };

#ifdef AVX

    __m256i BitmapOctoPixel; 

    for (int16_t YPixel = 0; YPixel < GameBitmap->BitmapInfo.bmiHeader.biHeight; YPixel++)
    {
        int16_t PixelsRemainingOnThisRow = GameBitmap->BitmapInfo.bmiHeader.biWidth;

        int16_t XPixel = 0;
        while (PixelsRemainingOnThisRow >= 8)
        {

            MemoryOffset = StartingScreenPixel + XPixel - (GAME_RES_WIDTH * YPixel);

            BitmapOffset = StartingBitmapPixel + XPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            
            BitmapOctoPixel = _mm256_load_si256((const __m256i*)((PIXEL32*)GameBitmap->Memory + BitmapOffset));
            

            __m256i Half1 = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(BitmapOctoPixel, 0));
            

            // Add the brightness adjustment to each 16-bit element, except alpha.
            Half1 = _mm256_add_epi16(Half1, _mm256_set_epi16(
                0, BrightnessAdjustment, BrightnessAdjustment, BrightnessAdjustment,
                0, BrightnessAdjustment, BrightnessAdjustment, BrightnessAdjustment,
                0, BrightnessAdjustment, BrightnessAdjustment, BrightnessAdjustment,
                0, BrightnessAdjustment, BrightnessAdjustment, BrightnessAdjustment));

            __m256i Half2 = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(BitmapOctoPixel, 1));

            Half2 = _mm256_add_epi16(Half2, _mm256_set_epi16(
                0, BrightnessAdjustment, BrightnessAdjustment, BrightnessAdjustment,
                0, BrightnessAdjustment, BrightnessAdjustment, BrightnessAdjustment,
                0, BrightnessAdjustment, BrightnessAdjustment, BrightnessAdjustment,
                0, BrightnessAdjustment, BrightnessAdjustment, BrightnessAdjustment));

            // Now we need to reassemble the two halves back into a single 256-bit group of 8 pixels.
            // _mm256_packus_epi16(a,b) takes the 16-bit signed integers in the 256-bit vectors a and b
            // and converts them to a 256-bit vector of 8-bit unsigned integers. The result contains the
            // first 8 integers from a, followed by the first 8 integers from b, followed by the last 8
            // integers from a, followed by the last 8 integers from b.
            // Values that are out of range are set to 0 or 255.
            __m256i Recombined = _mm256_packus_epi16(Half1, Half2);

            BitmapOctoPixel = _mm256_permute4x64_epi64(Recombined, _MM_SHUFFLE(3, 1, 2, 0));

            // Create a mask that selects only the pixels that have an Alpha == 255.
            __m256i Mask = _mm256_cmpeq_epi8(BitmapOctoPixel, _mm256_set1_epi8(-1));

            // Conditionally store the result to the global back buffer, based on the mask
            // we just created that selects only the pixels where Alpha == 255.
            _mm256_maskstore_epi32((int*)((PIXEL32*)gBackBuffer.Memory + MemoryOffset), Mask, BitmapOctoPixel);

            PixelsRemainingOnThisRow -= 8;

            XPixel += 8;
        }
        while (PixelsRemainingOnThisRow > 0)
        {
            MemoryOffset = StartingScreenPixel + XPixel - (GAME_RES_WIDTH * YPixel);

            BitmapOffset = StartingBitmapPixel + XPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            memcpy_s(&BitmapPixel, sizeof(PIXEL32), (PIXEL32*)GameBitmap->Memory + BitmapOffset, sizeof(PIXEL32));     //copy contents of bitmap pixel

            if (BitmapPixel.Colors.Alpha > 0)      ////not alpha blending, only drawing non 0 alpha pixels TOUSE: alphablending possibly?? 
            {
                BitmapPixel.Colors.Red = min(255, max((BitmapPixel.Colors.Red + BrightnessAdjustment), 0));
                BitmapPixel.Colors.Green = min(255, max((BitmapPixel.Colors.Green + BrightnessAdjustment), 0));
                BitmapPixel.Colors.Blue = min(255, max((BitmapPixel.Colors.Blue + BrightnessAdjustment), 0));

                memcpy_s((PIXEL32*)gBackBuffer.Memory + MemoryOffset, sizeof(PIXEL32), &BitmapPixel, sizeof(PIXEL32));     //place contents of bitmap pixel onto backbuffer
            }
            PixelsRemainingOnThisRow--;
            XPixel++;
        }
    }

#elif defined SSE2

    //TODO:

#else

    for (int16_t YPixel = 0; YPixel < GameBitmap->BitmapInfo.bmiHeader.biHeight; YPixel++)
    {
        for (int16_t XPixel = 0; XPixel < GameBitmap->BitmapInfo.bmiHeader.biWidth; XPixel++)
        {
            ////preventing pixels being drawn outside screen
            //if ((x < 1) || (x < GAME_RES_WIDTH - GameBitmap->BitmapInfo.bmiHeader.biWidth) || (y < 1) || (y < GAME_RES_HEIGHT - GameBitmap->BitmapInfo.bmiHeader.biHeight))         //////TODO: please optimize this way too many branches
            //{
            //    if (x < 1)
            //    {
            //        if (XPixel < -x)
            //        {
            //            break;
            //        }
            //    }
            //    else if (x > GAME_RES_WIDTH - GameBitmap->BitmapInfo.bmiHeader.biWidth)
            //    {
            //        if (XPixel > GAME_RES_WIDTH - x - 1)
            //        {
            //            break;
            //        }
            //    }

            //    if ( y < 1)
            //    {
            //        if (YPixel < -y)
            //        {
            //            break;
            //        }
            //    }
            //    else if (y > GAME_RES_HEIGHT - GameBitmap->BitmapInfo.bmiHeader.biHeight)
            //    {
            //        if (YPixel > GAME_RES_HEIGHT - y - 1)
            //        {
            //            break;
            //        }
            //    }
            //}
            MemoryOffset = StartingScreenPixel + XPixel - (GAME_RES_WIDTH * YPixel);
            
            BitmapOffset = StartingBitmapPixel + XPixel - (GameBitmap->BitmapInfo.bmiHeader.biWidth * YPixel);

            memcpy_s(&BitmapPixel, sizeof(PIXEL32), (PIXEL32*)GameBitmap->Memory + BitmapOffset, sizeof(PIXEL32));     //copy contents of bitmap pixel

            if (BitmapPixel.Alpha > 0)
            {
                BitmapPixel.Red = min(255, max((BitmapPixel.Red + BrightnessAdjustment), 0));
                BitmapPixel.Green = min(255, max((BitmapPixel.Green + BrightnessAdjustment), 0));
                BitmapPixel.Blue = min(255, max((BitmapPixel.Blue + BrightnessAdjustment), 0));

                memcpy_s((PIXEL32*)gBackBuffer.Memory + MemoryOffset, sizeof(PIXEL32), &BitmapPixel, sizeof(PIXEL32));     //place contents of bitmap pixel onto backbuffer
            }
        }
    }

#endif
}   

void BlitBackgroundToBuffer(_In_ GAMEBITMAP* GameBitmap, _In_ int16_t BrightnessAdjustment)
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

            BitmapOctoPixel = _mm256_loadu_si256((const __m256i*)((PIXEL32*)GameBitmap->Memory + BitmapOffset));


            //        AARRGGBBAARRGGBB-AARRGGBBAARRGGBB-AARRGGBBAARRGGBB-AARRGGBBAARRGGBB
            // YMM0 = FF5B6EE1FF5B6EE1-FF5B6EE1FF5B6EE1-FF5B6EE1FF5B6EE1-FF5B6EE1FF5B6EE1

            __m256i Half1 = _mm256_cvtepu8_epi16( _mm256_extracti128_si256(BitmapOctoPixel, 0));

            //        AAAARRRRGGGGBBBB-AAAARRRRGGGGBBBB-AAAARRRRGGGGBBBB-AAAARRRRGGGGBBBB
            // YMM0 = 00FF005B006E00E1-00FF005B006E00E1-00FF005B006E00E1-00FF005B006E00E1

            Half1 = _mm256_add_epi16(Half1, _mm256_set1_epi16(BrightnessAdjustment));

            //        AAAARRRRGGGGBBBB-AAAARRRRGGGGBBBB-AAAARRRRGGGGBBBB-AAAARRRRGGGGBBBB        
            // YMM0 = 0000FF5Cff6FFFE2-0000FF5Cff6FFFE2-0000FF5Cff6FFFE2-0000FF5Cff6FFFE2

            //      take apart 256 bits into 2x 256 bits (half1&2), so we can do math with each pixel conataining 16bits of info (intead of 8bits) and brightness (which is 16bits)
            //

            __m256i Half2 = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(BitmapOctoPixel, 1));

            //  
            //

            Half2 = _mm256_add_epi16(Half2, _mm256_set1_epi16(BrightnessAdjustment));
            
            //
            //

            __m256i Recombined = _mm256_packus_epi16(Half1, Half2);

            // packus doesnt retain order very well, so we need to shuffle
            // packus also clamps value between 255 and 0 so we dont have to
             
            BitmapOctoPixel = _mm256_permute4x64_epi64(Recombined, _MM_SHUFFLE(3, 1, 2, 0 ));

            _mm256_store_si256((__m256i*)((PIXEL32*)gBackBuffer.Memory + MemoryOffset), BitmapOctoPixel);
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

            BitmapPixel.Red = (uint8_t) min(255, max((BitmapPixel.Red + BrightnessAdjustment), 0));

            BitmapPixel.Blue = (uint8_t) min(255, max((BitmapPixel.Blue + BrightnessAdjustment), 0));

            BitmapPixel.Green = (uint8_t) min(255, max((BitmapPixel.Green + BrightnessAdjustment), 0));

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



    Result = RegGetValueA(RegKey, NULL, "TextSpeed", RRF_RT_DWORD, NULL, (BYTE*)&gRegistryParams.TextSpeed, &RegBytesRead);

    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(LL_INFO, "[%s] Registry value 'TextSpeed' not found. Using default of 2 (Medium).", __FUNCTION__);
            gRegistryParams.TextSpeed = 2;
        }
        else
        {
            LogMessageA(LL_ERROR, "[%s] Failed to read the 'TextSpeed' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }
    LogMessageA(LL_INFO, "[%s] TextSpeed is %d", __FUNCTION__, gRegistryParams.TextSpeed);


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
    


    Result = RegSetValueExA(RegKey, "TextSpeed", 0, REG_DWORD, (const BYTE*)&gRegistryParams.TextSpeed, sizeof(DWORD));

    if (Result != ERROR_SUCCESS)
    {
        LogMessageA(LL_ERROR, "[%s] Failed to set 'TextSpeed' in registry! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }
    LogMessageA(LL_INFO, "[%s] Saved 'TextSpeed' in registry: %d. ", __FUNCTION__, gRegistryParams.TextSpeed);

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

        /*sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FPS Raw: %.01f ", gGamePerformanceData.RawFPSAverage);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 0);*/
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FPS: %.01f (%.01f)", gGamePerformanceData.CookedFPSAverage, gGamePerformanceData.RawFPSAverage);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &LimeGreen, 0, 0);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Timer: %.02f/%.02f/%.02f", (gGamePerformanceData.CurrentTimerResolution / 10000.0f), (gGamePerformanceData.MinimumTimerResolution / 10000.0f), (gGamePerformanceData.MaximumTimerResolution / 10000.0f));
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &LimeGreen, 0, 8);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Memory: %llu KB ", (gGamePerformanceData.MemInfo.PrivateUsage / 1024));
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &LimeGreen, 0, 16);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "CPU: %.03f%%", gGamePerformanceData.CPUPercent);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &LimeGreen, 0, 24);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Handles  :  %lu ", gGamePerformanceData.HandleCount);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 32);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Frames   :%llu", gGamePerformanceData.TotalFramesRendered);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &White, 0, 40);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Screen: (%d,%d) ", gPlayer.ScreenPos.x, gPlayer.ScreenPos.y);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &SkyBlue, 0, 48);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "World: (%d,%d) ", gPlayer.WorldPos.x, gPlayer.WorldPos.y);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &SkyBlue, 0, 56);
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Camera: (%d,%d) ", gCamera.x, gCamera.y);
        BlitStringToBuffer(DebugTextBuffer, &g6x7Font, &SkyBlue, 0, 64);
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



DWORD LoadWaveFromMem(_In_ void* Buffer, _Inout_ GAMESOUND* GameSound)
{

    DWORD Error = ERROR_SUCCESS;

    //DWORD NumberOfBytesRead = 0;

    DWORD RIFF = 0;

    uint16_t DataChunkOffset = 0;

    DWORD DataChunkSearcher = 0;

    BOOL DataChunkFound = FALSE;

    DWORD DataChunkSize = 0;

    memcpy(&RIFF, Buffer, sizeof(DWORD));

    if (RIFF != 0x46464952)     //0x46464952 is "RIFF" backwards
    {
        Error = ERROR_FILE_INVALID;
        LogMessageA(LL_ERROR, "[%s] First four bytes of memory buffer are not 'RIFF'! Error 0x%08lx!", __FUNCTION__, Error);
        goto Exit;
    }

    memcpy(&GameSound->WaveFormat, (BYTE*)Buffer + 20, sizeof(WAVEFORMATEX));

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

    memcpy(&DataChunkSize, (BYTE*)Buffer + DataChunkOffset + 4, sizeof(DWORD));

    GameSound->Buffer.Flags = XAUDIO2_END_OF_STREAM;
    GameSound->Buffer.AudioBytes = DataChunkSize;
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

    return(Error);
}


DWORD LoadOggFromMem(_In_ void* Buffer, _In_ uint32_t BufferSize, _Inout_ GAMESOUND* GameSound)
{
    DWORD Error = ERROR_SUCCESS;

    int SamplesDecoded = 0;

    int Channels = 0;

    int SampleRate = 0;

    short* DecodedAudio = NULL;

    LogMessageA(LL_INFO, "[%s] Size of Ogg file: %lu.", __FUNCTION__, BufferSize);

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

void PauseGameMusic(void)
{
    gXAudioMusicSourceVoice->lpVtbl->Stop(gXAudioMusicSourceVoice, 0, 0);
    gMusicPaused = TRUE;
}

void StopGameMusic(void)
{
    gXAudioMusicSourceVoice->lpVtbl->Stop(gXAudioMusicSourceVoice, 0, 0);

    gXAudioMusicSourceVoice->lpVtbl->FlushSourceBuffers(gXAudioMusicSourceVoice);

    gMusicPaused = FALSE;
}

void PlayGameMusic(_In_ GAMESOUND* GameSound, _In_ BOOL Looping, _In_ BOOL Immediate)
{
    if (gMusicPaused == FALSE)
    {
        if (Immediate)
        {
            gXAudioMusicSourceVoice->lpVtbl->Stop(gXAudioMusicSourceVoice, 0, 0);

            gXAudioMusicSourceVoice->lpVtbl->FlushSourceBuffers(gXAudioMusicSourceVoice);
        }

        if (Looping == TRUE)
        {
            GameSound->Buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
        }
        else
        {
            GameSound->Buffer.LoopCount = 0;
        }

        gXAudioMusicSourceVoice->lpVtbl->SubmitSourceBuffer(gXAudioMusicSourceVoice, &GameSound->Buffer, NULL);

        gXAudioMusicSourceVoice->lpVtbl->Start(gXAudioMusicSourceVoice, 0, XAUDIO2_COMMIT_NOW);

    }
    else
    {
        gXAudioMusicSourceVoice->lpVtbl->Start(gXAudioMusicSourceVoice, 0, XAUDIO2_COMMIT_NOW);
    }
    gMusicPaused = FALSE;
}

BOOL MusicIsPlaying(void)
{
    XAUDIO2_VOICE_STATE  State = { 0 };

    gXAudioMusicSourceVoice->lpVtbl->GetState(gXAudioMusicSourceVoice, &State, 0);

    if ((State.BuffersQueued > 0) && (gMusicPaused == FALSE))
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

DWORD LoadAssetFromArchive(_In_ char* ArchiveName, _In_ char* AssetFileName, _Inout_ void* Resource)
{
    DWORD Error = ERROR_SUCCESS;

    mz_zip_archive Archive = { 0 };

    BYTE* DecompressedBuffer = NULL;

    size_t DecompressedSize = 0;

    BOOL FileFoundInArchive = FALSE;

    char* FileExtension = NULL;

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

    for (int i = strlen(AssetFileName) - 1; i > 0; i--)     //look for period and point to file extension
    {
        FileExtension = &AssetFileName[i];
        if (FileExtension[0] == '.')
        {
            break;
        }
    }

    if (FileExtension && _stricmp(FileExtension, ".bmpx") == 0)
    {
        Error = Load32BppBitmapFromMem(DecompressedBuffer, Resource);
    }
    else if (FileExtension && _stricmp(FileExtension, ".ogg") == 0)
    {
        Error = LoadOggFromMem(DecompressedBuffer, (uint32_t)DecompressedSize, Resource);
    }
    else if (FileExtension && _stricmp(FileExtension, ".wav") == 0)
    {
        Error = LoadWaveFromMem(DecompressedBuffer, Resource);
    }
    else if (FileExtension && _stricmp(FileExtension, ".tmx") == 0)
    {
        Error = LoadTileMapFromMem(DecompressedBuffer, (uint32_t)DecompressedSize, Resource);
    }
    else
    {
        ASSERT(FALSE, "Unknown resource type in LoadAssetFromArchive!");
    }


Exit:

    mz_zip_reader_end(&Archive);

    return(Error);
}


DWORD AssetLoadingThreadProc(_In_ LPVOID lpParam)
{
    UNREFERENCED_PARAMETER(lpParam);

    DWORD Error = ERROR_SUCCESS;

    typedef struct ASSET
    {
        char* Name;

        void* Destination;

    } ASSET;

    ASSET Assets[] = {
        {   "PixelFont(6x7).bmpx", &g6x7Font },
        {   "SplashNoise.wav", &gSoundSplashScreen },           // last essential asset before main menu
        {   "Overworld01.bmpx", &gOverWorld01.GameBitmap },
        {   "Overworld01.tmx", &gOverWorld01.TileMap },
        {   "menu.wav", &gSoundMenuNavigate },
        {   "item.wav", &gSoundMenuChoose },
        {   "NewAdventures.ogg", &gMusicOverWorld01 },
        {   "MysteriousDeep.ogg", &gMusicDungeon01 },
        {   "Suit0FacingDown0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_0] },
        {   "Suit0FacingDown1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_1] },
        {   "Suit0FacingDown2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_2] },
        {   "Suit0FacingLeft0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_0] },
        {   "Suit0FacingLeft1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_1] },
        {   "Suit0FacingLeft2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_2] },
        {   "Suit0FacingRight0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_0] },
        {   "Suit0FacingRight1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_1] },
        {   "Suit0FacingRight2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_2] },
        {   "Suit0FacingUp0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_0] },
        {   "Suit0FacingUp1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_1] },
        {   "Suit0FacingUp2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_2] },
        {   "ManFacingDown0.bmpx", &gCharacterSprite[0].Sprite[FACING_DOWN_0] },
        {   "ManFacingDown1.bmpx", &gCharacterSprite[0].Sprite[FACING_DOWN_1] },
        {   "ManFacingDown2.bmpx", &gCharacterSprite[0].Sprite[FACING_DOWN_2] },
        {   "ManFacingLeft0.bmpx", &gCharacterSprite[0].Sprite[FACING_LEFT_0] },
        {   "ManFacingLeft1.bmpx", &gCharacterSprite[0].Sprite[FACING_LEFT_1] },
        {   "ManFacingLeft2.bmpx", &gCharacterSprite[0].Sprite[FACING_LEFT_2] },
        {   "ManFacingRight0.bmpx", &gCharacterSprite[0].Sprite[FACING_RIGHT_0] },
        {   "ManFacingRight1.bmpx", &gCharacterSprite[0].Sprite[FACING_RIGHT_1] },
        {   "ManFacingRight2.bmpx", &gCharacterSprite[0].Sprite[FACING_RIGHT_2] },
        {   "ManFacingUp0.bmpx", &gCharacterSprite[0].Sprite[FACING_UP_0] },
        {   "ManFacingUp1.bmpx", &gCharacterSprite[0].Sprite[FACING_UP_1] },
        {   "ManFacingUp2.bmpx", &gCharacterSprite[0].Sprite[FACING_UP_2] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_DOWN_0] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_DOWN_1] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_DOWN_2] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_LEFT_0] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_LEFT_1] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_LEFT_2] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_RIGHT_0] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_RIGHT_1] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_RIGHT_2] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_UP_0] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_UP_1] },
        {   "EmptySprite.bmpx", &gCharacterSprite[1].Sprite[FACING_UP_2] },
        {   "ManFacingDown0.bmpx", &gCharacterSprite[2].Sprite[FACING_DOWN_0] },
        {   "ManFacingDown1.bmpx", &gCharacterSprite[2].Sprite[FACING_DOWN_1] },
        {   "ManFacingDown2.bmpx", &gCharacterSprite[2].Sprite[FACING_DOWN_2] },
        {   "ManFacingLeft0.bmpx", &gCharacterSprite[2].Sprite[FACING_LEFT_0] },
        {   "ManFacingLeft1.bmpx", &gCharacterSprite[2].Sprite[FACING_LEFT_1] },
        {   "ManFacingLeft2.bmpx", &gCharacterSprite[2].Sprite[FACING_LEFT_2] },
        {   "ManFacingRight0.bmpx", &gCharacterSprite[2].Sprite[FACING_RIGHT_0] },
        {   "ManFacingRight1.bmpx", &gCharacterSprite[2].Sprite[FACING_RIGHT_1] },
        {   "ManFacingRight2.bmpx", &gCharacterSprite[2].Sprite[FACING_RIGHT_2] },
        {   "ManFacingUp0.bmpx", &gCharacterSprite[2].Sprite[FACING_UP_0] },
        {   "ManFacingUp1.bmpx", &gCharacterSprite[2].Sprite[FACING_UP_1] },
        {   "ManFacingUp2.bmpx", &gCharacterSprite[2].Sprite[FACING_UP_2] },
        {   "ManFacingDown0.bmpx", &gCharacterSprite[3].Sprite[FACING_DOWN_0] },
        {   "ManFacingDown1.bmpx", &gCharacterSprite[3].Sprite[FACING_DOWN_1] },
        {   "ManFacingDown2.bmpx", &gCharacterSprite[3].Sprite[FACING_DOWN_2] },
        {   "ManFacingLeft0.bmpx", &gCharacterSprite[3].Sprite[FACING_LEFT_0] },
        {   "ManFacingLeft1.bmpx", &gCharacterSprite[3].Sprite[FACING_LEFT_1] },
        {   "ManFacingLeft2.bmpx", &gCharacterSprite[3].Sprite[FACING_LEFT_2] },
        {   "ManFacingRight0.bmpx", &gCharacterSprite[3].Sprite[FACING_RIGHT_0] },
        {   "ManFacingRight1.bmpx", &gCharacterSprite[3].Sprite[FACING_RIGHT_1] },
        {   "ManFacingRight2.bmpx", &gCharacterSprite[3].Sprite[FACING_RIGHT_2] },
        {   "ManFacingUp0.bmpx", &gCharacterSprite[3].Sprite[FACING_UP_0] },
        {   "ManFacingUp1.bmpx", &gCharacterSprite[3].Sprite[FACING_UP_1] },
        {   "ManFacingUp2.bmpx", &gCharacterSprite[3].Sprite[FACING_UP_2] },
        {   "StorageComputerOn.bmpx", &gCharacterSprite[4].Sprite[FACING_DOWN_0] },
        {   "StorageComputerOn.bmpx", &gCharacterSprite[4].Sprite[FACING_DOWN_1] },
        {   "StorageComputerOn.bmpx", &gCharacterSprite[4].Sprite[FACING_DOWN_2] },
        {   "StorageComputerOn.bmpx", &gCharacterSprite[4].Sprite[FACING_LEFT_0] },
        {   "StorageComputerOn.bmpx", &gCharacterSprite[4].Sprite[FACING_LEFT_1] },
        {   "StorageComputerOn.bmpx", &gCharacterSprite[4].Sprite[FACING_LEFT_2] },
        {   "StorageComputerOn.bmpx", &gCharacterSprite[4].Sprite[FACING_RIGHT_0] },
        {   "StorageComputerOn.bmpx", &gCharacterSprite[4].Sprite[FACING_RIGHT_1] },
        {   "StorageComputerOn.bmpx", &gCharacterSprite[4].Sprite[FACING_RIGHT_2] },
        {   "StorageComputerOff.bmpx", &gCharacterSprite[4].Sprite[FACING_UP_0] },
        {   "StorageComputerOff.bmpx", &gCharacterSprite[4].Sprite[FACING_UP_1] },
        {   "StorageComputerOff.bmpx", &gCharacterSprite[4].Sprite[FACING_UP_2] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_DOWN_0] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_DOWN_1] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_DOWN_2] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_LEFT_0] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_LEFT_1] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_LEFT_2] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_RIGHT_0] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_RIGHT_1] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_RIGHT_2] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_UP_0] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_UP_1] },
        {   "EmptySprite.bmpx", &gCharacterSprite[5].Sprite[FACING_UP_2] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_DOWN_0] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_DOWN_1] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_DOWN_2] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_LEFT_0] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_LEFT_1] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_LEFT_2] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_RIGHT_0] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_RIGHT_1] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_RIGHT_2] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_UP_0] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_UP_1] },
        {   "SmallBag.bmpx", &gCharacterSprite[6].Sprite[FACING_UP_2] },
        { "ManFacingDown0.bmpx", &gCharacterSprite[7].Sprite[FACING_DOWN_0] },
        { "ManFacingDown1.bmpx", &gCharacterSprite[7].Sprite[FACING_DOWN_1] },
        { "ManFacingDown2.bmpx", &gCharacterSprite[7].Sprite[FACING_DOWN_2] },
        { "ManFacingLeft0.bmpx", &gCharacterSprite[7].Sprite[FACING_LEFT_0] },
        { "ManFacingLeft1.bmpx", &gCharacterSprite[7].Sprite[FACING_LEFT_1] },
        { "ManFacingLeft2.bmpx", &gCharacterSprite[7].Sprite[FACING_LEFT_2] },
        { "ManFacingRight0.bmpx", &gCharacterSprite[7].Sprite[FACING_RIGHT_0] },
        { "ManFacingRight1.bmpx", &gCharacterSprite[7].Sprite[FACING_RIGHT_1] },
        { "ManFacingRight2.bmpx", &gCharacterSprite[7].Sprite[FACING_RIGHT_2] },
        {   "SmallBag.bmpx", &gLootPickup },
        {   "EmptySprite.bmpx", &gEmptySprite },
        {   "Sparkle01(3).bmpx", &gSparkle01[3]},
        {   "Sparkle01(2).bmpx", &gSparkle01[2]},
        {   "Sparkle01(1).bmpx", &gSparkle01[1]},
        {   "Sparkle01(0).bmpx", &gSparkle01[0]},
        {   "Sparkle02(3).bmpx", &gSparkle02[3]},
        {   "Sparkle02(2).bmpx", &gSparkle02[2]},
        {   "Sparkle02(1).bmpx", &gSparkle02[1]},
        {   "Sparkle02(0).bmpx", &gSparkle02[0]},
        {   "Sparkle03(3).bmpx", &gSparkle03[3]},
        {   "Sparkle03(2).bmpx", &gSparkle03[2]},
        {   "Sparkle03(1).bmpx", &gSparkle03[1]},
        {   "Sparkle03(0).bmpx", &gSparkle03[0]},
        {   "BattleBackgroundGrass01.bmpx", &gBattleScreen_Grass01 },
        {   "BattleBackgroundStoneBricks01.bmpx", &gBattleScreen_StoneBricks01 },
        {   "Wolf64Back01.bmpx", &gBattleSpriteBack[MONSTER_WOLF] },
        {   "Wolf64Front01.bmpx", &gBattleSpriteFront[MONSTER_WOLF] },
        {   "Wolf64Back02.bmpx",& gBattleSpriteBack[MONSTER_EARTHWOLF] },
        {   "Wolf64Front02.bmpx", &gBattleSpriteFront[MONSTER_EARTHWOLF] },
        {   "Wolf64Back03.bmpx", &gBattleSpriteBack[MONSTER_AIRWOLF] },
        {   "Wolf64Front03.bmpx", &gBattleSpriteFront[MONSTER_AIRWOLF] },
        {   "Wolf64Back04.bmpx", &gBattleSpriteBack[MONSTER_FIREWOLF] },
        {   "Wolf64Front04.bmpx", &gBattleSpriteFront[MONSTER_FIREWOLF] },
        {   "Wolf64Back05.bmpx", &gBattleSpriteBack[MONSTER_WATERWOLF] },
        {   "Wolf64Front05.bmpx", &gBattleSpriteFront[MONSTER_WATERWOLF] },
        {   "Wolf64Back06.bmpx", &gBattleSpriteBack[MONSTER_ELECTRICWOLF] },
        {   "Wolf64Front06.bmpx", &gBattleSpriteFront[MONSTER_ELECTRICWOLF] },
        {   "Wolf64Back07.bmpx", &gBattleSpriteBack[MONSTER_METALWOLF] },
        {   "Wolf64Front07.bmpx", &gBattleSpriteFront[MONSTER_METALWOLF] },
        {   "Wolf64Back08.bmpx", &gBattleSpriteBack[MONSTER_SOULWOLF] },
        {   "Wolf64Front08.bmpx", &gBattleSpriteFront[MONSTER_SOULWOLF] },
        {   "Wolf64Back09.bmpx", &gBattleSpriteBack[MONSTER_LIFEWOLF] },
        {   "Wolf64Front09.bmpx", &gBattleSpriteFront[MONSTER_LIFEWOLF] },
        {   "Wolf64Back10.bmpx", &gBattleSpriteBack[MONSTER_DEATHWOLF] },
        {   "Wolf64Front10.bmpx", &gBattleSpriteFront[MONSTER_DEATHWOLF] },
        {   "EmptySprite.bmpx", &gEmptySprite },
    };

    int FinalEssentialAssetIndex = 1;

    LogMessageA(LL_INFO, "[%s] Asset loading has begun.", __FUNCTION__);

    for (int i = 0; i < _countof(Assets); i++)
    {
        if ((Error = LoadAssetFromArchive(ASSET_FILE, Assets[i].Name, Assets[i].Destination)) != ERROR_SUCCESS)
        {
            MessageBoxA(NULL, "LoadAssetFromArchive failed! Check log file!", "Error!", MB_ICONERROR | MB_OK);
            LogMessageA(LL_ERROR, "[%s] Loading %s failed with error 0x%08lx!", __FUNCTION__, Assets[i].Name, Error);
            goto Exit;
        }

        if (i == FinalEssentialAssetIndex)
        {
            //end of assets needed for splash screen and titlescreen
            SetEvent(gEssentialAssetsLoadedEvent);
        }
    }



Exit:

    if (Error == ERROR_SUCCESS)
    {
        LogMessageA(LL_INFO, "[%s] Asset loading ended successfully.", __FUNCTION__);
    }
    else
    {
        LogMessageA(LL_ERROR, "[%s] Asset loading failed with result 0x%08lx!", __FUNCTION__, Error);
    }

    return(Error);
}


void InitializeGlobals(void)
{
    //TODO: saveblocks and pointers
    SetSaveBlockPointers(sizeof(gPlayer.Seed));

    ResetDriveStorageSystem();

    ClearAllGameFlags();

    ASSERT(_countof(gPassableTiles) == NUM_PASSABLE_TILES, "The gPassableTiles array is the wrong size!");

    ASSERT(_countof(gEncounterTiles) == NUM_ENCOUNTER_TILES, "The gEncounterTiles array is the wrong size!");

    ////make sure gPassableTiles array is large enough to fit all tiles (declared in main.h)
    gPassableTiles[0] = TILE_GRASS_01;
    gPassableTiles[1] = TILE_STONEBRICKS_FLOOR_01;
    gPassableTiles[2] = TILE_TELEPORT01;
    gPassableTiles[3] = TILE_TALLGRASS_01;
    gPassableTiles[4] = TILE_WOODFLOOR_01;
    gPassableTiles[5] = TILE_WOODFLOOR_04;
    gPassableTiles[6] = TILE_WOODFLOOR_05;
    gPassableTiles[7] = TILE_WOODFLOOR_06;
    gPassableTiles[8] = TILE_SLOPE_06;
    gPassableTiles[9] = TILE_HOUSEOUTSIDE_03;
    gPassableTiles[10] = TILE_FLOWERS_01;
    gPassableTiles[11] = TILE_FLOWERS_02;
    gPassableTiles[12] = TILE_FLOWERS_03;
    gPassableTiles[13] = TILE_FLOWERS_04;
    gPassableTiles[14] = TILE_STAIRS_01;
    gPassableTiles[15] = TILE_SLOPE_29;
    gPassableTiles[16] = TILE_SLOPE_30;
    gPassableTiles[17] = TILE_STONEFLOOR_01;
    gPassableTiles[18] = TILE_WATERBRIDGE_01;
    gPassableTiles[19] = TILE_WATERBRIDGE_02;
    gPassableTiles[20] = TILE_WATERBRIDGE_03;
    gPassableTiles[21] = TILE_WATERBRIDGE_04;
    gPassableTiles[22] = TILE_WATERBRIDGE_06;
    gPassableTiles[23] = TILE_WATERBRIDGE_05;
    gPassableTiles[24] = TILE_WATERBRIDGE_07;
    gPassableTiles[25] = TILE_WATERBRIDGE_08;
    gPassableTiles[26] = TILE_WATERBRIDGE_09;
    gPassableTiles[27] = TILE_WATERBRIDGE_10;
    gPassableTiles[28] = TILE_WATERBRIDGE_11;
    gPassableTiles[29] = TILE_WATERBRIDGE_12;
    gPassableTiles[30] = TILE_WATERBRIDGE_13;
    gPassableTiles[31] = TILE_WATERBRIDGE_14;
    gPassableTiles[32] = TILE_SLOPE_44;
    gPassableTiles[33] = TILE_SLOPE_49;
    gPassableTiles[34] = TILE_SLOPE_50;
    gPassableTiles[35] = TILE_SLOPE_51;
    gPassableTiles[36] = TILE_SLOPE_52;
    gPassableTiles[37] = TILE_SLOPE_53;
    gPassableTiles[38] = TILE_SLOPE_54;
    gPassableTiles[39] = TILE_SLOPE_55;
    gPassableTiles[40] = TILE_SLOPE_56;
    gPassableTiles[41] = TILE_TELEPORT03;
    gPassableTiles[42] = TILE_STONEFLOOR_02;
    gPassableTiles[43] = TILE_WOODFLOOR_21;
    gPassableTiles[44] = TILE_WOODFLOOR_22;
    gPassableTiles[45] = TILE_WOODFLOOR_17;
    gPassableTiles[46] = TILE_WOODFLOOR_18;
    gPassableTiles[47] = TILE_WOODFLOOR_23;
    gPassableTiles[48] = TILE_WOODFLOOR_24;
    gPassableTiles[49] = TILE_WOODFLOOR_35;
    gPassableTiles[50] = TILE_LAB_FLOOR_01;
    gPassableTiles[51] = TILE_LAB_FLOOR_02;
    gPassableTiles[52] = TILE_LAB_FLOOR_03;
    gPassableTiles[53] = TILE_LAB_FLOOR_04;
    gPassableTiles[54] = TILE_LAB_FLOOR_06;
    gPassableTiles[55] = TILE_LAB_FLOOR_07;
    gPassableTiles[56] = TILE_LAB_FLOOR_08;
    gPassableTiles[57] = TILE_LAB_FLOOR_13;
    gPassableTiles[58] = TILE_LAB_FLOOR_14;
    gPassableTiles[59] = TILE_LAB_FLOOR_15;
    gPassableTiles[60] = TILE_LAB_FLOOR_16;
    gPassableTiles[61] = TILE_LAB_FLOOR_17;
    gPassableTiles[62] = TILE_LAB_FLOOR_34;
    gPassableTiles[63] = TILE_LAB_FLOOR_35;
    gPassableTiles[64] = TILE_WOODFLOOR_36;
    gPassableTiles[65] = TILE_WOODFLOOR_52;
    gPassableTiles[66] = TILE_WOODFLOOR_51;
    gPassableTiles[67] = TILE_WOODFLOOR_49;
    gPassableTiles[68] = TILE_WOODFLOOR_48;
    gPassableTiles[69] = TILE_WOODFLOOR_46;
    gPassableTiles[70] = TILE_WOODFLOOR_45;
    gPassableTiles[71] = TILE_WOODFLOOR_43;
    gPassableTiles[72] = TILE_WOODFLOOR_02;
    gPassableTiles[73] = TILE_LAB_WALL_01;

    gEncounterTiles[0] = TILE_TALLGRASS_01;
    gEncounterTiles[1] = TILE_STONEBRICKS_FLOOR_01;

    gCliffEdgeTiles[0] = TILE_SLOPE_06;
    gCliffEdgeTiles[1] = TILE_SLOPE_44;

    gTeleportTiles[0] = TILE_TELEPORT01;
    gTeleportTiles[1] = TILE_HOUSEOUTSIDE_03;
    gTeleportTiles[2] = TILE_TELEPORT03;
    gTeleportTiles[3] = TILE_WOODFLOOR_35;
    gTeleportTiles[4] = TILE_WOODFLOOR_36;

    //whole game map
    gOverworldArea = (GAMEAREA){ .Name = "Overworld",
                                 .Area = (RECT){.left = 0, .top = 0, .right = 4240, .bottom = 4800 },
                                 .Music = &gMusicOverWorld01,
                                 .Index = 0 };

    //spawn point
    gHomeArea = (GAMEAREA){ .Name = "Home",
                                 .Area = (RECT){.left = 3856, .top = 256, .right = 4240, .bottom = 496 },
                                 .Music = &gMusicOverWorld01,
                                 .Index = 1 };


    //actual locations

    gStartingTownArea = (GAMEAREA){ .Name = "Starting Town",
                                    .Area = (RECT){.left = 96, .top = 4400, .right = 592, .bottom = 4720 },
                                    .Music = &gMusicOverWorld01,
                                    .Index = 2 };

    gRoute01Area = (GAMEAREA){ .Name = "Route01",
                                    .Area = (RECT){.left = 528, .top = 4288, .right = 1392, .bottom = 4720 },
                                    .Music = &gMusicOverWorld01,
                                    .Index = 3 };

    gBattleTown01Area = (GAMEAREA){ .Name = "BattleTown01",
                                    .Area = (RECT){.left = 1328, .top = 4256, .right = 2064, .bottom = 4784 },
                                    .Music = &gMusicOverWorld01,
                                    .Index = 4 };

    gForest01Area = (GAMEAREA){ .Name = "Forest01",
                                    .Area = (RECT){.left = 528, .top = 3808, .right = 1328, .bottom = 4288 },
                                    .Music = &gMusicOverWorld01,
                                    .Index = 5 };

    gRoute02Area = (GAMEAREA){ .Name = "Route02",
                                    .Area = (RECT){.left = 80, .top = 3648, .right = 592, .bottom = 4400 },
                                    .Music = &gMusicOverWorld01,
                                    .Index = 6 };

    gBattleTown02Area = (GAMEAREA){ .Name = "BattleTown02",
                                    .Area = (RECT){.left = 80, .top = 3152, .right = 1040, .bottom = 3696 },
                                    .Music = &gMusicOverWorld01,
                                    .Index = 7 };

    //"inside" locations i.e. caves, houses, etc

    gStartingLabArea = (GAMEAREA){ .Name = "Labratory",
                                 .Area = (RECT){.left = 3856, .top = 0, .right = 4240, .bottom = 240 },
                                 .Music = &gMusicOverWorld01,
                                 .Index = 8 };   

    gBedroomArea = (GAMEAREA){ .Name = "Your bedroom",
                                 .Area = (RECT){.left = 3856, .top = 512, .right = 4240, .bottom = 752 },
                                 .Music = &gMusicOverWorld01,
                                 .Index = 9 };

    gCurrentArea = gBedroomArea;

    gGameAreas[0] = gOverworldArea;
    gGameAreas[1] = gHomeArea;
    gGameAreas[2] = gStartingTownArea;
    gGameAreas[3] = gRoute01Area;
    gGameAreas[4] = gBattleTown01Area;
    gGameAreas[5] = gForest01Area;
    gGameAreas[6] = gRoute02Area;
    gGameAreas[7] = gBattleTown02Area;
    gGameAreas[8] = gStartingLabArea;
    gGameAreas[9] = gBedroomArea;

    ASSERT(_countof(gGameAreas) == NUM_GAME_AREAS, "Array gGameAreas is the wrong size!");
 
    gNullEncounterArea = (ENCOUNTERAREA){ 0 };

    gOverworld01EncounterArea = (ENCOUNTERAREA){ .Name = "Route1General",
                                                .Area = (RECT){.left = 560, .top = 4400, .right = 1328, .bottom = 4688 },
                                                .EncounterRate = 200,
                                                .MaxLevel = 6,
                                                .MinLevel = 4,
                                                .MonsterIndexChanceSlots = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

    gOverworld02EncounterArea = (ENCOUNTERAREA){ .Name = "Route1Special",
                                                .Area = (RECT){.left = 1152, .top = 4336, .right = 1280, .bottom = 4400 },
                                                .EncounterRate = 100,
                                                .MaxLevel = 10,
                                                .MinLevel = 7,
                                                .MonsterIndexChanceSlots = { 1, 1, 1, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 } };

    gOverworld03EncounterArea = (ENCOUNTERAREA){ .Name = "Forest1HardPath",
                                                .Area = (RECT){.left = 592, .top = 3840, .right = 752, .bottom = 4224 },
                                                .EncounterRate = 200,
                                                .MaxLevel = 13,
                                                .MinLevel = 11,
                                                .MonsterIndexChanceSlots = { 1, 1, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0 } };

    gOverworld04EncounterArea = (ENCOUNTERAREA){ .Name = "Forest1General",
                                                .Area = (RECT){.left = 752, .top = 3872, .right = 1296, .bottom = 4224 },
                                                .EncounterRate = 200,
                                                .MaxLevel = 11,
                                                .MinLevel = 8,
                                                .MonsterIndexChanceSlots = { 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0 } };

    gOverworld05EncounterArea = (ENCOUNTERAREA){ .Name = "Forest1Special",
                                                .Area = (RECT){.left = 816, .top = 3840, .right = 1182, .bottom = 3872 },
                                                .EncounterRate = 100,
                                                .MaxLevel = 9,
                                                .MinLevel = 7,
                                                .MonsterIndexChanceSlots = { 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

    gOverworld06EncounterArea = (ENCOUNTERAREA){ .Name = "Route2General",
                                                .Area = (RECT){.left = 240, .top = 3728, .right = 528, .bottom = 4288 },
                                                .EncounterRate = 200,
                                                .MaxLevel = 12,
                                                .MinLevel = 9,
                                                .MonsterIndexChanceSlots = { 1, 1, 1, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0 } };

    gOverworld07EncounterArea = (ENCOUNTERAREA){ .Name = "Route2Special",
                                                .Area = (RECT){.left = 144, .top = 3968, .right = 192, .bottom = 4304 },
                                                .EncounterRate = 100,
                                                .MaxLevel = 10,
                                                .MinLevel = 8,
                                                .MonsterIndexChanceSlots = { 6, 6, 7, 7, 8, 9, 10, 0, 0, 0, 0, 0, 0, 0, 0 } };

    gOverworld08EncounterArea = (ENCOUNTERAREA){ .Name = "Route3General",
                                                .Area = (RECT){.left = 1040, .top = 3168, .right = 1568, .bottom = 3456 },
                                                .EncounterRate = 250,
                                                .MaxLevel = 15,
                                                .MinLevel = 13,
                                                .MonsterIndexChanceSlots = { 1, 2, 2, 3, 3, 4, 4, 5, 6, 9, 0, 0, 0, 0, 0 } };


    gEncounterAreas[0] = gNullEncounterArea;
    gEncounterAreas[1] = gOverworld01EncounterArea;
    gEncounterAreas[2] = gOverworld02EncounterArea;
    gEncounterAreas[3] = gOverworld03EncounterArea;
    gEncounterAreas[4] = gOverworld04EncounterArea;
    gEncounterAreas[5] = gOverworld05EncounterArea;
    gEncounterAreas[6] = gOverworld06EncounterArea;
    gEncounterAreas[7] = gOverworld07EncounterArea;
    gEncounterAreas[8] = gOverworld08EncounterArea;


    gGameIsRunning = TRUE;

    ASSERT(_countof(gPortCoords) == UNIQUE_TELEPADS_COUNT, "The gTeleportCoords array is the wrong size!");

    ////used for transfering gCamera, changing GameArea and screen position
    //from starting town to route1
    gTransitionAreas[2][1] = (TransitionArea){.TilesArea = (RECT) {.left = 560, .top = 4624, .right = 576, .bottom = 4656 + 16},
                                            .CameraPosAdd = (POINT) {.x = 320, .y = 0 },
                                            .TransAreaDestIndex = 3
    };

    //from route1 to starting town
    gTransitionAreas[3][1] = (TransitionArea){.TilesArea = (RECT) {.left = 544, .top = 4624, .right = 560, .bottom = 4656 + 16 },
                                            .CameraPosAdd = (POINT) {.x = -320, .y = 0 },
                                            .TransAreaDestIndex = 2
    };

    //from route1 to first battle town
    gTransitionAreas[3][2] = (TransitionArea){ .TilesArea = (RECT) {.left = 1360, .top = 4448, .right = 1376, .bottom = 4464 + 16 },
                                            .CameraPosAdd = (POINT) {.x = 320, .y = 0 },    
                                            .TransAreaDestIndex = 4
    };

    //from first battle town to route1
    gTransitionAreas[4][1] = (TransitionArea){ .TilesArea = (RECT) {.left = 1344, .top = 4448, .right = 1360, .bottom = 4464 + 16 },
                                            .CameraPosAdd = (POINT) {.x = -320, .y = 0 },
                                            .TransAreaDestIndex = 3
    };

    //from route2 to second battle town
    gTransitionAreas[6][1] = (TransitionArea){ .TilesArea = (RECT) {.left = 336, .top = 3664, .right = 432, .bottom = 3680 },
                                            .CameraPosAdd = (POINT) {.x = 0, .y = -192 },   //3456 dest     //3648 src
                                            .TransAreaDestIndex = 7
    };
    
    //from second battle town to route4
    gTransitionAreas[7][3] = (TransitionArea){ .TilesArea = (RECT) {.left = 0, .top = 0, .right = 0, .bottom = 0 },
                                            .CameraPosAdd = (POINT) {.x = 0, .y = 0 },
                                            .TransAreaDestIndex = 9
    };

    //from second battle town to route3 (cave)
    gTransitionAreas[7][2] = (TransitionArea){ .TilesArea = (RECT) {.left = 0, .top = 0, .right = 0, .bottom = 0 },
                                            .CameraPosAdd = (POINT) {.x = 0, .y = 0 },
                                            .TransAreaDestIndex = 8
    };

    //from second battle town to route2
    gTransitionAreas[7][1] = (TransitionArea){ .TilesArea = (RECT) {.left = 336, .top = 3664, .right = 432, .bottom = 3680 },
                                            .CameraPosAdd = (POINT) {.x = 0, .y = 192 },
                                            .TransAreaDestIndex = 6
    };


    gTransitionsPerMap[0] = 0;
    gTransitionsPerMap[1] = 0;
    gTransitionsPerMap[2] = 1;
    gTransitionsPerMap[3] = 2;
    gTransitionsPerMap[4] = 1;
    gTransitionsPerMap[5] = 0;
    gTransitionsPerMap[6] = 1;
    gTransitionsPerMap[7] = 3;



    ////from forest1 to route1
    gTeleport000 = (PORTCOORDS){ .AreaDest = gRoute01Area,
                                    .CameraDest = (UPOINT)  {.x = 752, .y = 4288 },    //512, 4048 src        //752, 4288 dest
                                    .PortDestIndex = 1,        //944, 4400 dest
                                    .WorldPos = (UPOINT) {.x = 624,  .y = 4240 },
    };


    ////from route1 to forest1
    gTeleport001 = (PORTCOORDS){ .AreaDest = gForest01Area,
                                    .CameraDest = (UPOINT)  {.x = 512, .y = 4048 },    //512, 4048 dest        //752, 4288 src
                                    .PortDestIndex = 0,        //624, 4240 dest
                                    .WorldPos = (UPOINT) {.x = 944,  .y = 4400 },
    };


    ////from starttown to home
    gTeleport002 = (PORTCOORDS){ .AreaDest = gHomeArea,
                                    .CameraDest = (UPOINT)  {.x = 3856, .y = 256 },    //3856, 256 dest
                                    .PortDestIndex = 3,       //4048, 432
                                    .WorldPos = (UPOINT) {.x = 256,  .y = 4496 },     
    };


    ////from home to starttown
    gTeleport003 = (PORTCOORDS){ .AreaDest = gStartingTownArea,
                                    .CameraDest = (UPOINT)  {.x = 96, .y = 4400 },    //96, 4400 dest
                                    .PortDestIndex = 2,       //256, 4496
                                    .WorldPos = (UPOINT) {.x = 4048,  .y = 432 },
    };


    ////from forest1 to first battle town
    gTeleport004 = (PORTCOORDS){ .AreaDest = gBattleTown01Area,
                                    .CameraDest = (UPOINT)  {.x = 1616, .y = 4256 },    //1616, 4256 dest        //944, 4048 src
                                    .PortDestIndex = 5,        //1808, 4320 dest
                                    .WorldPos = (UPOINT) {.x = 1232,  .y = 4240 },
    };


    ////from first battle town to forest1
    gTeleport005 = (PORTCOORDS){ .AreaDest = gForest01Area,
                                    .CameraDest = (UPOINT)  {.x = 944, .y = 4048 },    //944, 4048 dest        //1616, 4256 src
                                    .PortDestIndex = 4,        //1232, 4240 dest
                                    .WorldPos = (UPOINT) {.x = 1808,  .y = 4320 },
    };


    ////from forest1 to route2
    gTeleport006 = (PORTCOORDS){ .AreaDest = gRoute02Area,
                                    .CameraDest = (UPOINT)  {.x = 176, .y = 4160 },    //176, 4160 dest        //560, 3808 src
                                    .PortDestIndex = 7,        //368, 4304 dest
                                    .WorldPos = (UPOINT) {.x = 752,  .y = 3840 },
    };


    ////from forest1 to route2
    gTeleport007 = (PORTCOORDS){ .AreaDest = gForest01Area,
                                    .CameraDest = (UPOINT)  {.x = 560, .y = 3808 },    //176, 4160 src        //560, 3808 dest
                                    .PortDestIndex = 6,        //752, 3840 dest
                                    .WorldPos = (UPOINT) {.x = 368,  .y = 4304 },
    };


    ////from home to bedroom
    gTeleport008 = (PORTCOORDS){ .AreaDest = gBedroomArea,
                                    .CameraDest = (UPOINT)  {.x = 3856, .y = 512 },
                                    .PortDestIndex = 9,
                                    .WorldPos = (UPOINT) {.x = 4112,  .y = 320 },   //4048, 592
    };


    ////from bedroom to home
    gTeleport009 = (PORTCOORDS){ .AreaDest = gHomeArea,
                                    .CameraDest = (UPOINT)  {.x = 3856, .y = 256 },
                                    .PortDestIndex = 8,
                                    .WorldPos = (UPOINT) {.x = 4080,  .y = 592 },   //4112, 320
    };


    ////from starttown to lab
    gTeleport010 = (PORTCOORDS){ .AreaDest = gStartingLabArea,
                                    .CameraDest = (UPOINT)  {.x = 3856, .y = 0 },
                                    .PortDestIndex = 11,
                                    .WorldPos = (UPOINT) {.x = 416,  .y = 4528 },   //3952, 272
    };


    ////from lab to starttown
    gTeleport011 = (PORTCOORDS){ .AreaDest = gStartingTownArea,
                                    .CameraDest = (UPOINT)  {.x = 176, .y = 4400 },  //96, 4400
                                    .PortDestIndex = 10,
                                    .WorldPos = (UPOINT) {.x = 3952,  .y = 208 },   //416, 4528
    };

    gPortCoords[0] = gTeleport000;
    gPortCoords[1] = gTeleport001;
    gPortCoords[2] = gTeleport002;
    gPortCoords[3] = gTeleport003;
    gPortCoords[4] = gTeleport004;
    gPortCoords[5] = gTeleport005;
    gPortCoords[6] = gTeleport006;
    gPortCoords[7] = gTeleport007;
    gPortCoords[8] = gTeleport008;
    gPortCoords[9] = gTeleport009;
    gPortCoords[10] = gTeleport010;
    gPortCoords[11] = gTeleport011;

}


// If WINDOW_FLAG_HORIZ_CENTERED is specified, the x coordinate is ignored and may be zero.
// If WINDOW_FLAG_VERT_CENTERED is specified, the y coordinate is ignored and may be zero.
// BackgroundColor is ignored and may be NULL if WINDOW_FLAG_OPAQUE is not set.
// BorderColor is ignored and may be NULL if WINDOW_FLAG_BORDERED is not set.
// Either the BORDERED or the OPAQUE flag needs to be set, or both, or else the window would just be
// transparent and invisible. The window border will cut into the inside of the window area.
void DrawWindow(
    _In_opt_ uint16_t x,
    _In_opt_ uint16_t y,
    _In_ int16_t Width,
    _In_ int16_t Height,
    _In_opt_ PIXEL32* BorderColor,
    _In_opt_ PIXEL32* BackgroundColor,
    _In_opt_ PIXEL32* ShadowColor,
    _In_ DWORD Flags)
{
    if (Flags & WINDOW_FLAG_HORIZ_CENTERED)
    {
        x = (GAME_RES_WIDTH / 2) - (Width / 2);
    }

    if (Flags & WINDOW_FLAG_VERT_CENTERED)
    {
        y = (GAME_RES_HEIGHT / 2) - (Height / 2);
    }

    ASSERT((x + Width <= GAME_RES_WIDTH) && (y + Height <= GAME_RES_HEIGHT), "Window is off the screen!");

    ASSERT((Flags & WINDOW_FLAG_BORDERED) || (Flags & WINDOW_FLAG_OPAQUE), "Window must have either the BORDERED or the OPAQUE flags (or both) set!");

    int32_t StartingScreenPixel = ((GAME_RES_WIDTH * GAME_RES_HEIGHT) - GAME_RES_WIDTH) - (GAME_RES_WIDTH * y) + x;

    if (Flags & WINDOW_FLAG_OPAQUE)
    {
        ASSERT(BackgroundColor != NULL, "WINDOW_FLAG_OPAQUE is set but BackgroundColor is NULL!");

        for (int Row = 0; Row < Height; Row++)
        {
            int MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * Row);

            // If the user wants rounded corners, don't draw the first and last pixels on the first and last rows.
            // Get a load of this sweet ternary action:
            for (int Pixel = ((Flags & WINDOW_FLAG_ROUNDED) && (Row == 0 || Row == Height - 1)) ? 1 : 0;
                Pixel < Width - ((Flags & WINDOW_FLAG_ROUNDED) && (Row == 0 || Row == Height - 1)) ? 1 : 0;
                Pixel++)
            {
                memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, BackgroundColor, sizeof(PIXEL32));
            }
        }
    }

    if (Flags & WINDOW_FLAG_BORDERED)
    {
        ASSERT(BorderColor != NULL, "WINDOW_FLAG_BORDERED is set but BorderColor is NULL!");

        // Draw the top of the border.
        int MemoryOffset = StartingScreenPixel;

        for (int Pixel = ((Flags & WINDOW_FLAG_ROUNDED) ? 1 : 0);
            Pixel < Width - ((Flags & WINDOW_FLAG_ROUNDED) ? 1 : 0);
            Pixel++)
        {
            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, BorderColor, sizeof(PIXEL32));
        }

        // Draw the bottom of the border.
        MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * (Height - 1));

        for (int Pixel = ((Flags & WINDOW_FLAG_ROUNDED) ? 1 : 0);
            Pixel < Width - ((Flags & WINDOW_FLAG_ROUNDED) ? 1 : 0);
            Pixel++)
        {
            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, BorderColor, sizeof(PIXEL32));
        }

        // Draw one pixel on the left side and the right for each row of the border, from the top down.
        for (int Row = 1; Row < Height - 1; Row++)
        {
            MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * Row);

            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset, BorderColor, sizeof(PIXEL32));

            MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * Row) + (Width - 1);

            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset, BorderColor, sizeof(PIXEL32));
        }

        // Recursion ahead!
        // If the user wants a thick window, just draw a smaller concentric bordered window inside the existing window.
        if (Flags & WINDOW_FLAG_THICK)
        {
            DrawWindow(x + 1, y + 1, Width - 2, Height - 2, BorderColor, NULL, NULL, WINDOW_FLAG_BORDERED);
        }
    }

    // TODO: If a window was placed at the edge of the screen, the shadow effect might attempt
    // to draw off-screen and crash! i.e. make sure there's room to draw the shadow before attempting!
    if (Flags & WINDOW_FLAG_SHADOWED)
    {
        ASSERT(ShadowColor != NULL, "WINDOW_FLAG_SHADOW is set but ShadowColor is NULL!");

        // Draw the bottom of the shadow.
        int MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * Height);

        for (int Pixel = 1;
            Pixel < Width + ((Flags & WINDOW_FLAG_ROUNDED) ? 0 : 1);
            Pixel++)
        {
            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, ShadowColor, sizeof(PIXEL32));
        }

        // Draw one pixel on the right side for each row of the border, from the top down.
        for (int Row = 1; Row < Height; Row++)
        {
            MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * Row) + Width;

            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset, ShadowColor, sizeof(PIXEL32));
        }

        // Draw one shadow pixel in the bottom-right corner to compensate for rounded corner.
        if (Flags & WINDOW_FLAG_ROUNDED)
        {
            MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * (Height - 1)) + (Width - 1);

            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset, ShadowColor, sizeof(PIXEL32));
        }
    }
}

void ApplyFadeIn(_In_ uint64_t FrameCounter, _In_ PIXEL32 DefaultTextColor, _Inout_ PIXEL32* TextColor, _Inout_opt_ int16_t* BrightnessAdjustment)
{
    ASSERT(_countof(gFadeBrightnessGradient) == FADE_DURATION_FRAMES, "gFadeBrightnessGradient has too few elements!");

    int16_t LocalBrightnessAdjustment;

    if (FrameCounter > FADE_DURATION_FRAMES)
    {
        return;
    }

    if (FrameCounter == FADE_DURATION_FRAMES)
    {
        gInputEnabled = TRUE;
        LocalBrightnessAdjustment = 0;
    }
    else
    {
        gInputEnabled = FALSE;
        LocalBrightnessAdjustment = gFadeBrightnessGradient[FrameCounter];
    }

    if (BrightnessAdjustment != NULL)
    {
        *BrightnessAdjustment = LocalBrightnessAdjustment;
    }

    TextColor->Colors.Red = (uint8_t)(min(255, max(0, DefaultTextColor.Colors.Red + LocalBrightnessAdjustment)));
    TextColor->Colors.Blue = (uint8_t)(min(255, max(0, DefaultTextColor.Colors.Blue + LocalBrightnessAdjustment)));
    TextColor->Colors.Green = (uint8_t)(min(255, max(0, DefaultTextColor.Colors.Green + LocalBrightnessAdjustment)));
}

void EnterDialogue(void)
{
    //&gSoundSurprised or something
    PlayGameSound(&gSoundMenuChoose);
    gGamePaused = TRUE;
    gOverWorldControls = FALSE;
}

void ExitDialogue(void)
{
    gGamePaused = FALSE;
    gDialogueControls = FALSE;
    gOverWorldControls = TRUE;
}

//MAX characters per row = 32, MAX rows = 7, only input needed is text
//Use character "\n" with no spaces behind for next row (spaces after will indent)
void DrawDialogueBox(_In_ char* Dialogue, _In_opt_ uint64_t Counter, _In_opt_ DWORD Flags)
{
    static uint8_t DialogueCharactersToShow;
    static uint8_t DialogueCharactersWritten;
    static uint8_t DialogueRowsToShow;
    char DialogueLineScratch[32] = { 0 };

    char InString[224] = { 0 };
    char* NextToken = NULL;
    char Separator[] = "\n";

    char* StrPtr[8];

    DrawWindow(1, 170, 192, 64, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_HORIZ_CENTERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED | WINDOW_FLAG_THICK | WINDOW_FLAG_BORDERED | WINDOW_FLAG_ROUNDED);
    if (strlen(Dialogue) <= 32 * MAX_DIALOGUE_ROWS && strlen(Dialogue) > 0)
    {
        strcpy_s(InString, 224, Dialogue);        ////need to define max msg length bc sizeof() and strlen() both result in errors

        StrPtr[1] = strtok_s(InString, Separator, &NextToken);       ////split string into pieces using \n as a separator
        StrPtr[2] = strtok_s(NULL, Separator, &NextToken);
        StrPtr[3] = strtok_s(NULL, Separator, &NextToken);
        StrPtr[4] = strtok_s(NULL, Separator, &NextToken);
        StrPtr[5] = strtok_s(NULL, Separator, &NextToken);
        StrPtr[6] = strtok_s(NULL, Separator, &NextToken);
        StrPtr[7] = strtok_s(NULL, Separator, &NextToken);

        if (((Counter % (gRegistryParams.TextSpeed + 1) == 0) && (gRegistryParams.TextSpeed < 4)) && gFinishedDialogueTextAnimation == FALSE)
        {
            if (DialogueCharactersToShow <= strlen(StrPtr[DialogueRowsToShow + 1]))
            {
                DialogueCharactersToShow++;
                DialogueCharactersWritten++;
            }
            else if (DialogueCharactersToShow > strlen(StrPtr[DialogueRowsToShow + 1]) && DialogueCharactersWritten < strlen(Dialogue))   ////TODO FIX BUG: when using \n, gFinishedDialogueTextAnimation will be set at shortest line finishing, not once all text is finished
            {
                DialogueCharactersToShow = 0;
                DialogueRowsToShow++;
            }

            if (DialogueCharactersWritten > strlen(Dialogue))
            {
                DialogueCharactersWritten = 0;
                DialogueCharactersToShow = 0;
                DialogueRowsToShow = 0;
                goto StartBlit;
            }
        }
        else if (gRegistryParams.TextSpeed == 4 || gFinishedDialogueTextAnimation == TRUE)
        {

        StartBlit:


            BlitStringToBuffer(StrPtr[1], &g6x7Font, &COLOR_BLACK, 100, 174 + ((0) * 8));                 //////every time \n is called add a row to the dialogue box
            if (StrPtr[2] != NULL)
            {
                BlitStringToBuffer(StrPtr[2], &g6x7Font, &COLOR_BLACK, 100, 174 + ((1) * 8));
            }
            if (StrPtr[3] != NULL)
            {
                BlitStringToBuffer(StrPtr[3], &g6x7Font, &COLOR_BLACK, 100, 174 + ((2) * 8));
            }
            if (StrPtr[4] != NULL)
            {
                BlitStringToBuffer(StrPtr[4], &g6x7Font, &COLOR_BLACK, 100, 174 + ((3) * 8));
            }
            if (StrPtr[5] != NULL)
            {
                BlitStringToBuffer(StrPtr[5], &g6x7Font, &COLOR_BLACK, 100, 174 + ((4) * 8));
            }
            if (StrPtr[6] != NULL)
            {
                BlitStringToBuffer(StrPtr[6], &g6x7Font, &COLOR_BLACK, 100, 174 + ((5) * 8));
            }
            if (StrPtr[7] != NULL)
            {
                BlitStringToBuffer(StrPtr[7], &g6x7Font, &COLOR_BLACK, 100, 174 + ((6) * 8));
            }
            BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, 276, 224);
            gFinishedDialogueTextAnimation = TRUE;
            gDialogueControls = TRUE;
            DialogueCharactersWritten = 0;
            DialogueCharactersToShow = 0;
            DialogueRowsToShow = 0;

        }

        if (!gFinishedDialogueTextAnimation)
        {
            switch (DialogueRowsToShow)
            {
                case 0:
                {
                    snprintf(DialogueLineScratch, DialogueCharactersToShow, "%s", StrPtr[1]);
                    BlitStringToBuffer(DialogueLineScratch, &g6x7Font, &COLOR_BLACK, 100, 174 + ((0) * 8));                 //////every time \n is called add a row to the dialogue box
                    break;
                }
                case 1:
                {
                    BlitStringToBuffer(StrPtr[1], &g6x7Font, &COLOR_BLACK, 100, 174 + ((0) * 8));
                    snprintf(DialogueLineScratch, DialogueCharactersToShow, "%s", StrPtr[2]);
                    BlitStringToBuffer(DialogueLineScratch, &g6x7Font, &COLOR_BLACK, 100, 174 + ((1) * 8));
                    break;
                }
                case 2:
                {
                    BlitStringToBuffer(StrPtr[1], &g6x7Font, &COLOR_BLACK, 100, 174 + ((0) * 8));
                    BlitStringToBuffer(StrPtr[2], &g6x7Font, &COLOR_BLACK, 100, 174 + ((1) * 8));
                    snprintf(DialogueLineScratch, DialogueCharactersToShow, "%s", StrPtr[3]);
                    BlitStringToBuffer(DialogueLineScratch, &g6x7Font, &COLOR_BLACK, 100, 174 + ((2) * 8));
                    break;
                }
                case 3:
                {
                    BlitStringToBuffer(StrPtr[1], &g6x7Font, &COLOR_BLACK, 100, 174 + ((0) * 8));
                    BlitStringToBuffer(StrPtr[2], &g6x7Font, &COLOR_BLACK, 100, 174 + ((1) * 8));
                    BlitStringToBuffer(StrPtr[3], &g6x7Font, &COLOR_BLACK, 100, 174 + ((2) * 8));
                    snprintf(DialogueLineScratch, DialogueCharactersToShow, "%s", StrPtr[4]);
                    BlitStringToBuffer(DialogueLineScratch, &g6x7Font, &COLOR_BLACK, 100, 174 + ((3) * 8));
                    break;
                }
                case 4:
                {
                    BlitStringToBuffer(StrPtr[1], &g6x7Font, &COLOR_BLACK, 100, 174 + ((0) * 8));
                    BlitStringToBuffer(StrPtr[2], &g6x7Font, &COLOR_BLACK, 100, 174 + ((1) * 8));
                    BlitStringToBuffer(StrPtr[3], &g6x7Font, &COLOR_BLACK, 100, 174 + ((2) * 8));
                    BlitStringToBuffer(StrPtr[4], &g6x7Font, &COLOR_BLACK, 100, 174 + ((3) * 8));
                    snprintf(DialogueLineScratch, DialogueCharactersToShow, "%s", StrPtr[5]);
                    BlitStringToBuffer(DialogueLineScratch, &g6x7Font, &COLOR_BLACK, 100, 174 + ((4) * 8));
                    break;
                }
                case 5:
                {
                    BlitStringToBuffer(StrPtr[1], &g6x7Font, &COLOR_BLACK, 100, 174 + ((0) * 8));
                    BlitStringToBuffer(StrPtr[2], &g6x7Font, &COLOR_BLACK, 100, 174 + ((1) * 8));
                    BlitStringToBuffer(StrPtr[3], &g6x7Font, &COLOR_BLACK, 100, 174 + ((2) * 8));
                    BlitStringToBuffer(StrPtr[4], &g6x7Font, &COLOR_BLACK, 100, 174 + ((3) * 8));
                    BlitStringToBuffer(StrPtr[5], &g6x7Font, &COLOR_BLACK, 100, 174 + ((4) * 8));
                    snprintf(DialogueLineScratch, DialogueCharactersToShow, "%s", StrPtr[6]);
                    BlitStringToBuffer(DialogueLineScratch, &g6x7Font, &COLOR_BLACK, 100, 174 + ((5) * 8));
                    break;
                }
                case 6:
                {
                    BlitStringToBuffer(StrPtr[1], &g6x7Font, &COLOR_BLACK, 100, 174 + ((0) * 8));
                    BlitStringToBuffer(StrPtr[2], &g6x7Font, &COLOR_BLACK, 100, 174 + ((1) * 8));
                    BlitStringToBuffer(StrPtr[3], &g6x7Font, &COLOR_BLACK, 100, 174 + ((2) * 8));
                    BlitStringToBuffer(StrPtr[4], &g6x7Font, &COLOR_BLACK, 100, 174 + ((3) * 8));
                    BlitStringToBuffer(StrPtr[5], &g6x7Font, &COLOR_BLACK, 100, 174 + ((4) * 8));
                    BlitStringToBuffer(StrPtr[6], &g6x7Font, &COLOR_BLACK, 100, 174 + ((5) * 8));
                    snprintf(DialogueLineScratch, DialogueCharactersToShow, "%s", StrPtr[7]);
                    BlitStringToBuffer(DialogueLineScratch, &g6x7Font, &COLOR_BLACK, 100, 174 + ((6) * 8));
                    break;
                }
            }
        }
    }
    else
    {
        BlitStringToBuffer("MSG UNDEFINED CHECK LOG FILE", &g6x7Font, &COLOR_BLACK, 101, 174);
        LogMessageA(LL_ERROR, "[%s] ERROR: String '%d' was over 224 (32chars * 7rows) characters!", __FUNCTION__, Dialogue);
    }

    /*if (Flags && DIALOGUE_FLAG_BATTLE && gFinishedDialogueTextAnimation)
    {

    }*/
}


void DrawMonsterHpBar(uint16_t x, uint16_t y, uint8_t percentHp100, uint8_t percentExp100, uint8_t monsterLevel, char* monsterNickname, BOOL showExpBar)
{
    DrawWindow(x - 3, y - 10, 100 + 16, 2 + 13, &COLOR_BLACK, &COLOR_DARK_WHITE, NULL, WINDOW_FLAG_ROUNDED | WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE);

    int32_t StartingScreenPixel = ((GAME_RES_WIDTH * GAME_RES_HEIGHT) - GAME_RES_WIDTH) - (GAME_RES_WIDTH * y) + x;

    for (int Row = 0; Row < 2; Row++)
    {

        for (int Pixel = 0; Pixel < 100; Pixel++)
        {
            int MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * Row);
            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, &COLOR_DARK_GREEN, sizeof(PIXEL32));
        }

        for (int Pixel = 0; Pixel < 100 - percentHp100; Pixel++)
        {
            int MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * Row);
            if (percentHp100 < 51)
            {
                memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, &COLOR_LIGHT_GREEN, sizeof(PIXEL32));
            }
            else if (percentHp100 > 50 && percentHp100 < 91)
            {
                memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, &COLOR_GOLD, sizeof(PIXEL32));
            }
            else if (percentHp100 > 90)
            {
                memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, &COLOR_DARK_RED, sizeof(PIXEL32));
            }
        }
    }
    if (showExpBar)
    {
        for (int Pixel = 0; Pixel < 100; Pixel++)
        {
            int MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * 3) + 8;
            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, &COLOR_DARK_BLUE, sizeof(PIXEL32));
        }

        for (int Pixel = 0; Pixel < 100 - percentExp100; Pixel++)
        {
            int MemoryOffset = StartingScreenPixel - (GAME_RES_WIDTH * 3) + 8;
            memcpy((PIXEL32*)gBackBuffer.Memory + MemoryOffset + Pixel, &COLOR_NEON_BLUE, sizeof(PIXEL32));
        }
    }

    uint16_t LevelSize = snprintf(NULL, 0, "%d", monsterLevel);
    char* LevelString = malloc(LevelSize + 6);
    snprintf(LevelString, LevelSize + 6, "Lvl: %d", monsterLevel);
    BlitStringToBuffer(LevelString, &g6x7Font, &COLOR_DARK_RED, x - 1, y - 8);

    BlitStringToBuffer(monsterNickname, &g6x7Font, &COLOR_DARK_RED, x + 11 + 37, y - 8);
}


//returns true if finished writting text so that wait screen may be initiated
BOOL BlitBattleStateTextBox_Text(BATTLESTATE battlestate_wait, uint8_t battleTextLineCount, uint64_t frameCounter)
{
    static uint8_t BattleTextLineCharactersToShow = 0;
    static uint16_t BattleTextLineCharactersWritten = 0;
    static uint8_t BattleTextRowsToShow = 0;

    char BattleTextLineScratch[40] = { 0 };

    if (gRegistryParams.TextSpeed == 4 || gFinishedBattleTextAnimation == TRUE)
    {
        gCurrentBattleState = battlestate_wait;
    }
    else
    {
        DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);

        if ((frameCounter % (gRegistryParams.TextSpeed + 1) == 0) && (gFinishedBattleTextAnimation == FALSE))
        {
            if (BattleTextLineCharactersToShow <= strlen(gBattleTextLine[BattleTextRowsToShow + 1]))
            {
                BattleTextLineCharactersToShow++;
                BattleTextLineCharactersWritten++;
            }
            else if ((BattleTextLineCharactersToShow > strlen(gBattleTextLine[BattleTextRowsToShow + 1])) && (BattleTextRowsToShow + 1 <= (battleTextLineCount)))
            {
                BattleTextLineCharactersToShow = 0;
                BattleTextRowsToShow++;
            }
            else if (BattleTextRowsToShow + 1 > (battleTextLineCount))
            {
                BattleTextLineCharactersToShow = 0;
                BattleTextLineCharactersWritten = 0;
                BattleTextRowsToShow = 0;
                gCurrentBattleState = battlestate_wait;
                gFinishedBattleTextAnimation = TRUE;

                return(TRUE);
            }
        }

        if (!gFinishedBattleTextAnimation)
        {
            switch (BattleTextRowsToShow)
            {
                case 0:
                {
                    snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[1]);
                    BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));                 //////every time \n is called add a row to the dialogue box
                    break;
                }
                case 1:
                {
                    BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                    snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[2]);
                    BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                    break;
                }
                case 2:
                {
                    BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                    snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[3]);
                    BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                    break;
                }
                case 3:
                {
                    BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                    snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[4]);
                    BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                    break;
                }
                case 4:
                {
                    BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                    snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[5]);
                    BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                    break;
                }
                case 5:
                {
                    BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                    snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[6]);
                    BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
                    break;
                }
                case 6:
                {
                    BlitStringToBuffer((char*)gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
                    BlitStringToBuffer((char*)gBattleTextLine[6], &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
                    snprintf(BattleTextLineScratch, BattleTextLineCharactersToShow, "%s", (char*)gBattleTextLine[7]);
                    BlitStringToBuffer(BattleTextLineScratch, &g6x7Font, &COLOR_BLACK, 66, 174 + ((7) * 8));
                    break;
                }
            }
        }
    }
    return(FALSE);
}

void BlitBattleStateTextBox_Wait(uint8_t battleTextLineCount)
{

    DrawWindow(64, 180, 256, 56, &COLOR_NES_WHITE, &COLOR_DARK_WHITE, &COLOR_DARK_GRAY, WINDOW_FLAG_BORDERED | WINDOW_FLAG_OPAQUE | WINDOW_FLAG_SHADOWED);


    BlitStringToBuffer(gBattleTextLine[1], &g6x7Font, &COLOR_BLACK, 66, 174 + ((1) * 8));
    if (battleTextLineCount > 1)
    {
        BlitStringToBuffer(gBattleTextLine[2], &g6x7Font, &COLOR_BLACK, 66, 174 + ((2) * 8));
    }
    if (battleTextLineCount > 2)
    {
        BlitStringToBuffer(gBattleTextLine[3], &g6x7Font, &COLOR_BLACK, 66, 174 + ((3) * 8));
    }
    if (battleTextLineCount > 3)
    {
        BlitStringToBuffer(gBattleTextLine[4], &g6x7Font, &COLOR_BLACK, 66, 174 + ((4) * 8));
    }
    if (battleTextLineCount > 4)
    {
        BlitStringToBuffer(gBattleTextLine[5], &g6x7Font, &COLOR_BLACK, 66, 174 + ((5) * 8));
    }
    if (battleTextLineCount > 5)
    {
        BlitStringToBuffer(gBattleTextLine[6], &g6x7Font, &COLOR_BLACK, 66, 174 + ((6) * 8));
    }
    if (battleTextLineCount > 6)
    {
        BlitStringToBuffer(gBattleTextLine[7], &g6x7Font, &COLOR_BLACK, 66, 174 + ((7) * 8));
    }
    BlitStringToBuffer("»", &g6x7Font, &COLOR_BLACK, 312, 228);
    gFinishedBattleTextAnimation = TRUE;

}

void ReSortUsableitems(void)
{
    uint16_t ItemCount = 0;
    for (uint16_t i = 0; i < NUM_USABLE_ITEMS; i++)
    {
        if (gUseableItems[i].Count > 0)
        {
            gUseableItems[i].HasItem = TRUE;
        }
        else
        {
            gUseableItems[i].HasItem = FALSE;
        }
        if (gUseableItems[i].HasItem == TRUE)
        {
            gUseableHasItemSort[ItemCount] = i;
            ItemCount++;
        }
        if (i == NUM_USABLE_ITEMS - 1)
        {
            gUseableItemCount = ItemCount;
            for (uint16_t j = 0; j < NUM_USABLE_ITEMS - ItemCount; j++)
            {
                gUseableHasItemSort[ItemCount + j] = 0xFFFF;
            }

        }
    }
}

void ReSortEquipableitems(void)
{
    uint16_t ItemCount = 0;
    for (uint16_t i = 0; i < NUM_EQUIP_ITEMS; i++)
    {
        if (gEquipableItems[i].Count > 0)
        {
            gEquipableItems[i].HasItem = TRUE;
        }
        else
        {
            gEquipableItems[i].HasItem = FALSE;
        }
        if (gEquipableItems[i].HasItem == TRUE)
        {
            gEquipHasItemSort[ItemCount] = i;
            ItemCount++;
        }
        if (i == NUM_EQUIP_ITEMS - 1)
        {
            gEquipItemCount = ItemCount;
            for (uint16_t j = 0; j < NUM_EQUIP_ITEMS - ItemCount; j++)
            {
                gEquipHasItemSort[ItemCount + j] = 0xFFFF;
            }

        }
    }
}

void ReSortValuableitems(void)
{
    uint16_t ItemCount = 0;
    for (uint16_t i = 0; i < NUM_VALUABLE_ITEMS; i++)
    {
        if (gValuableItems[i].Count > 0)
        {
            gValuableItems[i].HasItem = TRUE;
        }
        else
        {
            gValuableItems[i].HasItem = FALSE;
        }
        if (gValuableItems[i].HasItem == TRUE)
        {
            gValuableHasItemSort[ItemCount] = i;
            ItemCount++;
        }
        if (i == NUM_VALUABLE_ITEMS - 1)
        {
            gValuableItemCount = ItemCount;
            for (uint16_t j = 0; j < NUM_VALUABLE_ITEMS - ItemCount; j++)
            {
                gValuableHasItemSort[ItemCount + j] = 0xFFFF;
            }

        }
    }
}

void ReSortAdventureitems(void)
{
    uint16_t ItemCount = 0;
    for (uint16_t i = 0; i < NUM_ADVENTURE_ITEMS; i++)
    {
        if (gAdventureItems[i].Count > 0)
        {
            gAdventureItems[i].HasItem = TRUE;
        }
        else
        {
            gAdventureItems[i].HasItem = FALSE;
        }
        if (gAdventureItems[i].HasItem == TRUE)
        {
            gAdventureHasItemSort[ItemCount] = i;
            ItemCount++;
        }
        if (i == NUM_ADVENTURE_ITEMS - 1)
        {
            gAdventureItemCount = ItemCount;
            for (uint16_t j = 0; j < NUM_ADVENTURE_ITEMS - ItemCount; j++)
            {
                gAdventureHasItemSort[ItemCount + j] = 0xFFFF;
            }

        }
    }
}

void ReSortLearnableMovesFromMonster(struct DriveMonster* driveMonster)
{
    uint16_t LearnableCount = 0;
    uint16_t index = GetMonsterData(driveMonster, MONSTER_DATA_INDEX, NULL);
    uint16_t level = GetLevelFromDriveMonsterExp(driveMonster);

    for (uint16_t i = 0; gLevelUpMoves[index][i].move != LEVEL_UP_END; i++)
    {
        if (gLevelUpMoves[index][i].level > level)
        {
            break;
        }
        if (gLevelUpMoves[index][i].level == 0)
        {
            continue;
        }
        if (gLevelUpMoves[index][i].move != driveMonster->Moves[0] && gLevelUpMoves[index][i].move != driveMonster->Moves[1] && gLevelUpMoves[index][i].move != driveMonster->Moves[2] && gLevelUpMoves[index][i].move != driveMonster->Moves[3])
        {
            gLearnableMoveSort[LearnableCount] = gLevelUpMoves[index][i].move;
            LearnableCount++;
        }
    }
    gLearnableMoveCount = LearnableCount;

    for (uint16_t j = 0; j < NUM_BATTLEMOVES - LearnableCount; j++)
    {
        gLearnableMoveSort[LearnableCount + j] = 0xFFFF;
    }
}


void BlitItemDescription(char* description)
{
    //char DescLineScratch[30] = { 0 };

    char InString[210] = { 0 };
    char* NextToken = NULL;
    char Separator[] = "\n";

    char* StrPtr[8];

    if (strlen(description) <= 30 * MAX_DIALOGUE_ROWS && strlen(description) > 0)
    {
        strcpy_s(InString, 210, description);        ////need to define max msg length bc sizeof() and strlen() both result in errors

        StrPtr[1] = strtok_s(InString, Separator, &NextToken);       ////split string into pieces using \n as a separator
        StrPtr[2] = strtok_s(NULL, Separator, &NextToken);
        StrPtr[3] = strtok_s(NULL, Separator, &NextToken);
        StrPtr[4] = strtok_s(NULL, Separator, &NextToken);
        StrPtr[5] = strtok_s(NULL, Separator, &NextToken);
        StrPtr[6] = strtok_s(NULL, Separator, &NextToken);
        StrPtr[7] = strtok_s(NULL, Separator, &NextToken);

        BlitStringToBuffer(StrPtr[1], &g6x7Font, &COLOR_BLACK, 10, 155 + ((0) * 8));                 //////every time \n is called add a row to the dialogue box
        if (StrPtr[2] != NULL)
        {
            BlitStringToBuffer(StrPtr[2], &g6x7Font, &COLOR_BLACK, 10, 155 + ((1) * 8));
        }
        if (StrPtr[3] != NULL)
        {
            BlitStringToBuffer(StrPtr[3], &g6x7Font, &COLOR_BLACK, 10, 155 + ((2) * 8));
        }
        if (StrPtr[4] != NULL)
        {
            BlitStringToBuffer(StrPtr[4], &g6x7Font, &COLOR_BLACK, 10, 155 + ((3) * 8));
        }
        if (StrPtr[5] != NULL)
        {
            BlitStringToBuffer(StrPtr[5], &g6x7Font, &COLOR_BLACK, 10, 155 + ((4) * 8));
        }
        if (StrPtr[6] != NULL)
        {
            BlitStringToBuffer(StrPtr[6], &g6x7Font, &COLOR_BLACK, 10, 155 + ((5) * 8));
        }
        if (StrPtr[7] != NULL)
        {
            BlitStringToBuffer(StrPtr[7], &g6x7Font, &COLOR_BLACK, 10, 155 + ((6) * 8));
        }
    }
    else
    {
        BlitStringToBuffer("MSG UNDEFINED CHECK LOG FILE", &g6x7Font, &COLOR_BLACK, 10, 155);
        LogMessageA(LL_ERROR, "[%s] ERROR: String '%d' was over 224 (32chars * 7rows) characters!", __FUNCTION__, description);
    }
}


BOOL CanPlayerAffordCurrencyCost(uint32_t currencycost)
{
    if (gPlayer.Currency >= currencycost)
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

uint8_t* StringCopy(uint8_t* dest, const uint8_t* source)
{
    while (*source != END_OF_STRING)
    {
        *dest = *source; 
        dest++;
        source++;
    }

    *dest = END_OF_STRING;
    return dest;
}

//returns a random 16 bit unsigned int
uint16_t Random16(void)
{
    DWORD Random;
    uint16_t Random16;
    rand_s((unsigned int*)&Random);
    Random16 = (uint16_t)Random;
    return (Random16);
}
