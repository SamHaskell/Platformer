#pragma once

#include <stdint.h>
#include <assert.h>
#include <cstdarg>
#include <memory>
#include <string>

#define STATIC_ASSERT static_assert

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#define STRINGIFY(x) #x

#ifdef _MSC_VER
#define PRETTY_FUNCTION __FUNCSIG__
#else
#define PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

typedef unsigned char u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef char i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte(s).");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 byte(s).");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 byte(s).");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 byte(s).");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte(s).");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 byte(s).");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 byte(s).");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 byte(s).");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 byte(s).");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 byte(s).");

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

constexpr f64 PI = 3.14159265358979323846264;
constexpr f64 TAU = 2 * PI;
constexpr f64 EULER = 2.71828182845904523536028;

constexpr f32 DEG2RAD = (f32)PI / 180.0f;
constexpr f32 RAD2DEG = 180.0f / (f32)PI;

template <typename T>
void Swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

typedef enum LogLevel
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
} LogLevel;

typedef enum LogLevelBit
{
    LOG_LEVEL_FATAL_BIT = (1 << LOG_LEVEL_FATAL),
    LOG_LEVEL_ERROR_BIT = (1 << LOG_LEVEL_ERROR),
    LOG_LEVEL_WARN_BIT = (1 << LOG_LEVEL_WARN),
    LOG_LEVEL_INFO_BIT = (1 << LOG_LEVEL_INFO),
    LOG_LEVEL_DEBUG_BIT = (1 << LOG_LEVEL_DEBUG),
    LOG_LEVEL_TRACE_BIT = (1 << LOG_LEVEL_TRACE),
} LogLevelBit;

inline void CoreLog(LogLevel verbosity, const char *msg, const char *file, i32 line, ...)
{
    static const char *logLevelLabels[6] = {
        "[FATAL]",
        "[ERROR]",
        "[WARN]",
        "[INFO]",
        "[DEBUG]",
        "[TRACE]",
    };

#ifdef PLATFORM_WEB
    static const char *logLevelColors[6] = {
        "",
        "",
        "",
        "",
        "",
        "",
    };
#else
    static const char *logLevelColors[6] = {
        "\e[0;31m",
        "\e[0;31m",
        "\e[0;33m",
        "\e[0;32m",
        "\e[0;36m",
        "\e[0;37m",
    };
#endif

    const int buffer_size = 8192;
    char output[buffer_size];
    memset(output, 0, sizeof(output));

    __builtin_va_list p_args;
    va_start(p_args, line);
    vsnprintf(output, buffer_size, msg, p_args);
    va_end(p_args);

#ifdef PLATFORM_WEB
    printf("%s%-7s: %s Line: %i\n%-9s%s%s\n", logLevelColors[verbosity], logLevelLabels[verbosity], file, line, "", output, "");
#else
    printf("%s%-7s: %s Line: %i\n%-9s%s%s\n", logLevelColors[verbosity], logLevelLabels[verbosity], file, line, "", output, "\e[0m");
#endif
}

#ifdef NT_NO_LOGGING
#define NT_FATAL(msg, ...)
#define NT_ERROR(msg, ...)
#define NT_WARN(msg, ...)
#define NT_INFO(msg, ...)
#define NT_DEBUG(msg, ...)
#define NT_TRNT(msg, ...)
#else
#define NT_FATAL(msg, ...) CoreLog(LOG_LEVEL_FATAL, msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define NT_ERROR(msg, ...) CoreLog(LOG_LEVEL_ERROR, msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define NT_WARN(msg, ...) CoreLog(LOG_LEVEL_WARN, msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define NT_INFO(msg, ...) CoreLog(LOG_LEVEL_INFO, msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define NT_DEBUG(msg, ...) CoreLog(LOG_LEVEL_DEBUG, msg, __FILE__, __LINE__, ##__VA_ARGS__)
#define NT_TRACE(msg, ...) CoreLog(LOG_LEVEL_TRACE, msg, __FILE__, __LINE__, ##__VA_ARGS__)
#endif

inline void LogAssertionFailure(const char *expr, const char *file, i32 line, const char *msg)
{
    CoreLog(LOG_LEVEL_FATAL, "Assertion failed for expression %s. %s", file, line, expr, msg);
}

#define DebugBreak() __builtin_trap()

#ifdef NT_NO_ASSERTS
#define NT_ASSERT(expr, msg)
#define NT_DEBUGASSERT(expr, msg)
#else
#define NT_ASSERT(expr, msg)                                      \
    {                                                             \
        if (expr)                                                 \
        {                                                         \
        }                                                         \
        else                                                      \
        {                                                         \
            LogAssertionFailure(#expr, __FILE__, __LINE__, #msg); \
            DebugBreak();                                         \
        }                                                         \
    }

#ifdef DEBUG
#define NT_DEBUGASSERT(expr, msg)                                 \
    {                                                             \
        if (expr)                                                 \
        {                                                         \
        }                                                         \
        else                                                      \
        {                                                         \
            LogAssertionFailure(#expr, __FILE__, __LINE__, #msg); \
            DebugBreak();                                         \
        }                                                         \
    }
#else
#define NT_DEBUGASSERT(expr, msg)
#endif
#endif