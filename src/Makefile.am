## Process this file with automake to produce Makefile.in
##
## Hand written file: used as input into Autotools.
##
## NOTE:
## This file will be called by the Makefile.am in the top directory,
## so this file does not need to be called directly by automake to
## make the full NoiseChisel package.
##
## This is the automake instructions to build FitsStats. FitsStats will
## read the FITS image statistics and print the important statistics
## and save a histogram. It is free software released under GNU GPLv3+.
##
## Copyright (C) 2014 Mohammad Akhlaghi
## Tohoku University Astronomical Institute, Sendai, Japan.
## http://astr.tohoku.ac.jp/~akhlaghi/

##AM_CFLAGS = -Wall -O3
AM_CFLAGS = -Wall -g -O0
AM_LDFLAGS = -lcfitsio -lpthread -lm

bin_PROGRAMS = fitsstats
fitsstats_SOURCES = main.c fitsstats.c fitsstats.h ui.c ui.h		\
fitsarrayvv.c fitsarrayvv.h argpparser.h arraymanip.c arraymanip.h	\
stats.c stats.h outputs.c outputs.h
