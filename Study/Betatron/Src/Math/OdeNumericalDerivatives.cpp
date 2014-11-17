/**********************************************************************

File     : OdeNumericalDerivatives.cpp
Project  : Bach Simulation
Purpose  : Source file for the base ODE system of equation's derivative's class.
Revisions: Original definition by Lawrence Gunn.
           92/11/25.
           This is a class for returning numerically approximated derivatives of an
           ordinary system of differential equations.

           Updated by Lawrence Gunn.
           2013/01/27


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "OdeNumericalDerivatives.h"
#include "NDimAccuracySpec.h"
#include "RiddersExtrapolation.h"
#include "Jacobian.h"
#include "OdeData.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //***************************
  //* OdeNumericalDerivatives *
  //***************************

OdeNumericalDerivatives::OdeNumericalDerivatives(int size, double tol) : m_stepSize(size), m_tempValues(size) {
  m_stepSize.fill(1.0);

  m_derivatives = shared_ptr<MultivariateDerivatives>(new RiddersExtrapolation(size));
  m_accuracySpec = shared_ptr<NDimAccuracySpec>(new NDimAccuracySpec(size));
  m_accuracySpec->SetTolerance(tol);

  m_jacobian = shared_ptr<Jacobian>(new Jacobian());

  m_jacobian->SetDerivativeFinder(m_derivatives);
  m_jacobian->SetAccuracySpec(m_accuracySpec);

  m_xStepSize = 1.0;
}

OdeNumericalDerivatives::OdeNumericalDerivatives(shared_ptr<MultivariateDerivatives> derivatives, shared_ptr<NDimAccuracySpec> accuracySpec) :
  m_stepSize(derivatives->GetLength()),
  m_tempValues(derivatives->GetLength())
{
  m_stepSize.fill(1.0);

  m_derivatives = derivatives;
  m_accuracySpec = accuracySpec;

  m_jacobian = shared_ptr<Jacobian>(new Jacobian());

  m_jacobian->SetDerivativeFinder(m_derivatives);
  m_jacobian->SetAccuracySpec(m_accuracySpec);

  m_xStepSize = 1.0;
}

void OdeNumericalDerivatives::GetDerivatives(double x, const Eigen::VectorXd& y, Eigen::VectorXd& dfdx, Eigen::MatrixXd& dfdy, shared_ptr<OdeData> odeData) {
  odeData->GetOdeSystem()->SetLogging(false);
  GetTimeDeriv(x,y,odeData);
  dfdx = m_derivatives->GetDerivatives();
  GetJacobian(x, y, odeData);
  dfdy = m_jacobian->GetDerivatives();
  odeData->GetOdeSystem()->SetLogging(true);
}

void OdeNumericalDerivatives::SetTolerance(double tol) {
  m_accuracySpec->SetTolerance(tol);
}

void OdeNumericalDerivatives::GetTimeDeriv(double x,const Eigen::VectorXd& y, shared_ptr<OdeData> odeData) {
  shared_ptr<OdeEquations> system = odeData->GetOdeSystem();
  m_derivatives->Start(x, m_xStepSize);

  while(!m_derivatives->GetIsFinished(m_accuracySpec)) {
    system->Evaluate(m_derivatives->GetNewValue(), y, m_tempValues, odeData);
    m_derivatives->SetFunctionValues(m_tempValues);
  }
}

void OdeNumericalDerivatives::GetJacobian(double x,const Eigen::VectorXd& y, shared_ptr<OdeData> odeData) {
  shared_ptr<OdeEquations> system = odeData->GetOdeSystem();
  m_jacobian->Start(y, m_stepSize);

  while(!m_jacobian->GetIsFinished()) {
    system->Evaluate(x, m_jacobian->GetNewValue(), m_tempValues, odeData);
    m_jacobian->SetFunctionValues(m_tempValues);
  }
}
