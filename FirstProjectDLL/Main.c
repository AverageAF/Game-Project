#ifdef _WIN32

////platform independent game binary
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)


BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(Reserved);
    // Perform actions based on the reason for calling.
    switch (Reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.
            break;
        }

        case DLL_THREAD_ATTACH:
        {

            // Do thread-specific initialization.
            break;
        }

        case DLL_THREAD_DETACH:
        {

            // Do thread-specific cleanup.
            break;
        }

        case DLL_PROCESS_DETACH:
        {

            //if (Reserved != nullptr)
            //{
            //    break; // do not do cleanup if process termination scenario
            //}

            // Perform any necessary cleanup.
            break;
        }

    }
    return (TRUE);  // Successful DLL_PROCESS_ATTACH.
}
#endif

