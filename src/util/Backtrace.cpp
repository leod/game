#ifdef WIN32 // For MinGW32
#include <windows.h>

struct LoadBacktrace {
    LoadBacktrace() {
        LoadLibraryA("backtrace.dll");
    }
};

static LoadBacktrace backtrace;

/*void load() __attribute__ ((constructor)) {
    LoadLibraryA("backtrace.dll");
}*/

#endif
