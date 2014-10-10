/*********************************************************************
fitsarrayvv - FITS image to C array and vice versa.

Copyright (C) 2013, 2014 Mohammad Akhlaghi
Tohoku University Astronomical Institute, Sendai, Japan.
http://astr.tohoku.ac.jp/~akhlaghi/

fitsarrayvv is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fitsarrayvv is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with fitsarrayvv. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "config.h"
#include "fitsarrayvv.h"



/*************************************************************
 ***********        Find number of extentions      ***********
 *************************************************************/
void
numextinfits(char *inname, int *numext)
{
  int status=0;
  fitsfile *fptr;

  fits_open_file(&fptr, inname, READONLY, &status);  
  fits_report_error(stderr, status);

  fits_get_num_hdus(fptr, numext, &status);
  fits_report_error(stderr, status);

  fits_close_file(fptr, &status);
  fits_report_error(stderr, status);
}




















/*************************************************************
 ***********         FITS to array functions:      ***********
 *************************************************************/
/* Allocate space for the various types */
void
f2a_readalloc(void **array, size_t size, int bitpix, 
	      int *datatype, char *ffname)
{
  short **s;  long **l; unsigned char **u;
  float **f; double **d; /* long long **ll; */

  switch(bitpix)
    {
    case BYTE_IMG:
      *datatype=TBYTE;
      u=(unsigned char **)array;
      assert( ( *u=malloc(size* sizeof**u) ) !=NULL);
      break;

    case SHORT_IMG:
      *datatype=TSHORT;
      s=(short **)array;
      assert( ( *s=malloc(size* sizeof**s) ) !=NULL);
      break;

    case LONG_IMG:
      *datatype=TLONG;
      l=(long **)array;
      assert( ( *l=malloc(size* sizeof**l) ) !=NULL);
      break;

    case FLOAT_IMG:
      *datatype=TFLOAT;
      f=(float **)array;
      assert( ( *f=malloc(size* sizeof**f) )!=NULL);
      break;

    case DOUBLE_IMG:
      *datatype=TDOUBLE;
      d=(double **)array;
      assert( ( *d=malloc(size* sizeof**d) ) !=NULL);
      break;

    case LONGLONG_IMG:
      /*
       *datatype=TLONGLONG;
      ll=(long long **)array;
      assert( ( *ll=malloc(size* sizeof**ll) ) !=NULL);
      */
      printf("\n\n%s. BITPIX=%d (long long) Not supported!\n\n",
	     ffname, bitpix);
      exit(EXIT_FAILURE);

    default:
      printf("\n\n%s. BITPIX=%d, Not recognized!\n\n",
	     ffname, bitpix);
      exit(EXIT_FAILURE);
    }
}





/* Read a FITS image into an array corresponding to fitstype and also
   save the size of the array.  */
void 
fits_to_array(char *fits_name, int exten, int *bitpix, void **array, 
	      size_t *s0, size_t *s1, int withwcs, int *nwcs, 
	      struct wcsprm **wcs)
{
  long b;
  fitsfile *fptr;
  float *nulval=NULL;
  long fpixel[2]={1,1}, naxes[]={0,0};
  int status=0, anynul=0, maxdim=2, w_status;
  char *ffname, *fullheader, comment[FLEN_COMMENT];
  int datatype, nkeys, relax=WCSHDR_all, ctrl=0, nreject=0;
  
  /* Add extension to the fits_name: */
  ffname=malloc(FLEN_FILENAME*sizeof(char));   
  sprintf(ffname, "%s[%d]", fits_name, exten);

  /* Open the FITS file. */
  fits_open_file(&fptr, ffname, READONLY, &status);  
  fits_report_error(stderr, status);

  /* Get the WCS information. */
  if(withwcs)
    {
      fits_hdr2str(fptr, 1, NULL, 0, &fullheader, &nkeys, &status);
      if (status!=0)
	{
	  fits_report_error(stderr, status);
	  exit(EXIT_FAILURE);
	}
      w_status = wcspih(fullheader, nkeys, relax, ctrl, &nreject, nwcs, wcs);
      if(w_status)
	{
	  fprintf(stderr, PACKAGE": Failed to read the WCS parameters of "
		  "%s.\n", fits_name);
	  exit(EXIT_FAILURE);
	}
      w_status=wcsset(*wcs);
      free(fullheader);
    }
  
  /* Get the image type: */
  fits_read_key(fptr, TLONG, "BITPIX", &b, comment, &status);
  fits_report_error(stderr, status);
  *bitpix=b;

  /* Get the image size: */
  fits_get_img_size(fptr, maxdim, naxes, &status);
  fits_report_error(stderr, status);
  *s0=naxes[1];
  *s1=naxes[0];
  if(*s0 * *s1 == 0)
    {
      fprintf(stderr, PACKAGE": %s has no data! ", ffname);
      if(exten==0)
	fprintf(stderr, "In some FITS files, the first extension is empty. "
	       "Try asking for the next extension.\n");
      else fprintf(stderr, "\n");
      exit(EXIT_FAILURE);
    }

  /* Allocate space for the array to keep the image. */
  f2a_readalloc(array, *s0 * *s1, *bitpix, &datatype, ffname);

  /* Put the image into the array. */
  fits_read_pix(fptr, datatype, fpixel,
                naxes[0]*naxes[1], nulval,
                *array, &anynul, &status);
  fits_report_error(stderr, status);

  fits_close_file(fptr, &status);
  fits_report_error(stderr, status);

  free(ffname);
}




















/*************************************************************
 ***********         Array to FITS functions:      ***********
 *************************************************************/
void
a2f_setdatatype(int bitpix, int *datatype)
{
  switch(bitpix)
    {
    case BYTE_IMG:
      *datatype=TBYTE;
      break;
    case SHORT_IMG:
      *datatype=TSHORT;
      break;
    case LONG_IMG:
      *datatype=TLONG;
      break;
    case FLOAT_IMG:
      *datatype=TFLOAT;
      break;
    case DOUBLE_IMG:
      *datatype=TDOUBLE;
      break;
    case LONGLONG_IMG:
      *datatype=TLONGLONG;
      break;
    default:
      printf("\n\nInput bixpix=%d, Not recognized!\n\n", bitpix);
      exit(EXIT_FAILURE);
    }
}





void
addheaderinfo(fitsfile *fptr, int *status, struct wcsprm *wcs)
{
  size_t i;
  time_t rawtime;
  int nkeyrec, h;
  char comment[1000];
  char startblank[]="                   / ";
  char *wcsheader, *cp, *cpf, blankrec[80], titlerec[80];

  time(&rawtime);

  /* Set the last element of the blank array. */
  cpf=blankrec+79;
  *cpf='\0';
  titlerec[79]='\0';
  cp=blankrec; do *cp=' '; while(++cp<cpf);

  /* Add the WCS information: */
  fits_write_record(fptr, blankrec, status);
  sprintf(titlerec, "%sWCS INFORMATION", startblank);
  for(i=strlen(titlerec);i<79;++i)
    titlerec[i]=' ';
  fits_write_record(fptr, titlerec, status);
  wcshdo(0, wcs, &nkeyrec, &wcsheader);
  for(h=0;h<nkeyrec-1;++h)
    {
      cp=&wcsheader[h*80];
      wcsheader[(h+1)*80-1]='\0';
      fits_write_record(fptr, cp, status);
    }
  free(wcsheader);

  /*Print all the other information in the header:  */
  fits_write_record(fptr, blankrec, status);
  sprintf(titlerec, "%s%s comments:", startblank, PACKAGE_STRING);
  for(i=strlen(titlerec);i<79;++i)
    titlerec[i]=' ';
  fits_write_record(fptr, titlerec, status);  
  sprintf(comment, "FITS created with CFITSIO v%.2f on %s", 
	  CFITSIO_VERSION, ctime(&rawtime));
  fits_write_comment(fptr, comment, status);
  sprintf(comment, "%s is available under the GNU GPL v3+.", PACKAGE_NAME);
  fits_write_comment(fptr, comment, status);
  fits_write_comment(fptr, PACKAGE_URL, status);
  sprintf(comment, " ");sprintf(comment, " ");
  fits_write_comment(fptr, comment, status);
  sprintf(comment, "Copyright 2013-2014, Mohammad Akhlaghi.");
  fits_write_comment(fptr, comment, status);
  sprintf(comment, "http://www.astr.tohoku.ac.jp/~akhlaghi/");
  fits_write_comment(fptr, comment, status);
}





void
array_to_fits(char *fits_name, struct keywords *keys, char *EXTname, 
	      int bitpix, void *array, size_t s0, size_t s1, 
	      struct wcsprm *wcs)
{
  size_t i;
  fitsfile *fptr;
  char comm[100];
  int status=0, datatype;
  long fpixel=1, naxis=2, nelements, naxes[2];

  a2f_setdatatype(bitpix, &datatype);

  naxes[1]=s0;
  naxes[0]=s1;
  nelements=naxes[0]*naxes[1];

  if(access(fits_name,F_OK) != -1 )
    fits_open_file(&fptr,fits_name, READWRITE, &status);
  else 
    fits_create_file(&fptr, fits_name, &status);

  fits_create_img(fptr, bitpix, naxis, naxes, &status);
  fits_write_img(fptr, datatype, fpixel, nelements, array, &status);

  fits_write_key(fptr, TSTRING, "EXTNAME", EXTname, "", &status);
  if(keys!=NULL)
    {
      for(i=0;i<keys->num_f;++i)
	fits_write_key(fptr, TFLOAT, keys->names_f[i], 
		       &keys->values_f[i], keys->comments_f[i], 
		       &status);
      for(i=0;i<keys->num_s;++i)
	fits_write_key(fptr, TSTRING, keys->names_s[i], 
		       keys->values_s[i], keys->comments_s[i], 
		       &status);
    }

  if(wcs)
    addheaderinfo(fptr, &status, wcs);
  else
    {
      sprintf(comm, "Created with CFITSIO v%.2f", CFITSIO_VERSION);
      fits_write_comment(fptr, comm, &status);
    }

  fits_close_file(fptr, &status);
  fits_report_error(stderr, status);
}
