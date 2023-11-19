AC_INIT([imported-gnulib], [1.0.0]) # mandatory; set up a project

AM_INIT_AUTOMAKE([foreign 1.16 subdir-objects]) # initialize automake, no GNU Coding Style files such as NEWS, README, AUTHORS

AC_CONFIG_MACRO_DIRS([m4])

AC_PROG_CC # determine the C compiler to use

# init gnulib
gl_EARLY
gl_INIT

AC_CONFIG_FILES([Makefile lib/Makefile]) # create file by copying the corresponding.in template file with substuted variables

AC_CONFIG_HEADERS([config.h])

AC_OUTPUT # mark the end of autoconf
