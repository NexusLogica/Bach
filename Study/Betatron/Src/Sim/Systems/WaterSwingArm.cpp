/**********************************************************************

File     : WaterSwingArm.cpp
Project  : Bach Simulation
Purpose  : Source file for an ordinary differential wrapper class around a water bond rotation molecule model.
Revisions: Original definition by Lawrence Gunn.
           2014/06/23

Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#include "WaterSwingArm.h"
#include "OdeData.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*****************
  //* WaterSwingArm *
  //*****************

shared_ptr<WaterSwingArm> WaterSwingArm::CreateInstance() {
  shared_ptr<WaterSwingArm> instance(new WaterSwingArm());
  instance->m_weakThis = instance;
  instance->InitializeModel();
  return instance;
}

WaterSwingArm::WaterSwingArm() {
}

WaterSwingArm::~WaterSwingArm() {
}

void WaterSwingArm::InitializeModel() {
  SetStateLength(3);
  SetInternalLength(0);
}

void WaterSwingArm::Initialize(shared_ptr<Bach::OdeData> odeData) {
}

void WaterSwingArm::Evaluate(double time, const Eigen::VectorXd& y, Eigen::VectorXd& dydt, shared_ptr<Bach::OdeData> odeData) {
  LogPlain(L"Iteration %e", time);
  

/*
  count++;
  LogPlain(L"Iteration %d", count);
  shared_ptr<Bond> bond;
  shared_ptr<BondElectrons> bondElectrons;
  int i = 0;
  int j = 0;
  for(; i<y.size(); i += 2, j++) {
    bond = m_molecule->GetBondByIndex(j);
    bondElectrons = bond->GetBondElectrons();
    LogPlain(L"Pos[%d]: %e", j, y[i]);
    LogPlain(L"Vel%d]: %e", j, y[i+1]);
    bondElectrons->SetLinearPosition(y[i]); // Set the position relative to the first atom.
    bondElectrons->SetLinearVelocity(y[i+1]);
// LAG - all wrong...    bondElectrons->CalculatePositionAndVelocity();
  }

  Eigen::VectorXd internalValues(odeData->GetInternalLength());
  PSReal acceleration;
  i = 0;
  j = 0;
  for(; i<y.size(); i += 2, j++) {
    bondElectrons = m_molecule->GetBondByIndex(j)->GetBondElectrons();
    acceleration = bondElectrons->CalculateAcceleration();

    dydt[i] = y[i+1];
    dydt[i+1] = acceleration;

    LogPlain(L"dxdt[%d]: %e", j, dydt[i]);
    LogPlain(L"dvdt[%d]: %e", j, dydt[i+1]);
    
    internalValues[i] = bondElectrons->GetElectricalInducedForce();
    internalValues[i+1] = bondElectrons->GetMagneticInducedForce();
  }
  
  odeData->SetInternalValues(internalValues);
*/
}
