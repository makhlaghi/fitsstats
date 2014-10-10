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

#include "arraymanip.h"


/****************************************************************
 ********************          Copy         *********************
 ****************************************************************/
void
floatarrcpy(float *in, size_t size, float **out)
{
  float *ipt, *opt, *fpt;
  ipt=in;
  *out=malloc(sizeof(float)*size);
  assert(*out!=NULL);
  fpt=*out+size;
  opt=*out;
  do
    *opt=*ipt++;
  while(++opt<fpt);
}




















/****************************************************************
 ********************        Convert        *********************
 ****************************************************************/
/* Convert any of the known types to float. The assertion that the
   output array be NULL before comming into this array is to make sure
   it was not allocated before this.  */
void
converttofloat(void *in, int bitpix, size_t size, float **out, 
	       int byt_code, int short_code, int long_code, 
	       int float_code, int double_code)
{
  float *o;
  unsigned char *u, *up;
  short         *s, *sp;
  long          *l, *lp;
  float         *f, *fp;
  double        *d, *dp;

  if(bitpix==float_code)
    {
      *out=in;
      return;
    }

  assert( *out==NULL );
  assert( (o=*out=malloc(size*sizeof *o))!=NULL  );

  if(bitpix==byt_code)
    {u=in; up=u+size; do *o++=*u; while(++u<up);}
  else if(bitpix==short_code)
    {s=in; sp=s+size; do *o++=*s; while(++s<sp);}
  else if(bitpix==long_code)
    {l=in; lp=l+size; do *o++=*l; while(++l<lp);}
  else if(bitpix==float_code)
    {f=in; fp=f+size; do *o++=*f; while(++f<fp);}
  else if(bitpix==double_code)
    {d=in; dp=d+size; do *o++=*d; while(++d<dp);}
  else
    {
      printf("\n\nbitpix=%d Not recognized in converting to float!\n",
	     bitpix);
      printf("Recognized bitpix values are:\n"); 
      printf("\tUnsigned char: %d\n", byt_code);
      printf("\tShort: %d\n", short_code);
      printf("\tLong: %d\n", long_code);
      printf("\tFloat: %d\n", float_code);
      printf("\tDouble: %d\n", double_code);
      exit(EXIT_FAILURE);
    }
}





/* Similar to convertofloat(), but convert to unsigne char. */
void
converttouchar(void *in, int bitpix, size_t size, unsigned char **out, 
	       int byt_code, int short_code, int long_code, 
	       int float_code, int double_code)
{
  unsigned char *o;
  unsigned char *u, *up;
  short         *s, *sp;
  long          *l, *lp;
  float         *f, *fp;
  double        *d, *dp;

  if(bitpix==byt_code) 
    {
      *out=in;
      return;
    }

  assert( *out==NULL );
  assert( (o=*out=malloc(size*sizeof *o))!=NULL  );

  if(bitpix==byt_code)
    {u=in; up=u+size; do *o++=*u>0; while(++u<up);}
  else if(bitpix==short_code)
    {s=in; sp=s+size; do *o++=*s>0; while(++s<sp);}
  else if(bitpix==long_code)
    {l=in; lp=l+size; do *o++=*l>0; while(++l<lp);}
  else if(bitpix==float_code)
    {f=in; fp=f+size; do *o++=*f>0; while(++f<fp);}
  else if(bitpix==double_code)
    {d=in; dp=d+size; do *o++=*d>0; while(++d<dp);}
  else
    {
      printf("\n\nbitpix=%d Not recognized in converting to uchar!\n",
	     bitpix);
      printf("Recognized bitpix values are:\n"); 
      printf("\tUnsigned char: %d\n", byt_code);
      printf("\tShort: %d\n", short_code);
      printf("\tLong: %d\n", long_code);
      printf("\tFloat: %d\n", float_code);
      printf("\tDouble: %d\n", double_code);
      exit(EXIT_FAILURE);
    }
}




















/****************************************************************
 ********************         Crop          *********************
 ****************************************************************/
/* Check to see if a box defined by the two points (x1,y1) and (x2,y2)
   is inside an array of size size1 and size2. If it doesn't overlap,
   then x1=x2 and y1=y2.*/
void
checkifinarray(int *x1, int *y1, int *x2, int *y2, int s0, int s1)
{
  int temp;
  if(*x1==*x2 && *y1==*y2) return;        

  if(*x2<*x1){temp=*x1;*x1=*x2;*x2=temp;} 
  if(*y2<*y1){temp=*y1;*y1=*y2;*y2=temp;}

  if(*x1<0) *x1=0;    if(*x1>s0) *x1=s0;
  if(*y1<0) *y1=0;    if(*y1>s1) *y1=s1;
  if(*x2<0) *x2=0;    if(*x2>s0) *x2=s0;
  if(*y2<0) *y2=0;    if(*y2>s1) *y2=s1;
}





/* We have a large array of size (size1*size2). We want to shrink this
   array, such that (x1,y1) comes down to point (0,0) and the new
   array now only extends to the old (x2,y2). So the size of the new
   array is: w1*w2 where w1=x2-x1 and w2=y2-y1. 
   
   If the desired region is totally out of the array, a NULL pointer
   is returned.*/
void
floatcroparray(float **in, int size1, int size2,
	       int x1, int y1, int x2, int y2)
{
  float *ifpt, *ofpt, *rowstart;
  size_t i, ux1, uy1, us2, w1, w2;

  checkifinarray(&x1, &y1, &x2, &y2, size1, size2);
  if(x1==x2 || y1==y2) 		/* The required region does not */
    {				/* overlap with the array. */
      free(*in);
      *in=NULL;
      return;
    }
  /* The region covers the whole image, no need for the next step. */
  if(x1==0 && y1==0 && x2==size1 && y2==size2) return;

  w1=x2-x1;  w2=y2-y1;
  ux1=x1; uy1=y1; us2=size2;  /* The inputs are int (can be negative,
				 which is allowed: will become zero).
				 but pointers are unsigned, so to 
				 faciliate the process in the loop, 
				 they are converted to size_t. */
  for(i=0;i<w1;++i)
    {
      ofpt=rowstart=*in+i*w2;
      ifpt=*in+(ux1+i)*us2+uy1;
      do
	*ofpt=*ifpt++;
      while(++ofpt<rowstart+w2);
 
    }      
  *in=(float *)realloc(*in, w1*w2*sizeof(float));
  assert(*in!=NULL);    
}





void
ucharcroparray(unsigned char **in, int size1, int size2,
	       int x1, int y1, int x2, int y2)
{
  size_t i, ux1, uy1, us2, w1, w2;
  unsigned char *ifpt, *ofpt, *rowstart;

  checkifinarray(&x1, &y1, &x2, &y2, size1, size2);
  if(x1==x2 || y1==y2) 		/* The required region does not */
    {				/* overlap with the array. */
      free(*in);
      *in=NULL;
      return;
    }
  /* The region covers the whole image, no need for the next step. */
  if(x1==0 && y1==0 && x2==size1 && y2==size2) return;

  w1=x2-x1;  w2=y2-y1;
  ux1=x1; uy1=y1; us2=size2;  /* The inputs are int (can be negative,
				 which is allowed: will become zero).
				 but pointers are unsigned, so to 
				 faciliate the process in the loop, 
				 they are converted to size_t. */
  for(i=0;i<w1;++i)
    {
      ofpt=rowstart=*in+i*w2;
      ifpt=*in+(ux1+i)*us2+uy1;
      do
	*ofpt=*ifpt++;
      while(++ofpt<rowstart+w2);
 
    }      
  *in=(unsigned char *)realloc(*in, w1*w2*sizeof(unsigned char));
  assert(*in!=NULL);    
}
