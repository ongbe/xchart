// vim: expandtab

#ifndef GRAPH_GLOBAL_H
#define GRAPH_GLOBAL_H

#include <qglobal.h>

// GRAPH_VERSION is (major << 16) + (minor << 8) + patch.

#define GRAPH_VERSION       0x050201
#define GRAPH_VERSION_STR   "5.2.1"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

#if defined(_MSC_VER) /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)
#endif // _MSC_VER

#ifdef GRAPH_DLL

#if defined(GRAPH_MAKEDLL)     // create a Qwt DLL library 
#define GRAPH_EXPORT  __declspec(dllexport)
#define GRAPH_TEMPLATEDLL
#else                        // use a Qwt DLL library
#define GRAPH_EXPORT  __declspec(dllimport)
#endif

#endif // GRAPH_DLL

#endif // Q_WS_WIN || Q_WS_S60

#ifndef GRAPH_EXPORT
#define GRAPH_EXPORT
#endif

// #define GRAPH_NO_COMPAT 1 // disable withdrawn functionality

#endif // GRAPH_GLOBAL_H

