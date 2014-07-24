#################################################################
#   GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY    #
#   Please read the README.txt file in the model directory.     #
#################################################################
lib_LTLIBRARIES = libzmon.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libzmon.pc

include_HEADERS = \
    ../include/zmon.h \
    ../include/zmon_val.h \
    ../include/zmon_gtop.h \
    ../include/zmon_gtop_proc.h

libzmon_la_SOURCES = \
    platform.h \
    zmon_val.c \
    zmon_gtop.c \
    zmon_gtop_proc.c

AM_CFLAGS = -g
AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = zmon_selftest zmon_sensor_gtop
zmon_selftest_LDADD = libzmon.la
zmon_selftest_SOURCES = zmon_selftest.c
zmon_sensor_gtop_LDADD = libzmon.la
zmon_sensor_gtop_SOURCES = zmon_sensor_gtop.c

libzmon_la_LDFLAGS = -version-info @LTVER@

TESTS = zmon_selftest
