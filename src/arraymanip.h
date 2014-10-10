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
#ifndef ARRAYMANIP_H
#define ARRAYMANIP_H

void
floatarrcpy(float *in, size_t size, float **out);

void
converttofloat(void *in, int bitpix, size_t size, float **out, 
	       int byt_code, int short_code, int long_code, 
	       int float_code, int double_code);

void
converttouchar(void *in, int bitpix, size_t size, unsigned char **out, 
	       int byt_code, int short_code, int long_code, 
	       int float_code, int double_code);

void
floatcroparray(float **in, int size1, int size2,
	       int x1, int y1, int x2, int y2);

void
ucharcroparray(unsigned char **in, int size1, int size2,
	       int x1, int y1, int x2, int y2);

#endif
