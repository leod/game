#pragma once

#include <iostream>
#include <cstdlib>

#ifdef WIN32
// Force crash to allow the backtrace to work
#define ASSERT_FORCE_CRASH { int* x = nullptr; *x = 42; }
#endif

#ifndef ASSERT_FORCE_CRASH
#    define ASSERT_FORCE_CRASH
#endif

#ifndef NDEBUG
#    define ASSERT_MSG(condition, message) \
     do { \
         if (!(condition)) { \
             std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                       << ", line " << __LINE__ \
                       << ": " << message << std::endl; \
             ASSERT_FORCE_CRASH; \
             std::exit(EXIT_FAILURE); \
         } \
     } while (false)
#else
#    define ASSERT_MSG(condition, message) do { } while (false)
#endif

#define ASSERT(condition) ASSERT_MSG(condition, "No message.");
