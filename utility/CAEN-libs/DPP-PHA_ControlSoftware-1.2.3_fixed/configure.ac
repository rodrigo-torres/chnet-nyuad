#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([DPP-PHA_ControlSoftware], [1.2.3], [])
AM_INIT_AUTOMAKE([DPP-PHA_ControlSoftware], [1.2.3])
AC_OUTPUT([Makefile src/Makefile GUI/Makefile])
AC_CONFIG_SRCDIR([inc/Analyzer.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([math.h stdint.h stdlib.h string.h sys/time.h sys/timeb.h termios.h unistd.h CAENDigitizer.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit gettimeofday memset select])
AC_CHECK_LIB(m, [sqrt], [], [
        echo "Error! Mathematic Library not found or invalid."
        exit -1
        ])
AC_CHECK_LIB(pthread, [pthread_mutex_init], [], [
        echo "Error! pthread Library not found or invalid."
        exit -1
        ])
AC_CHECK_LIB(c, [malloc], [], [
        echo "Error! C Library not found or invalid."
        exit -1
        ])
AC_CHECK_LIB(CAENDigitizer, CAEN_DGTZ_SetDPP_PHA_VirtualProbe, [], [
        echo "Error! version of CAENDigitizer Library >= 2.0.0 required."
        exit -1
        ])
AC_CHECK_PROG(HAVE_JAVA, java, yes, no)
if test "x$HAVE_JAVA" = "xno"; then
	AC_MSG_ERROR([*** Java Runtime Environment not found])
fi
AC_CHECK_PROG(HAVE_GNUPLOT, gnuplot, yes, no)
if test "x$HAVE_GNUPLOT" = "xno"; then
	AC_MSG_ERROR([*** Gnuplot not found])
fi

AC_OUTPUT
