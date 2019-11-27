/**
 * Copyright (c) 2015 Northwestern University.
 * Copyright (c) 2015 The Ohio State University.
 * All rights reserved.
 * 
 * Developed by: Northwestern University, and The Ohio State University
 * 	      http://users.eecs.northwestern.edu/~hardav/projects/elastic_fidelity/
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal with
 * the Software without restriction, including without limitation the rights to 
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * * Redistributions of source code must retain the above copyright notice, this 
 *   list of conditions and the following disclaimers.
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimers in the documentation and/or 
 *   other materials provided with the distribution.
 * * Any use of the Software, copies, or derivative works must cite the following
 *   publication: Georgios Tziantzioulis, Ali Murat Gok, S M Faisal, Nikos
 *   Hardavellas,  Seda Ogrenci-Memik, and Srinivasan Parthasarathy. “b-HiVE: A
 *   Bit-Level History-Based Error Model with Value Correlation for Voltage-Scaled
 *   Integer and Floating Point Units.” In Proceedings of the 52nd Design Automation
 *   Conference (DAC), San Francisco, USA, 2015.
 *   http://dx.doi.org/10.1145/2744769.2744805
 * * Neither the names of Northwestern University, or The Ohio State University, 
 *   nor the names of its contributors may be used to endorse or promote products 
 *   derived from this Software without specific prior written permission.
 * 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH 
 * THE SOFTWARE.
 */

#include "xorFaultModel.hh"

const int xorFaultModel::v12[64][5] = {
#include "faultModel/bHiVE/xor/v12/raw.data"
};
const int xorFaultModel::v11[64][5] = {
#include "faultModel/bHiVE/xor/v11/raw.data"
};
const int xorFaultModel::v10[64][5] = {
#include "faultModel/bHiVE/xor/v10/raw.data"
};
const int xorFaultModel::v9[64][5] = {
#include "faultModel/bHiVE/xor/v9/raw.data"
};
const int xorFaultModel::v8[64][5] = {
#include "faultModel/bHiVE/xor/v8/raw.data"
};
const int xorFaultModel::v7[64][5] = {
#include "faultModel/bHiVE/xor/v7/raw.data"
};
const int xorFaultModel::v6[64][5] = {
#include "faultModel/bHiVE/xor/v6/raw.data"
};
const int xorFaultModel::v5[64][5] = {
#include "faultModel/bHiVE/xor/v5/raw.data"
};
