/**********************************************************************

File     : RandomGenerator.h
Project  : PlaySpace Game Engine Class Library
Purpose  : Header file for a random number generator.
Revisions: Original definition by Lawrence Gunn.
           1996/08/01

           Added to PlaySpace by Lawrence Gunn
           2012/02/24

Copyright (c) 1995-2012 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __PS_RANDOMGENERATOR_H__
#define __PS_RANDOMGENERATOR_H__

#include "BachDefs.h"
#include <boost/smart_ptr.hpp>

#define NXRG_TABLESIZE 55

namespace Bach {

    //*************************
    //* Bach::RandomGenerator *
    //*************************

  class RandomGenerator {
  public:
    static boost::shared_ptr<RandomGenerator> CreateInstance();
    static boost::shared_ptr<RandomGenerator> CreateInstance(long seed);
    static boost::shared_ptr<RandomGenerator> GetDefaultRandomGenerator();

    ~RandomGenerator();

    long GetSeed();
    void SetSeed(long seed);

    long GetRandomInteger();
    long GetRandomIntegerInRange(long minValue, long maxValue);
    double GetRandomDouble(double minValue, double maxValue);

  private:

    RandomGenerator();

    void ReinitializeArray();

    long m_alTable[NXRG_TABLESIZE]; // A table filled with random longs
    bool m_tableInitialized;
    int m_index;                  // Index into the table
    long m_seed;                   // The start up seed
    bool m_seedSet;

    static boost::shared_ptr<RandomGenerator> s_randomGenerator;
  };
};

#endif // __NX_RANDOMGENERATOR_H__
