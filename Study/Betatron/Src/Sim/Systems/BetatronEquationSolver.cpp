/**********************************************************************

File     : BetatronEquationSolver.cpp
Project  : Bach Simulation
Purpose  : Source file for the ODE solver for a charge in a betatron.
Revisions: Original definition by Lawrence Gunn.
           2014/11/06

Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#include "BetatronEquationSolver.h"
#include "BetatronEquations.h"
#include "BetatronFieldController.h"
#include "NDimAccuracySpec.h"
#include "SampledData.h"
#include "SampledDerivedData.h"
#include "BaderDeuflhardOde.h"
#include "OdeData.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //**************************
  //* BetatronEquationSolver *
  //**************************

shared_ptr<BetatronEquationSolver> BetatronEquationSolver::CreateInstance() {
  shared_ptr<BetatronEquationSolver> instance(new BetatronEquationSolver());
  instance->m_weakThis = instance;
  return instance;
}

BetatronEquationSolver::BetatronEquationSolver() :
  m_startTime(0.0),
  m_endTime(1.0),
  m_magneticFieldMagnitude(0.0),
  m_initialPosition(3),
  m_initialVelocity(3),
  m_stepSize(0.00001),
  m_iterationCount(0)
{
  m_fieldController = BetatronFieldController::CreateInstance();
}

BetatronEquationSolver::~BetatronEquationSolver() {
}

void BetatronEquationSolver::SetInitialConditionsFromRadiusAndSpeed(double radius, double speed) {
  m_magneticFieldMagnitude = 0.0;
  m_initialPosition(0) = radius; // positive X
  m_initialPosition(1) = 0.0;
  m_initialPosition(2) = 0.0;
  m_initialVelocity(0) = 0.0;
  m_initialVelocity(1) = speed;  // positive Y
  m_initialVelocity(2) = 0.0;
  
  double centripetalForce = Bach::ELECTRON_MASS*speed*speed/radius;
  // F = e * V x B
  m_magneticFieldMagnitude = centripetalForce/(Bach::ELECTRIC_CHARGE*speed);
  
  m_fieldController->SetAsConstantInward(m_magneticFieldMagnitude);
  
  // Calculate an appropriate stepsize.
  double distanceTraveledOverOneDegree = 2.0*NXGR_PI*radius/360.0;
  double timeToTravelOneDegree = distanceTraveledOverOneDegree/speed;
  m_stepSize = timeToTravelOneDegree;
}

void BetatronEquationSolver::Initialize() {
  m_equations = BetatronEquations::CreateInstance();
  m_equations->SetFieldController(m_fieldController);

  m_odeData = OdeData::CreateInstance(m_equations);
  m_odeData->SetStartTime(m_startTime);
  m_odeData->SetEndTime(m_endTime);

  Eigen::VectorXd initialConditions(m_equations->GetStateLength());
  initialConditions(0) = m_initialPosition(0);
  initialConditions(1) = m_initialPosition(1);
  initialConditions(2) = m_initialPosition(2);
  initialConditions(3) = m_initialVelocity(0);
  initialConditions(4) = m_initialVelocity(1);
  initialConditions(5) = m_initialVelocity(2);
  m_odeData->SetInitialConditions(initialConditions);

  m_solver = BaderDeuflhardOde::CreateInstance();
  m_solver->SetStepSize(m_stepSize);
}

void BetatronEquationSolver::Run() {
  m_solver->Solve(m_odeData);
}
