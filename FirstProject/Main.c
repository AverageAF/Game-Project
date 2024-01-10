
#pragma warning(push, 3)
#include <stdio.h>
#include <windows.h>

#include <psapi.h>

#include <emmintrin.h>
#pragma warning(pop)

#include <stdint.h>
#include "Main.h"

#pragma comment(lib, "Winmm.lib")

HWND gGameWindow;

BOOL gGameIsRunning;

GAMEBITMAP gBackBuffer;

GAMEBITMAP g6x7Font;

GAME_PERFORMANCE_DATA gGamePerformanceData;

PLAYER gPlayer;

BOOL gWindowHasFocus;

REGISTRYPARAMS gRegistryParams;

int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, int CmdShow)
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

    if ((NtDllModuleHandle = GetModuleHandleA("ntdll.dll")) == NULL)
    {
        LogMessageA(LogError, "[%s] Couldn't load ntdll.dll! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Couldn't load ntdll.dll!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if ((NtQueryTimerResolution = (_NtQueryTimerResolution)GetProcAddress(NtDllModuleHandle, "NtQueryTimerResolution")) == NULL)
    {
        LogMessageA(LogError, "[%s] Couldn't find function NtQueryTimerResolution in ntdll.dll! GetProcAddress Failed! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Couldn't find function NtQueryTimerResolution in ntdll.dll!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    NtQueryTimerResolution(&gGamePerformanceData.MinimumTimerResolution, &gGamePerformanceData.MaximumTimerResolution, &gGamePerformanceData.CurrentTimerResolution);

    GetSystemInfo(&gGamePerformanceData.SystemInfo);
    GetSystemTimeAsFileTime((FILETIME*)&gGamePerformanceData.PreviousSystemTime);


        
    if (GameIsAlreadyRunning() == TRUE)
    {
        LogMessageA(Warn, "[%s] Another instance is already running!", __FUNCTION__);
        MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (timeBeginPeriod(1) == TIMERR_NOCANDO)
    {
        LogMessageA(LogError, "[%s] Failed to set global timer resolution!", __FUNCTION__);
        MessageBoxA(NULL, "Failed to set timer resolution!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }

    if (SetPriorityClass(ProcessHandle, HIGH_PRIORITY_CLASS) == 0)
    {
        LogMessageA(LogError, "[%s] Failed to set process priority! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed to set process priority!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST) == 0)
    {
        LogMessageA(LogError, "[%s] Failed to set thread priority! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed to set thread priority!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (CreateMainGameWindow() != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] CreateMainGameWindow failed!", __FUNCTION__);
        goto Exit;
    }

    if ((Load32BppBitmapFromFile("..\\Assets\\PixelFont(6x7).bmpx", &g6x7Font)) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading PixelFont(6x7).bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
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
        LogMessageA(LogError, "[%s] Failed to allocate memory for backbuffer! Error 0x%08lx!", __FUNCTION__, GetLastError());
        MessageBoxA(NULL, "Failed to allocate memory for drawing surface!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    memset(gBackBuffer.Memory, 0x00, GAME_DRAWING_AREA_MEMORY_SIZE);

    if (InitializePlayer() != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Failed to initialize player sprite!", __FUNCTION__);
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
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = MainWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = GetModuleHandleA(NULL);
    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    WindowClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = GAME_NAME "_WINDOWCLASS";

    if (RegisterClassExA(&WindowClass) == 0)
    {
        Result = GetLastError();
        LogMessageA(LogError, "[%s] Window Registration Failed! RegisterClassExa Failed! Error 0x%08lx!", __FUNCTION__, Result);
            MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            goto Exit;
    }

    gGameWindow = CreateWindowExA(0, WindowClass.lpszClassName, "Window Title Name", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, GetModuleHandleA(NULL), NULL);

    if (gGameWindow == NULL)
    {
        Result = GetLastError();
        LogMessageA(LogError, "[%s] CreateWindowExA failed! Error 0x%08lx!", __FUNCTION__, Result);
            MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            goto Exit;
    }

    gGamePerformanceData.MonitorInfo.cbSize = sizeof(MONITORINFO);

    if (GetMonitorInfoA(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gGamePerformanceData.MonitorInfo) == 0)
    {
        Result = ERROR_MONITOR_NO_DESCRIPTOR;       // GetMonitorInfoA cannot Result = GetLastError
        LogMessageA(LogError, "[%s] GetMonitorInfoA(MonitorFromWindow()) failed! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    gGamePerformanceData.MonitorWidth = gGamePerformanceData.MonitorInfo.rcMonitor.right - gGamePerformanceData.MonitorInfo.rcMonitor.left;
    gGamePerformanceData.MonitorHeight = gGamePerformanceData.MonitorInfo.rcMonitor.bottom - gGamePerformanceData.MonitorInfo.rcMonitor.top;

    if (SetWindowLongPtrA(gGameWindow, GWL_STYLE, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_OVERLAPPEDWINDOW) == 0)
    {
        Result = GetLastError();
        LogMessageA(LogError, "[%s] SetWindowLongPtrA failed! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
    }

    if (SetWindowPos(gGameWindow, HWND_TOP, gGamePerformanceData.MonitorInfo.rcMonitor.left, gGamePerformanceData.MonitorInfo.rcMonitor.top, gGamePerformanceData.MonitorWidth, gGamePerformanceData.MonitorHeight, SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
    {
        Result = GetLastError();
        LogMessageA(LogError, "[%s] SetWindowPos failed! Error 0x%08lx!", __FUNCTION__, Result);
        goto Exit;
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
    int16_t EscapeKeyPressed = GetAsyncKeyState(VK_ESCAPE);
    int16_t DebugKeyPressed = GetAsyncKeyState(VK_F1);
    static int16_t DebugKeyAlreadyPressed;


    int16_t SDownKeyPressed = GetAsyncKeyState('S') | GetAsyncKeyState(VK_DOWN);
    static int16_t SDownKeyAlreadyPressed;
    int16_t ALeftKeyPressed = GetAsyncKeyState('A') | GetAsyncKeyState(VK_LEFT);       // WASD and ArrowKey movement
    static int16_t ALeftKeyAlreadyPressed;
    int16_t DRightKeyPressed = GetAsyncKeyState('D') | GetAsyncKeyState(VK_RIGHT);       
    static int16_t DRightKeyAlreadyPressed;
    int16_t WUpKeyPressed = GetAsyncKeyState('W') | GetAsyncKeyState(VK_UP);       
    static int16_t WUpKeyAlreadyPressed;

    if (EscapeKeyPressed)
    {
        SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
    }
    if (DebugKeyPressed && !DebugKeyAlreadyPressed)
    {
        gGamePerformanceData.DisplayDebugInfo = !gGamePerformanceData.DisplayDebugInfo;
    }

    if (!gPlayer.MovementRemaining)
    {
        if (SDownKeyPressed)
        {   
            if (gPlayer.ScreenPosY < GAME_RES_HEIGHT - 16)
            {
                gPlayer.Direction = Down;
                gPlayer.MovementRemaining = 16;
            }
        }
        else if (ALeftKeyPressed)
        {
            if (gPlayer.ScreenPosX > 0)
            {
                gPlayer.Direction = Left;
                gPlayer.MovementRemaining = 16;
            }
        }
        else if (DRightKeyPressed)
        {
            if (gPlayer.ScreenPosX < GAME_RES_WIDTH - 16)
            {
                gPlayer.Direction = Right;
                gPlayer.MovementRemaining = 16;
            }
        }
        else if (WUpKeyPressed)
        {
            if (gPlayer.ScreenPosY > 0)
            {
                gPlayer.Direction = Up;
                gPlayer.MovementRemaining = 16;
            }
        }
    }
    else
    {
        gPlayer.MovementRemaining--;

        if (gPlayer.Direction == Down)
        {
            gPlayer.ScreenPosY++;
        }
        else if (gPlayer.Direction == Left)
        {
            gPlayer.ScreenPosX--;
        }
        else if (gPlayer.Direction == Right)
        {
            gPlayer.ScreenPosX++;
        }
        else if (gPlayer.Direction == Up)
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

    /*if (SDownKeyPressed)
    {
        if (gPlayer.ScreenPosY < GAME_RES_HEIGHT - 16)
        {
            gPlayer.ScreenPosY++;
        }
    }
    if (ALeftKeyPressed)
    {
        if (gPlayer.ScreenPosX > 0)
        {
            gPlayer.ScreenPosX--;
        }
    }
    if (DRightKeyPressed)
    {
        if (gPlayer.ScreenPosX < GAME_RES_WIDTH - 16)
        {
            gPlayer.ScreenPosX++;
        }
    }
    if (WUpKeyPressed)
    {   
        if (gPlayer.ScreenPosY > 0)
        {
            gPlayer.ScreenPosY--;
        }
    }*/
    DebugKeyAlreadyPressed = DebugKeyPressed;
    ALeftKeyAlreadyPressed = ALeftKeyPressed;
    DRightKeyAlreadyPressed = DRightKeyPressed;
    WUpKeyAlreadyPressed = WUpKeyPressed;
    SDownKeyAlreadyPressed = SDownKeyPressed;
}      //error seems to need this semicolon??? now error is gone, leaving this here tho

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
        goto Exit;
    }

    if (ReadFile(FileHandle, &BitmapHeader, 2, &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError(); 
        goto Exit;
    }

    if (BitmapHeader != 0x4d42)     //0x4d42 is "BM" backwards
    {
        Error = ERROR_FILE_INVALID;
        goto Exit;
    }

    if (SetFilePointer(FileHandle, 0xA, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        goto Exit;
    }

    if (ReadFile(FileHandle, &PixelDataOffset, sizeof(DWORD), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        goto Exit;
    }

    if (SetFilePointer(FileHandle, 0xE, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        goto Exit;
    }

    if (ReadFile(FileHandle, &GameBitmap->BitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        goto Exit;
    }

    if ((GameBitmap->Memory = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, GameBitmap->BitmapInfo.bmiHeader.biSizeImage)) == NULL)
    {
        Error = ERROR_NOT_ENOUGH_MEMORY;
        goto Exit;
    }

    if (SetFilePointer(FileHandle, PixelDataOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = GetLastError();
        goto Exit;
    }

    if (ReadFile(FileHandle, GameBitmap->Memory, GameBitmap->BitmapInfo.bmiHeader.biSizeImage, &NumberOfBytesRead, NULL) == 0)
    {
        Error = GetLastError();
        goto Exit;
    }

Exit:
    if (FileHandle && (FileHandle != INVALID_HANDLE_VALUE))
    {
        CloseHandle(FileHandle);
    }
    return(Error);
}

DWORD InitializePlayer(void)
{
    DWORD Error = ERROR_SUCCESS;

    gPlayer.ScreenPosX = 32;
    gPlayer.ScreenPosY = 32;
    gPlayer.CurrentSuit = SUIT_0;
    gPlayer.Direction = Down;
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingDown0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingDown0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingDown1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingDown1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingDown2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_DOWN_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingDown2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingLeft0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingLeft0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingLeft1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingLeft1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingLeft2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_LEFT_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingLeft2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingRight0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingRight0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingRight1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingRight1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingRight2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_RIGHT_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingRight2.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingUp0.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_0])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingUp0.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingUp1.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_1])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingUp1.bmpx failed!", __FUNCTION__);
        MessageBoxA(NULL, "Load32BppBitmapFromFile failed!", "Error!", MB_ICONERROR | MB_OK);
        goto Exit;
    }
    if ((Error = Load32BppBitmapFromFile("..\\Assets\\Suit0FacingUp2.bmpx", &gPlayer.Sprite[SUIT_0][FACING_UP_2])) != ERROR_SUCCESS)
    {
        LogMessageA(LogError, "[%s] Loading Suit0FacingUp2.bmpx failed!", __FUNCTION__);
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

        switch (String[Character])
        {
            case'A':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth;
                break;
            }
            case'B':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + CharWidth;
                break;
            }
            case'C':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 2);
                break;
            }
            case'D':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 3);
                break;
            }
            case'E':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 4);
                break;
            }
            case'F':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 5);
                break;
            }
            case'G':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 6);
                break;
            }
            case'H':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 7);
                break;
            }
            case'I':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 8);
                break;
            }
            case'J':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 9);
                break;
            }
            case'K':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 10);
                break;
            }
            case'L':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 11);
                break;
            }
            case'M':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 12);
                break;
            }
            case'N':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 13);
                break;
            }
            case'O':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 14);
                break;
            }
            case'P':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 15);
                break;
            }
            case'Q':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 16);
                break;
            }
            case'R':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 17);
                break;
            }
            case'S':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 18);
                break;
            }
            case'T':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 19);
                break;
            }
            case'U':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 20);
                break;
            }
            case'V':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 21);
                break;
            }
            case'W':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 22);
                break;
            }
            case'X':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 23);
                break;
            }
            case'Y':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 24);
                break;
            }
            case'Z':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 25);
                break;
            }
            case'a':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 26);
                break;
            }
            case'b':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 27);
                break;
            }
            case'c':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 28);
                break;
            }
            case'd':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 29);
                break;
            }
            case'e':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 30);
                break;
            }
            case'f':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 31);
                break;
            }
            case'g':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 32);
                break;
            }
            case'h':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 33);
                break;
            }
            case'i':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 34);
                break;
            }
            case'j':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 35);
                break;
            }
            case'k':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 36);
                break;
            }
            case'l':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 37);
                break;
            }
            case'm':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 38);
                break;
            }
            case'n':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 39);
                break;
            }
            case'o':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 40);
                break;
            }
            case'p':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 41);
                break;
            }
            case'q':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 42);
                break;
            }
            case'r':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 43);
                break;
            }
            case's':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 44);
                break;
            }
            case't':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 45);
                break;
            }
            case'u':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 46);
                break;
            }
            case'v':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 47);
                break;
            }
            case'w':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 48);
                break;
            }
            case'x':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 49);
                break;
            }
            case'y':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 50);
                break;
            }
            case'z':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 51);
                break;
            }
            case'0':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 52);
                break;
            }
            case'1':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 53);
                break;
            }
            case'2':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 54);
                break;
            }
            case'3':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 55);
                break;
            }
            case'4':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 56);
                break;
            }
            case'5':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 57);
                break;
            }
            case'6':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 58);
                break;
            }
            case'7':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 59);
                break;
            }
            case'8':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 60);
                break;
            }
            case'9':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 61);
                break;
            }
            case'`':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 62);
                break;
            }
            case'~':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 63);
                break;
            }
            case'!':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 64);
                break;
            }
            case'@':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 65);
                break;
            }
            case'#':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 66);
                break;
            }
            case'$':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 67);
                break;
            }
            case'%':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 68);
                break;
            }
            case'^':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 69);
                break;
            }
            case'&':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 70);
                break;
            }
            case'*':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 71);
                break;
            }
            case'(':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 72);
                break;
            }
            case')':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 73);
                break;
            }
            case'-':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 74);
                break;
            }
            case'=':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 75);
                break;
            }
            case'_':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 76);
                break;
            }
            case'+':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 77);
                break;
            }
            case'\\':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 78);
                break;
            }
            case'|':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 79);
                break;
            }
            case'[':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 80);
                break;
            }
            case']':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 81);
                break;
            }
            case'{':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 82);
                break;
            }
            case'}':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 83);
                break;
            }
            case';':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 84);
                break;
            }
            case'\'':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 85);
                break;
            }
            case':':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 86);
                break;
            }
            case'"':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 87);
                break;
            }
            case',':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 88);
                break;
            }
            case'<':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 89);
                break;
            }
            case'>':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 90);
                break;
            }
            case'.':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 91);
                break;
            }
            case'/':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 92);
                break;
            }
            case'?':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 93);
                break;
            }
            case' ':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 94);
                break;
            }
            case'«':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 95);
                break;
            }
            case'»':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 96);
                break;
            }
            case'\xf2':
            {
                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 97);
                break;
            }
            default:
            {
                //TODO: assert?     currently prints '?' for unknown character

                StartingFontSheetPixel = (FontSheet->BitmapInfo.bmiHeader.biWidth * FontSheet->BitmapInfo.bmiHeader.biHeight) - FontSheet->BitmapInfo.bmiHeader.biWidth + (CharWidth * 93);
                break;
            }
        }

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
#ifdef SIMD
    __m128i QuadPixel = { 0x3f, 0x00, 0x00, 0xff, 0x3f, 0x00, 0x00, 0xff, 0x3f, 0x00, 0x00, 0xff, 0x3f, 0x00, 0x00, 0xff };     //load 4 pixels worth of info
    ClearScreenColor(&QuadPixel);
#else
    PIXEL32 Pixel = { 0xff, 0x00, 0x00, 0xff };         //load 1 pixel
    ClearScreenColor(&Pixel);
#endif

    PIXEL32 Green = { 0x00, 0xFF, 0x00, 0xFF };

    BlitStringToBuffer("GAME OVER", &g6x7Font, &Green, 128, 128);

    Blit32BppBitmapToBuffer(&gPlayer.Sprite[gPlayer.CurrentSuit][gPlayer.Direction + gPlayer.SpriteIndex], gPlayer.ScreenPosX, gPlayer.ScreenPosY);


    if (gGamePerformanceData.DisplayDebugInfo == TRUE)
    {
        DrawDebugInfo();
    }

    HDC DeviceContext = GetDC(gGameWindow);
    StretchDIBits(DeviceContext, 0, 0, gGamePerformanceData.MonitorWidth, gGamePerformanceData.MonitorHeight, 0, 0, GAME_RES_WIDTH, GAME_RES_HEIGHT, gBackBuffer.Memory, &gBackBuffer.BitmapInfo, DIB_RGB_COLORS, SRCCOPY);

    ReleaseDC(gGameWindow, DeviceContext);
}

#ifdef SIMD
__forceinline void ClearScreenColor(_In_ __m128i* Color)
{
    for (int x = 0; x < GAME_RES_WIDTH * GAME_RES_HEIGHT; x += 4)      //paint the screen 4 pixels at a time
    {
        _mm_store_si128((PIXEL32*)gBackBuffer.Memory + x, *Color);
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
        LogMessageA(LogError, "[%s] RegCreateKey failed with error code 0x%08lx!", __FUNCTION__, Result);

        goto Exit;
    }
    if (RegDisposition == REG_CREATED_NEW_KEY)
    {
        LogMessageA(Info, "[%s] Registry key did not exist; created new key HKCU\\SOFTWARE\\%s.", __FUNCTION__, GAME_NAME);
    }
    else
    {
        LogMessageA(Info, "[%s] Opened existig registry key HCKU\\SOFTWARE\\%s", __FUNCTION__, GAME_NAME);
    }
    Result = RegGetValueA(RegKey, NULL, "LogLevel", RRF_RT_DWORD, NULL, (BYTE*)&gRegistryParams.LogLevel, &RegBytesRead);

    if (Result != ERROR_SUCCESS)
    {
        if (Result == ERROR_FILE_NOT_FOUND)
        {
            Result = ERROR_SUCCESS;
            LogMessageA(Info, "[%s] Registry value 'LogLevel' not found. Using default of 0. (LOG_LEVEL_NONE)", __FUNCTION__);
            gRegistryParams.LogLevel = None;
        }
        else
        {
            LogMessageA(LogError, "[%s] Failed to read the 'LogLevel' registry value! Error 0x%08lx!", __FUNCTION__, Result);
            goto Exit;
        }
    }

    LogMessageA(Info, "[%s] LogLevel is %d", __FUNCTION__, gRegistryParams.LogLevel);
    //...

Exit:
    if (RegKey)
    {
        RegCloseKey(RegKey);
    }
    return(Result);
}

void LogMessageA(_In_ DWORD LogLevel, _In_ char* Message, _In_ ...)
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

    if (gRegistryParams.LogLevel < LogLevel)
    {
        return;
    }
    if (MessageLength < 1 || MessageLength > 4096)
    {
        return;
    }
    switch (LogLevel)
    {   
        case None:
        {
            return;
        }
        case Info:
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[INFO]");
            break;
        }
        case Warn:
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[WARN]");
            break;
        }
        case LogError:
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[ERROR]");
            break;
        }
        case Debug:
        {
            strcpy_s(SeverityString, sizeof(SeverityString), "[DEBUG]");
            break;
        }
        default:
        {

            //assert?? pop-up error?
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
        //assert? if log function fails?
        return;
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
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "C.Timer Res:%.02f", gGamePerformanceData.CurrentTimerResolution / 10000.0f);
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