#pragma once

#include <iostream>
#include <cstdlib>

#ifndef NDEBUG
#    define ASSERT_MSG(condition, message) \
     do { \
         if (!(condition)) { \
             std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                       << ", line " << __LINE__ \
                       << ": " << message << std::endl; \
             std::exit(EXIT_FAILURE); \
         } \
     } while (false)
#else
#    define ASSERT_MSG(condition, message) do { } while (false)
#endif

#define ASSERT(condition) ASSERT_MSG(condition, "No message.");
