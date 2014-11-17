/**********************************************************************

File     : HermiteInterp.cpp
Project  : Bach Simulation
Purpose  : Source file for a rational interpolation class.
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

#include "HermiteInterp.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*****************
  //* HermiteInterp *
  //*****************

double HermiteInterp::Interpolate(double xTarget,double x0,double x1, double y0,double y1,double dy0,double dy1) {
  double range = x1-x0;
  double d1 = (xTarget-x0)/range;
  double d2 = d1*d1;
  double d3 = d2*d1;
  double coeff2 = 3.0*d2-2.0*d3;
  double coeff1 = 1.0-coeff2;
  double coeff4 = (d3-d2)*range;
  double coeff3 = coeff4+(d1-d2)*range;

  return coeff1*y0 + coeff2*y1 + coeff3*dy0 + coeff4*dy1;
}

double HermiteInterp::Interpolate(double xTarget,const Eigen::VectorXd& x, const Eigen::VectorXd& y,const Eigen::VectorXd& dy,int index) {
  int indexP1 = index+1;

  double range = x(indexP1)-x(index);
  double d1 = (xTarget-x(index))/range;
  double d2 = d1*d1;
  double d3 = d2*d1;
  double coeff2 = 3.0*d2-2.0*d3;
  double coeff1 = 1.0-coeff2;
  double coeff4 = (d3-d2)*range;
  double coeff3 = coeff4+(d1-d2)*range;

  return coeff1*y(index) + coeff2*y(indexP1) + coeff3*dy(index) + coeff4*dy(indexP1);
}

void HermiteInterp::Interpolate(
  double xTarget,
  double x0,
  double x1,
  const Eigen::VectorXd& y0,
  const Eigen::VectorXd& y1,
  const Eigen::VectorXd& dy0,
  const Eigen::VectorXd& dy1,
  Eigen::VectorXd& yResult)
{
  double range = x1-x0;
  double d1 = (xTarget-x0)/range;
  double d2 = d1*d1;
  double d3 = d2*d1;
  double coeff2 = 3.0*d2-2.0*d3;
  double coeff1 = 1.0-coeff2;
  double coeff4 = (d3-d2)*range;
  double coeff3 = coeff4+(d1-d2)*range;

  yResult = coeff1*y0 + coeff2*y1 + coeff3*dy0 + coeff4*dy1;
}

  //*******************
  //* HermiteInterp1D *
  //*******************

shared_ptr<HermiteInterp1D> HermiteInterp1D::CreateInstance(Real x0, Real x1, Real y0, Real y1, Real dydx0, Real dydx1) {
  shared_ptr<HermiteInterp1D> instance(new HermiteInterp1D(x0, x1, y0, y1, dydx0, dydx1));
  return instance;
}

HermiteInterp1D::HermiteInterp1D(Real x0, Real x1, Real y0, Real y1, Real dydx0, Real dydx1) :
  m_x0(x0),
  m_x1(x1),
  m_y0(y0),
  m_y1(y1),
  m_dydx0(dydx0),
  m_dydx1(dydx1),
  m_range(x1-x0)
{
}

Real HermiteInterp1D::Interpolate(Real x) {
  Real d1 = (x-m_x0)/m_range;
  Real d2 = d1*d1;
  Real d3 = d2*d1;
  Real coeff2 = 3.0*d2-2.0*d3;
  Real coeff1 = 1.0-coeff2;
  Real coeff4 = (d3-d2)*m_range;
  Real coeff3 = coeff4+(d1-d2)*m_range;

  return coeff1*m_y0 + coeff2*m_y1 + coeff3*m_dydx0 + coeff4*m_dydx1;
}
