/**********************************************************************

File     : RiddersExtrapolation.cpp
Project  : Bach Simulation
Purpose  : Source file for multivariate derivatives using Ridder's extrapolation.
Revisions: Original definition by Lawrence Gunn.
           92/11/17.

           Updated by Lawrence Gunn.
           2013/01/28


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "RiddersExtrapolation.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

  //************************
  //* RiddersExtrapolation *
  //************************

RiddersExtrapolation::RiddersExtrapolation(int size) :
  MultivariateDerivatives(size),
  m_firstHalfVector(size),
  m_isFinished(size)
{
  m_numberOfTables = size;
  m_tableSize = 10;

  m_tables.resize(m_numberOfTables);
  for(int i=0;i<m_numberOfTables;i++) {
    m_tables[i].resize(m_tableSize, m_tableSize);
  }

  m_decreaseFactor = 1.4;
  m_decreaseFactorSquared = m_decreaseFactor*m_decreaseFactor;
}

RiddersExtrapolation::~RiddersExtrapolation() {
}

void RiddersExtrapolation::Start(double target, double step) {
  MultivariateDerivatives::Start(target, step);
  m_firstHalf = true;
  m_iteration = 0;
  m_xTarget = target;
  m_initialStepSize = step;
  m_stepSize = step;
  m_isFinished.fill(false);
  m_leftToFinish = m_numberOfTables;
}

double RiddersExtrapolation::GetNewValue() {
  if(m_firstHalf) {
    return m_xTarget+m_stepSize;
  }
  else {
    return m_xTarget-m_stepSize;
  }
}

void RiddersExtrapolation::SetFunctionValues(const Eigen::VectorXd& f) {

  if(m_firstHalf) {

    // This should not happen
    if(m_iteration == m_tableSize) {
      return;
    }
    m_firstHalfVector = f;
    m_firstHalf = false;
  }
  else {
    m_firstHalfVector -= f;
    m_firstHalfVector /= (2.0*m_stepSize);

    int number;
    for(number=0; number < m_numberOfTables; number++) {
      if(m_isFinished(number)) {
        continue;
      }
      Eigen::MatrixXd* a = &m_tables[number];

      int i = m_iteration;
      (*a)(0, i) = m_firstHalfVector(number);
      double fac = m_decreaseFactorSquared;
      for(int j=1;j<=m_iteration;j++) {
        (*a)(j,i) = ((*a)(j-1,i)*fac-(*a)(j-1,i-1)) / (fac-1.0);
        fac *= m_decreaseFactorSquared;
        double errorThisStep = fabs((*a)(j,i)-(*a)(j-1,i));
        double errorLastStep = fabs((*a)(j,i)-(*a)(j-1,i-1));
        errorThisStep = (errorThisStep > errorLastStep) ?
        errorThisStep : errorLastStep;

        if(errorThisStep < m_errorVec(number) || m_iteration == 1) {
          m_extrapolatedValue(number) = (*a)(j,i);
          m_errorVec(number) = errorThisStep;
        }
        if(fabs((*a)(i,i)-(*a)(i-1,i-1)) >= 2.0*m_errorVec(number)) {
          m_isFinished(number) = true;
          m_leftToFinish--;
        }
      }
    }

    m_firstHalf = true;
    m_iteration++;
    m_stepSize /= m_decreaseFactor;
  }
}

bool RiddersExtrapolation::GetIsFinished(shared_ptr<NDimAccuracySpec> spec) {
  if(m_iteration == m_tableSize) {
    return true;
  }
  else if(m_leftToFinish == 0) {
    return true;
  }
  else if(spec && m_iteration > 1) {
    return AccuracySpecTest(spec);
  }
  return false;
}

bool RiddersExtrapolation::AccuracySpecTest(shared_ptr<NDimAccuracySpec> spec) {
  if(spec->GetNormalizedError(m_errorVec, m_extrapolatedValue) <= 1.0) {
    return true;
  }
  return false;
}
