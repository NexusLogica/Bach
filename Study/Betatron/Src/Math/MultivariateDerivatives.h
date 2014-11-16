/**********************************************************************

File     : MultivariateDerivatives.h
Project  : Bach Simulation
Purpose  : Header file for the base multivariate derivatives object.
Revisions: Original definition by Lawrence Gunn.
           92/11/17.

           Updated by Lawrence Gunn.
           2013/01/28


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_MULTIVARIATE_DERIVATIVES_H__
#define __BACH_MULTIVARIATE_DERIVATIVES_H__

#include "NDimAccuracySpec.h"

namespace Bach {

  //***************************
  //* MultivariateDerivatives *
  //***************************

  class MultivariateDerivatives {
  public:
    MultivariateDerivatives(int size) : m_extrapolatedValue(size), m_errorVec(size) { }
    virtual ~MultivariateDerivatives() {}

    virtual void Start(double target,double step) {
      m_extrapolatedValue.fill(step);
      m_errorVec.fill(target);
    }

    virtual double GetNewValue() = 0;
    virtual void SetFunctionValues(const Eigen::VectorXd& values) = 0;
    virtual bool GetIsFinished(boost::shared_ptr<NDimAccuracySpec> spec) = 0;

    const Eigen::VectorXd& GetDerivatives() { return m_extrapolatedValue; }
    const Eigen::VectorXd& GetError()       { return m_errorVec; }

    int GetLength() { return (int) m_errorVec.rows(); }

  protected:
    Eigen::VectorXd m_extrapolatedValue;
    Eigen::VectorXd m_errorVec;
  };
};

#endif // __BACH_MULTIVARIATE_DERIVATIVES_H__
