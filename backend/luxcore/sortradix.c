// Copyright (C) 2004-2011 Christoph Kubisch 
// This file is part of the "Luxinia Engine".
// See copyright notice in luxplatform.h

#include <luxinia/luxcore/sortradix.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////
// RadixSort
/**
* Contains source code from the article "Radix Sort Revisited".
* \file   IceRevisitedRadix.cpp
* \author   Pierre Terdiman
* \date   April, 4, 2000
*/

static struct RADIXData_s{
  uint32  histogram[1024];
  uint32  offset[256];
} l_RADIXData;


// -----------------------------------
// Radix Sort Array
// -----------------------------------

#define CHECK_PASS_VALIDITY(pass)                               \
  /* Shortcut to current counters */                              \
  curCount = &l_RADIXData.histogram[pass<<8];                             \
  \
  /* Reset flag. The sorting pass is supposed to be performed. (default) */         \
  performPass = LUX_TRUE;                                     \
  \
  /* Check pass validity */                                 \
  \
  /* If all values have the same byte, sorting is useless. */                 \
  /* It may happen when sorting bytes or words instead of dwords. */              \
  /* This routine actually sorts words faster than dwords, and bytes */           \
  /* faster than words. Standard running time (O(4*n))is reduced to O(2*n) */         \
  /* for words and O(n) for bytes. Running time for floats depends on actual values... */   \
  \
  /* Get first byte */                                    \
  uniqueVal = *(((uchar*)values)+pass);                           \
  \
  /* Check that byte's counter */                               \
  if(curCount[uniqueVal]==size) performPass=LUX_FALSE;

#ifdef LUX_ENDIAN_LITTLE
#define RADIX_SIGN_PASS 3
#else
#define RADIX_SIGN_PASS 0
#endif

// Main call for INT or UINT
// -------------------------
LUX_API uint32* lxSortRadixArrayInt(const uint32 *values, unsigned int size, booln sign, uint32 *outindices1, uint32 *outindices2){
  int32 i,j;
  uchar *p,*pe;
  uint32  *h0,*h1,*h2,*h3;
  int32 valS, prevValS;
  uint32 valU, prevValU;
  uchar alreadySorted;

  uint32 sizeNegativeValues;
  uint32* tmp;
  uint32* curCount;
  uchar performPass;
  uchar uniqueVal;

  uchar* inputBytes;
  uint32 *indices1 = outindices1;
  uint32 *indices2 = outindices2;
  uint32 *indicesCur;
  uint32 *indicesEnd;

  // reset counters & histogram
  memset(l_RADIXData.histogram,0,sizeof(unsigned int)*1024);
  memset(l_RADIXData.offset,0,sizeof(unsigned int)*256);

  p = (uchar*)values;                       
  pe = &p[size*4];                        
  h0= &l_RADIXData.histogram[0];  /* Histogram for first pass (LSB) */  
  h1= &l_RADIXData.histogram[256];  /* Histogram for second pass    */
  h2= &l_RADIXData.histogram[512];  /* Histogram for third pass     */
  h3= &l_RADIXData.histogram[768];  /* Histogram for last pass (MSB)  */


  alreadySorted = LUX_TRUE; /* optimism */

  if (sign){
    prevValS = (int32)values[indices1[0]];
    valS = (int32)values[indices1[0]];

    while(p!=pe)
    {
      /* Read input buffer in previous sorted order */
      valS = (int32)values[*indices1++];
      /* Check whether already sorted or not */
      if(valS<prevValS) { alreadySorted = LUX_FALSE; break; } /* Early out */
      /* Update for next iteration */
      prevValS = valS;

      /* Create histograms */
      h0[*p++]++; h1[*p++]++; h2[*p++]++; h3[*p++]++;
    }
  }
  else{
    prevValU = (uint32)values[indices1[0]];
    valU = (uint32)values[indices1[0]];

    while(p!=pe)
    {
      valU = (uint32)values[*indices1++];
      if(valU<prevValU) { alreadySorted = LUX_FALSE; break; }
      prevValU = valU;

      /* Create histograms */
      h0[*p++]++; h1[*p++]++; h2[*p++]++; h3[*p++]++;
    }
  }

  /* If all input values are already sorted, we just have to return and leave the */
  /* previous list unchanged. That way the routine may take advantage of temporal */
  /* coherence, for example when used to sort transparent faces.          */
  if(alreadySorted) { return outindices1; }

  /* Else there has been an early out and we must finish computing the histograms */
  while(p!=pe)
  {
    /* Create histograms without the previous overhead */
    h0[*p++]++; h1[*p++]++; h2[*p++]++; h3[*p++]++;
  }

  // Compute #negative values involved if needed
  sizeNegativeValues = 0;
  if(sign)
  {
    // An efficient way to compute the number of negatives values we'll have to deal with is simply to sum the 128
    // last values of the last histogram. Last histogram because that's the one for the Most Significant byte,
    // responsible for the sign. 128 last values because the 128 first ones are related to positive numbers.

#ifdef LUX_ENDIAN_LITTLE
    for(i=128;i<256;i++)  sizeNegativeValues += h3[i];  // 768 for last histogram, 128 for negative part
#else
    for(i=128;i<256;i++)  sizeNegativeValues += h0[i];
#endif
  }
  // reset before sorting starts
  indices1 = outindices1;
  // Radix sort, j is the pass number (0=LSB, 3=MSB)
#ifdef LUX_ENDIAN_LITTLE
  for(j=0;j<4;j++)
#else
  for(j=3;j>=0;j--)
#endif
  {
    CHECK_PASS_VALIDITY(j);
    // Sometimes the fourth (negative) pass is skipped because all numbers are negative and the MSB is 0xFF (for example). This is
    // not a problem, numbers are correctly sorted anyway.
    if(performPass)
    {
      // Should we care about negative values?
      if(j!=RADIX_SIGN_PASS || !sign)
      {
        // Here we deal with positive values only

        // Create offsets
        l_RADIXData.offset[0] = 0;
        for(i=1;i<256;i++)    l_RADIXData.offset[i] = l_RADIXData.offset[i-1] + curCount[i-1];
      }
      else
      {
        // This is a special case to correctly handle negative integers. They're sorted in the right order but at the wrong place.

        // Create biased offsets, in order for negative numbers to be sorted as well
        l_RADIXData.offset[0] = sizeNegativeValues;                       // First positive number takes place after the negative ones
        for(i=1;i<128;i++)    l_RADIXData.offset[i] = l_RADIXData.offset[i-1] + curCount[i-1];  // 1 to 128 for positive numbers

        // Fixing the wrong place for negative values
        l_RADIXData.offset[128] = 0;
        for(i=129;i<256;i++)      l_RADIXData.offset[i] = l_RADIXData.offset[i-1] + curCount[i-1];

      }

      // Perform Radix Sort
      inputBytes  = (uchar*)values;
      indicesCur  = indices1;
      indicesEnd  = &indices1[size];
      inputBytes += j;
      while(indicesCur!=indicesEnd)
      {
        uint32 id = *indicesCur++;
        indices2[l_RADIXData.offset[inputBytes[id<<2]]++] = id;
      }
      // Swap pointers for next pass. Valid indices - the most recent ones - are in mIndices after the swap.
      tmp = indices1; indices1 = indices2; indices2 = tmp;
    }
  }

  return indices1;
}

// Main call for FLOAT
// -------------------
LUX_API uint32* lxSortRadixArrayFloat(const float *values2, uint size, uint32 *outindices1, uint32 *outindices2){
  int32 i,j;
  uchar *p,*pe;
  uint32  *h0,*h1,*h2,*h3;
  float val, prevVal;
  uchar alreadySorted;

  uint32 sizeNegativeValues;
  uint32* tmp;
  uint32* curCount;
  uchar performPass;
  uchar uniqueVal;

  uchar* inputBytes;
  uint32 *indices1 = outindices1;
  uint32 *indices2 = outindices2;
  uint32 *indicesCur;
  uint32 *indicesEnd;
  int32 radix;
  uint32 *values = (uint32*)values2;

  // reset counters & histogram
  memset(l_RADIXData.histogram,0,sizeof(unsigned int)*1024);
  memset(l_RADIXData.offset,0,sizeof(unsigned int)*256);

  // Create histograms (counters). Counters for all passes are created in one run.
  // Pros:  read input buffer once instead of four times
  // Cons:  mHistogram is 4Kb instead of 1Kb
  // Floating-point values are always supposed to be signed values, so there's only one code path there.
  // Please note the floating point comparison needed for temporal coherence! Although the resulting asm code
  // is dreadful, this is surprisingly not such a performance hit - well, I suppose that's a big one on first
  // generation Pentiums....We can't make comparison on integer representations because, as Chris said, it just
  // wouldn't work with mixed positive/negative values....

  // create counters
  p = (uchar*)values;                       
  pe = &p[size*4];                        
  h0= &l_RADIXData.histogram[0];  /* Histogram for first pass (LSB) */  
  h1= &l_RADIXData.histogram[256];  /* Histogram for second pass    */
  h2= &l_RADIXData.histogram[512];  /* Histogram for third pass     */
  h3= &l_RADIXData.histogram[768];  /* Histogram for last pass (MSB)  */


  prevVal = (float)values[indices1[0]];
  val = (float)values[indices1[0]];

  alreadySorted = LUX_TRUE; /* optimism */

  while(p!=pe)
  {
    /* Read input buffer in previous sorted order */
    val = (float)values[*indices1++];
    /* Check whether already sorted or not */
    if(val<prevVal) { alreadySorted = LUX_FALSE; break; } /* Early out */
    /* Update for next iteration */
    prevVal = val;

    /* Create histograms */
    h0[*p++]++; h1[*p++]++; h2[*p++]++; h3[*p++]++;
  }


  /* If all input values are already sorted, we just have to return and leave the */
  /* previous list unchanged. That way the routine may take advantage of temporal */
  /* coherence, for example when used to sort transparent faces.          */
  if(alreadySorted) { return outindices1; }

  /* Else there has been an early out and we must finish computing the histograms */
  while(p!=pe)
  {
    /* Create histograms without the previous overhead */
    h0[*p++]++; h1[*p++]++; h2[*p++]++; h3[*p++]++;
  }

  // Compute #negative values involved if needed
  sizeNegativeValues = 0;
  // An efficient way to compute the number of negatives values we'll have to deal with is simply to sum the 128
  // last values of the last histogram. Last histogram because that's the one for the Most Significant byte,
  // responsible for the sign. 128 last values because the 128 first ones are related to positive numbers.
  //h3= &l_RADIXData.histogram[768];
  for( i=128;i<256;i++) sizeNegativeValues += h3[i];  // 768 for last histogram, 128 for negative part
  // reset before sorting starts
  indices1 = outindices1;
  // Radix sort, j is the pass number (0=LSB, 3=MSB)
#ifdef LUX_ENDIAN_LITTLE
  for(j=0;j<4;j++)
#else
  for(j=3;j>=0;j--)
#endif
  {
    // Should we care about negative values?
    if(j!=RADIX_SIGN_PASS)
    {
      // Here we deal with positive values only
      CHECK_PASS_VALIDITY(j);

      if(performPass)
      {
        // Create offsets
        l_RADIXData.offset[0] = 0;
        for( i=1;i<256;i++)   l_RADIXData.offset[i] = l_RADIXData.offset[i-1] + curCount[i-1];

        // Perform Radix Sort
        inputBytes  = (uchar*)values;
        indicesCur  = indices1;
        indicesEnd  = &indices1[size];
        inputBytes += j;
        while(indicesCur!=indicesEnd)
        {
          uint32 id = *indicesCur++;
          indices2[l_RADIXData.offset[inputBytes[id<<2]]++] = id;
        }

        // Swap pointers for next pass. Valid indices - the most recent ones - are in mIndices after the swap.
        tmp = indices1; indices1 = indices2; indices2 = tmp;
      }
    }
    else
    {
      // This is a special case to correctly handle negative values
      CHECK_PASS_VALIDITY(j);

      if(performPass)
      {
        // Create biased offsets, in order for negative numbers to be sorted as well
        l_RADIXData.offset[0] = sizeNegativeValues;                       // First positive number takes place after the negative ones
        for(i=1;i<128;i++)    l_RADIXData.offset[i] = l_RADIXData.offset[i-1] + curCount[i-1];  // 1 to 128 for positive numbers

        // We must reverse the sorting order for negative numbers!
        l_RADIXData.offset[255] = 0;
        for(i=0;i<127;i++)    
          l_RADIXData.offset[254-i] = l_RADIXData.offset[255-i] + curCount[255-i];  // Fixing the wrong order for negative values
        for(i=128;i<256;i++)  
          l_RADIXData.offset[i] += curCount[i];             // Fixing the wrong place for negative values

        // Perform Radix Sort
        for(i=0;i< (int)size;i++)
        {
          radix = values[indices1[i]]>>24;                // Radix byte, same as above. AND is useless here (uint32).
          // ### cmp to be killed. Not good. Later.
          if(radix<128) indices2[l_RADIXData.offset[radix]++] = indices1[i];    // Number is positive, same as above
          else      indices2[--l_RADIXData.offset[radix]] = indices1[i];    // Number is negative, flip the sorting order

        }

        // Swap pointers for next pass. Valid indices - the most recent ones - are in mIndices after the swap.
        tmp = indices1; indices1 = indices2; indices2 = tmp;
      }
      else
      {
        // The pass is useless, yet we still have to reverse the order of current list if all values are negative.
        if(uniqueVal>=128)
        {
          for(i=0;i<(int)size;i++)  indices2[i] = indices1[size-i-1];

          // Swap pointers for next pass. Valid indices - the most recent ones - are in mIndices after the swap.
          tmp = indices1; indices1 = indices2; indices2 = tmp;
        }
      }
    }
  }

  return indices1;
}

#undef CHECK_PASS_VALIDITY

