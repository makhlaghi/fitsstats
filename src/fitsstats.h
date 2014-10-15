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
#ifndef FITSSTATS_H
#define FITSSTATS_H

#define HISTPOSTFIX   "_hist.txt"
#define CFPPOSTFIX    "_cfp.txt"

#define PRINTINT      "%.0f\n"
#define PRINTFLT      "%.6f\n"

#define MAXCONVERGE_T   "30"
#define MAXCONVERGE_V   30

#define DP_ASCIIHISTNB 60
#define DP_ASCIIHISTH  10


struct uiparams 
{
  /* Inputs: */
  char       *imgname;		/* Input image name.                 */
  int          imgext;		/* Image image extension.            */
  char      *maskname;		/* Name of mask image.               */
  int         maskext;		/* Name of mask image.               */
  int       maskgiven;		/* ==1: There is a mask.             */
  int              x1;		/* Bottom left corner of crop axis 1.*/
  int              y1;		/* Bottom left corner of crop axis 2.*/
  int              x2;		/* Top right corner of crop axis 1.  */
  int              y2;		/* Top right corner of crop axis 2.  */

  /* Operating mode: */
  unsigned char *mask;	        /* Mask array.                       */
  int         maskmin;		/* ==1: The minimum value is masked. */
  int printalloptions;		/* ==1: Print all input parmeters.   */

  /* Internal parameters: */
  size_t           s0;		/* Size of input along first C axis. */
  size_t           s1;          /* Size of input along second C axis.*/
};





struct fitsstatsparams
{
  /* Input/output */
  struct uiparams  up;	        /* For input file names.             */
  float          *img;		/* Array of input image.             */
  size_t         size;		/* Size of array.                    */

  /* Operating mode: */
  int            verb;		/* ==1: Print steps info.            */

  /* Histogram: */
  char      *histname;		/* Name of histogram file.           */ 
  int        normhist;		/* ==1: a normalized histogram.      */
  int      maxhistone;		/* ==1: Maximum histogram value=1    */
  int       binonzero;		/* ==1: One bin will start at 0.     */
  size_t  histnumbins;		/* Number of bins in the histogram.  */
  float       histmin;		/* Minimum histogram value.          */
  float       histmax;		/* Maximum histogram value.          */
  float     histquant;		/* Hist's quantile above, below.     */

  /* Cumulative frequency plot: */
  char       *cfpname;		/* Name of CFP file.                 */
  int maxcfpeqmaxhist;		/* ==1: as name suggests.            */
  int         normcfp;		/* ==1: Normalize the CFP.           */
  int      cfpsimhist;		/* ==1: Range and num equals hist's. */
  size_t       cfpnum;		/* Number of points for CFP.         */
  float        cfpmin;		/* Minimum CFP value.                */
  float        cfpmax;		/* Maximum CFP value.                */
  float      cfpquant;		/* CFP's quantile above and below.   */  
  
  /* Sigma-clipping: */
  int       sigmaclip;		/* ==1: Do sigma clipping.           */
  float     sigmultip;		/* Multiple of sigma to clip.        */
  float      converge;		/* Convergance difference fraction.  */
  size_t    timesclip;		/* Times to continue clipping.       */

  /* Internal */
  size_t  asciihistnb;	        /* Number of ASCII histogram bins.   */
  size_t   asciihisth;		/* Height of ASCII histogram.        */
  float       *sorted;		/* The sorted input array.           */
  float         *bins;		/* The bins used in the hist and CFP.*/
  float      binwidth;		/* The width of a hist or CFP bin.   */
};

void
fitsstats(struct fitsstatsparams *p);

#endif
