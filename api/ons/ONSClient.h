#pragma once

#ifndef ONS_NAMESPACE
#define ONS_NAMESPACE ons
#endif

#ifndef ONS_NAMESPACE_BEGIN
#define ONS_NAMESPACE_BEGIN namespace ONS_NAMESPACE {
#endif

#ifndef ONS_NAMESPACE_END
#define ONS_NAMESPACE_END }
#endif

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define ONSCLIENT_API __attribute__ ((dllexport))
    #else
      #define ONSCLIENT_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define ONSCLIENT_API __attribute__ ((dllimport))
    #else
      #define ONSCLIENT_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define ONSCLIENT_API __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define ONSCLIENT_API
    #define DLL_LOCAL
  #endif
#endif