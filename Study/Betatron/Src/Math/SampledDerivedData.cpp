/**********************************************************************

File     : SampledDerivedData.cpp
Project  : Bach Simulation
Purpose  : Source file for the sampled derived data that depends on a variable.
Revisions: Original definition by Lawrence Gunn.
           1990/05/08

           Minor revisions.
           1990/10/18.

           Major revisions and updating.  Base class of NXDependentData
           created and data class changed to NXSampledDerivedData.  Interface
           is simplified.
           1992/08/02.

           Updated by Lawrence Gunn.
           2013/01/29

Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "SampledDerivedData.h"
#include "HermiteInterp.h"
#include "SequentialAccessHunt.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //**********************
  //* SampledDerivedData *
  //**********************

SampledDerivedData::SampledDerivedData(int numDependent, int maxNumberOfSamples) :
  m_maxNumberOfSamples(maxNumberOfSamples),
  m_numberOfSamples(0)
{
  m_numberOfDependent = numDependent;

  m_x.resize(m_maxNumberOfSamples);
  m_yArray.resize(m_maxNumberOfSamples);

  m_indexHunter = boost::shared_ptr<TableSearch>(new SequentialAccessHunt());
}

SampledDerivedData::~SampledDerivedData() {
  size_t numYArray = m_yArray.size();
  for(size_t i=0; i<numYArray; i++) {
    if(m_yArray[i] != NULL) {
      delete m_yArray[i];
      m_yArray[i] = NULL;
    }
    else {
      break;
    }
  }
}

void SampledDerivedData::Store(double x, const Eigen::VectorXd& y, const Eigen::VectorXd& dy) {
  if(m_numberOfSamples >= m_maxNumberOfSamples) {
    Log(L"ERROR: SampledDerivedData::Store: Attempt to store too many m_x/states (%u)",m_numberOfSamples);
    throw std::exception();
  }

  m_x(m_numberOfSamples) = x;

  m_yArray[m_numberOfSamples] = new Eigen::VectorXd(2*m_numberOfDependent);
  m_yArray[m_numberOfSamples]->block(0, 0, m_numberOfDependent, 1) = y;
  m_yArray[m_numberOfSamples]->block(m_numberOfDependent, 0, m_numberOfDependent, 1) = dy;
  m_numberOfSamples++;
}

double SampledDerivedData::operator()(int index) const {
  if(!CheckBounds(index)) {
    Log(L"ERROR: SampledDerivedData::operator()(int): Bounds error at %u [0 to %d]", index, m_numberOfSamples-1);
    throw std::exception();
  }
  return m_x(index);
}

double SampledDerivedData::operator()(int i, int j) const {
  if(!CheckBounds(i, j)) {
    Log(L"ERROR: SampledDerivedData::operator()(int i,int j): State bounds error at (%u,%u) [is (0,0) to (%u,%u)]", i, j, m_numberOfSamples-1, m_numberOfDependent-1);
    throw std::exception();
  }
  return (*m_yArray[i])(j);
}

void SampledDerivedData::Retrieve(int index, double& x, Eigen::VectorXd& y, Eigen::VectorXd& dy) const {
  if(!CheckBounds(index)) {
    Log(L"ERROR: SampledDerivedData::Retrieve(i,d,v,v): Bounds error at %u [0 to %d]", index, m_numberOfSamples-1);
    throw std::exception();
  }

  x  = m_x(index);
  y  = m_yArray[index]->block(0, 0, m_numberOfDependent, 1);
  dy = m_yArray[index]->block(m_numberOfDependent, 0, m_numberOfDependent, 1);
}

double SampledDerivedData::Retrieve(double xTarget, int yIndex) {
  if(!CheckBounds(0, yIndex*2)) {
    Log(L"ERROR: SampledDerivedData::Retrieve(d,i): State bounds error at %u [0 to %d]", yIndex, m_numberOfSamples-1);
    throw std::exception();
  }

  int indexLow = m_indexHunter->Find(xTarget, m_x.block(0, 0, m_numberOfSamples, 1));
  indexLow = InterpolationIndex::ChooseInterpolationIndex(m_numberOfSamples, indexLow, 2);

  return HermiteInterp::Interpolate(
    xTarget,
    m_x(indexLow),
    m_x(indexLow+1),
    (*m_yArray[indexLow])(yIndex),
    (*m_yArray[indexLow+1])(yIndex),
    (*m_yArray[indexLow])(yIndex+m_numberOfDependent),
    (*m_yArray[indexLow+1])(yIndex+m_numberOfDependent));
}

void SampledDerivedData::Retrieve(double xTarget, Eigen::VectorXd& ydy) {
  if(m_numberOfDependent != ydy.rows()) {
    Log(L"ERROR: SampledDerivedData::Retrieve(d,v): State bounds error (%u vs %d)", ydy.rows(), m_numberOfDependent);
    throw std::exception();
  }

  int indexLow = m_indexHunter->Find(xTarget, m_x.block(0, 0, m_numberOfSamples, 1));
  indexLow = InterpolationIndex::ChooseInterpolationIndex(m_numberOfSamples, indexLow, 2);

  HermiteInterp::Interpolate(
    xTarget,
    m_x(indexLow),
    m_x(indexLow+1),
    m_yArray[indexLow]->block(0, 0, m_numberOfDependent, 1),
    m_yArray[indexLow+1]->block(0, 0, m_numberOfDependent, 1),
    m_yArray[indexLow]->block(m_numberOfDependent, 0, m_numberOfDependent, 1),
    m_yArray[indexLow+1]->block(m_numberOfDependent, 0, m_numberOfDependent, 1),
    ydy);
}

void SampledDerivedData::Reset() {
  m_numberOfSamples = 0;
}

void SampledDerivedData::Resize(int newSize) {
  // Increase the size.
  if(newSize > m_maxNumberOfSamples) {
    Eigen::VectorXd copy = m_x.block(0, 0, m_numberOfSamples, 1);
    m_x.resize(newSize);
    m_x.block(0, 0, m_numberOfSamples, 1) = copy;

    //Eigen::VectorXd** newState = new Eigen::VectorXd*[newSize];
    m_yArray.resize(newSize, NULL);

    m_maxNumberOfSamples = newSize;
  }

    // Decrease the size;
  else if(newSize < m_maxNumberOfSamples) {
    // Copy but keep the existing values.
    Eigen::VectorXd copy = m_x.block(0, 0, newSize, 1);
    m_x.resize(newSize);
    m_x.block(0, 0, newSize, 1) = copy;

    for(int i=newSize-1; i<m_numberOfSamples; i++)  {
      delete m_yArray[i];
    }

    m_yArray.resize(newSize, NULL);

    if(m_numberOfSamples < newSize) {
      m_numberOfSamples = newSize;
    }
    m_maxNumberOfSamples = newSize; 
  }
}

int SampledDerivedData::Max(int nstate) const {
  if(m_numberOfSamples < 1) {
    return -1;
  }

  int index = 0;
  double maxValue = (*m_yArray[0])(nstate);

  for(int i=1;i<m_numberOfSamples;i++)  {
    if(maxValue < (*m_yArray[i])(nstate))  {
      maxValue = (*m_yArray[i])(nstate);
      index = i;
    }
  }
  return index;
}

int SampledDerivedData::Min(int nstate) const {
  int index = 0;
  double minValue = (*m_yArray[0])(nstate);

  for(int i=1;i<m_numberOfSamples;i++)  {
    if(minValue > (*m_yArray[i])(nstate))  {
      minValue = (*m_yArray[i])(nstate);
      index = i;
    }
  }
  return index;
}

void SampledDerivedData::WriteToLog() {
  IOFormat fmt(StreamPrecision, DontAlignCols, ", ", ", ", "", "", "", "");
  Eigen::VectorXd y(m_numberOfDependent);
  for(int i=0; i<m_numberOfSamples; i++) {
    std::stringstream stream;
    stream << m_x(i) << ", " << m_yArray[i]->format(fmt);
    LogPlain(WideStringFromUTF8(stream.str()).c_str());
  }
}
