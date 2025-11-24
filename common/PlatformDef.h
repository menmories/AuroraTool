#ifndef PLATFORMDEF_H
#define PLATFORMDEF_H

#ifdef _MSC_VER
#define PLATFORM_WINDOWS
#endif

#ifdef __linux__
#define PLATFORM_LINUX
#endif

#ifdef PLATFORM_WINDOWS
#include <WinSock2.h>
#include <Windows.h>
#include <tchar.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;

#define AURORA_INLINE __forceinline

#ifdef  AURORA_EXPORT
#define AURORA_API __declspec(dllexport)
#else 
#define AURORA_API __declspec(dllimport)
#endif  // FANTACY_EXPORT


#endif  // !PLATFORM_WINDOWS




#ifdef PLATFORM_LINUX

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;

#include <unistd.h>

#define AURORA_API
#define AURORA_INLINE __forceinline

#endif  // !PLATFORM_LINUX



#endif  // !PLATFORMDEF_H

