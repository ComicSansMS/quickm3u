#include <quickm3u/ui/main_window.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
#include <QApplication>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef WIN32
#include <Windows.h>
#include <Dbghelp.h>
#endif

namespace {
#ifdef WIN32
LONG WINAPI win32CrashHandler(EXCEPTION_POINTERS* exceptionInfo)
{
    HANDLE hfile = CreateFile("crash.dmp", GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
    if (hfile != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION md_exc_info;
        md_exc_info.ThreadId = GetCurrentThreadId();
        md_exc_info.ExceptionPointers = exceptionInfo;
        md_exc_info.ClientPointers = TRUE;
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hfile,
                          MiniDumpNormal, &md_exc_info, nullptr, nullptr);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

void prepareCrashHandler()
{
    SetUnhandledExceptionFilter(win32CrashHandler);
}
#else
void prepareCrashHandler()
{
}
#endif
}

int main(int argc, char* argv[])
{
    prepareCrashHandler();
    QApplication the_app(argc, argv);

    ui::MainWindow main_window;
    main_window.show();

    return the_app.exec();
}
