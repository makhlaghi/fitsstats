# Process this file with autoconf to produce a configure script.
#
# Hand written file: used as input into GNU Autotools (autoconf).
#
# This is the autoconf instructions to build FitsStats. FitsStats will
# read the FITS image statistics and print the important statistics
# and save a histogram. It is free software released under GNU GPLv3+.
#
# Copyright (C) 2014 Mohammad Akhlaghi
# Tohoku University Astronomical Institute, Sendai, Japan.
# http://astr.tohoku.ac.jp/~akhlaghi/



# Definitions:
AC_PREREQ([2.69]) 
AC_INIT([FitsStats], [0.1], [makhlaghi@gmail.com], 
        [fitsstats], [https://github.com/makhlaghi/fitsstats])
AM_INIT_AUTOMAKE([-Wall -Werror foreign]) 
AC_CONFIG_SRCDIR([src/ui.h])
AC_CONFIG_HEADERS([config.h])


# Checks for programs.
${CFLAGS=""}
AC_PROG_CC


# Checks for libraries.
# FIXME: Replace `main' with a function in `-lcfitsio':
AC_CHECK_LIB([cfitsio], [main], [], [AC_MSG_ERROR([Cannot continue.])])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main], [], [AC_MSG_ERROR([Cannot continue.])])
# FIXME: Replace `main' with a function in `-lwcslib':
AC_CHECK_LIB([wcs], [main], [], [])


# Checks for header files.
AC_CHECK_HEADERS([argp.h float.h stdlib.h string.h sys/time.h unistd.h], 
                 [], [AC_MSG_ERROR([Cannot continue.])])


# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T




# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([gettimeofday memset sqrt strtol])




#Read the compilation date.
AC_MSG_CHECKING([for date of compilation for verbose mode])
AC_DEFINE_UNQUOTED([COMPILEDATE], ["`date +"%d %B %Y"`"], 
                   [Date of compilation])
AC_MSG_RESULT([done])




#Read the default values:
AC_MSG_CHECKING([defaults from `default.ac`])
source $srcdir/defaults.ac
AC_DEFINE_UNQUOTED([DP_IMGEXT_T], ["$imgext"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_IMGEXT_V], [$imgext], [Default parameter])
AC_DEFINE_UNQUOTED([DP_MASK], ["$mask"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_MASKEXT_T], ["$maskext"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_MASKEXT_V], [$maskext], [Default parameter])
AC_DEFINE_UNQUOTED([DP_X1_T], ["$x1"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_X1_V], [$x1], [Default parameter])
AC_DEFINE_UNQUOTED([DP_Y1_T], ["$y1"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_Y1_V], [$y1], [Default parameter])
AC_DEFINE_UNQUOTED([DP_X2_T], ["$x2"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_X2_V], [$x2], [Default parameter])
AC_DEFINE_UNQUOTED([DP_Y2_T], ["$y2"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_Y2_V], [$y2], [Default parameter])
AC_DEFINE_UNQUOTED([DP_HISTMIN_T], ["$histmin"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_HISTMIN_V], [$histmin], [Default parameter])
AC_DEFINE_UNQUOTED([DP_HISTMAX_T], ["$histmax"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_HISTMAX_V], [$histmax], [Default parameter])
AC_DEFINE_UNQUOTED([DP_HISTNUMBINS_T], ["$histnumbins"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_HISTNUMBINS_V], [$histnumbins], [Default parameter])
AC_DEFINE_UNQUOTED([DP_HISTQUANT_T], ["$histquant"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_HISTQUANT_V], [$histquant], [Default parameter])
AC_DEFINE_UNQUOTED([DP_CFPMIN_T], ["$cfpmin"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_CFPMIN_V], [$cfpmin], [Default parameter])
AC_DEFINE_UNQUOTED([DP_CFPMAX_T], ["$cfpmax"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_CFPMAX_V], [$cfpmax], [Default parameter])
AC_DEFINE_UNQUOTED([DP_CFPNUM_T], ["$cfpnum"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_CFPNUM_V], [$cfpnum], [Default parameter])
AC_DEFINE_UNQUOTED([DP_CFPQUANT_T], ["$cfpquant"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_CFPQUANT_V], [$cfpquant], [Default parameter])
AC_DEFINE_UNQUOTED([DP_SIGMULTIP_T], ["$sigmultip"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_SIGMULTIP_V], [$sigmultip], [Default parameter])
AC_DEFINE_UNQUOTED([DP_CONVERGE_T], ["$converge"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_CONVERGE_V], [$converge], [Default parameter])
AC_DEFINE_UNQUOTED([DP_TIMESCLIP_T], ["$timesclip"], [Default parameter])
AC_DEFINE_UNQUOTED([DP_TIMESCLIP_V], [$timesclip], [Default parameter])
AC_MSG_RESULT([done])



#Tell configure which files to work on:
AC_CONFIG_FILES([Makefile
                 src/Makefile])


#Make the outputs:
AC_OUTPUT
 

#Print a bye-bye message:
echo
echo "==================================================="
echo "AC_PACKAGE_STRING is now configured:"
echo
echo "You are now ready to compile AC_PACKAGE_STRING".
echo "Simply run the commands: \`make\` then \`make install\`"
echo "==================================================="
echo
