/**********************************************************************

File     : HermiteInterp.h
Project  : Bach Simulation
Purpose  : Header file for a hermite interpolation class.
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

#ifndef __BACH_HERMITE_INTERP_H__
#define __BACH_HERMITE_INTERP_H__

#include "Interpolator.h"

namespace Bach {

  //*****************
  //* HermiteInterp *
  //*****************

  class HermiteInterp {
  public:
    static Real Interpolate(Real xTarget, Real x0, Real x1, Real y0, Real y1, Real dy0, Real dy1);
    static Real Interpolate(Real xTarget, const Eigen::VectorXd& x, const Eigen::VectorXd& y, const Eigen::VectorXd& dy, int index = 0);
    static void   Interpolate(Real xTarget, Real x0, Real x1, const Eigen::VectorXd& y0, const Eigen::VectorXd& y1, const Eigen::VectorXd& dy0, const Eigen::VectorXd& dy1, Eigen::VectorXd& yResult);
  };

  //*******************
  //* HermiteInterp1D *
  //*******************

  class HermiteInterp1D {
  public:
    static boost::shared_ptr<HermiteInterp1D> CreateInstance(Real x0, Real x1, Real y0, Real y1, Real dydx0, Real dydx1);

    Real Interpolate(Real x);
  private:
    HermiteInterp1D(Real x0, Real x1, Real y0, Real y1, Real dydx0, Real dydx1);

    Real m_x0;
    Real m_x1;
    Real m_y0;
    Real m_y1;
    Real m_dydx0;
    Real m_dydx1;
    Real m_range;
  };
};

#endif // __BACH_HERMITE_INTERP_H__
