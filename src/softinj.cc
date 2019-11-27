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

#include <cassert>
#include <iostream>

#include "bHiVEModels/basicFaultModel.hh"
#include "bHiVEModels/addFaultModel.hh"
#include "bHiVEModels/andFaultModel.hh"
#include "bHiVEModels/basicFaultModel.hh"
#include "bHiVEModels/moveFaultModel.hh"
#include "bHiVEModels/mul64FaultModel.hh"
#include "bHiVEModels/orrFaultModel.hh"
#include "bHiVEModels/xorFaultModel.hh"
#include "bHiVEModels/fpAddFaultModel.hh"
#include "bHiVEModels/fpMulFaultModel.hh"
#include "bHiVEModels/fpDivFaultModel.hh"

#include "MersenneTwister.h"

#include "softinj.hh"

namespace softinj {
  
  bool softwareFaultInjectionStatus = false;
  int voltageLevel = 12;
  int startBit = 0;
  int endBit = 64;
  int debugLevel = 0;

  basicFaultModel *faultModels[OPCODE_MAX];

  Result previousCorrectOutput[FU_IDX_MAX];
  Result previousObservedOutput[FU_IDX_MAX];

  MTRand randomNumGen;

  char *fuEnumToString[] = {
    "FU_INT_ALU",
    "FU_INT_MUL",
    "FU_INT_DIV",
    "FU_FP_ADD",
    "FU_FP_MUL",
    "FU_FP_DIV",
    "FU_IDX_MAX"
  };


  // Statistics

  uint64_t executedInstructionPerFunctionalUnit[FU_IDX_MAX];

  uint64_t totalErroneousBitsOfOperations;
  uint64_t totalBitsOfOperations;
  uint64_t numOfInjectedOperations;

  // Pre Declaration of Functions
  int getFUIdx(int op_code);



  // Function Implementation
  void
  initializeStatistics()
  {
    for (int i=0; i<FU_IDX_MAX; i++) {
      executedInstructionPerFunctionalUnit[i] = 0;
    }

    totalErroneousBitsOfOperations = 0;
    totalBitsOfOperations = 0;
    numOfInjectedOperations = 0;

  }

  void
  initializeFaultModels()
  {
    int i;
    for (i=0; i<OPCODE_MAX; i++) {
      faultModels[i] = NULL;
    }
    
    faultModels[OPCODE_INT_ADD] = new addFaultModel();
    faultModels[OPCODE_INT_SUB] = faultModels[OPCODE_INT_ADD];
    faultModels[OPCODE_INT_MOV] = new moveFaultModel();
    faultModels[OPCODE_INT_OR] = new orrFaultModel();
    faultModels[OPCODE_INT_AND] = new andFaultModel();
    faultModels[OPCODE_INT_XOR] = new xorFaultModel();
    faultModels[OPCODE_INT_SHIFT] = NULL;
    faultModels[OPCODE_INT_MUL] = new mul64FaultModel();
    faultModels[OPCODE_INT_MLA] = faultModels[OPCODE_INT_MUL];
    faultModels[OPCODE_INT_DIV] = NULL;

    faultModels[OPCODE_FP_SINGLE_ADD] = new fpAddFaultModel();
    faultModels[OPCODE_FP_SINGLE_SUB] = faultModels[OPCODE_FP_SINGLE_ADD];
    faultModels[OPCODE_FP_SINGLE_MOV] = NULL;
    faultModels[OPCODE_FP_SINGLE_MUL] = new fpMulFaultModel();
    faultModels[OPCODE_FP_SINGLE_MLA] = faultModels[OPCODE_FP_SINGLE_MUL];
    faultModels[OPCODE_FP_SINGLE_DIV] = new fpDivFaultModel();

    faultModels[OPCODE_FP_DOUBLE_ADD] = faultModels[OPCODE_FP_SINGLE_ADD];
    faultModels[OPCODE_FP_DOUBLE_SUB] = faultModels[OPCODE_FP_SINGLE_ADD];
    faultModels[OPCODE_FP_DOUBLE_MOV] = NULL;
    faultModels[OPCODE_FP_DOUBLE_MUL] = faultModels[OPCODE_FP_SINGLE_MUL];
    faultModels[OPCODE_FP_DOUBLE_MLA] = faultModels[OPCODE_FP_SINGLE_MUL];
    faultModels[OPCODE_FP_DOUBLE_DIV] = faultModels[OPCODE_FP_SINGLE_DIV];
  }

  /*
  void initializeThroughFileFaultModel(int opcode, char **filename)
  {
    if (faultModels[opcode] != NULL) {
      delete faultModels[opcode];
      faultModels[opcode] = NULL;
    } 
    faultModels[opcode] = customFaultModel(filename);
  }
  */

  void
  initializePreviousOutputs()
  {
    int i;
    for (i=0; i<FU_IDX_MAX; i++) {
      previousCorrectOutput[i].ui64 = 0;
      previousObservedOutput[i].ui64 = 0;
    }
  }

  void
  initialize(bool enable_softinj,
	     int voltage,
	     int start_bit, int end_bit,
	     long seed,
	     int debug_level)
  {
    softwareFaultInjectionStatus = enable_softinj;
    voltageLevel = voltage;
    startBit = start_bit;
    endBit = end_bit;
    debugLevel = debug_level;

    // Voltage assertions: range [5,12]
    assert(voltageLevel >= 5 &&
	   voltageLevel <= 12);

    // Bit range assertions: range [0,64), start <= end
    assert(startBit <= endBit);
    assert(startBit >=0 &&
	   startBit <= 64);
    assert(endBit >=0 &&
	   endBit <= 64);
    

    initializeFaultModels();

    initializePreviousOutputs();

    initializeStatistics();

    randomNumGen = MTRand(seed);

    fprintf(stderr,"[SoftInj] status: %d, \
voltage: %d, \
injection in bits [%d,%d)\n",
	    softwareFaultInjectionStatus,
	    voltageLevel,
	    startBit, endBit);

    fprintf(stderr,"[SoftInj] sizeof(int) %d, \
sizeof(long) %d, \
sizeof(float) %d \
sizeof(double) %d\n",
	    sizeof(int),
	    sizeof(long),
	    sizeof(float),
	    sizeof(double));
	    
  }

  void
  finalize()
  {
    float bitErrorRate = totalBitsOfOperations ? 
      (((float) totalErroneousBitsOfOperations / totalBitsOfOperations) * 100) :
      0;

    fprintf(stderr,"[SoftInj] : totalErroneousBitsOfOperations %d, \
totalBitsOfOperations %d, \
numOfInjectedOperations %d, \
bitErrorRatePercentage %f\n",
	    totalErroneousBitsOfOperations,
	    totalBitsOfOperations,
	    numOfInjectedOperations,
	    bitErrorRate
	    );

    for (int i=0; i<FU_IDX_MAX; i++) {
      fprintf(stderr, "[SoftInj] : executedInstructionPerFU[%s] = %lu\n",
	      fuEnumToString[i],
	      executedInstructionPerFunctionalUnit[i]);
    }
  }

  void
  updateStatistics(int op_code)
  {
    int fuIdx = getFUIdx(op_code);
    executedInstructionPerFunctionalUnit[fuIdx]++;
  }

  void
  updateBitErrorRate(uint64_t mask, int opBitwidth)
  {
    //Record fault injection
    numOfInjectedOperations++;
    
    // Record bits of operation
    totalBitsOfOperations += opBitwidth;

    //Record erroneous bits of operation
    int setBits;
    for (setBits = 0; mask; setBits++) {
      mask &= mask - 1; // clear the least significant bit set
    }
    totalErroneousBitsOfOperations += setBits;
  }

  int
  getOperationBitwidth(int op_type)
  {
    int size;
    switch (op_type) {
    case (SHORT):
      {	size = sizeof(short)*8; break; }
    case (INTEGER): 
      {	size = sizeof(int)*8; break; }
    case (LONG):
      {	size = sizeof(long)*8; break; }
    case (FP_SINGLE):
      {	size = sizeof(float)*8; break; }
    case (FP_DOUBLE):
      {	size = sizeof(double)*8; break; }
    default:
      {	assert(0 && "Unknown type of operation"); }
    }
    return size;
  }

  void
  fixStartBitEndBit(int *start_bit, int *end_bit, int op_type)
  {
    int max_bit;

    switch (op_type) {
    case (SHORT):
      { max_bit = sizeof(short)*8; break; }
    case (INTEGER): 
      {	max_bit = sizeof(int)*8; break; }
    case (LONG):
      {	max_bit = sizeof(long)*8; break; }
    case (FP_SINGLE):
      {	max_bit = sizeof(float)*8; break; }
    case (FP_DOUBLE):
      {	max_bit = sizeof(double)*8; break; }
    default:
      {	assert(0 && "Unknown type of operation"); }
    }

    if (*end_bit > max_bit) 
      { *end_bit = max_bit; }

    if (*start_bit > max_bit) 
      { *start_bit = max_bit; }
  }

  
  //Fault Injection 

  int
  getFUIdx(int op_code)
  {
    switch (op_code) {
    case (OPCODE_INT_ADD):
    case (OPCODE_INT_SUB):
    case (OPCODE_INT_MOV):
    case (OPCODE_INT_OR):
    case (OPCODE_INT_AND):
    case (OPCODE_INT_XOR):
    case (OPCODE_INT_SHIFT):
      {	return FU_INT_ALU; }
    case (OPCODE_INT_MUL):
    case (OPCODE_INT_MLA):
      { return FU_INT_MUL; }
    case (OPCODE_INT_DIV):
      { return FU_INT_DIV; }      
    case (OPCODE_FP_SINGLE_ADD):
    case (OPCODE_FP_SINGLE_SUB):
    case (OPCODE_FP_DOUBLE_ADD):
    case (OPCODE_FP_DOUBLE_SUB):
      { return FU_FP_ADD; }
    case (OPCODE_FP_SINGLE_MUL):
    case (OPCODE_FP_SINGLE_MLA):
    case (OPCODE_FP_DOUBLE_MUL):
    case (OPCODE_FP_DOUBLE_MLA):
      { return FU_FP_MUL; }
    case (OPCODE_FP_SINGLE_DIV):     
    case (OPCODE_FP_DOUBLE_DIV):
      { return FU_FP_DIV; }
    default: 
      {assert(0 && "Unknown Operation");}
    }
  }
  
  void
  updatePreviousCorrectOutput(Result r, int op_code)
  {
    int fuIdx = getFUIdx(op_code);
    previousCorrectOutput[fuIdx] = r;
  }

  void
  updatePreviousObservedOutput(Result r, int op_code)
  {
    int fuIdx = getFUIdx(op_code);
    previousObservedOutput[fuIdx] = r;
  }

  Result
  getPreviousCorrectOutput(int op_code)
  {
    int fuIdx = getFUIdx(op_code);
    return previousCorrectOutput[fuIdx];
  }

  Result
  getPreviousObservedOutput(int op_code)
  {
    int fuIdx = getFUIdx(op_code);
    return previousObservedOutput[fuIdx];
  }

  Result
  fault_injection(Result r, int op_code, int op_type)
  {
    int voltage = softinj::voltageLevel;
    int start_bit = softinj::startBit;
    int end_bit = softinj::endBit;
    int i;

    uint64_t mask = 0; 

    //assert(0 && voltage && start_bit && end_bit && "Not Implemented Yet");

    // Fix the position of start and end bits based on the size of 
    // the operation
    fixStartBitEndBit(&start_bit, &end_bit, op_type);

    basicFaultModel * model = faultModels[op_code];
    assert(model != NULL && "Missing Error Model");

    Result prevCorrectOutput = getPreviousCorrectOutput(op_code);
    Result prevObservedOutput = getPreviousObservedOutput(op_code);
      

    // generate mask
    for (i = start_bit; i < end_bit; i++) {
        uint64_t bitMask = (1UL<<i);
        int correctRange = 
	  model->getIt(voltage,	i, BHIVE_CORRECT_RANGE);
        int glitchRange = 
	  correctRange + model->getIt(voltage, i, BHIVE_GLITCH_RANGE);
        int prevCorrectRange = 
	  glitchRange + model->getIt(voltage, i, BHIVE_PREV_CORRECT_RANGE);


        uint64_t notXorPC_CC = ~ ( prevCorrectOutput.ui64 ^ r.ui64);
        uint64_t notXorPO_CC = ~ ( prevObservedOutput.ui64 ^ r.ui64);

        int rVal = randomNumGen.randInt() % BHIVE_MAX_RANGE;

        ///// OR between mask and bitmask? i.e., mask |= bitmask
        if (rVal <= correctRange) { // Correct
            //Do nothing
        } else if (rVal <= glitchRange) { // Glitch
            mask = mask ^ bitMask;
        } else if (rVal <= prevCorrectRange) { // Previous Correct
            bool bitNotEqual = ! ((notXorPC_CC & bitMask) != 0);
            if (bitNotEqual) {
                mask = mask ^ bitMask;
            }
        } else { // Previous Observed
            bool bitNotEqual = ! ((notXorPO_CC & bitMask) != 0);
            if (bitNotEqual) {
                mask = mask ^ bitMask;
            }
        }
    }
    
    // Update the bit error rate counter
    int operationBitwidth = getOperationBitwidth(op_type);
    updateBitErrorRate(mask, operationBitwidth);

    updatePreviousCorrectOutput(r, op_code);

    /* // For debugging (only valid for integer operation)
    if ((voltage == 12 ||
	 voltage == 11 ||
	 voltage == 10 ) && mask) {
      assert(0);
    }
    */


    // the actual fault injection
    r.ui64 = r.ui64 ^ mask;
    //r.ui32 = r.ui32 ^ mask;

    updatePreviousObservedOutput(r, op_code);

    updateStatistics(op_code);

    return r;
  }

  /*
   * The following region implements the operations of 
   * Floating Point (Single/Double Precision)
   * functional units
   */
 
  // ADD
  //WRAPPER_DECLARATION2(float, ADD, +, f, OPCODE_FP_SINGLE_ADD, FP_SINGLE)
  WRAPPER_DECLARATION2(double, ADD, +, d, OPCODE_FP_DOUBLE_ADD, FP_DOUBLE)

  // SUB
  //WRAPPER_DECLARATION2(float, SUB, -, f, OPCODE_FP_SINGLE_SUB, FP_SINGLE)
  WRAPPER_DECLARATION2(double, SUB, -, d, OPCODE_FP_DOUBLE_SUB, FP_DOUBLE)

  // MUL
  //WRAPPER_DECLARATION2(float, MUL, *, f, OPCODE_FP_SINGLE_MUL, FP_SINGLE)
  WRAPPER_DECLARATION2(double, MUL, *, d, OPCODE_FP_DOUBLE_MUL, FP_DOUBLE)
  
  // MLA
  //WRAPPER_DECLARATION3(float, MLA, *, +, f, OPCODE_FP_SINGLE_MLA, FP_SINGLE)
  WRAPPER_DECLARATION3(double, MLA, *, +, d, OPCODE_FP_DOUBLE_MLA, FP_DOUBLE)

  // DIV
  //WRAPPER_DECLARATION2(float, DIV, /, f, OPCODE_FP_SINGLE_DIV, FP_SINGLE)
  WRAPPER_DECLARATION2(double, DIV, /, d, OPCODE_FP_DOUBLE_DIV, FP_DOUBLE)

  /*
   * The following region implements the operations of 
   * Integer
   * functional units
   */

  // ADD
  WRAPPER_DECLARATION2(short, ADD, +, i32, OPCODE_INT_ADD, SHORT)
  WRAPPER_DECLARATION2(int, ADD, +, i32, OPCODE_INT_ADD, INTEGER)
  WRAPPER_DECLARATION2(long, ADD, +, i64, OPCODE_INT_AND, LONG)

  // SUB
  WRAPPER_DECLARATION2(short, SUB, -, i32, OPCODE_INT_SUB, SHORT)
  WRAPPER_DECLARATION2(int, SUB, -, i32, OPCODE_INT_SUB, INTEGER)
  WRAPPER_DECLARATION2(long, SUB, -, i64, OPCODE_INT_SUB, LONG)

  // MOV
  WRAPPER_DECLARATION1(short, MOV, i32, OPCODE_INT_MOV, SHORT)
  WRAPPER_DECLARATION1(int, MOV, i32, OPCODE_INT_MOV, INTEGER)
  WRAPPER_DECLARATION1(long, MOV, i64, OPCODE_INT_MOV, LONG)

  // OR
  WRAPPER_DECLARATION2(short, OR, |, i32, OPCODE_INT_OR, SHORT)
  WRAPPER_DECLARATION2(int, OR, |, i32, OPCODE_INT_OR, INTEGER)
  WRAPPER_DECLARATION2(long, OR, |, i64, OPCODE_INT_OR, LONG)
  
  // AND
  WRAPPER_DECLARATION2(short, AND, &, i32, OPCODE_INT_AND, SHORT)
  WRAPPER_DECLARATION2(int, AND, &, i32, OPCODE_INT_AND, INTEGER)
  WRAPPER_DECLARATION2(long, AND, &, i64, OPCODE_INT_AND, LONG)

  // XOR
  WRAPPER_DECLARATION2(short, XOR, ^, i32, OPCODE_INT_XOR, SHORT)
  WRAPPER_DECLARATION2(int, XOR, ^, i32, OPCODE_INT_XOR, INTEGER)
  WRAPPER_DECLARATION2(long, XOR, ^, i64, OPCODE_INT_XOR, LONG)

  // MUL
  WRAPPER_DECLARATION2(short, MUL, *, i32, OPCODE_INT_MUL, SHORT)
  WRAPPER_DECLARATION2(int, MUL, *, i32, OPCODE_INT_MUL, INTEGER)
  WRAPPER_DECLARATION2(long, MUL, *, i64, OPCODE_INT_MUL, LONG)

  // MLA
  WRAPPER_DECLARATION3(short, MLA, *, +, i32, OPCODE_INT_MLA, SHORT)
  WRAPPER_DECLARATION3(int, MLA, *, +, i32, OPCODE_INT_MLA, INTEGER)
  WRAPPER_DECLARATION3(long, MLA, *, +, i64, OPCODE_INT_MLA, LONG)

  /*
  // DIV
  WRAPPER_DECLARATION2(short, DIV, /, i32, OPCODE_INT_DIV, SHORT)
  WRAPPER_DECLARATION2(int, DIV, /, i32, OPCODE_INT_DIV, INTEGER)
  WRAPPER_DECLARATION2(long, DIV, /, i64, OPCODE_INT_DIV, LONG)
  */


}

