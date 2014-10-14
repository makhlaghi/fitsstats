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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "config.h"
#include "fitsstats.h"

#include "stats.h"		/* Needs fitsstats.h */




/****************************************************************
 *****************         For qsort()       ********************
 ****************************************************************/
int
floatincreasing(const void * a, const void * b)
{
  float ta=*(float*)a;
  float tb=*(float*)b;
  return (ta > tb) - (ta < tb);
}




















/****************************************************************
 *****************    Mininum and Maximum    ********************
 ****************************************************************/
void
floatmin(float *in, size_t size, float *min)
{
  float tmin=MAXFD, *fpt;
  fpt=in+size;
  do
    if(*in<tmin) tmin=*in;
  while(++in<fpt);
  *min=tmin;
}





void
floatmax(float *in, size_t size, float *max)
{
  float tmax=MINFD, *fpt;
  fpt=in+size;
  do
    if(*in>tmax) tmax=*in;
  while(++in<fpt);
  *max=tmax;
}





void
fminmax(float *in, size_t size, float *min, float *max)
{
  float tmin=MAXFD, tmax=MINFD, *fpt;
  fpt=in+size;
  do
    {
      if     (*in>tmax) tmax=*in;
      else if(*in<tmin) tmin=*in;
    }
  while(++in<fpt);
  *max=tmax;    
  *min=tmin;    
}




void
floatminmasked(float *in, unsigned char *mask, size_t size, float *min)
{
  float tmin=MAXFD, *fpt;
  fpt=in+size;
  do
    if(*mask++==0 && *in<tmin) 
      tmin=*in;
  while(++in<fpt);
  *min=tmin;
}





void
floatmaxmasked(float *in, unsigned char *mask, size_t size, float *max)
{
  float tmax=MINFD, *fpt;
  fpt=in+size;
  do
    if(*mask++==0 && *in>tmax) 
      tmax=*in;
  while(++in<fpt);
  *max=tmax;
}




















/****************************************************************
 *****************         Average and STD       ****************
 ****************************************************************/
void
floatavestd(float *data, size_t size, float *ave, float *std, 
	    double *sum)
{
  double tave, tsum;
  float *f, *fp;

  /* Sum over all the elements to find the average: */
  *sum=0.0f;
  fp=(f=data)+size;
  do *sum+=*f; while(++f<fp);
  tave=*sum/size;

  /* Find the differeces, square them, add them and divide by the
     total number to find the standard deviation: */
  tsum=0.0f;
  fp=(f=data)+size;
  do tsum+=(*f-tave)*(*f-tave); while(++f<fp);
  *std=sqrt(tsum/size);
  *ave=tave;
}




















/****************************************************************
 *****************          Quantiles            ****************
 ****************************************************************/
/* Find the index corresponding to a certain quantile, considering the
   rounding that might be needed. */
size_t 
indexfromquantile(size_t size, float quant)
{
  float floatindex;

  assert(quant<=1.0f);

  /* Find the index of the quantile. */
  floatindex=(float)size*quant;
  /*
  printf("quant: %f, size: %lu, findex: %f\n", quant, size, floatindex);
  */
  /* Note that in the conversion from float to size_t, the floor
     integer value of the float will be used. */
  if( floatindex - (int)floatindex > 0.5 ) 
    return floatindex+1;
  else
    return floatindex;
}




















/****************************************************************
 *************        Histogram and CDF        ******************
 ****************************************************************/
/* Set the bin lower values for all the bins: */
#define CHECKBINS 0
void
setbins(struct fitsstatsparams *p, int h0c1)
{
  float *sorted=p->sorted;
  size_t i, numbins, size=p->size;
  float tosubtract, *bins, binwidth, min, max, quant;

  assert(h0c1==0 || h0c1==1);
  if(h0c1==0) 
    {numbins=p->histnumbins; min=p->histmin; 
      max=p->histmax; quant=p->histquant;}
  else if(h0c1==1)
    {numbins=p->cfpnum; min=p->cfpmin; 
      max=p->cfpmax; quant=p->cfpquant;}

  /* Allocate space for the array. The extra bin is only for internal
     purposes (so the loops for the histogram and CFP can see the end
     of the last bin). It will never be seen by the user. */
  assert( (bins=p->bins=calloc((numbins+1)*2,sizeof *bins))!=NULL );

  /* If the range is not defined, find it and set the bin width. */
  if(min==max)
    {
      if(quant!=0.0f)
	{
	  min=sorted[ indexfromquantile(size, quant)   ];
	  max=sorted[ indexfromquantile(size, 1-quant) ];
	}
      else
	{
	  min=sorted[0];
	  max=sorted[size-1];
	  /* The number of elements in each bin is counted by those
	     equal or smaller to the smaller bin side and smaller than
	     the right of the bin. Therefore, if the maximum bin side
	     equals the maximum element value, it will not be
	     counted. So we slightly increase the maximum before
	     calculating the bin width */
	  max+=(max-min)/10000;
	}
    }
  p->binwidth=binwidth=(max-min)/numbins;

  /* Set all the bin smaller sides: */
  for(i=0;i<numbins+1;++i) 
    bins[i*2]=min+i*binwidth;

  /* If one bin is to be placed on zero. */
  if(p->binonzero)
    {
      for(i=0;i<numbins+1;++i) 
	if(bins[i*2]>=0.0f) break;
      tosubtract=bins[i*2];
      for(i=0;i<numbins+1;++i) 
	bins[i*2]-=tosubtract;
    }  

  /* In case you want to check the bins: */
  if(CHECKBINS)
    for(i=0;i<numbins;++i)
      printf("%lu: %.4f\n", i+1, bins[i*2]);
}





#define CHECKHIST 0
void
histogram(struct fitsstatsparams *p)
{
  float max=MINFD, *sorted=p->sorted, *bins=p->bins;
  size_t histrow, i, size=p->size, numbins=p->histnumbins;

  /* Fill the histogram. */
  histrow=0;
  for(i=0;i<size;++i)
    {
      if(sorted[i]<bins[histrow*2]) continue;
      while (sorted[i]>=bins[(histrow+1)*2])
	if(++histrow>=numbins) break;
      if(histrow>=numbins) break;
      ++bins[histrow*2+1];
    }

  /* In case a normalized histogram is desired: */
  if(p->normhist)
    for(i=0;i<numbins;++i)
      bins[i*2+1]/=size;

  /* In case the maximum value is to become one. */
  if(p->maxhistone)
    {
      for(i=0;i<numbins;++i)
	if(bins[i*2+1]>max) 
	  max=bins[i*2+1];
      for(i=0;i<numbins;++i)
	bins[i*2+1]/=max;
    }

  /* In case you want to see the histogram: */
  if(CHECKHIST)
    for(i=0;i<numbins;++i)
      printf("%lu: %.4f %.4F\n", i+1, bins[i*2], bins[i*2+1]);
}





#define CHECKCFP 0
void
cumulativefp(struct fitsstatsparams *p)
{
  size_t numbins=p->cfpnum;
  size_t cfprow=0, i, numinds=0, size=p->size;
  float prevind=0, *sorted=p->sorted, *bins=p->bins;

  /* Fill the Cumulative frequency plot: */
  for(i=0;i<size;++i)
    {
      if(sorted[i]<bins[cfprow*2]) continue;
      while (sorted[i]>=bins[(cfprow+1)*2])
	{
	  if(numinds>0)
	    prevind=bins[cfprow*2+1]/=numinds;
	  else
	    bins[cfprow*2+1]=prevind;
	  numinds=0;
	  if(++cfprow>=numbins) 
	    break;
	}
      if(cfprow>=numbins) break;
      bins[cfprow*2+1]+=i;	/* Sum of indexs */
      ++numinds;
    }

  /* For a normalized CFP: */
  if(p->normcfp)
    for(i=0;i<numbins;++i)
      bins[i*2+1]/=size;

  /* In case you want to see the CFP: */
  if(CHECKCFP)
    for(i=0;i<numbins;++i)
      printf("%lu: %.4f %.4F\n", i+1, bins[i*2], bins[i*2+1]);
}




















/****************************************************************
 *****************        Sigma clip         ********************
 ****************************************************************/
/* This function will repeatedly sigma clip the data and return the
   median. It is assumed that the data have been ordered.  */
void
sigmaclip_converge(struct fitsstatsparams *p)
{
  double sum;
  size_t counter=0, numelem=p->size;
  float oldstd=0, oldmed=0, oldave=0;
  float med, ave, std, accuracy=p->converge;
  float *start, *oldstart, *dpt, sigm=p->sigmultip;

  start=p->sorted;
  while(1 && numelem>=p->size/3)
    {
      oldstart=start;

      floatavestd(start, numelem, &ave, &std, &sum);
      med=start[indexfromquantile(numelem, 0.5f)];
       
      printf("      %lu: (%f, %f, %f, %lu)\n", counter+1, med, ave, 
	     std, numelem);

      /* Normally, oldstd should be larger than std, because the
	 possible outliers have been removed. If it is not, it means
	 that we have clipped too much! */
      if(counter>0 && ((oldstd-std)/std)<accuracy) 
	{
	  std=oldstd; ave=oldave; med=oldmed;
	  return;
	}

      for(dpt=start; dpt<start+numelem; ++dpt)
	if ( *dpt>(med-sigm*std) ) 
	  {
	    start=dpt; 
	    break;
	  }

      for(dpt=oldstart+numelem-1;dpt>start;dpt--)
	if ( *dpt<(med+sigm*std) )
	  {
	    numelem=dpt-start+1; 
	    break;
	  }
      
      oldave=ave;
      oldmed=med;
      oldstd=std;

      if(counter++>=MAXCONVERGE_V-1)
	{
	  printf("      ####Convergence not reached!\n");
	  return;
	}
    }
}





/* This function will do a certain number of sigma clips and return
   the final average, median and std. o1_n0: 1: initially ordered. 2:
   initially not ordered.*/
void
sigmaclip_certainnum(struct fitsstatsparams *p)
{
  double sum;
  float *start, *oldstart, *dpt;
  size_t counter=0, numelem=p->size;
  float med, ave, std, sigm=p->sigmultip;

  start=p->sorted;
  while(counter<p->timesclip)
    {
      oldstart=start;

      floatavestd(start, numelem, &ave, &std, &sum);
      med=start[indexfromquantile(numelem, 0.5f)];
       
      printf("      %lu: (%f, %f, %f, %lu)\n", counter+1, med, ave,
	     std, numelem);

      for(dpt=start; dpt<start+numelem; ++dpt)
	if ( *dpt>(med-sigm*std) ) 
	  {
	    start=dpt; 
	    break;
	  }

      for(dpt=oldstart+numelem-1;dpt>start;dpt--)
	if ( *dpt<(med+sigm*std) )
	  {
	    numelem=dpt-start+1; 
	    break;
	  }

      ++counter;
    }
}
