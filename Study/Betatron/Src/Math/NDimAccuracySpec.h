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

#ifndef __BACH_NDIM_ACCURACY_SPEC_H__
#define __BACH_NDIM_ACCURACY_SPEC_H__

#include "BetatronDefs.h"

namespace Bach {

  //********************
  //* NDimAccuracySpec *
  //********************

  class NDimAccuracySpec {
  public:
    NDimAccuracySpec(unsigned int size, double eps = 0.0001);

    virtual ~NDimAccuracySpec();

    virtual double GetNormalizedError(const Eigen::VectorXd& error, const Eigen::VectorXd& y);
    Eigen::VectorXd GetNormalizedErrorVector() { return m_normalizedError; }

    int GetLength() { return (int) m_normalizedError.rows(); }

    void  SetTolerance(double eps) { m_tolerance = fabs(eps); }
    double GetTolerance() { return m_tolerance; }

    void SetAbsoluteError(double e)                 { m_absoluteError.fill(fabs(e)); }
    void SetAbsoluteError(const Eigen::VectorXd& e) { m_absoluteError = e.array().abs(); }
    void SetRelativeError(double e)                 { m_relativeError.fill(fabs(e)); }
    void SetRelativeError(const Eigen::VectorXd& e) { m_relativeError = e.array().abs(); }

    Eigen::VectorXd GetAbsoluteError() { return m_absoluteError; }
    Eigen::VectorXd GetRelativeError() { return m_relativeError; }

  protected:
    double m_tolerance;

    Eigen::VectorXd m_absoluteError;
    Eigen::VectorXd m_relativeError;

    Eigen::VectorXd m_normalizedError;
  };
};

#endif // __BACH_NDIM_ACCURACY_SPEC_H__
