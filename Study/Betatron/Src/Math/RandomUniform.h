/**********************************************************************

File     : RandomUniform.h
Project  : PlaySpace Game Engine Class Library
Purpose  : Header file for a uniform random number generator.
Revisions: Original definition by Lawrence Gunn.
           1996/07/30

           Added to PlaySpace by Lawrence Gunn
           2012/02/24

Copyright (c) 1995-2012 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __PS_RANDOMUNIFORM_H__
#define __PS_RANDOMUNIFORM_H__

#include "RandomGenerator.h"

namespace Bach {

    //***********************
    //* Bach::RandomUniform *
    //***********************

  class RandomUniform {
  public:

    static boost::shared_ptr<RandomUniform> CreateInstance();
    static boost::shared_ptr<RandomUniform> CreateInstance(boost::shared_ptr<RandomGenerator> seedObject);

    ~RandomUniform();

    boost::shared_ptr<RandomGenerator> GetSeedObject() { return m_seedObject; }
    void SetSeedObject(boost::shared_ptr<RandomGenerator> seedObject) { m_seedObject = seedObject; }

    Real GetLowerBound() { return m_lowerBound; }
    void SetLowerBound(Real lowerBound) { m_lowerBound = lowerBound; }

    Real GetUpperBound() { return m_upperBound; }
    void SetUpperBound(Real upperBound) { m_upperBound = upperBound; }

    Real GetRandomNum();
    Real GetRandomNum(Real lowerBound, Real upperBound);
    Real GetRandomDoubleNum(Real lowerBound, Real upperBound);
    Eigen::Vector3d GetRandomUnitVector();

    Real GetRandomNumWithSeedAndBounds(boost::shared_ptr<RandomGenerator> seedObj, Real lowerBound, Real upperBound);

  protected:

    RandomUniform();

    boost::shared_ptr<RandomGenerator> m_seedObject;
    Real m_lowerBound;
    Real m_upperBound;
  };
};

#endif // __PS_RANDOMUNIFORM_H__



