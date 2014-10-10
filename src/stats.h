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
#ifndef STATS_H
#define STATS_H

#define MINFD -1e30
#define MAXFD 1e30

int
floatincreasing(const void * a, const void * b);

void
floatmin(float *in, size_t size, float *min);

void
floatmax(float *in, size_t size, float *max);

void
fminmax(float *in, size_t size, float *min, float *max);

void
floatminmasked(float *in, unsigned char *mask, size_t size, float *min);

void
floatmaxmasked(float *in, unsigned char *mask, size_t size, float *max);

void
floatavestd(float *data, size_t size, float *ave, float *std);

size_t 
indexfromquantile(size_t size, float quant);

void
setbins(struct fitsstatsparams *p, int h0c1);

void
histogram(struct fitsstatsparams *p);

void
cumulativefp(struct fitsstatsparams *p);

void
sigmaclip_converge(struct fitsstatsparams *p);

void
sigmaclip_certainnum(struct fitsstatsparams *p);

#endif
