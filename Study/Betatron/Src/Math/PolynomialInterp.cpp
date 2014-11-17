/**********************************************************************

File     : PolynomialInterp.cpp
Project  : Bach Simulation
Purpose  : Source file for a polynomial interpolation class.
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

#include "PolynomialInterp.h"
#include "RationalInterp.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //********************
  //* PolynomialInterp *
  //********************

PolynomialInterp::PolynomialInterp(int i) : m_C(i), m_D(i), m_differenceVector(i) {
  m_interpSize = i;
  m_aTinyValue = 1.0e-25;
}

double PolynomialInterp::Interpolate(double xTarget, const Eigen::VectorXd& x, const Eigen::VectorXd& y, int index) {
  if(index) {
    return PerformPolynomialInterpolation(xTarget,x.block(index, 0, m_interpSize, 1), y.block(index, 0, m_interpSize, 1));
  }
  else {
    return PerformPolynomialInterpolation(xTarget, x, y);
  }
}

double PolynomialInterp::PerformPolynomialInterpolation(double xTarget, const Eigen::VectorXd& x, const Eigen::VectorXd& y) {
  if(m_checkExtrapFlag) {
    CheckForExtrapolation(xTarget,x);
  }

  m_C = y;
  m_D = y.array()+m_aTinyValue;

  m_differenceVector = (x.array()-xTarget).abs();
  int indexSmallestDiff;
  m_differenceVector.minCoeff(&indexSmallestDiff);

  if(m_differenceVector(indexSmallestDiff) == 0.0)  {
    m_errorEstimateSingle = 0.0;
    return y(indexSmallestDiff);
  }

  double yTarget = y(indexSmallestDiff);
  indexSmallestDiff--;

  // For each column in the interpolation table...
  for(int m=1; m<m_interpSize; m++) {

    // This loop could be vectorized but with the small values
    // of i likely encountered, the vector method may involve
    // substancially more overhead. 
    for (int i=0; i<m_interpSize-m; i++) {
      double idif = x(i)-xTarget;
      double imdif = x(i+m)-xTarget;
      double width = m_C(i+1)-m_D(i);
      double denominator = idif-imdif;

      if(denominator == 0.0) {
        // If the polynomial interpolation fails then it is reasable that a rational
        // interpolation will succeed.
        RationalInterp tryRationalInterp(m_interpSize);
        return tryRationalInterp.Interpolate(xTarget, x, y);

        // NXError(NX_ERROR,"NXPolynomialInterp::polynomialInterp",
        //  "Two x vector elements are equal");
      }

      denominator = width/denominator;
      m_C(i) = idif*denominator;
      m_D(i) = imdif*denominator;
    }

    if(2*(indexSmallestDiff+1) < (m_interpSize-m))  {
      m_errorEstimateSingle = m_C(indexSmallestDiff+1);
    }
    else  {
      m_errorEstimateSingle = m_D(indexSmallestDiff);
      indexSmallestDiff--;
    }

    yTarget += m_errorEstimateSingle;
  }

  return yTarget;
}

void PolynomialInterp::SetNumberOfPoints(int np) {
  m_interpSize = np;
  m_C.resize(np);
  m_D.resize(np);
  m_differenceVector.resize(np);
}
