dnl Process this file with autoconf to create configure.  Use autoreconf.
AC_PREREQ(2.67)
AC_INIT([swat], [1.53], [mzimbres@gmail.com],[],[http://www.ifi.unicamp.br/~mzimbres])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CXX

# BOOST
#AX_BOOST_BASE([1.42], , [AC_MSG_ERROR(BOOST >= 1.50 or above is a prerequisite.)])
#AX_BOOST_PROGRAM_OPTIONS

# FFTW
PKG_CHECK_MODULES(FFTW3, fftw3 >= 3.0)

# ROOT
ROOT_PATH(5.30, , AC_MSG_ERROR(Your ROOT version is too old))
ROOT_FEATURE([fftw3],,[AC_MSG_ERROR([This programs needs fftw3 enabled in root])])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

