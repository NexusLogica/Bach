/**********************************************************************

File     : RandomUniform.cpp
Project  : PlaySpace Game Engine Class Library
Purpose  : Source file for a uniform random number generator.
Revisions: Original definition by Lawrence Gunn.
           1996/07/30

           Added to PlaySpace by Lawrence Gunn
           2012/02/24

Copyright (c) 1995-2012 by Lawrence Gunn
All Rights Reserved.

*/

#include "RandomUniform.h"
#include "RandomGenerator.h"

using namespace Bach;
using namespace boost;

    //*********************
    //* PS::RandomUniform *
    //*********************

RandomUniform::RandomUniform() :  m_lowerBound(0.0), m_upperBound(1.0) {
  m_seedObject = RandomGenerator::GetDefaultRandomGenerator();
}

RandomUniform::~RandomUniform() {
}

shared_ptr<RandomUniform> RandomUniform::CreateInstance() {
  return shared_ptr<RandomUniform>(new RandomUniform());
}

shared_ptr<RandomUniform> RandomUniform::CreateInstance(shared_ptr<RandomGenerator> seedObject) {
  shared_ptr<RandomUniform> instance(new RandomUniform());
  instance->SetSeedObject(seedObject);
  return instance;
}

Real RandomUniform::GetRandomNum() {
  long randomInteger = m_seedObject->GetRandomInteger();

  double ratio = (double) randomInteger/LONG_MAX;  // -1 <= rRatio  <= 1
  double x     = 0.5*(ratio+1.0);           // 0  <= rX      <= 1
  double value = x*m_lowerBound+(1-x)*m_upperBound;

  if(m_lowerBound < m_upperBound) { // check for roundoff in boundary cases
    if(value < m_lowerBound) {
      value = m_lowerBound;
    }
    if(value > m_upperBound) {
      value = m_upperBound;
    }
  }
  else {
    if(value < m_upperBound) {
      value = m_upperBound;
    }
    if(value > m_lowerBound) {
      value = m_lowerBound;
    }
  }

  return (Real) value;
}

Real RandomUniform::GetRandomNum(Real lowerBound, Real upperBound) {
  long randomInteger = m_seedObject->GetRandomInteger();

  double ratio = (double) randomInteger/LONG_MAX;  // -1 <= rRatio  <= 1
  double x     = 0.5*(ratio+1.0);                  // 0  <= rX      <= 1
  double value = x*lowerBound+(1-x)*upperBound;

  if(lowerBound < upperBound) {                    // check for roundoff in boundary cases
    if(value < lowerBound) {
      value = lowerBound;
    }
    if(value > upperBound) {
      value = upperBound;
    }
  }
  else {
    if(value < upperBound) {
      value = upperBound;
    }
    if(value > lowerBound) {
      value = lowerBound;
    }
  }

  return (Real) value;
}

Real RandomUniform::GetRandomDoubleNum(Real lowerBound, Real upperBound) {
  long randomInteger = m_seedObject->GetRandomInteger();

  Real ratio = (Real) randomInteger/LONG_MAX;  // -1 <= rRatio  <= 1
  Real x     = 0.5*(ratio+1.0);           // 0  <= rX      <= 1
  Real value = x*lowerBound+(1-x)*upperBound;

  if(lowerBound < upperBound) { // check for roundoff in boundary cases
    if(value < lowerBound) {
      value = lowerBound;
    }
    if(value > upperBound) {
      value = upperBound;
    }
  }
  else {
    if(value < upperBound) {
      value = upperBound;
    }
    if(value > lowerBound) {
      value = lowerBound;
    }
  }

  return value;
}

Eigen::Vector3d RandomUniform::GetRandomUnitVector() {
  Real z = 2.0f*GetRandomNum()-1.0f;
  Real normalizingLength = sqrt(1.0f-z*z);

  Real azimuth = 2.0f*NXGR_PI*GetRandomNum();
  Real x = cos(azimuth)*normalizingLength;
  Real y = sin(azimuth)*normalizingLength;

  return Eigen::Vector3d(x, y, z);
}

Real RandomUniform::GetRandomNumWithSeedAndBounds(boost::shared_ptr<RandomGenerator> seedObj, Real lowerBound, Real upperBound) {
  long randomInteger = seedObj->GetRandomInteger();

  double ratio = (double) randomInteger/LONG_MAX;  // -1 <= rRatio  <= 1
  double x     = 0.5*(ratio+1.0);           // 0  <= rX      <= 1
  double value = x*lowerBound+(1-x)*upperBound;
  if(lowerBound < upperBound) {                   // check for roundoff in boundary cases
    if(value < lowerBound) {
      value = lowerBound;
    }
    if(value > upperBound) {
      value = upperBound;
    }
  }
  else {
    if(value < upperBound) {
      value = upperBound;
    }
    if(value > lowerBound) {
      value = lowerBound;
    }
  }

  return (Real) value;
}
