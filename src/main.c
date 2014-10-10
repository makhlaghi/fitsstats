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
#include <sys/time.h>

#include "config.h"
#include "fitsstats.h"

#include "ui.h"			/* Needs fitsstats.h */

int
main(int argc, char *argv[])
{
  double dt;
  time_t rawtime;
  struct timeval t1, t2;
  struct fitsstatsparams p; 

  /* Get the starting time. We still don't know if the user wants
     verbatim output or not. So in any case, we have to find the
     needed times. */
  time(&rawtime); 
  gettimeofday(&t1, NULL);
  
  /* Read the input parameters: */
  setparams(argc, argv, &p, &rawtime);

  /* Run FitsStats: */
  fitsstats(&p);

  /* Free the allocated arrays: */
  freeparams(&p);
  
  /* Print the finishing dashes. */
  if(p.verb)
    {
      gettimeofday(&t2, NULL);
      dt= ( ((double)t2.tv_sec+(double)t2.tv_usec/1e6) - 
	    ((double)t1.tv_sec+(double)t1.tv_usec/1e6) );
      printf("%s finished in %.4f seconds\n", PACKAGE_STRING, dt);
      printdashes(0);
    }

  /* Finish the program: */
  return 0;
}
