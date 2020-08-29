CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
PROJECT("slirp4tinc"
    DESCRIPTION "Make a Tinc daemon a VPN exit node without administator privileges."
    HOMEPAGE_URL "https://github.com/joluxer/slirp4tinc"
    VERSION 0.1.0
    LANGUAGES C CXX)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FORTIFY_SOURCE=2 -O2 -pedantic -Wall -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2 -O2 -pedantic -Wall -g")

INCLUDE(GNUInstallDirs)

# find libslirp library
FIND_LIBRARY(libSlirp slirp)
IF(NOT libSlirp)
  MESSAGE(FATAL_ERROR "library libslirp not found")
ENDIF(NOT libSlirp)

# find libvdeslirp library
FIND_LIBRARY(libVdeSlirp vdeslirp)
IF(NOT libVdeSlirp)
  MESSAGE(FATAL_ERROR "library libvdeslirp not found")
ENDIF(NOT libVdeSlirp)

# find libvdeslirp header
FIND_PATH(libVdeSlirpPath slirp/libvdeslirp.h)
IF(NOT libVdeSlirpPath)
  MESSAGE(FATAL_ERROR "header slirp/libvdeslirp.h not found")
ENDIF(NOT libVdeSlirpPath)
INCLUDE_DIRECTORIES(${libVdeSlirpPath})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR})

ADD_SUBDIRECTORY(arg)
ADD_SUBDIRECTORY(src)
