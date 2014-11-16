/**********************************************************************

File     : PolynomialInterp.h
Project  : Bach Simulation
Purpose  : Header file for a polynomial interpolation class.
Revisions: Original definition by Lawrence Gunn.
           1990/05/08
           The interpolation index classes provide means of choosing an interpolation
           index based upon an index in a table.

           Revised for RW library and better readability.
           1992/07/29.

           Table search functions and interpolation index class separated into
           two different classes.
           1992/09/08.

           Updated by Lawrence Gunn.
           2013/01/29

Copyright (c) 1990-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_POLYNOMIAL_INTERP_H__
#define __BACH_POLYNOMIAL_INTERP_H__

#include "Interpolator.h"

namespace Bach {

  //********************
  //* PolynomialInterp *
  //********************

  class PolynomialInterp : public Interpolator {
  public:
    PolynomialInterp(int i = 3);

    virtual double Interpolate(double xTarget, const Eigen::VectorXd& x,const Eigen::VectorXd& y, int index = 0);
    virtual void SetNumberOfPoints(int n);

  protected:
    double m_aTinyValue;

    Eigen::VectorXd m_C;
    Eigen::VectorXd m_D;
    Eigen::VectorXd m_differenceVector;

    double PerformPolynomialInterpolation(double,const Eigen::VectorXd&,const Eigen::VectorXd&);
  };
};

#endif // __BACH_POLYNOMIAL_INTERP_H__
