#pragma once

#define GAME_NAME "A_Game"

#define GAME_RES_WIDTH 384	//maybe 400
#define GAME_RES_HEIGHT 240
#define GAME_BPP 32
#define GAME_DRAWING_AREA_MEMORY_SIZE (GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP/8))

#define CALCULATE_AVG_FPS_EVERY_X_FRAMES 10	//goal of 120fps
#define GOAL_MICROSECONDS_PER_FRAME 16667		//16667 = 60fps

#define SIMD

typedef LONG(NTAPI* _NtQueryTimerResolution) (OUT PULONG MinimumResolution, OUT PULONG MaximumResolution, OUT PULONG CurrentResolution);

_NtQueryTimerResolution NtQueryTimerResolution;

typedef struct GAMEBITMAP
{
	BITMAPINFO BitmapInfo;
	void* Memory;
} GAMEBITMAP;

typedef struct PIXEL32
{
	uint8_t Blue;
	uint8_t Green;
	uint8_t Red;
	uint8_t Alpha;

} PIXEL32;

typedef struct GAME_PERFORMANCE_DATA
{
	uint64_t TotalFramesRendered;
	float RawFPSAverage;
	float CookedFPSAverage;
	int64_t PerfFrequency;

	MONITORINFO MonitorInfo;
	int MonitorWidth;
	int MonitorHeight;

	BOOL DisplayDebugInfo;
	LONG MinimumTimerResolution;		//if warning C4057 make ULONG instead
	LONG MaximumTimerResolution;		//c4057
	LONG CurrentTimerResolution;		//C4057

	DWORD HandleCount;
	PROCESS_MEMORY_COUNTERS_EX MemInfo;
	SYSTEM_INFO SystemInfo;
	int64_t CurrentSystemTime;
	int64_t PreviousSystemTime;
	FILETIME ProcessCreationTime;
	FILETIME ProcessExitTime;
	int64_t CurrentUserCPUTime;
	int64_t CurrentKernelCPUTime;
	int64_t PreviousUserCPUTime;
	int64_t PreviousKernelCPUTime;
	double CPUPercent;

} GAME_PERFORMANCE_DATA;

typedef struct PLAYER
{
	char Name[12];
	int32_t WorldPosX;
	int32_t WorldPosY;
} PLAYER;

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam);

DWORD CreateMainGameWindow(void);

BOOL GameIsAlreadyRunning(void);

void ProcessPlayerInput(void);
void RenderFrameGraphics(void);

#ifdef SIMD
void ClearScreenColor(_In_ __m128i* Color);
#else
void ClearScreenColor(_In_ PIXEL32* Color);
#endif