/**********************************************************************

File     : RandomGenerator.cpp
Project  : PlaySpace Game Engine Class Library
Purpose  : Source file for a random number generator.
Revisions: Original definition by Lawrence Gunn.
           1996/08/01

           Added to PlaySpace by Lawrence Gunn
           2012/02/24

Copyright (c) 1995-2012 by Lawrence Gunn
All Rights Reserved.

*/

#include "RandomGenerator.h"
#include <time.h> // Used for seeding the default generator...

using namespace Bach;
using namespace boost;

    //***********************
    //* PS::RandomGenerator *
    //***********************

boost::shared_ptr<RandomGenerator> RandomGenerator::s_randomGenerator;

RandomGenerator::RandomGenerator() : m_tableInitialized(false), m_seedSet(false) {
}

RandomGenerator::~RandomGenerator() {
}

shared_ptr<RandomGenerator> RandomGenerator::CreateInstance() {
  return shared_ptr<RandomGenerator>(new RandomGenerator());
}

shared_ptr<RandomGenerator> RandomGenerator::CreateInstance(long seed) {
  shared_ptr<RandomGenerator> instance(new RandomGenerator());
  instance->SetSeed(seed);
  return instance;
}

shared_ptr<RandomGenerator> RandomGenerator::GetDefaultRandomGenerator() {
  if(!s_randomGenerator.get()) {
    s_randomGenerator = RandomGenerator::CreateInstance();
    long seed = (long) time(0);
    s_randomGenerator->SetSeed(seed);
  }

  return s_randomGenerator;
}

void RandomGenerator::ReinitializeArray() {
  if(!m_seedSet) {
    m_seed = RandomGenerator::GetDefaultRandomGenerator()->GetRandomInteger();
    m_seedSet = true;
  }

  long temp = m_seed;
  long index;
  for(index=0; index<4; index++) {
    temp = temp*84589+45989;            // Warm up.
  }

  for(index=NXRG_TABLESIZE; index--;) {
    temp = 45989 + temp*84589;
    m_alTable[(index * 21) % NXRG_TABLESIZE] = temp;   // Fill and shuffle.
  }

  m_index = 0;
  m_tableInitialized = true;
}

long RandomGenerator::GetSeed() {
  if(!m_seedSet) {
    ReinitializeArray();
  }

  return m_seed;
}

void RandomGenerator::SetSeed(long seed) {
  m_seedSet = true;
  m_seed = seed;
  ReinitializeArray();
}

long RandomGenerator::GetRandomInteger() {
  if(!m_tableInitialized) {
    ReinitializeArray();
  }

  m_index = (m_index + 1) % NXRG_TABLESIZE;
  m_alTable[m_index] =
    m_alTable[(m_index + 23) % NXRG_TABLESIZE] +
    m_alTable[(m_index + 54) % NXRG_TABLESIZE];

  return m_alTable[m_index];
}

long RandomGenerator::GetRandomIntegerInRange(long minValue, long maxValue) {
  long randomValue = GetRandomInteger();
  return (abs(randomValue) % (maxValue-minValue+1)) + minValue;
}


double RandomGenerator::GetRandomDouble(double minValue, double maxValue) {
  long random = GetRandomInteger();

  double ratio = (double) random/LONG_MAX;  // -1 <= ratio  <= 1
  double x     = 0.5*(ratio+1.0);           // 0  <= x      <= 1
  double value = x*minValue+(1-x)*maxValue;
  if(minValue < maxValue) { // check for roundoff in boundary cases
    if(value < minValue) {
      value = minValue;
    }
    if(value > maxValue) {
      value = maxValue;
    }
  }
  else {
    if(value < maxValue) {
      value = maxValue;
    }
    if(value > minValue) {
      value = minValue;
    }
  }

  return value;
}
