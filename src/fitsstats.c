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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fitsstats.h"
#include "arraymanip.h"

#include "stats.h"		/* Needs fitsstats.h */
#include "outputs.h"		/* Needs fitsstats.h */


#define FNAMEVAL "   -- %-45s%f\n"
#define SNAMEVAL "   -- %-45s%lu\n"





/* This function will report the simple immediate statistics of the
   data. For the average and standard deviation, the unsorted data is
   used so we don't suddenly encounter rounding errors. */
void
reportsimplestats(struct fitsstatsparams *p)
{
  double sum;
  float ave, std, med;
  floatavestd(p->img, p->size, &ave, &std, &sum);
  med=p->sorted[indexfromquantile(p->size, 0.5f)];

  printf(SNAMEVAL FNAMEVAL FNAMEVAL, "Number of points", p->size, 
	 "Minimum", p->sorted[0], "Maximum", p->sorted[p->size-1]);
  printf(FNAMEVAL FNAMEVAL FNAMEVAL FNAMEVAL, "Sum", sum, "Mean", ave, 
	 "Standard deviation", std, "Median", med);
}





void
printasciihist(struct fitsstatsparams *p)
{
  size_t j, tnum=p->histnumbins;
  int i, h0c1=0, height=p->asciihisth;
  int tbzero=p->binonzero, tnorm=p->normhist, tmaxone=p->maxhistone;

  /* Find the histogram for the ASCII plot: */
  p->normhist=0;
  p->binonzero=0;
  p->maxhistone=1;
  p->histnumbins=p->asciihistnb;
  setbins(p, h0c1);
  histogram(p);

  /* It's maximum value is set to one. Multiply that by the desired
     height in pixels. */
  for(j=0;j<p->asciihistnb;++j) 
    p->bins[j*2+1]*=height; 

  /* Plot the ASCII histogram: */
  printf("   -- ASCII histogram in the range: %f - %f:\n\n", 
	 p->histmin, p->histmax);
  for(i=height;i>=0;--i)
    {
      printf("    |");
      for(j=0;j<p->asciihistnb;++j)
	{
	  if(p->bins[j*2+1]>=((float)i-0.5) 
	     && p->bins[j*2+1]>0.0f) printf("*");
	  else printf(" ");
	}
      printf("\n");
    }
  printf("    |"); 
  for(j=0;j<p->asciihistnb;++j) printf("-"); 
  printf("\n\n");
  
  /* Free the allocated array and set the parameters back to what they
     were. */
  free(p->bins);
  p->normhist=tnorm;
  p->histnumbins=tnum;
  p->binonzero=tbzero;
  p->maxhistone=tmaxone;
}





void
fitsstats(struct fitsstatsparams *p)
{
  int h0c1;
  size_t i;
  float maxhist=MINFD;

  /* Initialize p->bins so there is no problem in the freeing of the
     end: */
  p->bins=NULL;
 
  /* Make a sorted array for most of the next jobs: */
  floatarrcpy(p->img, p->size, &p->sorted);
  qsort(p->sorted, p->size, sizeof *p->sorted, floatincreasing);

  /* Report the simple statistics: */
  if(p->verb)
    {
      reportsimplestats(p);
      printasciihist(p);
    }

  /* Make the histogram: */
  if(p->histname)
    {
      h0c1=0;
      setbins(p, h0c1);
      histogram(p);
      printhistcfp(p, h0c1);

      if(p->maxcfpeqmaxhist)
	for(i=0;i<p->histnumbins;++i) 
	  if(p->bins[i*2+1]>maxhist) 
	    maxhist=p->bins[i*2+1];
    }

  /* Make the cumulative distribution function: */
  if(p->cfpname)
    {
      h0c1=1;
      if(p->cfpsimhist)
	{
	  p->cfpnum=p->histnumbins;
	  for(i=0;i<p->cfpnum;++i) 
	    p->bins[i*2+1]=0.0f;
	}
      else
	{
	  if(p->histname) free(p->bins);
	  setbins(p, h0c1);
	}
      cumulativefp(p);

      if(p->maxcfpeqmaxhist)
	for(i=0;i<p->cfpnum;++i) 
	  p->bins[i*2+1]*=(maxhist/p->size); 

      printhistcfp(p, h0c1);
    }

  /* Print out the Sigma clippings: */
  if(p->verb && p->sigmaclip)
    {
      printf(" - %.2f sigma-clipping by convergence (med, mean, "
	     "std, number):\n", p->sigmultip);
      sigmaclip_converge(p);
      printf(" - %.2f sigma-clipping %lu times (med, mean, std, number):\n", 
	     p->sigmultip, p->timesclip);
      sigmaclip_certainnum(p);
    }

  /* Free the allocated arrays: */
  free(p->sorted);
  if(p->histname || p->cfpname) free(p->bins);
}
