#################################################################
#   GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY    #
#   Please read the README.txt file in the model directory.     #
#################################################################
lib_LTLIBRARIES = libzmon.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libzmon.pc

include_HEADERS = \
    ../include/zmon.h \
    ../include/zsuper.h \
    ../include/zmon_api.h

libzmon_la_SOURCES = \
    platform.h \
    zsuper.c \
    zmon_api.c

AM_CFLAGS = -g
AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = zmon_selftest
zmon_selftest_LDADD = libzmon.la
zmon_selftest_SOURCES = zmon_selftest.c

libzmon_la_LDFLAGS = -version-info @LTVER@

TESTS = zmon_selftest
