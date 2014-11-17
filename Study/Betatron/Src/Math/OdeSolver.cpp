/**********************************************************************

File     : OdeSolver.cpp
Project  : Bach Simulation
Purpose  : Source file for the base ODE solver.
Revisions: Original definition by Lawrence Gunn.
           90/05/04.
           Parts of this are based on routines from Numerical Recipes in C, the Harwell libraries and other sources.

           Updated by Lawrence Gunn.
           2013/01/26


Copyright (c) 1990-2012 by Lawrence Gunn
All Rights Reserved.

*/

#include "OdeSolver.h"
#include "OdeData.h"
#include "OdeAccuracySpec.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*************
  //* OdeSolver *
  //*************

void OdeSolver::InitializeAccuracySpec(shared_ptr<OdeData> odeData) {
  if(!m_accuracySpec)  {
    m_accuracySpec = shared_ptr<OdeAccuracySpec>(new OdeAccuracySpec(odeData->GetStateLength()));
  }

  else if(m_accuracySpec->GetLength() != odeData->GetStateLength()) {
    m_accuracySpec = shared_ptr<OdeAccuracySpec>(new OdeAccuracySpec(odeData->GetStateLength()));
  }
}
