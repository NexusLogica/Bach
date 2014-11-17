/**********************************************************************

File     : NDimNewtonRaphson.cpp
Project  : Bach Simulation
Purpose  : Source file for an N dimensional NewtonRaphson.
Revisions: Original definition by Lawrence Gunn.
           2013/02/09.

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "NDimNewtonRaphson.h"
#include "NDimAccuracySpec.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*********************
  //* NDimNewtonRaphson *
  //*********************

void NDimNewtonRaphson::Solve(shared_ptr<RootSolverEquations> equations, Eigen::VectorXd initialY, int maximumIterations) {

  Integer n = equations->GetStateLength();
  m_roots = initialY;

  Eigen::VectorXd functionValues(n);
  Eigen::MatrixXd jacobian(n, n);
  Eigen::VectorXd inverseSolution(n);

  for(Integer k=0; k < maximumIterations; k++) {
    functionValues = equations->GetEquationValues(m_roots);

    Real errf = m_rootValueAccuracySpec->GetNormalizedError(functionValues.array().abs(), functionValues);
    if(errf <= 1.0) {
      break;
    }

    inverseSolution.array() = -functionValues.array();
    
    jacobian = equations->GetJacobian(m_roots);

    ColPivHouseholderQR<MatrixXd> aDecomposition = jacobian.colPivHouseholderQr();
    inverseSolution = aDecomposition.solve(inverseSolution);

    m_roots.array() += inverseSolution.array();

    double errx = m_derivativeAccuracySpec->GetNormalizedError(inverseSolution.array().abs(), functionValues);
    if(errx <= 1.0) {
      break;
    }
  }
}
