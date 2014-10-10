/*********************************************************************
FitsStats - Get the basic statistics of an image.

Copyright (C) 2014 Mohammad Akhlaghi
Tohoku University Astronomical Institute, Sendai, Japan.
http://astr.tohoku.ac.jp/~akhlaghi/

FitsStats is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

FitsStats is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FitsStats. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/
#ifndef UI_H
#define UI_H

#define NUMDASHES 70

void
printdashes(int s1_e0);

void
intelzero(char *optarg, int *var, char *lo, char so);

void
intlzero(char *optarg, int *var, char *lo, char so);

void
sizetelzero(char *optarg, size_t *var, char *lo, char so);

void
sizetlzero(char *optarg, size_t *var, char *lo, char so);

void
floatl0(char *optarg, float *var, char *lo, char so);

void
floatl0s1(char *optarg, float *var, char *lo, char so);

void
anyfloat(char *optarg, float *var);

void
anydouble(char *optarg, double *var);

void
setparams(int argc, char *argv[], struct fitsstatsparams *p, 
	  time_t *rawtime);

void
freeparams(struct fitsstatsparams *p);

#endif
