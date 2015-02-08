/**********************************************************************

File     : ChargeCircuitFieldCalculator.cpp
Project  : Bach Simulation
Purpose  : Source file for flow through a planar circuit.
Revisions: Original definition by Lawrence Gunn.
           2015/02/07

Copyright (c) 2015 by Lawrence Gunn
All Rights Reserved.

*/

#include "ChargeCircuitFieldCalculator.h"
#include "ChargeMagneticField.h"
#include "NDimAccuracySpec.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //********************************
  //* ChargeCircuitFieldCalculator *
  //********************************

shared_ptr<ChargeCircuitFieldCalculator> ChargeCircuitFieldCalculator::CreateInstance() {
  shared_ptr<ChargeCircuitFieldCalculator> instance(new ChargeCircuitFieldCalculator);
  return instance;
}

ChargeCircuitFieldCalculator::ChargeCircuitFieldCalculator() {
}

ChargeCircuitFieldCalculator::~ChargeCircuitFieldCalculator() {
}

void ChargeCircuitFieldCalculator::SetCharge(double chargeMagnitude, const Eigen::Vector4d& position, const Eigen::Vector4d& velocity) {
  m_magneticField = ChargeMagneticField::CreateInstance(chargeMagnitude);
  Eigen::Vector3d position3d(position(0), position(1), position(2));
  Eigen::Vector3d velocity3d(velocity(0), velocity(1), velocity(2));
  m_magneticField->SetPosition(position3d);
  m_magneticField->SetVelocity(velocity3d);
}

void ChargeCircuitFieldCalculator::SetCircuit(
  const Eigen::Matrix4d& centerTransform,
  double width,    // in centerTransform x direction
  double length)  // in centerTransform y direction
{
  m_centerTransform = centerTransform;
  m_width = width;
  m_length = length;
}

MagneticFieldVectorAtPoint ChargeCircuitFieldCalculator::GetMagneticFieldAtCircuitCoord(double widthCoord, double lengthCoord) {
  MagneticFieldVectorAtPoint vecAtPt;
  vecAtPt.point = GetPointFromCircuitCoord(widthCoord, lengthCoord);
  vecAtPt.magneticField = Vec3dTo4d(m_magneticField->GetMagneticFieldVector(Vec4dTo3d(vecAtPt.point)));
  return vecAtPt;
}


std::vector<MagneticFieldVectorAtPoint> ChargeCircuitFieldCalculator::GetFieldVectorsOnGrid(int samplesWidthwise, int samplesLengthwise) {
  std::vector<MagneticFieldVectorAtPoint> points;
  double x = -0.5*m_width;
  double xInc = m_width/(samplesWidthwise-1);
  double yInc = m_length/(samplesLengthwise-1);

  for(int i=0; i<samplesWidthwise; i++) {
    double y = -0.5*m_length;
    for(int j=0; j<samplesLengthwise; j++) {
      MagneticFieldVectorAtPoint atPoint = GetMagneticFieldAtCircuitCoord(x, y);
      points.push_back(atPoint);
      y += yInc;
    }
    x += xInc;
  }
  return points;
}

Eigen::Vector4d ChargeCircuitFieldCalculator::GetPointFromCircuitCoord(double widthCoord, double lengthCoord) {
  Eigen::Vector4d coord(widthCoord, lengthCoord, 0.0, 1.0);
  Eigen::Vector4d point = m_centerTransform*coord;
  return point;
}
