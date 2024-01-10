
#pragma warning(push, 3)
#include <stdio.h>
#include <windows.h>

#include <psapi.h>

#include <emmintrin.h>
#pragma warning(pop)

#include <stdint.h>
#include "Main.h"

HWND gGameWindow;

BOOL gGameIsRunning;

GAMEBITMAP gBackBuffer;

GAME_PERFORMANCE_DATA gGamePerformanceData;

PLAYER gPlayer;

int WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, PSTR CommandLine, int CmdShow)
{   
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(PreviousInstance);
    UNREFERENCED_PARAMETER(CommandLine);
    UNREFERENCED_PARAMETER(CmdShow);

    MSG Message = { 0 };

    int64_t FrameStart = 0;
    int64_t FrameEnd = 0;
    int64_t ElapsedMicroseconds;
    int64_t ElapsedMicrosecondsPerFrameAccumulatorRaw = 0;
    int64_t ElapsedMicrosecondsPerFrameAccumulatorCooked = 0;

    HMODULE NtDllModuleHandle;

    if ((NtDllModuleHandle = GetModuleHandleA("ntdll.dll")) == NULL)
    {
        MessageBoxA(NULL, "Couldn't load ntdll.dll!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if ((NtQueryTimerResolution = (_NtQueryTimerResolution)GetProcAddress(NtDllModuleHandle, "NtQueryTimerResolution")) == NULL)
    {
        MessageBoxA(NULL, "Couldn't find function NtQueryTimerResolution in ntdll.dll!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    NtQueryTimerResolution(&gGamePerformanceData.MinimumTimerResolution, &gGamePerformanceData.MaximumTimerResolution, &gGamePerformanceData.CurrentTimerResolution);

    GetSystemInfo(&gGamePerformanceData.SystemInfo);
        
    if (GameIsAlreadyRunning() == TRUE)
    {
        MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    if (CreateMainGameWindow() != ERROR_SUCCESS)
    {
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
        MessageBoxA(NULL, "Failed to allocate memory for drawing surface!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    memset(gBackBuffer.Memory, 0x00, GAME_DRAWING_AREA_MEMORY_SIZE);

    gPlayer.WorldPosX = 25;
    gPlayer.WorldPosY = 25;

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

                if (ElapsedMicroseconds < ((int64_t)GOAL_MICROSECONDS_PER_FRAME - (gGamePerformanceData.CurrentTimerResolution * 0.1f)))
                {
                    Sleep(1);       //can be between 1ms and one system tick (~15.625ms)
                }
            }
            ElapsedMicrosecondsPerFrameAccumulatorCooked += ElapsedMicroseconds;

        if ((gGamePerformanceData.TotalFramesRendered % CALCULATE_AVG_FPS_EVERY_X_FRAMES) == 0)
        {
            GetSystemTimeAsFileTime((FILETIME*) &gGamePerformanceData.CurrentSystemTime);
            GetProcessTimes(GetCurrentProcess(), &gGamePerformanceData.ProcessCreationTime, &gGamePerformanceData.ProcessExitTime, (FILETIME*)&gGamePerformanceData.CurrentKernelCPUTime, (FILETIME*)&gGamePerformanceData.CurrentUserCPUTime);

            gGamePerformanceData.CPUPercent = (gGamePerformanceData.CurrentKernelCPUTime - gGamePerformanceData.PreviousKernelCPUTime) + (gGamePerformanceData.CurrentUserCPUTime - gGamePerformanceData.PreviousUserCPUTime);
            gGamePerformanceData.CPUPercent /= (gGamePerformanceData.CurrentSystemTime - gGamePerformanceData.PreviousSystemTime);
            gGamePerformanceData.CPUPercent /= gGamePerformanceData.SystemInfo.dwNumberOfProcessors; //kept returning 0 processors and then dividing by 0
            gGamePerformanceData.CPUPercent *= 100;

            GetProcessHandleCount(GetCurrentProcess(), &gGamePerformanceData.HandleCount);
            K32GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&gGamePerformanceData.MemInfo, sizeof(gGamePerformanceData.MemInfo));

            gGamePerformanceData.RawFPSAverage = 1.0f / ((ElapsedMicrosecondsPerFrameAccumulatorRaw / CALCULATE_AVG_FPS_EVERY_X_FRAMES) * 0.000001f);
            gGamePerformanceData.CookedFPSAverage = 1.0f / ((ElapsedMicrosecondsPerFrameAccumulatorCooked / CALCULATE_AVG_FPS_EVERY_X_FRAMES) * 0.000001f);
            ElapsedMicrosecondsPerFrameAccumulatorRaw = 0; 
            ElapsedMicrosecondsPerFrameAccumulatorCooked = 0;

            gGamePerformanceData.PreviousKernelCPUTime = gGamePerformanceData.CurrentKernelCPUTime;
            gGamePerformanceData.PreviousUserCPUTime = gGamePerformanceData.CurrentUserCPUTime;
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
            MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            goto Exit;
    }

    gGameWindow = CreateWindowExA(0, WindowClass.lpszClassName, "Window Title Name", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, GetModuleHandleA(NULL), NULL);

    if (gGameWindow == NULL)
    {
        Result = GetLastError();
            MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            goto Exit;
    }

    gGamePerformanceData.MonitorInfo.cbSize = sizeof(MONITORINFO);

    if (GetMonitorInfoA(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gGamePerformanceData.MonitorInfo) == 0)
    {
        Result = ERROR_MONITOR_NO_DESCRIPTOR;       // GetMonitorInfoA cannot Result = GetLastError
        goto Exit;
    }

    gGamePerformanceData.MonitorWidth = gGamePerformanceData.MonitorInfo.rcMonitor.right - gGamePerformanceData.MonitorInfo.rcMonitor.left;
    gGamePerformanceData.MonitorHeight = gGamePerformanceData.MonitorInfo.rcMonitor.bottom - gGamePerformanceData.MonitorInfo.rcMonitor.top;

    if (SetWindowLongPtrA(gGameWindow, GWL_STYLE, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_OVERLAPPEDWINDOW) == 0)
    {
        Result = GetLastError(); 
        goto Exit;
    }

    if (SetWindowPos(gGameWindow, HWND_TOP, gGamePerformanceData.MonitorInfo.rcMonitor.left, gGamePerformanceData.MonitorInfo.rcMonitor.top, gGamePerformanceData.MonitorWidth, gGamePerformanceData.MonitorHeight, SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
    {
        Result = GetLastError();
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
    int16_t EscapeKeyPressed = GetAsyncKeyState(VK_ESCAPE);
    int16_t DebugKeyPressed = GetAsyncKeyState(VK_F1);
    static int16_t DebugKeyAlreadyPressed;

    int16_t ALeftKeyPressed = GetAsyncKeyState('A') | GetAsyncKeyState(VK_LEFT);       // WASD and ArrowKey movement
    static int16_t ALeftKeyAlreadyPressed;
    int16_t DRightKeyPressed = GetAsyncKeyState('D') | GetAsyncKeyState(VK_RIGHT);       
    static int16_t DRightKeyAlreadyPressed;
    int16_t WUpKeyPressed = GetAsyncKeyState('W') | GetAsyncKeyState(VK_UP);       
    static int16_t WUpKeyAlreadyPressed;
    int16_t SDownKeyPressed = GetAsyncKeyState('S') | GetAsyncKeyState(VK_DOWN);       
    static int16_t SDownKeyAlreadyPressed;

    if (EscapeKeyPressed)
    {
        SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
    }
    if (DebugKeyPressed && !DebugKeyAlreadyPressed)
    {
        gGamePerformanceData.DisplayDebugInfo = !gGamePerformanceData.DisplayDebugInfo;
    }
    if (ALeftKeyPressed /*&& !ALeftKeyAlreadyPressed*/)
    {
        if (gPlayer.WorldPosX > 0)
        {
            gPlayer.WorldPosX--;
        }
    }
    if (DRightKeyPressed /*&& !DRightKeyAlreadyPressed*/)
    {
        if (gPlayer.WorldPosX < GAME_RES_WIDTH - 16)
        {
            gPlayer.WorldPosX++;
        }
    }
    if (WUpKeyPressed /*&& !WUpKeyAlreadyPressed*/)
    {   
        if (gPlayer.WorldPosY > 0)
        {
            gPlayer.WorldPosY--;
        }
    }
    if (SDownKeyPressed /*&& !SDownKeyAlreadyPressed*/)
    {
        if (gPlayer.WorldPosY < GAME_RES_HEIGHT - 16)
        {
            gPlayer.WorldPosY++;
        }
    }
    DebugKeyAlreadyPressed = DebugKeyPressed;
    ALeftKeyAlreadyPressed = ALeftKeyPressed;
    DRightKeyAlreadyPressed = DRightKeyPressed;
    WUpKeyAlreadyPressed = WUpKeyPressed;
    SDownKeyAlreadyPressed = SDownKeyPressed;
}      //error seems to need this semicolon??? now error is gone, leaving this here tho

void RenderFrameGraphics(void)
{
#ifdef SIMD
    __m128i QuadPixel = { 0x3f, 0x00, 0x00, 0xff, 0x3f, 0x00, 0x00, 0xff, 0x3f, 0x00, 0x00, 0xff, 0x3f, 0x00, 0x00, 0xff };     //load 4 pixels worth of info
    ClearScreenColor(&QuadPixel);
#else
    PIXEL32 Pixel = { 0xff, 0x00, 0x00, 0xff };         //load 1 pixel
    ClearScreenColor(&Pixel);
#endif
    int32_t ScreenX = gPlayer.WorldPosX;
    int32_t ScreenY = gPlayer.WorldPosY;
    int32_t StartingScreenPixel = ((GAME_RES_WIDTH * GAME_RES_HEIGHT) - GAME_RES_WIDTH) - (GAME_RES_WIDTH * ScreenY) + ScreenX;     //Create a starting pixel based on x,y

    for (int32_t y = 0; y < 16; y++)
    {
        for (int32_t x = 0;x < 16; x++)
        {
            memset((PIXEL32*)gBackBuffer.Memory + (uintptr_t)StartingScreenPixel + x - ((uintptr_t)GAME_RES_WIDTH * y), 0xFF, sizeof(PIXEL32));
        }
    }

    HDC DeviceContext = GetDC(gGameWindow);
    StretchDIBits(DeviceContext, 0, 0, gGamePerformanceData.MonitorWidth, gGamePerformanceData.MonitorHeight, 0, 0, GAME_RES_WIDTH, GAME_RES_HEIGHT, gBackBuffer.Memory, &gBackBuffer.BitmapInfo, DIB_RGB_COLORS, SRCCOPY);

    if (gGamePerformanceData.DisplayDebugInfo == TRUE)
    {
        SelectObject(DeviceContext, (HFONT)GetStockObject(ANSI_FIXED_FONT));
        char DebugTextBuffer[64] = { 0 };


        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "CPU: %.03f%%", gGamePerformanceData.CPUPercent);         //display cpu first for readability reasons during flickering
        TextOutA(DeviceContext, 0, 91, DebugTextBuffer, (int)strlen(DebugTextBuffer));
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FPS Raw: %.01f ", gGamePerformanceData.RawFPSAverage);
        TextOutA(DeviceContext, 0, 0, DebugTextBuffer, (int)strlen(DebugTextBuffer));
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "FPS Cook : %.01f ", gGamePerformanceData.CookedFPSAverage);
        TextOutA(DeviceContext, 0, 13, DebugTextBuffer, (int)strlen(DebugTextBuffer));
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Min Timer: %.03f", gGamePerformanceData.MinimumTimerResolution / 10000.0f);
        TextOutA(DeviceContext, 0, 26, DebugTextBuffer, (int)strlen(DebugTextBuffer));
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Max Timer:  %.03f", gGamePerformanceData.MaximumTimerResolution / 10000.0f);
        TextOutA(DeviceContext, 0, 39, DebugTextBuffer, (int)strlen(DebugTextBuffer));
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Current T:  %.03f", gGamePerformanceData.CurrentTimerResolution / 10000.0f);
        TextOutA(DeviceContext, 0, 52, DebugTextBuffer, (int)strlen(DebugTextBuffer));
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Handles  :  %lu ", gGamePerformanceData.HandleCount);
        TextOutA(DeviceContext, 0, 65, DebugTextBuffer, (int)strlen(DebugTextBuffer));
        sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "Memory: %llu KB ", (gGamePerformanceData.MemInfo.PrivateUsage / 1024));
        TextOutA(DeviceContext, 0, 78, DebugTextBuffer, (int)strlen(DebugTextBuffer));
        //sprintf_s(DebugTextBuffer, _countof(DebugTextBuffer), "CPU: %.03f%%", gGamePerformanceData.CPUPercent);         
        //TextOutA(DeviceContext, 0, 91, DebugTextBuffer, (int)strlen(DebugTextBuffer));
    }

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
