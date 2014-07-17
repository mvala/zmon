lib_LTLIBRARIES = libzmon.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libzmon.pc

include_HEADERS = \
    ../include/zmon.h 

libzmon_la_SOURCES = \
    platform.h 

AM_CFLAGS = -g
AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = zmon_selftest
zmon_selftest_LDADD = libzmon.la
zmon_selftest_SOURCES = zmon_selftest.c
libzmon_la_LDFLAGS = -version-info @LTVER@
TESTS = zmon_selftest

# Produce generated models; do this manually in src directory
#code:
#	gsl -q zmon_msg.xml

