/**********************************************************************

File     : OdeSolverWithDerivs.cpp
Project  : Bach Simulation
Purpose  : Source file for the OdeSolver derived class for using supplied derivatives.
Revisions: Original definition by Lawrence Gunn.
           92/12/12.
           This class is the base class for ordinary differential equation solvers
           which require the derivatives of the system equations (i.e. Jacobian and
           time dependent derivatives)

           Updated by Lawrence Gunn.
           2013/01/26


Copyright (c) 1992-2012 by Lawrence Gunn
All Rights Reserved.

*/

#include "OdeSolverWithDerivs.h"
#include "OdeNumericalDerivatives.h"
#include "OdeData.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

  //***********************
  //* OdeSolverWithDerivs *
  //***********************

void OdeSolverWithDerivs::InitializeDifferentiator(shared_ptr<OdeData> odeData) {
  if(!m_differentiator) {
    m_differentiator = shared_ptr<OdeNumericalDerivatives>(new OdeNumericalDerivatives(odeData->GetStateLength()));
  }
}
