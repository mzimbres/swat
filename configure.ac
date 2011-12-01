AC_PREREQ(2.67)
AC_INIT([Swat], [1.41], [mzimbres@gmail.com],[],[http://www.ifi.unicamp.br/~mzimbres])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CXX

# Looking for ROOT
ROOT_PATH(5.20, , AC_MSG_ERROR(Your ROOT version is too old))
ROOT_FEATURE([fftw3],,[AC_MSG_ERROR([This programs needs fftw3 enabled in root])])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

