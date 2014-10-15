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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

#include "config.h"
#include "fitsstats.h"

#include "ui.h"			/* Needs fitsstats.h           */
#include "stats.h"		/* Needs fitsstats.h           */






void
printheaders(struct fitsstatsparams *p, int h0c1, FILE *fp)
{
  time_t rawtime;
  char *datatype;

  /* Get the time to print on the report. */
  time(&rawtime);   

  if(h0c1)
    datatype="Cumulative frequency plot";
  else
    datatype="Histogram";
  
  fprintf(fp, "# %s %s\n# Created at %s", PACKAGE_STRING, datatype,
	  ctime(&rawtime));
  fprintf(fp, "# Input image: %s[%d]\n", p->up.imgname, p->up.imgext);
  if(p->up.maskgiven)
    fprintf(fp, "# Mask image: %s[%d]\n", p->up.maskname, p->up.maskext);
  if(p->up.x1!=p->up.x2 && p->up.y1!=p->up.y2)
    fprintf(fp, "# Pixels used: (%d, %d) to (%d, %d)\n",
	    p->up.x1, p->up.y1, p->up.x2, p->up.y2);
  
  fprintf(fp, "# Column 1: Flux in the middle of each bin\n");
  if(h0c1)
    fprintf(fp, "# Column 2: Average sorted index of all points "
	    "in this bin.\n");
  else
    fprintf(fp, "# Column 2: Number of points in this bin. \n");
}





void
printhistcfp(struct fitsstatsparams *p, int h0c1)
{
  float d;
  int f0i1;
  FILE *out;
  size_t i, numbins;
  char *filename, *actionname;

  /* Set half the bin width to add to the left most value: */
  d=p->binwidth/2;

  /* Set the name of the output */
  assert(h0c1==0 || h0c1==1);
  if(h0c1) 
    {
      actionname="CFP";
      numbins=p->cfpnum;
      filename=p->cfpname;
      if(p->normcfp || p->maxcfpeqmaxhist)   f0i1=0;
      else                                   f0i1=1;
    }
  else 
    {
      actionname="Histogram";
      numbins=p->histnumbins;
      filename=p->histname;
      if(p->normhist || p->maxhistone) f0i1=0;
      else                             f0i1=1;
    }
 
  /* Open the file: */
  assert( (out=fopen(filename, "w"))!=NULL );

  /* Print the header information first: */
  printheaders(p, h0c1, out);

  /* Put the data in the file: */
  if(f0i1)
    for(i=0;i<numbins;++i)
      fprintf(out, "%-20.6f"PRINTINT, p->bins[i*2]+d, p->bins[i*2+1]);
  else 
    for(i=0;i<numbins;++i)
      fprintf(out, "%-20.6f"PRINTFLT, p->bins[i*2]+d, p->bins[i*2+1]);
  fclose(out);
  if(p->verb)
    printf(" - %s saved in %s (%lu rows)\n", actionname, 
	   filename, numbins);
}
