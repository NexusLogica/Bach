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
#include "OdeDataCollector.h"
#include "SampledData.h"
#include "SampledDerivedData.h"

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
  m_forceDueToB(0.0, 0.0, 0.0),
  m_forceDueTodBdt(0.0, 0.0, 0.0),
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

  odeData->GetCollector()->InitializeWithSizes(6, 5);

  std::vector<std::string> names;
  names.push_back("| B |");
  names.push_back("| dBbt |");
  names.push_back("v");
  names.push_back("r");
  names.push_back("θ");

  std::vector<std::string> units;
  units.push_back("Teslas");
  units.push_back("Teslas/s");
  units.push_back("m/s");
  units.push_back("m");
  units.push_back("rad");
  
  odeData->GetCollector()->GetInternalData()->SetIndependentName("time");
  odeData->GetCollector()->GetInternalData()->SetIndependentUnit("s");
  odeData->GetCollector()->GetInternalData()->SetArrayColumnNames(names);
  odeData->GetCollector()->GetInternalData()->SetArrayColumnUnits(units);

  std::vector<std::string> stateNames;
  stateNames.push_back("x");
  stateNames.push_back("y");
  stateNames.push_back("z");
  stateNames.push_back("dx/dt");
  stateNames.push_back("dy/dt");
  stateNames.push_back("dz/dt");

  std::vector<std::string> stateUnits;
  stateUnits.push_back("m");
  stateUnits.push_back("m");
  stateUnits.push_back("m");
  stateUnits.push_back("m/s");
  stateUnits.push_back("m/s");
  stateUnits.push_back("m/s");

  odeData->GetCollector()->GetStateData()->SetIndependentName("time");
  odeData->GetCollector()->GetStateData()->SetIndependentUnit("s");
  odeData->GetCollector()->GetStateData()->SetArrayColumnNames(stateNames);
  odeData->GetCollector()->GetStateData()->SetArrayColumnUnits(stateUnits);
}

void BetatronEquations::Evaluate(double time, const Eigen::VectorXd& y, Eigen::VectorXd& dydt, shared_ptr<Bach::OdeData> odeData) {

  m_iterationCount++;

  m_position[0] = y[0];
  m_position[1] = y[1];
  m_position[2] = y[2];

  double rotationalPosition = 0.0;
  double rotationalAngle = atan2(y[1], y[0]);
  if(rotationalAngle < 0.0) {
    rotationalAngle = 2.0*NXGR_PI+rotationalAngle;
  }
  rotationalPosition = rotationalAngle/(2.0*NXGR_PI);
  m_fieldController->GetField(time, m_position, m_magneticField);

  Log(L"RotaationalPos: %f y,x = %f, %f      B = %e", rotationalPosition, y[1], y[0], m_charge*m_magneticField->B());

  m_velocity[0] = y[3];
  m_velocity[1] = y[4];
  m_velocity[2] = y[5];

  m_forceDueToB = m_velocity.cross(m_magneticField->UnitVectorB())*m_charge*m_magneticField->B()*Bach::ELECTRIC_CHARGE;

  m_forceDueTodBdt = m_magneticField->UnitVectorB().cross(m_magneticField->dDelBdt());
  
  double massInv = 1.0/m_mass;

  dydt[0] = y[3];
  dydt[1] = y[4];
  dydt[2] = y[5];

  dydt[3] = m_forceDueToB[0]*massInv;
  dydt[4] = m_forceDueToB[1]*massInv;
  dydt[5] = m_forceDueToB[2]*massInv;

  if(odeData->StoringThisCall()) {
    m_internalValues[0] = m_magneticField->B();
    m_internalValues[1] = m_magneticField->dBdt();
    m_internalValues[2] = m_velocity.norm();
    m_internalValues[3] = sqrt(Square(y[0])+Square(y[1])+Square(y[2]));
    m_internalValues[4] = NXGR_RAD_TO_DEG*rotationalAngle;

    odeData->SetInternalValues(m_internalValues);
  }
}
