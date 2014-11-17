/**********************************************************************

File     : Circuit.cpp
Project  : Bach Simulation
Purpose  : Source file for the ordinary differential equations of a charge in a betatron.
Revisions: Original definition by Lawrence Gunn.
           2014/10/04

Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#include "Circuit.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //***********
  //* Circuit *
  //***********

shared_ptr<Circuit> Circuit::CreateInstance() {
  shared_ptr<Circuit> instance(new Circuit());
  instance->m_weakThis = instance;
  return instance;
}

Circuit::Circuit() :
  m_radius(1.0),  // meters
  m_current(1.0), // amps
  m_segments(100)
{
}

Circuit::~Circuit() {
}

Eigen::Vector3d Circuit::GetField(const Eigen::Vector3d& point) {
  double segmentLength = 2.0*NXGR_PI*m_radius/m_segments;
  double segmentCoeff = PERMIABILITY_OVER_4PI*segmentLength*m_current;

  Eigen::Vector3d B(0.0);
  Eigen::Vector3d pos(0.0, 0.0, 0.0);
  Eigen::Vector3d up(0.0, 0.0, 1.0);

  double angle = 0.0;
  double angleInc = 2.0*NXGR_PI/m_segments;
  for(int i=0; i<m_segments; i++) {
    pos[0] = m_radius*cos(angle);
    pos[1] = m_radius*sin(angle);
    Eigen::Vector3d flow = pos.cross(up);
    flow.normalize();
    Eigen::Vector3d segmentB = GetFieldFromPoint(point, pos, flow, segmentCoeff);
    B.array() += segmentB.array();
    angle += angleInc;
  }
  return B;
}

Eigen::Vector3d Circuit::GetFieldFromPoint(
  const Eigen::Vector3d& point,
  const Eigen::Vector3d& segmentCenter,
  const Eigen::Vector3d& flowDirection,
  double segmentCoeff
) {
  Eigen::Vector3d r = segmentCenter.array()-point.array();
  double rLengthSquared = r.squaredNorm(); // distance squared.
  Eigen::Vector3d rUnit = r.normalized();
  Eigen::Vector3d Vcrossr = flowDirection.cross(rUnit);

  Eigen::Vector3d B = segmentCoeff/rLengthSquared*Vcrossr.array();
  return B;
}
