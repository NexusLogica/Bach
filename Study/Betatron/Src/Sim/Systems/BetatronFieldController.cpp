/**********************************************************************

File     : BetatronFieldController.cpp
Project  : Bach Simulation
Purpose  : Source file for the ordinary differential equations of a charge in a betatron.
Revisions: Original definition by Lawrence Gunn.
           2014/10/04

Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#include "BetatronFieldController.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //***************************
  //* BetatronFieldController *
  //***************************

shared_ptr<BetatronFieldController> BetatronFieldController::CreateInstance() {
  shared_ptr<BetatronFieldController> instance(new BetatronFieldController());
  instance->m_weakThis = instance;
  return instance;
}

BetatronFieldController::BetatronFieldController() :
  m_controlType(ConstantB),
  m_constantB(1.0),
  m_fractionalIncreaseBPerSecond(0.0) {
  m_directionOfConstantB[2] = 1.0;
}

BetatronFieldController::~BetatronFieldController() {
}

void BetatronFieldController::SetAsConstantB(double B, const Eigen::Vector3d &direction) {
  m_controlType = ConstantB;
  m_constantB = B;
  m_directionOfConstantB = direction;
}

void BetatronFieldController::SetAsBetatronAcceleration(double initialB, double targetAccelerationRate) {
  m_controlType = BetatronAcceleration;
  m_initialB = initialB;
}

void BetatronFieldController::GetField(
  double t,
  const Eigen::Vector3d& positionInField,
  boost::shared_ptr<PointMagneticField>& field) {

  switch(m_controlType) {
    case ConstantB : {
      double totalB = m_constantB*(1.0+m_fractionalIncreaseBPerSecond*t);
      field->SetDirection(m_directionOfConstantB);
      field->SetB(totalB);
      field->SetdBdt(0.0);
      field->SetdDelBdt(Eigen::Vector3d(0.0, 0.0, 0.0));
      break;
    }
    case BetatronAcceleration : {
      break;
    }
  }
}
