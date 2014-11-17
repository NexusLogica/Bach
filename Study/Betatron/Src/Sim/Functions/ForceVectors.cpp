/**********************************************************************

File     : ForceVectors.cpp
Project  : Bach Simulation
Purpose  : Source file for an ForceVectors.
Revisions: Original definition by Lawrence Gunn.
           2013/06/21

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "ForceVectors.h"
#include "Molecule.h"
#include "SpOrbital.h"
#include <sstream>

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //****************
  //* ForceVectors *
  //****************

shared_ptr<ForceVectors> ForceVectors::CreateInstance() {
  boost::shared_ptr<ForceVectors> instance(new ForceVectors);
  return instance;
}

void ForceVectors::AddForceVector(const std::wstring& name, const Eigen::Vector3d& force) {
  m_forceNames.push_back(name);
  m_forceVectors.push_back(force);
}

std::wstring ForceVectors::GetForceName(Integer index) {
  BACH_ASSERT(index >= 0 && index < m_forceVectors.size());
  return m_forceNames[index];
}

Eigen::Vector3d ForceVectors::GetForceVector(Integer index) {
  BACH_ASSERT(index >= 0 && index < m_forceVectors.size());
  return m_forceVectors[index];
}

void ForceVectors::WriteToLog() {
  LogPlain(L"Force On: %s", m_targetName.c_str());
  if(m_targetTravelContraintSet) {
    LogPlain(L"  Force Constraint: %s", Vec3dToString(m_targetTravelConstraint).c_str());
  }
  
  for(Integer i=0; i<m_forceVectors.size(); i++) {
    const wchar_t* padding = L"                                  ";
    int padLen = 25-m_forceNames[i].size();
    if(padLen < 0)  { padLen = 0; } // Avoid negative length
    LogPlain(L"  %s%*.*s: %s", m_forceNames[i].c_str(), padLen, padLen, padding, Vec3dToStringFixed(m_forceVectors[i].array()*NEWTONS_TO_NANONEWTONS).c_str());
  }
}

  //********************
  //* ForceVectorsList *
  //********************
  
shared_ptr<ForceVectorsList> ForceVectorsList::CreateInstance() {
  boost::shared_ptr<ForceVectorsList> instance(new ForceVectorsList);
  return instance;
}

void ForceVectorsList::AddForceVector(boost::shared_ptr<ForceVectors> vectors) {
  m_forceVectorList.push_back(vectors);
}

shared_ptr<ForceVectors> ForceVectorsList::GetByIndex(Integer index) {
  BACH_ASSERT(index >= 0 && index < m_forceVectorList.size());
  return m_forceVectorList[index];
}

void ForceVectorsList::WriteToLog() {
  for(Integer i=0; i<m_forceVectorList.size(); i++) {
    m_forceVectorList[i]->WriteToLog();
  }
}