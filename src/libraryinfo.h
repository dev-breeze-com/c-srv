// Tsert.Com Copyright (C) 2015 <CreativeCommons-NC>.
#ifndef HEADER_SRC_APPINFO_H_INCLUDED
#define HEADER_SRC_APPINFO_H_INCLUDED

/* #include <boost/preprocessor/stringize.hpp> */

/*
 * AUTO-GENERATION WARNING:
 *     This file has been automatically generated from "appinfo.h.in.cmake".
 *     DO NOT edit this file, as any changes will be automatically discarded.
 */
 
#define APPLICATION_NAME               "qhttpsrv"
#define APPLICATION_CODENAME           "qhttpsrv"
#define APPLICATION_COPYRIGHT_YEARS    "2016"
/* #undef APPLICATION_VERSION_MAJOR */
#define APPLICATION_VERSION_MINOR      9
/* #undef APPLICATION_VERSION_PATCH */
#define APPLICATION_VERSION_TYPE       "Release"
#   define APPLICATION_VERSION_STRING APPLICATION_VERSION_MAJOR "." APPLICATION_VERSION_MINOR "." APPLICATION_VERSION_PATCH "-" APPLICATION_VERSION_TYPE
#define APPLICATION_VENDOR_ID          "tsert.com"
#define APPLICATION_VENDOR_NAME        "Tsert.Com"
#define APPLICATION_VENDOR_URL         "www.breezeos.com"
#define APPLICATION_ID                 "tsert.com"

#ifndef APPLICATION_NAME
#   error "Please specify the application name in the top-level CMakeLists.txt file."
#endif

#ifndef APPLICATION_CODENAME
#   error "Please specify the application codename in the top-level CMakeLists.txt file."
#endif

#ifndef APPLICATION_COPYRIGHT_YEARS
#   error "Please specify the application copyright years (years during which this application was created, edited, or modified) in the top-level CMakeLists.txt file."
#endif

#ifndef APPLICATION_VERSION_MAJOR
#   define APPLICATION_VERSION_MAJOR 0
#endif

#ifndef APPLICATION_VERSION_MINOR
#   define APPLICATION_VERSION_MINOR 0
#endif

#ifndef APPLICATION_VERSION_PATCH
#   define APPLICATION_VERSION_PATCH 0
#endif

#ifndef APPLICATION_VERSION_TYPE
#   define APPLICATION_VERSION_TYPE "SNAPSHOT"
#endif

#ifndef APPLICATION_VERSION_STRING
#   define APPLICATION_VERSION_STRING APPLICATION_VERSION_MAJOR "." APPLICATION_VERSION_MINOR "." APPLICATION_VERSION_PATCH "-" APPLICATION_VERSION_TYPE
#endif

#ifndef APPLICATION_VENDOR_ID
#   error "Please specify the application vendor id in the top-level CMakeLists.txt file."
#endif

#ifndef APPLICATION_VENDOR_NAME
#   error "Please specify the application vendor name in the top-level CMakeLists.txt file."
#endif

#ifndef APPLICATION_VENDOR_URL
#   error "Please specify the application vendor URL in the top-level CMakeLists.txt file."
#endif

#ifndef APPLICATION_ID
#   define APPLICATION_ID APPLICATION_VENDOR_ID "." APPLICATION_CODENAME
#endif

#endif
