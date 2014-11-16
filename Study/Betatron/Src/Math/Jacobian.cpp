/**********************************************************************

File     : Jacobian.cpp
Project  : Bach Simulation
Purpose  : Source file for multivariate derivatives using Ridder's extrapolation.
Revisions: Original definition by Lawrence Gunn.
           92/11/17.

           Updated by Lawrence Gunn.
           2013/01/28


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "Jacobian.h"
#include "MultivariateDerivatives.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //************
  //* Jacobian *
  //************

void Jacobian::Start(const Eigen::VectorXd& t, const Eigen::VectorXd& step) {
  if(m_numberOfColumns != m_derivativeFinder->GetLength()) {
    m_numberOfColumns = m_derivativeFinder->GetLength();

    m_jacobian.resize(m_numberOfColumns, m_numberOfColumns);
    m_errorMatrix.resize(m_numberOfColumns, m_numberOfColumns);
    m_target.resize(m_numberOfColumns);
    m_stepSize.resize(m_numberOfColumns);
  }

  m_target = t;
  m_stepSize = step;

  m_complete = false;
  m_column = 0;
  m_derivativeFinder->Start(m_target(0), step(0));
  m_tempTarget = m_target(0);
}

const Eigen::VectorXd& Jacobian::GetNewValue() {
  m_target(m_column) = m_derivativeFinder->GetNewValue();
  return m_target;
}

void Jacobian::SetFunctionValues(const Eigen::VectorXd& f) {
  m_derivativeFinder->SetFunctionValues(f);
  if(m_derivativeFinder->GetIsFinished(m_accuracySpec)) {

    // Copy the results.
    m_jacobian.col(m_column) = m_derivativeFinder->GetDerivatives();
    m_errorMatrix.col(m_column) = m_derivativeFinder->GetError();
    m_target(m_column) = m_tempTarget;

    m_column++;
    if(m_column == m_numberOfColumns) {
      m_complete = true;
    }
    else {
      m_derivativeFinder->Start(m_target(m_column), m_stepSize(m_column));
      m_tempTarget = m_target(m_column);
    }
  }
}

bool Jacobian::GetIsFinished() {
  return m_complete;
}
