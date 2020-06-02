#pragma once
// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

// add headers that you want to pre-compile here

#include <afxwin.h>         // MFC core and standard components

#include <afxsock.h>            // MFC socket extensions

#include "PackageHelper.h"
#include "StringHelper.h"
#include "MxObject.h"




#endif //PCH_H
