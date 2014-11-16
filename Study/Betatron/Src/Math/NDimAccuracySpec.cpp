/**********************************************************************

File     : NDimAccuracySpec.h
Project  : Bach Simulation
Purpose  : Header file for the base class for a multidimensional accuracy specification object.
Revisions: Original definition by Lawrence Gunn.
           92/11/23.

           Updated by Lawrence Gunn.
           2013/01/27


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "NDimAccuracySpec.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //********************
  //* NDimAccuracySpec *
  //********************

NDimAccuracySpec::NDimAccuracySpec(unsigned int size, double eps) :
  m_absoluteError(size),
  m_relativeError(size),
  m_normalizedError(size)
{
  SetTolerance(eps);
  m_absoluteError.fill(1.0e-10);
  m_relativeError.fill(1.0);
  m_normalizedError.fill(0.0);
}

NDimAccuracySpec::~NDimAccuracySpec() {
}

double NDimAccuracySpec::GetNormalizedError(const Eigen::VectorXd& error, const Eigen::VectorXd& y) {
  m_normalizedError = (error.array()/(m_absoluteError.array()+m_relativeError.array()*y.array())).cwiseAbs();
  m_normalizedError *= 1.0/m_tolerance;

  double maxError = m_normalizedError.maxCoeff();
  return maxError;
}
