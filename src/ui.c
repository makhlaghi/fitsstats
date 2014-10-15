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
#include <argp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/time.h>

#include "config.h"
#include "fitsstats.h"
#include "arraymanip.h"
#include "fitsarrayvv.h"

#include "stats.h"		/* Needs fitsstats.h          */
#include "ui.h"			/* Needs fitsstats.h          */
#include "argpparser.h"		/* Needs fitsstats.h and ui.h */









/****************************************************************
 *****************     Print information:    ********************
 ****************************************************************/
/* s1_e0 tells it if it is printing the dashes at the start or the
   end of the report. */
void
printdashes(int s1_e0)
{
  size_t i=0;
  if(s1_e0) printf("\n\n");
  while(i++<NUMDASHES) printf("-");
  if(!s1_e0) printf("\n\n");
  else printf("\n");
}




















/****************************************************************
 *****************      Check the inputs     ********************
 ****************************************************************/
void
intelzero(char *optarg, int *var, char *lo, char so)
{
  long tmp;
  char *tailptr;
  tmp=strtol(optarg, &tailptr, 0);
  if(strlen(tailptr))
    {
      fprintf(stderr, PACKAGE": the argument to option `-%c`: `%s` was not "
	     "readable as a number!\n", so, optarg);
      exit(EXIT_FAILURE);
    }
  if(tmp<0)
    {
      fprintf(stderr, PACKAGE": argument to `--%s (-%c)` should be >=0, "
	     "it is: %ld\n", lo, so, tmp);
      exit(EXIT_FAILURE);
    }
  *var=tmp;  
}





void
intlzero(char *optarg, int *var, char *lo, char so)
{
  long tmp;
  char *tailptr;
  tmp=strtol(optarg, &tailptr, 0);
  if(strlen(tailptr))
    {
      fprintf(stderr, PACKAGE": the argument to option `-%c`: `%s` was not "
	     "readable as a number!\n", so, optarg);
      exit(EXIT_FAILURE);
    }
  if(tmp<=0)
    {
      fprintf(stderr, PACKAGE": argument to `--%s (-%c)` should be >0, "
	     "it is: %ld\n", lo, so, tmp);
      exit(EXIT_FAILURE);
    }
  *var=tmp;  
}





void
sizetelzero(char *optarg, size_t *var, char *lo, char so)
{
  long tmp;
  char *tailptr;
  tmp=strtol(optarg, &tailptr, 0);
  if(strlen(tailptr))
    {
      fprintf(stderr, PACKAGE": the argument to option `-%c`: `%s` was not "
	     "readable as a number!\n", so, optarg);
      exit(EXIT_FAILURE);
    }
  if(tmp<0)
    {
      fprintf(stderr, PACKAGE": argument to `--%s (-%c)` should be >=0, it "
	     "is: %ld\n", lo, so, tmp);
      exit(EXIT_FAILURE);
    }
  *var=tmp;  
}





void
sizetlzero(char *optarg, size_t *var, char *lo, char so)
{
  long tmp;
  char *tailptr;
  tmp=strtol(optarg, &tailptr, 0);
  if(strlen(tailptr))
    {
      fprintf(stderr, PACKAGE": the argument to option `-%c`: `%s` was not "
	     "readable as a number!\n", so, optarg);
      exit(EXIT_FAILURE);
    }
  if(tmp<=0)
    {
      fprintf(stderr, PACKAGE": argument to `--%s (-%c)` should be >0, it "
	     "is: %ld\n", lo, so, tmp);
      exit(EXIT_FAILURE);
    }
  *var=tmp;  
}





void
floatl0(char *optarg, float *var, char *lo, char so)
{
  float tmp;
  char *tailptr;
  tmp=strtof(optarg, &tailptr);
  if(strlen(tailptr))
    {
      fprintf(stderr, PACKAGE": the argument to option `-%c`: `%s` was not "
	     "readable as a number!\n", so, optarg);
      exit(EXIT_FAILURE);
    }
  if(tmp<0)
    {
      fprintf(stderr, PACKAGE": `--%s (-%c)` > 0, but it is: %.3f\n", 
	     lo, so, tmp);
      exit(EXIT_FAILURE);      
    }
  *var=tmp;
}





void
floatl0s1(char *optarg, float *var, char *lo, char so)
{
  float tmp;
  char *tailptr;
  tmp=strtof(optarg, &tailptr);
  if(strlen(tailptr))
    {
      fprintf(stderr, PACKAGE": the argument to option `-%c`: `%s` was not "
	     "readable as a number!\n", so, optarg);
      exit(EXIT_FAILURE);
    }
  if(tmp>1.0f || tmp<0)
    {
      fprintf(stderr, PACKAGE": 0 <= `--%s (-%c)` <=1, but it is: %.3f\n", 
	     lo, so, tmp);
      exit(EXIT_FAILURE);      
    }
  *var=tmp;
}




void
anyfloat(char *optarg, float *var)
{
  char *tailptr;
  *var=strtof(optarg, &tailptr);
}





void
anydouble(char *optarg, double *var)
{
  char *tailptr;
  *var=strtof(optarg, &tailptr);
}




















/****************************************************************
 *******************    Set the names:    ***********************
 ****************************************************************/
/* Check if a file exists and report if it doesn't: */
void
checkfile(char *filename)
{
  FILE *tmpfile;
  if ((tmpfile = fopen(filename, "r")) != NULL) 
    fclose(tmpfile);
  else
    {
      fprintf(stderr, PACKAGE": Cannot open %s\n", filename);
      exit(EXIT_FAILURE);
    }
}





/* Check if a file exists. If so, remove it. */
void
checkremovefile(char *filename)
{
  FILE *tmpfile;
  if ((tmpfile = fopen(filename, "r")) != NULL) 
    {
      fclose(tmpfile);
      if(unlink(filename)==-1)
	{
	  fprintf(stderr, "%s: '%s' already exists and "
		  "could not be removed\n", 
		 PACKAGE, filename); 
	  exit(EXIT_FAILURE);
	}
    }
}





void
changenameedning(char *in, char *append, char **outname)
{
  size_t i, l, al;
  char *out;

  l=strlen(in);
  al=strlen(append);
  assert( (out=malloc((l+al+5)*sizeof *out))!=NULL );
  strcpy(out, in);
  for(i=l;i!=0;i--)
    if(out[i]=='.')
      {
	out[i]='\0';
	strcat(out, append);
	break;
      }
  if(i==0)
    {
      fprintf(stderr, "%s: %s does not have a '.' in the name\n", 
	     PACKAGE, in);
      exit(EXIT_FAILURE);
    }
  *outname=out;
}





void
setoutputnames(struct fitsstatsparams *p)
{
  if(p->histname)
    {
      changenameedning(p->up.imgname, HISTPOSTFIX, &p->histname);
      checkremovefile(p->histname);
    }
  if(p->cfpname)
    {
      changenameedning(p->up.imgname, CFPPOSTFIX, &p->cfpname);
      checkremovefile(p->cfpname);
    }
}




















/****************************************************************
 ***************      Read the input images      ****************
 ****************************************************************/
void
readimg(struct fitsstatsparams *p)
{
  void *array;
  struct timeval t1;
  int bitpix, s0, s1;
  struct uiparams *up=&p->up;

  /* Read the image into the used array: */
 if(p->verb) gettimeofday(&t1, NULL);
  fits_to_array(up->imgname, up->imgext, &bitpix, &array, 
		&up->s0, &up->s1, 0, NULL, NULL);
  converttofloat(array, bitpix, up->s0 * up->s1, &p->img, 
		 BYTE_IMG, SHORT_IMG, LONG_IMG, FLOAT_IMG, DOUBLE_IMG);
  /* If the image type was something else, a new array has been
     allocated. So the array that fits_to_array() allocated is no
     longer needed and should be freed. */
  if(bitpix!=FLOAT_IMG) 
    free(array);
  if(p->verb)
    printf(" - Input %s[%d] is read.\n", up->imgname, up->imgext);

  /* If the user has asked to crop the image, crop it. Note: The
     positions of the sides are in C format, not FITS format. that is
     why the final four arguments have y first then x. The user is
     assumed to input in the FITS format. */
  s0=up->s0; s1=up->s1;
  if(up->x1!=up->x2 && up->y1!=up->y2)
    {
      /* Check to see if x2 and y2 are smaller than s1 and s0: */
      if(up->x2-1>s1 || up->y2-1>s0)
	{
	  fprintf(stderr, PACKAGE": Input image size is (%d, %d) while top "
		  "right crop box corner is set to (%d, %d). Aborted.\n", 
		  s1, s0, up->x2, up->y2);
	  exit(EXIT_FAILURE);
	}

      floatcroparray(&p->img, up->s0, up->s1, 
		     up->y1-1, up->x1-1, up->y2-1, up->x2-1);
      if(p->verb)
	printf(" - Region (%d, %d) -- (%d, %d) cropped from input image.\n", 
	       up->x1, up->y1, up->x2, up->y2);

      /* Set the size of the image. */
      p->size = (up->x2 - up->x1) * (up->y2 - up->y1);
    }
  else
    p->size = up->s0 * up->s1;
}





void
readmask(struct fitsstatsparams *p)
{
  int bitpix;
  void *array;
  long *l, *lf;
  short *s, *sf;
  size_t ms0, ms1;
  struct uiparams *up=&p->up;
  unsigned char *u, *b, *bf;

  /* Read the mask image: */
  fits_to_array(up->maskname, up->maskext, &bitpix, 
		&array, &ms0, &ms1, 0, NULL, NULL);

  /* Check the mask image type: */
  if(bitpix!=SHORT_IMG && bitpix!=LONG_IMG && bitpix!=BYTE_IMG)
    {
      fprintf(stderr, PACKAGE": The mask image has to have a "
	      "bitpix of %d (byte), %d (short) or %d (long).\n", 
	      BYTE_IMG, SHORT_IMG, LONG_IMG);
      exit(EXIT_FAILURE);
    }

  /* Check the mask image size: */
  if(ms0!=up->s0 || ms1!=up->s1)
    {
      fprintf(stderr, PACKAGE": Input image (%s, ext %d) and mask "
	      "(%s, ext: %d) are a different size (%lux%lu, %lux%lu, "
	      "respectively).\n",
	     up->imgname, up->imgext, up->maskname, up->maskext,
	     up->s1, up->s0, ms1, ms0);
      exit(EXIT_FAILURE);
    }
   
  /* Set the mask image in NoiseChisel's format (0=good pixel, 1=masked) */
  assert( (u=up->mask=malloc(p->size*sizeof *up->mask))!=NULL );
  if(bitpix==BYTE_IMG)
    { bf=(b=array)+p->size; do *u++=*b!=0; while(++b<bf); }
  else if(bitpix==SHORT_IMG)
    { sf=(s=array)+p->size; do *u++=*s!=0; while(++s<sf); }
  else if(bitpix==LONG_IMG)
    { lf=(l=array)+p->size; do *u++=*l!=0; while(++l<lf); }
  else
    {
      fprintf(stderr, PACKAGE": The mask FITS image bitpix is not "
	      "recognized.\n");
      exit(EXIT_FAILURE);
    }
  free(array);

  /* Report timing: */
  if(p->verb)
    printf(" - Mask %s[%d] is read.\n", up->maskname, up->maskext);

  /* Crop masked region if desired. Similar to readimg(). */
  if(up->x1!=up->x2 && up->y1!=up->y2)
    {
      ucharcroparray(&up->mask, up->s0, up->s1, 
		     up->y1-1, up->x1-1, up->y2-1, up->x2-1);
      if(p->verb)
	printf(" - Region (%d, %d) -- (%d, %d) cropped from mask image.\n",
	       up->x1, up->y1, up->x2, up->y2);
    }
}





void
applymask(struct fitsstatsparams *p)
{
  size_t counter=0;
  unsigned char *m;
  struct uiparams *up=&p->up;
  float *f, *fp, min, *img=p->img;

  /* If a mask image was not given, no mask array was allocated, So
     allocate an array: */
  if(up->maskgiven==0)
    assert( (up->mask=calloc(p->size, sizeof *up->mask))!=NULL );

  /* In case the minimum (nonmasked) pixels are to be masked do
     that. */
  if(up->maskmin)
    {
      m=up->mask;
      floatminmasked(img, up->mask, p->size, &min);
      fp=(f=img)+p->size;
      do
	{
	  /* In case the pixel is not masked, it will check to see
	     if the value of the pixel equals the minimum or not. */
	  *m = *m || *f==min;
	  ++m;
	}
      while(++f<fp);
    }

  /* Apply the mask, there is no need to re-allocate the data,
     because no function will pass p->size. */
  m=up->mask;
  fp=(f=img)+p->size;
  do 
    if(*m++==0) 
      img[counter++]=*f;
  while(++f<fp);

  /* Report the number of masked pixels: */
  if(p->verb)
    printf(" - %lu masked pixels removed.\n", p->size-counter);

  /* Change the size: */
  p->size=counter;

  
  
  free(up->mask);
}


















/****************************************************************
 *****************        Read params        ********************
 ****************************************************************/
void
consistencycheck(struct fitsstatsparams *p)
{
  struct uiparams *up=&p->up;

  /* Check if what should be done with the mask and image names and
     extensions: */
  if(strlen(up->maskname)==0)			/* No mask specified. */
    {
      if(up->maskext>=0)   /* mask extension >0 (by default or given).*/
	{
	  if(up->maskext!=up->imgext) 
	    {
	      up->maskgiven=1;
	      up->maskname=up->imgname;
	    }
	  else
	    {
	      fprintf(stderr, PACKAGE": In the absence of a mask name, it "
		      "defaults to the image name, therefore the extensions "
		      "have to be different, but they are the same: %d\n", 
		      up->maskext);
	      exit(EXIT_FAILURE);
	    }
	}
      else up->maskgiven=0;
    }
  else				        /* A mask name is specified. */
    {
      if(up->maskext>=0)
	{
	  if(strcmp(up->maskname, up->imgname)) /* names different.*/
	    {
	      if(up->maskext!=up->imgext)
		up->maskgiven=1;
	      else
		{
		  fprintf(stderr, PACKAGE": The mask name and extension are "
			  "the same as the image name and extension! This "
			  "is not possible!\n");
		  exit(EXIT_FAILURE);
		}
	    }
	  else up->maskgiven=1;
	}
      else
	{
	  if(strcmp(up->maskname, up->imgname)) /* names different */
	    {
	      up->maskgiven=1;
	      up->maskext=0;
	    }
	  else
	    {
	      if(up->imgext==0)
		{
		  fprintf(stderr, PACKAGE": When no mask extension is "
			  "specified, the value of zero will be used. But "
			  "this results in the same name and extension for "
			  "the mask and input images with the settings you "
			  "have provided.\n");
		  exit(EXIT_FAILURE);
		}
	      else
		{
		  up->maskgiven=1;
		  up->maskext=0;
		}
	    }
	}
    }

  /* Check if the box size is correct: */
  if(up->x1>up->x2 || up->y1>up->y2)
    {
      fprintf(stderr, PACKAGE": The bottom left point (%d, %d) has "
	      "larger value(s) than the top right point (%d, %d)\n",
	      up->x1, up->y1, up->x2, up->y2);
      exit(EXIT_FAILURE);
    }

  /* Check if a histogram is asked for if the cumulative frequency
     plot is to be using the same parameters: */
  if(!p->histname && p->cfpname)
    {
      if(p->cfpsimhist)
	fprintf(stderr, PACKAGE": Without a histogram, `--cfpsimhist` is "
		"meaningless.\n");
      else if (p->maxcfpeqmaxhist)
	fprintf(stderr, PACKAGE": Without a histogram, `--maxcfpeqmaxhist` "
		"is meaningless.\n");
      else
	fprintf(stderr, PACKAGE": Impossible condition in "
		"`consistencycheck()`!");
      exit(EXIT_FAILURE);      
    }

  /* Check if `--maxcfpeqmaxhist` and `--normcfp` are not called
     together: */
  if(p->normcfp && p->maxcfpeqmaxhist)
    {
      fprintf(stderr, PACKAGE": `--normcfp` and `--maxcfpeqmaxhist` "
	      "cannot be called together.\n");
      exit(EXIT_FAILURE);
    }

  /* Check if `normhist` and `maxhistone` are not called together: */
  if(p->normhist && p->maxhistone)
    {
      fprintf(stderr, PACKAGE": `--normhist` and `--histnumbins` cannot be "
	      "called together.\n");
      exit(EXIT_FAILURE);
    }

  /* Check if the input image, mask and kernel fits image files exist
     or not: */
  checkfile(up->imgname);
  if(up->maskgiven) 
    checkfile(up->maskname);
}





void
setparams(int argc, char *argv[], struct fitsstatsparams *p, 
	  time_t *rawtime)
{
  struct uiparams *up=&p->up;

  /* Internal paramters to ui.c: */
  up->imgname         = NULL;
  up->imgext          = DP_IMGEXT_V;
  up->maskname        = DP_MASK;
  up->maskext         = DP_MASKEXT_V;
  up->x1              = DP_X1_V;
  up->y1              = DP_Y1_V;
  up->x2              = DP_X2_V;
  up->y2              = DP_Y2_V;
  up->printalloptions = 0;
  up->maskmin         = 0;

  /* Input: */
  p->img             = NULL;

  /* Operating modes: */
  p->verb            = 1;

  /* Histogram: */
  p->histname        = (void *) 1; 
  p->maxhistone      = 0;
  p->normhist        = 0;
  p->binonzero       = 0;
  p->histnumbins     = DP_HISTNUMBINS_V;
  p->histmin         = DP_HISTMIN_V;
  p->histmax         = DP_HISTMAX_V;
  p->histquant       = DP_HISTQUANT_V;

  /* Cumulative frequency plot: */
  p->cfpname         = (void *) 1;
  p->maxcfpeqmaxhist = 0;
  p->normcfp         = 0;
  p->cfpsimhist      = 0;
  p->cfpnum          = DP_CFPNUM_V;
  p->cfpmin          = DP_CFPMIN_V;
  p->cfpmax          = DP_CFPMAX_V;
  p->cfpquant        = DP_CFPQUANT_V;

  /* Sigma-clipping: */
  p->sigmaclip       = 1;
  p->sigmultip       = DP_SIGMULTIP_V;
  p->converge        = DP_CONVERGE_V;
  p->timesclip       = DP_TIMESCLIP_V;

  /* Internal: */
  p->asciihistnb     = DP_ASCIIHISTNB;
  p->asciihisth      = DP_ASCIIHISTH;

  /* Read the arguments: */
  argp_parse(&argp, argc, argv, 0, 0, p);
  
  /* Check consistency: */
  consistencycheck(p);

  if(p->verb)
    {
      printdashes(1);
      printf("%s (%s) started on %s", PACKAGE_STRING, COMPILEDATE, 
	     ctime(rawtime));
    }

  /* Read the input data: */
  readimg(p);
  if(up->maskgiven)
    readmask(p);
  if(up->maskgiven || up->maskmin)
    applymask(p);

  /* Set the names of the output: */
  setoutputnames(p);
}





void
freeparams(struct fitsstatsparams *p)
{
  free(p->img);
  if(p->histname) free(p->histname);
  if(p->cfpname) free(p->cfpname);
}
