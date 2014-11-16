/**********************************************************************

File     : RootSolverEquationsOdeWrapper.cpp
Project  : Bach Simulation
Purpose  : Source file for the wrapper of the base root solver equations for ODE systems.
Revisions: Original definition by Lawrence Gunn.
           90/05/04.
           Parts of this are based on routines from Numerical Recipes in C, the Harwell libraries and other sources.

           Updated by Lawrence Gunn.
           2013/02/13

Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "RootSolverEquationsOdeWrapper.h"
#include "OdeData.h"
#include "Jacobian.h"
#include "RiddersExtrapolation.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*********************************
  //* RootSolverEquationsOdeWrapper *
  //*********************************

shared_ptr<RootSolverEquationsOdeWrapper> RootSolverEquationsOdeWrapper::CreateInstance(shared_ptr<OdeData> odeData, double stepSize) {
  shared_ptr<RootSolverEquationsOdeWrapper> instance(new RootSolverEquationsOdeWrapper(odeData, stepSize));
  return instance;
}

RootSolverEquationsOdeWrapper::RootSolverEquationsOdeWrapper(shared_ptr<OdeData> odeData, double stepSize) : m_x(0.0), m_stepSize(stepSize), m_odeData(odeData) {
  SetStateLength(m_odeData->GetStateLength());
  SetInternalLength(m_odeData->GetInternalLength());

  m_stepSizeVector.resize(GetStateLength());
  m_stepSizeVector.fill(stepSize);
  m_tempValues.resize(m_odeData->GetStateLength());

  m_jacobianFinder = shared_ptr<Jacobian>(new Jacobian());
  shared_ptr<MultivariateDerivatives> derivatives = shared_ptr<MultivariateDerivatives>(new RiddersExtrapolation(m_odeData->GetStateLength()));
  m_jacobianFinder->SetDerivativeFinder(derivatives);
}

void RootSolverEquationsOdeWrapper::SetJacobianAccuracySpec(boost::shared_ptr<NDimAccuracySpec> accuracySpec) {
  m_jacobianFinder->SetAccuracySpec(accuracySpec);
}

void RootSolverEquationsOdeWrapper::Initialize() {
  m_odeData->GetOdeSystem()->Initialize(m_odeData);
}

Eigen::VectorXd RootSolverEquationsOdeWrapper::GetEquationValues(const Eigen::VectorXd& y) {

  Eigen::VectorXd functionValues(GetStateLength());

  shared_ptr<OdeEquations> system = m_odeData->GetOdeSystem();
  m_odeData->SetStoringThisCall(true);
  system->Evaluate(m_x, y, functionValues, m_odeData);

  SetInternals(m_odeData->GetInternalValues());
  m_odeData->SetStoringThisCall(false);
  
  return functionValues;
}

Eigen::MatrixXd RootSolverEquationsOdeWrapper::GetJacobian(const Eigen::VectorXd& y) {

  shared_ptr<OdeEquations> system = m_odeData->GetOdeSystem();
  m_jacobianFinder->Start(y, m_stepSizeVector);

  while(!m_jacobianFinder->GetIsFinished()) {
    system->Evaluate(m_x, m_jacobianFinder->GetNewValue(), m_tempValues, m_odeData);
    m_jacobianFinder->SetFunctionValues(m_tempValues);
  }

  return m_jacobianFinder->GetDerivatives();
}
