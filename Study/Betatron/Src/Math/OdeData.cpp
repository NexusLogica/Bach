/**********************************************************************

File     : OdeData.h
Project  : Bach Simulation
Purpose  : Source file for the ODE data class.
Revisions: Original definition by Lawrence Gunn.
           90/05/04.
           Parts of this are based on routines from Numerical Recipes in C, the Harwell libraries and other sources.

           Updated by Lawrence Gunn.
           2013/01/26


Copyright (c) 1990-2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "OdeData.h"
#include "OdeDataCollector.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

  //***********
  //* OdeData *
  //***********

OdeData::OdeData(boost::shared_ptr<OdeEquations> system) :
  m_system(system),
  m_initialValues(system->GetStateLength()),
  m_finalValues(system->GetStateLength()),
  m_internalValues(system->GetInternalLength())
{
  SetStartTime(0.0);
  SetEndTime(0.0);
  SetStopFlag(false);
}

shared_ptr<OdeData> OdeData::CreateInstance(boost::shared_ptr<OdeEquations> system) {
  shared_ptr<OdeData> instance = shared_ptr<OdeData>(new OdeData(system));
  instance->m_weakThis = instance;
  return instance;
}

void OdeData::ResetStorage() {
  if(m_collector) {
    m_collector->Restart();
  }
}

void OdeData::StoreData(double x,const Eigen::VectorXd& y, const Eigen::VectorXd& dy) {
  if(m_collector) { 
    m_collector->StoreData(x, y, dy, m_weakThis.lock());
  }
}
