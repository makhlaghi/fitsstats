FitsStats
=========

FitsStats is discontinued. Its core algorithms are now implemented in 
ImageStatistics and the other tools in GNU Astronomy Utilities:

https://www.gnu.org/software/gnuastro/

Please visit there for a much better user experience and more accurate
programs.

Get the basic statistics of a FITS image or a region.

This is a very simple program to read the statistics of a full FITS
image or part of it. A mask can also be specified. If a mask is
specified, only pixels that are not masked (have a zero mask value) 
will be used in the analysis. The histogram and cumulative frequency 
plot of the desired pixels can also be saved. A simple ASCII
histogram will also be plotted on the standard output for a simple and
fast comparison before going onto the details.

Installation and running 
------------------------

FitsStats uses the standard [GNU
autoconf](https://www.gnu.org/software/autoconf/) and [GNU
automake](https://www.gnu.org/software/automake/). To configure, build
and install FitsStats you just have to run these three commands in the
unpacked directory (`$` is for users and `#` is for the root):

    $ ./configure
    $ make
    $ su
    # make install

This will install FitsStats in the standard directories for you to use
it from any where. The building and installation process is highly
configurable, read the AutoConf and AutoMake manuals for more
information.

To see the full options along with a short explanation, you can run
`fitsstats --help`.

Output:
-------------------

Here is a sample output on the standard output:

    FitsStats 0.1 (10 October 2014) started on Fri Oct 10 22:28:12 2014
     - Input ./mock.fits[2] is read.
       -- Number of points                             40401
       -- Minimum                                      -397.605865
       -- Maximum                                      8099.541016
       -- Mean                                         15.890585
       -- Standard deviation                           146.829926
       -- Median                                       11.987528
       -- ASCII histogram in the range: -250.000000 - 300.000000:

        |                         ******                             
        |                       ************                         
        |                     ***************                        
        |                  ********************                      
        |                 ***********************                    
        |               ***************************                  
        |             *******************************                
        |          *************************************             
        |       *******************************************          
        |  ******************************************************    
        |************************************************************
        |------------------------------------------------------------
    
     - Histogram saved in ./mock_hist.txt (400 rows)
     - CFP saved in ./mock_cfp.txt (400 rows)
     - 4.00 sigma-clipping by convergence (med, mean, std, number):
          1: (15.890585, 11.987528, 146.829926, 40401)
          2: (12.297149, 11.775166, 102.861244, 40320)
          3: (12.011212, 11.667698, 102.215263, 40296)
     - 4.00 sigma-clipping 5 times (med, mean, std, number):
          1: (15.890585, 11.987528, 146.829926, 40401)
          2: (12.297149, 11.775166, 102.861244, 40320)
          3: (12.011212, 11.667698, 102.215263, 40296)
          4: (12.021378, 11.667698, 102.196159, 40295)
          5: (12.021378, 11.667698, 102.196159, 40295)
    FitsStats 0.1 finished in 0.0471 seconds

The histogram you see above is only for a very fast and immediate
analysis in 60 bins. By default FitsStats will print the information
to plot the histogram and cumulative frequency plot of the desired
pixels in two columns in a simple `.txt` files. You can feed this
`.txt` file to any plotting tool (I recommend
[pgfplots](http://pgfplots.sourceforge.net/)).

Future updates and issues:
--------------------------

1. Add much more functionality and generality.
2. Work on a better user experience.
3. Fix any bugs I have not found yet!

Comments and suggestions:
-------------------------

I hope FitsStats will be useful for you. If you find any problems in
this program please contact me so I can correct them. I would also be
very glad to hear any suggestions or comments you might have, thank
you.

makhlaghi@gmail.com 

akhlaghi@astr.tohoku.ac.jp

http://astr.tohoku.ac.jp/~akhlaghi/

----------------------------------------
Copyright:
----------------------------------------
Copyright (C) 2014 Mohammad Akhlaghi

Tohoku University Astronomical Institute

http://astr.tohoku.ac.jp/~akhlaghi/

FitsStats is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

FitsStats is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FitsStats.  If not, see <http://www.gnu.org/licenses/>.
