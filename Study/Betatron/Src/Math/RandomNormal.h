/**********************************************************************

File     : RandomNormal.h
Project  : PlaySpace Game Engine Class Library
Purpose  : Header file for a normal distribution random number generator.
Revisions: Original definition by Lawrence Gunn.
           2012/08/01

Notes    : The algorithm and code is taken from http://www.jstatsoft.org/v05/i08 by George Marsaglia and Wai Wan Tsang,
           "The Ziggurat Method for Generating Random Variables" in Journal of Statistical Software, Oct 2000.

           To use this, pass in an average and standard deviation. Almost all values out will be within +/- 2 times
           the standard deviation. So to get a value between 800 and 1200, pass in an average of 1000 and std dev of 100.

Copyright (c) 2012 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __PS_RANDOMNORMAL_H__
#define __PS_RANDOMNORMAL_H__

#include "RandomGenerator.h"

namespace Bach {

    //**********************
    //* Bach::RandomNormal *
    //**********************

  class RandomNormal {
  public:

    static boost::shared_ptr<RandomNormal> CreateInstance();

    ~RandomNormal();

    boost::shared_ptr<RandomGenerator> GetSeedObject() { return m_seedObject; }
    void SetSeedObject(boost::shared_ptr<RandomGenerator> seedObject) { m_seedObject = seedObject; }

    Real GetRandomNum(Real average, Real deviation);

    void Test();

  protected:

    RandomNormal();
    void ZigguratInitialize(unsigned long jsrSeed);
    Real NormalGen();
    Real ExponentialGen();

    boost::shared_ptr<RandomGenerator> m_seedObject;
    unsigned long m_currentSeed;

    unsigned long jz;
    unsigned long jsr;
    unsigned long m_jsrInitial;

    long hz;
    unsigned long iz;
    unsigned long kn[128];
    unsigned long ke[256];
    Real wn[128];
    Real fn[128];
    Real we[256];
    Real fe[256];
  };
};

#endif // __PS_RANDOMNORMAL_H__



