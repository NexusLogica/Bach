/**********************************************************************

File     : OdeAccuracySpec.cpp
Project  : Bach Simulation
Purpose  : Source file for the base ODE accuracy specification class.
Revisions: Original definition by Lawrence Gunn.
           92/10/25.
           This is an extention of the accuracy specification technique in Harwell's DC01AD.

           Updated by Lawrence Gunn.
           2013/01/27


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "OdeAccuracySpec.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*******************
  //* OdeAccuracySpec *
  //*******************

OdeAccuracySpec::OdeAccuracySpec(unsigned int size, double eps) :
  NDimAccuracySpec(size, eps),
  m_relativeErrorForMaximums(size),
  m_relativeErrorOffsets(size),
  m_maximumValues(size)
{
  m_relativeErrorForMaximums.fill(0.0);
  m_relativeErrorOffsets.fill(0.0);
  m_maximumValues.fill(0.0);
}

void OdeAccuracySpec::CheckForMaximumValues(const Eigen::VectorXd& y) {
  ULong size = m_maximumValues.rows();
  Eigen::VectorXd temp = (y-m_relativeErrorOffsets).array().abs();
  for(ULong i=0;i<size;i++) {
    if(m_maximumValues(i) > temp(i))  m_maximumValues(i) = temp(i);
  }
}

double OdeAccuracySpec::GetNormalizedError(const Eigen::VectorXd& rsError, const Eigen::VectorXd& y) {
  m_normalizedError = 
      (
        rsError.array()/
        (
          m_absoluteError.array()
          +m_relativeError.array()*(y.array()-m_relativeErrorOffsets.array())
          +m_relativeErrorForMaximums.array()*(m_maximumValues.array()-m_relativeErrorOffsets.array()).array().abs()
        ).array()
      ).array().abs();
  m_normalizedError.array() *= 1.0/m_tolerance;
  double maxErr = m_normalizedError.maxCoeff();
  return maxErr;
}

double OdeAccuracySpec::GetOdeNormalizedError(const Eigen::VectorXd& rsError, const Eigen::VectorXd& y, shared_ptr<OdeSolver> solver, shared_ptr<OdeData> odeData) {
  return GetNormalizedError(rsError, y);
}
