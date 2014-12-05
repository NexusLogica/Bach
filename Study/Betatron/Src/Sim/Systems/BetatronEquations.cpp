/**********************************************************************

File     : BetatronEquations.cpp
Project  : Bach Simulation
Purpose  : Source file for the ordinary differential equations of a charge in a betatron.
Revisions: Original definition by Lawrence Gunn.
           2014/10/04


Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#include "BetatronEquations.h"
#include "BetatronFieldController.h"
#include "OdeData.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*********************
  //* BetatronEquations *
  //*********************

shared_ptr<BetatronEquations> BetatronEquations::CreateInstance() {
  shared_ptr<BetatronEquations> instance(new BetatronEquations());
  instance->m_weakThis = instance;
  return instance;
}

BetatronEquations::BetatronEquations() :
  m_charge(-1.0),
  m_mass(Bach::ELECTRON_MASS),
  m_iterationCount(0),
  m_magneticField(new PointMagneticField),
  m_internalValues(5) {  // B, dBdt, velocity, distance from origin, angle
  // y[0] = x position
  // y[1] = y position
  // y[2] = z position
  // y[3] = x velocity
  // y[4] = y velocity
  // y[5] = z velocity
  SetStateLength(6);
  SetInternalLength((unsigned int) m_internalValues.size());
}

BetatronEquations::~BetatronEquations() {
}

void BetatronEquations::Initialize(shared_ptr<Bach::OdeData> odeData) {
  m_iterationCount = 0;
}

void BetatronEquations::Evaluate(double time, const Eigen::VectorXd& y, Eigen::VectorXd& dydt, shared_ptr<Bach::OdeData> odeData) {

  m_iterationCount++;

  m_position[0] = y[0];
  m_position[1] = y[1];
  m_position[2] = y[2];

  m_fieldController->GetField(time, m_position, m_magneticField);
  m_velocity[0] = y[3];
  m_velocity[1] = y[4];
  m_velocity[2] = y[5];

  m_force = m_velocity.cross(m_magneticField->UnitVectorB())*m_charge*m_magneticField->B()*Bach::ELECTRIC_CHARGE;
  double massInv = 1.0/m_mass;

  dydt[0] = y[3];
  dydt[1] = y[4];
  dydt[2] = y[5];

  dydt[3] = m_force[0]*massInv;
  dydt[4] = m_force[1]*massInv;
  dydt[5] = m_force[2]*massInv;

  if(odeData->StoringThisCall()) {
    m_internalValues[0] = m_magneticField->B();
    m_internalValues[1] = m_magneticField->dBdt();
    m_internalValues[2] = m_velocity.norm();
    m_internalValues[3] = sqrt(Square(y[0])+Square(y[1])+Square(y[2]));
    m_internalValues[4] = NXGR_RAD_TO_DEG*atan2(y[1], y[0]);

    odeData->SetInternalValues(m_internalValues);
  }
}
