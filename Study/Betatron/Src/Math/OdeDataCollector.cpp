/**********************************************************************

File     : OdeDataCollector.h
Project  : Bach Simulation
Purpose  : Header file for the ODE data collector class.
Revisions: Original definition by Lawrence Gunn.
           90/05/04.
           Updated 92/10/01.

           Updated by Lawrence Gunn.
           2013/01/29


Copyright (c) 1990-2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "OdeDataCollector.h"
#include "OdeData.h"
#include "SampledDerivedData.h"
#include "SampledData.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

namespace {
  int INITIAL_DATA_SIZE = 2000;
};

  //********************
  //* OdeDataCollector *
  //********************

OdeDataCollector::OdeDataCollector() {
  m_stateStorageIsStopped = false;
  m_internalStorageIsStopped = false;
}

OdeDataCollector::~OdeDataCollector() {
}

void OdeDataCollector::Restart() {
  if(m_stateData) {
    m_stateData->Reset();
  }
  if(m_internalData) {
    m_internalData->Reset();
  }
}

void OdeDataCollector::StoreData(double x, const Eigen::VectorXd& y, const Eigen::VectorXd& dy, boost::shared_ptr<OdeData> odeData) {
  if(!m_stateData) {
    m_stateData = shared_ptr<SampledDerivedData>(new SampledDerivedData(odeData->GetStateLength(), INITIAL_DATA_SIZE));
  }
  if(!m_internalData) {
    m_internalData = shared_ptr<SampledData>(new SampledData(odeData->GetInternalLength(), INITIAL_DATA_SIZE));
  }

  StoreStates(x, y, dy,odeData);
  StoreInternals(x, odeData);
}

void OdeDataCollector::StoreStates(double x, const Eigen::VectorXd& y, const Eigen::VectorXd& dy, boost::shared_ptr<OdeData> odeData) {
  if(!m_stateStorageIsStopped) {
    if(m_stateData->GetNumberOfSamples() >= m_stateData->GetMaxNumberOfSamples()) {
      HandleStateDataOverflow(x, odeData);
    }

    if(!m_stateStorageIsStopped) {
      m_stateData->Store(x, y, dy);
    }
  }
}

void OdeDataCollector::StoreInternals(double x, boost::shared_ptr<OdeData> odeData) {
  if(!m_internalStorageIsStopped) {
    if(m_internalData->GetNumberOfSamples() >= m_internalData->GetMaxNumberOfSamples()) {
      HandlerInternalDataOverflow(x, odeData);
    }

    if(!m_internalStorageIsStopped)
      m_internalData->Store(x, odeData->GetInternalValues());
    }
}

void OdeDataCollector::HandleStateDataOverflow(double x, boost::shared_ptr<OdeData> odeData) {
  int newLength = GetNewStateSamplesSize(x, odeData);
  m_stateData->Resize(newLength);
}

void OdeDataCollector::HandlerInternalDataOverflow(double x, boost::shared_ptr<OdeData> odeData) {
  int newLength = GetNewInternalSamplesSize(x, odeData);
  m_internalData->Resize(newLength);
}

int OdeDataCollector::GetNewStateSamplesSize(double time, boost::shared_ptr<OdeData> odeData) {
  int presentLength = m_stateData->GetNumberOfSamples();
  double start = odeData->GetStartTime();
  double end   = odeData->GetEndTime();
  int newLength = presentLength*(int)ceil((end-start)/(time-start));
  newLength = (int)(newLength*1.30);
  return newLength;
}

int OdeDataCollector::GetNewInternalSamplesSize(double time, boost::shared_ptr<OdeData> odeData)
{
  int presentLength = m_internalData->GetNumberOfSamples();
  double start = odeData->GetStartTime();
  double end   = odeData->GetEndTime();
  int newLength = presentLength*(int)ceil((end-start)/(time-start));
  newLength = (int)(newLength*1.30);
  return newLength;
}
