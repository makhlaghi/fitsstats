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
#ifndef ARGPPARSER_H
#define ARGPPARSER_H




/* Definition parameters for argp: */
const char *argp_program_version=PACKAGE_STRING
  "\nCopyright (C) 2014 Mohammad Akhlaghi.\n"
  "License GPLv3+: GNU GPL version 3 or later "
  "<http://gnu.org/licenses/gpl.html>\n"
  "This is free software: you are free to change and redistribute it.\n"
  "There is NO WARRANTY, to the extent permitted by law.";

const char *argp_program_bug_address=PACKAGE_BUGREPORT;

static char args_doc[] = "FITSimage.fits";

const char doc[] = 

  /* Before the list of options: */
  "\n" PACKAGE_STRING" -- Get the basic statistics of a FITS image.\n\n"
  "When calling the short format of the options, simply put your input "
  "value after the short version of the option, an equal sign (=) should "
  "not be used. For example, `-e4` or `-e 4` are the same as calling "
  "`--imgext=4`.\n\n"
  "The three capital letters after the long options indicate that providing "
  "a value is mandatory if the option is called. The letters show the "
  "format your input will be stored in:\n"
  "   INT:  An integer.\n"
  "   FLT:  A float value.\n"
  "The default values for options with a value are shown in the `[...]` "
  "immediately before the description for that option. Defaults are set "
  "and can be changed before running `./configure` in the `defaults.ac` "
  "file.\n\n"
  "`basename` is the same name of the input image prior to `.fits`.\n\n"
  "Please consult the full documentation for more information."

  /* After the list of options: */
  "\v"
  PACKAGE_NAME" home page: "PACKAGE_URL;




















/* The options are classified into these categories:
   1. Operating mode like quiet, help and version.
   2. Input (image, mask and kernel name, extensions and ...)
   3. Meshs and threads.
   4. Detection 

   Available letters for short options:
   a d f j k l o r s v w y z
   C D E F G H I J K L M N O P R S T V V W Z 

   Options with keys (second structure element) larger than 500 do not
   have a short version.
 */
static struct argp_option options[] =
  {
    /* Such cases are group headers */
    {
      0, 0, 0, 0,  		/* These have to be zero for groups. */
      "Operating modes:", 	/* Explanation for the group. */
      -1			/* Group ID. */
    },
    {
      "quiet",		      /* Long name for this option.         */
      'q',		      /* Short name or key for this option. */
      0,		      /* Informative type of value it gets. */
      OPTION_ARG_OPTIONAL,    /* Flags for this option. */
      "By default all the steps are reported. If this option is called, "
      "only errors or warnings will be reported.\n",
      -1		      /* Option group ID. */
    },





    {
      0, 0, 0, 0,  		/* These have to be zero for groups. */
      "Inputs:",		/* Explanation for the group. */
      1				/* Group ID. */
    },
    {
      "maskmin",
      501,
      0,
      0,
      "The minimum value in the image will be used as a mask. If a mask "
      "image is provided, the minimum of non-masked pixels will be used.\n",
      1
    },
    {
      "imgext", 		
      'e', 		
      "INT",
      0,
      "["DP_IMGEXT_T"] The input image fits extension number, starting "
      "from 0 (zero).\n",
      1
    },
    {
      "mask",
      'm', 		
      "STR",
      0,
      "["DP_MASK"] Mask image FITS file name. If this option is not "
      "given and mask extension is different from the input image extension, "
      "then it is assumed that the mask name is the same as the input image "
      "name.\n",
      1
    },
    {
      "maskext",
      'n',
      "INT",
      0,
      "["DP_MASKEXT_T"] Mask image FITS extension number, starting from 0 "
      "(zero). The default shown value might be -1. This is for cases "
      "where the input image is on a non-zero extension and no mask is "
      "desired. If a different mask image is provided, and the shown default "
      "value is -1, it will become zero. See the explanation for `--mask`.\n",
      1
    },
    {
      "x1",
      'A',
      "INT",
      0,
      "["DP_X1_T"] Bottom left crop box corner position first axis "
      "coordinate (horizontal axis when viewed with SAO ds9). If equal "
      "to x2, then no cropping is done. Has to be >1.\n",
      1
    },
    {
      "y1",
      'B',
      "INT",
      0,
      "["DP_Y1_T"] Bottom left crop box corner position second axis "
      "coordinate (vertical axis when viewed with SAO ds9). If equal "
      "to y2, then no cropping is done. Has to be >1.\n",
      1
    },
    {
      "x2",
      'X',
      "INT",
      0,
      "["DP_X2_T"] Top right crop box corner position first axis "
      "coordinate. Has to be >1.\n",
      1
    },
    {
      "y2",
      'Y',
      "INT",
      0,
      "["DP_Y2_T"] Top right crop box corner position second axis "
      "coordinate. Has to be >1.\n",
      1
    },




    





    {
      0, 0, 0, 0,  
      "Histogram",
      2
    },
    {
      "nohist",
      502,
      0,
      0,
      "Don't find or save the histogram of the image. By default it found "
      "and saved in `basename"HISTPOSTFIX"`.\n",
      2
    },    
    {
      "normhist",
      503,
      0,
      0,
      "Normalize the  histogram (sum of all bins 1).\n",
      2
    },
    {
      "maxhistone",
      504,
      0,
      0,
      "Divide the number in all bins by the number in the bin containing "
      "the most data points. This is good for plotting the histogram and "
      "cumulative frequency plots together (when the CFP is normalized).\n",
      2
    },
    {
      "binonzero",
      505,
      0,
      0,
      "Set the bin sides such that one bin's smallest value is zero. "
      "This is useful in the histograms of mock images with no noise, where "
      "there are no pixels below zero.\n",
      2
    },    
    {
      "histnumbins",
      'b',
      "INT",
      0,
      "["DP_HISTNUMBINS_T"] Number of bins in the histogram (rows in the "
      "output text file).\n",
      2
    },
    {
      "histmin",
      'i',
      "FLT",
      0,
      "["DP_HISTMIN_T"] Minimum value (included) to use in the histogram "
      "If  equal with to `histmax` full range is used.\n",
      2
    },
    {
      "histmax",
      'x',
      "FLT",
      0,
      "["DP_HISTMAX_T"] Maximum value (included) to use in the histogram "
      "If  equal with to `histmin` full range is used.\n",
      2
    },
    {
      "histquant",
      'Q',
      "FLT",
      0,
      "["DP_HISTQUANT_T"] The histogram will be in the quantile range "
      "of (Q, 1-Q). This is useful when the image is strongly skewed and "
      "you don't want the extremes. If a range is specified, this is \n"
      "ignored.",
      2
    },





    {
      0, 0, 0, 0,  
      "Cumulative frequency plot",
      3
    },
    {
      "nocfp",
      506,
      0,
      0,
      "Don't find or save the cumulative frequency plot of the image. "
      "By default it found and saved in `basename"CFPPOSTFIX"`.\n",
      3
    },
    {
      "normcfp",
      507,
      0,
      0,
      "Normalize the cumulative frequency plot.\n",
      3
    },
    {
      "cfpsimhist",
      508,
      0,
      0,
      "Use the same settings as the histogram: flux range and number of "
      "points.\n",
      3
    },
    {
      "cfpnum",
      'p',
      "INT",
      0,
      "["DP_CFPNUM_T"] Number of points to show the CFP value for. Images "
      "are often large and plotting the CFP for everypoint is too heavy.\n",
      3
    },
    {
      "cfpmin",
      'g',
      "FLT",
      0,
      "["DP_CFPMIN_T"] Minimum value to use in the CFP.\n",
      3
    },
    {
      "cfpmax",
      'h',
      "FLT",
      0,
      "["DP_CFPMIN_T"] Maximum value to use in the CFP.\n",
      3
    },
    {
      "cfpquant",
      'U',
      "FLT",
      0,
      "["DP_CFPQUANT_T"] Similar to `histquant`.\n",
      3
    },
    





    {
      0, 0, 0, 0,  
      "Sigma clipping",
      4
    },
    {
      "nosclip",
      509,
      0,
      0,
      "Don't run sigma-clipping on the image. By default the histogram is "
      "saved in `basename"HISTPOSTFIX"`.\n",
      4
    },
    {
      "sigmultip",
      'u',
      "FLT",
      0,
      "["DP_SIGMULTIP_T"] Multiple of the standard deviation to clip.\n",
      4
    },
    {
      "converge",
      'c',
      "FLT",
      0,
      "["DP_CONVERGE_T"] In convergence, sigma clipping will halt when "
      "the change in the standard deviation is below this fraction. If it "
      "converge until "MAXCONVERGE_T" iterations, it will stop.\n",
      4
    }, 
    {
      "timesclip",
      't',
      "INT",
      0,
      "["DP_TIMESCLIP_T"] Fixed number of times to continue clipping the "
      "data.\n",
      4
    }, 



    {0}
  };



















/* Parse a single option: */
static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
  /* Save the arguments structure: */
  struct fitsstatsparams *p = state->input;
  
  /* In case the user incorrectly uses the equal sign (for example
     with a short format or with space in the long format, then `arg`
     start with (if the short version was called) or be (if the long
     version was called with a space) the equal sign. So, here we
     check if the first character of arg is the equal sign, then the
     user is warned and the program is stopped: */
  if(arg && arg[0]=='=')
    argp_error(state, "Incorrect use of the equal sign (`=`). For short "
	       "options, `=` should not be used and for long options, "
	       "there should be no space between the option, equal sign "
	       "and value.");
 
  switch(key)
    {
    /* Operating modes: */
    case 'q':
      p->verb=0;
      break;

    /* Input parameters: */
    case 'e':
      intelzero(arg, &p->up.imgext, "imgext", key);
      break;
    case 'm':
      p->up.maskname=arg;
      break;
    case 'n':
      intelzero(arg, &p->up.maskext, "maskext", key);
      break;
    case 'A':
      intlzero(arg, &p->up.x1, "x1", key);
      break;
    case 'B':
      intlzero(arg, &p->up.y1, "y1", key);
      break;
    case 'X':
      intlzero(arg, &p->up.x2, "x2", key);
      break;
    case 'Y':
      intlzero(arg, &p->up.y2, "y2", key);
      break;
    case 501:
      p->up.maskmin=1;
      break;

    /* Histogram */
    case 502:
      p->histname=NULL;
      break;
    case 503:
      p->normhist=1;
      break;
    case 504:
      p->maxhistone=1;
      break;
    case 505:
      p->binonzero=1;
      break;
    case 'b':
      sizetlzero(arg, &p->histnumbins, "histnumbins", key);
      break;
    case 'i':
      anyfloat(arg, &p->histmin);
      break;
    case 'x':
      anyfloat(arg, &p->histmax);
      break;
    case 'Q':
      floatl0s1(arg, &p->histquant, "histquant", key);
      break;

    /* Cumulative frequency plot: */
    case 506:
      p->cfpname=NULL;
      break;
    case 507:
      p->normcfp=1;
      break;
    case 508:
      p->cfpsimhist=1;
      break;
    case 'p':
      sizetlzero(arg, &p->cfpnum, "cfpnum", key);
      break;
    case 'g':
      anyfloat(arg, &p->cfpmin);
      break;
    case 'h':
      anyfloat(arg, &p->cfpmax);
      break;
    case 'U':
      floatl0s1(arg, &p->cfpquant, "cfpquant", key);
      break;

    /* Sigma-clipping: */
    case 509:
      p->sigmaclip=0;
      break;
    case 'u':
      anyfloat(arg, &p->sigmultip);
      break;
    case 'c':
      floatl0s1(arg, &p->converge, "converge", key);
      break;
    case 't':
      sizetlzero(arg, &p->timesclip, "numbins", key);
      break;
      
    /* Read the non-option argument: */
    case ARGP_KEY_ARG:
      if (state->arg_num >= 1)
	argp_error(state, "Too many arguments! Only one (input "
		   "FITS name) is required.");
      p->up.imgname=arg;
      break;
      
    /* Make sure an argument is given: */
    case ARGP_KEY_END:
      if (state->arg_num==0)
	argp_error(state, "No input FITS image!");
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}



















/* Basic structure defining the whole argument reading process. */
static struct argp argp = {options, parse_opt, args_doc, doc};

#endif
