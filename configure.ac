#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([Swat], [1.35], [mzimbres@gmail.com],[],[http://www.ifi.unicamp.br/~mzimbres])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AC_PROG_LIBTOOL
#AC_PREFIX_DEFAULT(${HOME})

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Looking for ROOT
ROOT_PATH(5.20, , AC_MSG_ERROR(Your ROOT version is too old))
ROOT_FEATURE([fftw3],,[AC_MSG_ERROR([This programs needs fftw3 enabled in root])])

# Test for Healpix
#AC_ARG_WITH([healpix-incdir],
#   [AS_HELP_STRING([--with-healpix-incdir],[use healpix (default is no)])],
#   [AC_SUBST([healpix_inc_dir],[$withval])
#    AM_CONDITIONAL([HEALPIXINCDIR],[test xtrue = xtrue])
#    AC_MSG_NOTICE([Healpix include dir provided])],
#   [AM_CONDITIONAL([HEALPIXINCDIR],[test xtrue != xtrue])
#    AC_MSG_NOTICE([Healpix include dir not provided])])
#
## Test for Healpix
#AC_ARG_WITH([healpix-libdir],
#   [AS_HELP_STRING([--with-healpix-libdir],[use healpix (default is no)])],
#   [AC_SUBST([healpix_lib_dir],[$withval])
#    AM_CONDITIONAL([HEALPIXLIBDIR],[test xtrue = xtrue])
#    AC_MSG_NOTICE([Healpix lib dir provided])],
#   [AM_CONDITIONAL([HEALPIXLIBDIR],[test xtrue != xtrue])
#    AC_MSG_NOTICE([Healpix lib dir not provided])])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

