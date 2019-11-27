
This is softinj a software implemented fault injection library.

* WEBSITE
  http://users.eecs.northwestern.edu/~hardav/projects/elastic_fidelity/

* DEPENDENCIES
  Requires a compiler with support of the c++11 standard
  Has been tested with  g++-4.8.3
  
* COMPILE
  To compile the shared library file simply execute the following commands
  (where <SOFTINJ_DIR> is the root directory of the softinj root directory):

  # cd <SOFTINJ_DIR>
  # cd src/
  # make

  After this the src/ directory should contain the static and shared versions
  of the library (i.e. libsoftinj.a, and libsoftinj.so).

* USE

  To use the library you need to perform the following steps:

  * Include the softinj.h header file to the source code files that make use of any
    of the initialization or wrapper functions of the library.
  * At the main function of your application call the initialization function of
    the library.
  * In case you want to print the collected statistics you can call the finalize function
    (should be called after the initialization function).
  * Modify your build framework so that the softinj library is correctly linked with
    your code. An example makefile can be found on the example/ folder

  * For an example application that use thes softinj library you can look at the example/
    folder

* LICENSE

Copyright (c) 2015 Northwestern University.
Copyright (c) 2015 The Ohio State University.
All rights reserved.

Developed by: Northwestern University, and The Ohio State University
	      http://users.eecs.northwestern.edu/~hardav/projects/elastic_fidelity/

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal with
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

* Redistributions of source code must retain the above copyright notice, this 
  list of conditions and the following disclaimers.
* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimers in the documentation and/or 
  other materials provided with the distribution.
* Any use of the Software, copies, or derivative works must cite the following
  publication: Georgios Tziantzioulis, Ali Murat Gok, S M Faisal, Nikos
  Hardavellas,  Seda Ogrenci-Memik, and Srinivasan Parthasarathy. “b-HiVE: A
  Bit-Level History-Based Error Model with Value Correlation for Voltage-Scaled
  Integer and Floating Point Units.” In Proceedings of the 52nd Design Automation
  Conference (DAC), San Francisco, USA, 2015.
  http://dx.doi.org/10.1145/2744769.2744805
* Neither the names of Northwestern University, or The Ohio State University, 
  nor the names of its contributors may be used to endorse or promote products 
  derived from this Software without specific prior written permission.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH 
THE SOFTWARE.

