#pragma once
#ifndef _TINYPBRT_EXPORT_HPP_
#define _TINYPBRT_EXPORT_HPP_

#ifdef TINYPBRT_SHARED_LIBS
#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef TINYPBRT_CPP_EXPORTS
#define TPBRT_CPP_API __declspec(dllexport)
#else
#define TPBRT_CPP_API __declspec(dllimport)
#endif
#elif defined(__GNUC__) || defined(__clang__)
#define TPBRT_CPP_API __attribute__((visibility("default")))
#else
#define TPBRT_CPP_API
#endif
#else
#define TPBRT_CPP_API
#endif

#endif // _TINYPBRT_EXPORT_HPP_
