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

#ifndef __SOFTINJ_HH__
#define __SOFTINJ_HH__

#include <iostream>
#include <cstdint>


#ifndef SOFTINJ_STATUS
#define SOFTINJ_STATUS false
#endif

#ifndef SOFTINJ_VOLTAGE
#define SOFTINJ_VOLTAGE 12
#endif


#ifndef SOFTINJ_START_BIT
#define SOFTINJ_START_BIT 0
#endif

#ifndef SOFTINJ_END_BIT
#define SOFTINJ_END_BIT 64
#endif

#ifndef SOFTINJ_RANDOM_SEED
#define SOFTINJ_RANDOM_SEED 0
#endif

#ifndef SOFTINJ_DEBUG_LEVEL
#define SOFTINJ_DEBUG_LEVEL 0
#endif

#define BHIVE_MAX_RANGE 1000000


#define DPRINTF(msg) if(softinj::debugLevel) {	\
    fprintf(stderr,"[SoftInj] %s\n", msg);	\
  }


#define WRAPPER_DECLARATION1(WD_TYPE, MNEMONIC, WD_UNION_TYPE, WD_OPCODE, WD_OP_TYPE) \
  WD_TYPE								\
  MNEMONIC(WD_TYPE a)							\
  {									\
    DPRINTF(__PRETTY_FUNCTION__);					\
    WD_TYPE currentCorrectOutput = a;					\
    if (!softwareFaultInjectionStatus) { /* No Fault Injection */	\
      return currentCorrectOutput;					\
    }									\
    Result result;							\
    result.WD_UNION_TYPE = currentCorrectOutput;			\
    /*Call Fault Injection Function */					\
    result = fault_injection(result, WD_OPCODE, WD_OP_TYPE);		\
    return result.WD_UNION_TYPE;					\
  }


#define WRAPPER_DECLARATION2(WD_TYPE, MNEMONIC, OP_SYMBOL, WD_UNION_TYPE, WD_OPCODE, WD_OP_TYPE) \
  WD_TYPE								\
  MNEMONIC(WD_TYPE a, WD_TYPE b)					\
  {									\
    DPRINTF(__PRETTY_FUNCTION__);					\
    WD_TYPE currentCorrectOutput = a OP_SYMBOL b;			\
    if (!softwareFaultInjectionStatus) { /* No Fault Injection */	\
      return currentCorrectOutput;					\
    }									\
    Result result;							\
    result.WD_UNION_TYPE = currentCorrectOutput;			\
    /*Call Fault Injection Function */					\
    result = fault_injection(result, WD_OPCODE, WD_OP_TYPE);		\
    return result.WD_UNION_TYPE;					\
  }


#define WRAPPER_DECLARATION3(WD_TYPE, MNEMONIC, OP_SYMBOL_1, OP_SYMBOL_2, WD_UNION_TYPE, WD_OPCODE, WD_OP_TYPE) \
  WD_TYPE								\
  MNEMONIC(WD_TYPE a, WD_TYPE b, WD_TYPE c)				\
  {									\
    DPRINTF(__PRETTY_FUNCTION__);					\
    WD_TYPE currentCorrectOutput = (a OP_SYMBOL_1 b) OP_SYMBOL_2 c;	\
    if (!softwareFaultInjectionStatus) { /* No Fault Injection */	\
      return currentCorrectOutput;					\
    }									\
    Result result;							\
    result.WD_UNION_TYPE = currentCorrectOutput;			\
    /*Call Fault Injection Function */					\
    result = fault_injection(result, WD_OPCODE, WD_OP_TYPE);		\
    return result.WD_UNION_TYPE;					\
  }

/*
  Supported Instruction:
  
  Integer: 
  add, sub
  mov
  or
  and
  xor
  shift
  mul, mla
  
  
  Floating Point (Single/Double Precision)
  add
  sub
  mov
  mul, mla
  
*/


namespace softinj {

  /** Enumeration of b-HiVE error model classes' indexes
   */
  enum {
    BHIVE_CORRECT_RANGE = 0,
    BHIVE_GLITCH_RANGE = 1,
    BHIVE_PREV_CORRECT_RANGE = 2,
    BHIVE_PREV_OBSERVED_RANGE = 3
  };
  
  /** Enumerate all possibe instruction operations
   */
  enum {
    OPCODE_INT_ADD = 0,
    OPCODE_INT_SUB,
    OPCODE_INT_MOV,
    OPCODE_INT_OR,
    OPCODE_INT_AND,
    OPCODE_INT_XOR,
    OPCODE_INT_SHIFT,
    OPCODE_INT_MUL,
    OPCODE_INT_MLA,
    OPCODE_INT_DIV,

    OPCODE_FP_SINGLE_ADD,
    OPCODE_FP_SINGLE_SUB,
    OPCODE_FP_SINGLE_MOV,
    OPCODE_FP_SINGLE_MUL,
    OPCODE_FP_SINGLE_MLA,
    OPCODE_FP_SINGLE_DIV,

    OPCODE_FP_DOUBLE_ADD,
    OPCODE_FP_DOUBLE_SUB,
    OPCODE_FP_DOUBLE_MOV,
    OPCODE_FP_DOUBLE_MUL,
    OPCODE_FP_DOUBLE_MLA,
    OPCODE_FP_DOUBLE_DIV,
    OPCODE_MAX
  };

  /** Enumeration of operations types 
   */
  enum {
    SHORT = 0,
    INTEGER,
    LONG,
    FP_SINGLE,
    FP_DOUBLE
  };


  /** Enumeration of different functional unit types
   */ 
  enum {
    FU_INT_ALU = 0,
    FU_INT_MUL,
    FU_INT_DIV,
    FU_FP_ADD,
    FU_FP_MUL,
    FU_FP_DIV,
    FU_IDX_MAX
  };
  
  extern char *fuEnumToString[];


  /** Union type that can hold all possible data types
   */
  union Result {
    uint64_t ui64;
    uint32_t ui32;
    int64_t i64;
    int32_t i32;
    short s;
    int i;
    long l;
    float f;
    double d;
  };
  
  extern bool softwareFaultInjectionStatus;
  extern int voltageLevel;
  extern int startBit;
  extern int endBit;
  extern int debugLevel;


  // Initialize Software Fault Injection Library
  void initialize(bool enable_softinj,
		    int voltage,
		    int start_bit, int end_bit,
		    long seed,
		    int debug_level);


  // Finalize Software Fault Injection Library
  // Used to print collected statistics
  void finalize();

  //Fault Injection 
  Result fault_injection(Result r, int op_code);

  // Integer/Long
  // ADD
  short ADD(short  a, short   b);
  int  ADD(int  a, int   b);
  long ADD(long a, long  b);
  // SUB
  short  SUB(short  a, short   b);
  int  SUB(int  a, int   b);
  long SUB(long a, long  b);
  // MOV
  short  MOV(short  a);
  int  MOV(int  a);
  long MOV(long a);
  // OR
  short  OR(short  a, short   b);
  int  OR(int  a, int   b);
  long OR(long a, long  b);
  // AND
  short  AND(short  a, short   b);
  int  AND(int  a, int   b);
  long AND(long a, long  b);
  // XOR
  short  XOR(short  a, short   b);
  int  XOR(int  a, int   b);
  long XOR(long a, long  b);
  // SHIFT
  // MUL
  short  MUL(short  a, short   b);
  int  MUL(int  a, int   b);
  long MUL(long a, long  b);
  // MLA
  short  MLA(short  a, short   b, short c);
  int  MLA(int  a, int   b, int c);
  long MLA(long a, long  b, long c);

  // Floating Point (Single/Double Precision)
  /*
    Notice: float wrappers are commented out as no characterization has
    been performed for single precision 32bit floating point units.
   */
  
  // ADD
  //float  ADD(float  a, float  b);
  double ADD(double a, double b);
  // SUB
  //float  SUB(float  a, float  b);
  double SUB(double a, double b);
  //MUL
  //float  MUL(float  a, float  b);
  double MUL(double a, double b);
  //MUL
  //float  MLA(float  a, float  b, float  c);
  double MLA(double a, double b, double c);
  //DIV
  //float  DIV(float  a, float  b);
  double DIV(double a, double b);
  
    
}



#endif //__SOFTINJ_HH__
