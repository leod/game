#ifdef WIN32 // For MinGW32
#include <windows.h>

struct LoadBacktrace {
    HMODULE module;

    LoadBacktrace() {
        module = LoadLibraryA("backtrace.dll");
    }

    ~LoadBacktrace() {
        if (module)
            FreeLibrary(module);
    }
};

static LoadBacktrace backtrace;

/*void load() __attribute__ ((constructor)) {
    LoadLibraryA("backtrace.dll");
}*/

#endif
