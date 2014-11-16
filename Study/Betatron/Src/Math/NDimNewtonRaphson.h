/**********************************************************************

File     : NDimNewtonRaphson.h
Project  : Bach Simulation
Purpose  : Header file for an N dimensional NewtonRaphson.
Revisions: Original definition by Lawrence Gunn.
           2013/02/09.

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_NDIM_NEWTON_RAPHSON_H__
#define __BACH_NDIM_NEWTON_RAPHSON_H__

#include "RootSolverEquations.h"

namespace Bach {

  //*********************
  //* NDimNewtonRaphson *
  //*********************

  class NDimNewtonRaphson {
  public:
    NDimNewtonRaphson() {}
    virtual ~NDimNewtonRaphson() {}

    void SetRootValueAccuracySpec(boost::shared_ptr<NDimAccuracySpec> accuracySpec) { m_rootValueAccuracySpec = accuracySpec; }
    void SetDerivativesAccuracySpec(boost::shared_ptr<NDimAccuracySpec> accuracySpec) { m_derivativeAccuracySpec = accuracySpec; }

    void Solve(boost::shared_ptr<RootSolverEquations> equations, Eigen::VectorXd initialY, int maximumIterations = 100);

    Eigen::VectorXd GetRoots() { return m_roots; }
    Eigen::VectorXd GetInternalValues() { return m_internalValues; }

  protected:
    boost::shared_ptr<NDimAccuracySpec> m_rootValueAccuracySpec;
    boost::shared_ptr<NDimAccuracySpec> m_derivativeAccuracySpec;
    int m_maximumIterations;
    Eigen::VectorXd m_roots;
    Eigen::VectorXd m_internalValues;
  };
};

#endif // __BACH_NDIM_NEWTON_RAPHSON_H__
