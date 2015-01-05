/**********************************************************************

File     : SampledData.cpp
Project  : Bach Simulation
Purpose  : Source file for the sampled data where for each x (time) there is one y vector stored.
Revisions: Original definition by Lawrence Gunn.
           1990/05/08

           Minor revisions.
           1990/10/18.

           Major revisions and updating.  Base class of NXDependentData
           created and data class changed to SampledData. Interface is simplified.
           1992/08/02.

           Creation of an alternate constructor for use as parent constructor
           for any derived classes.  Is protected and allows for the selective
           creation of the search object, interp object and the[sic]

           Updated by Lawrence Gunn.
           2013/01/29


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "SampledData.h"
#include "InterpolationIndex.h"
#include "SequentialAccessHunt.h"
#include "PolynomialInterp.h"
#include <boost/lexical_cast.hpp>

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //***************
  //* SampledData *
  //***************

SampledData::SampledData(int numDependent, int maxNumberOfSamples) :
  m_maxNumberOfSamples(maxNumberOfSamples),
  m_numberOfSamples(0)
{
  m_numberOfDependent = numDependent;

  m_x.resize(m_maxNumberOfSamples);
  m_yArray.resize(m_maxNumberOfSamples);

  m_interpVectorsGood = false;
  m_lastInterpIndex = 0;

  m_indexHunter = boost::shared_ptr<TableSearch>(new SequentialAccessHunt());
  m_interpolator = boost::shared_ptr<Interpolator>(new PolynomialInterp());

  int interpolationSize = m_interpolator->GetInterpolationSize();

  m_independentName = "time";
  m_independentUnits = "seconds";
  m_interpVectorArray.resize(m_numberOfDependent);
  for(int i=0; i<numDependent; i++) {
    m_interpVectorArray[i].resize(interpolationSize);
    m_arrayColumnNames.push_back("data"+lexical_cast<std::string>(i));
    m_arrayColumnUnits.push_back("");
  }
}

SampledData::~SampledData() {
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

void SampledData::Store(double x, const Eigen::VectorXd& dependent) {
  if(m_numberOfSamples >= m_maxNumberOfSamples)  {
    Log(L"ERROR: SampledData::Store: Attempt to store too many m_x/states (%u)",m_numberOfSamples);
    throw std::exception();
  }

  m_x(m_numberOfSamples) = x;
  m_yArray[m_numberOfSamples] = new Eigen::VectorXd(dependent);
  m_numberOfSamples++;
}

double SampledData::operator()(int n) const {
  if(!CheckBounds(n)) {
    Log(L"ERROR: SampledData::operator()(int): Bounds error at %u [0 to %d]", n, m_numberOfSamples-1);
    throw std::exception();
  }

  return m_x(n);
}

double SampledData::operator()(int i, int j) const {
  if(!CheckBounds(i,j)) {
    Log(L"ERROR: SampledData::operator()(int i,int j): State bounds error at (%u,%u) [is (0,0) to (%u,%u)]", i, j, m_numberOfSamples-1, m_numberOfDependent-1);
    throw std::exception();
  }
  return (*m_yArray[i])(j);
}

void SampledData::Retrieve(int index, double& x, Eigen::VectorXd& y) const {
  if(!CheckBounds(index)) {
    Log(L"ERROR: SampledData::Retrieve(i,d,v): Bounds error at %u [0 to %d]", index, m_numberOfSamples-1);
    throw std::exception();
  }

  x = m_x(index);
  y = (*m_yArray[index]);
}

double SampledData::Retrieve(double xTarget, int yIndex) {
  if(!CheckBounds(0, yIndex)) {
    Log(L"ERROR: SampledData::Retrieve(d,i): State bounds error at %u [0 to %d]", yIndex, m_numberOfSamples-1);
    throw std::exception();
  }

  // Only do the search within the block that is from 0 to the current number if of samples stored.
  int indexLow = m_indexHunter->Find(xTarget, m_x.block(0, 0, m_numberOfSamples, 1));
  indexLow = InterpolationIndex::ChooseInterpolationIndex(m_numberOfSamples, indexLow, m_interpolator->GetInterpolationSize());
  SetInterpVectors(indexLow);

  return m_interpolator->Interpolate(xTarget, m_x.block(indexLow, 0, m_interpolator->GetInterpolationSize(), 1), m_interpVectorArray[yIndex]);
}

void SampledData::Retrieve(double xTarget, Eigen::VectorXd& y) {
  int order = m_interpolator->GetInterpolationSize();
  if(m_numberOfDependent != y.rows()) {
    Log(L"ERROR: SampledData::Retrieve(d,v): State bounds error (%u vs %d)", y.rows(), m_numberOfDependent);
    throw std::exception();
  }

  int indexLow = m_indexHunter->Find(xTarget, m_x.block(0, 0, m_numberOfSamples, 1));
  indexLow = InterpolationIndex::ChooseInterpolationIndex(m_numberOfSamples, indexLow, m_interpolator->GetInterpolationSize());
  SetInterpVectors(indexLow);

  for(int i=0;i<m_numberOfDependent;i++) {
    y(i) = m_interpolator->Interpolate(xTarget, m_x.block(indexLow, 0, order, 1),m_interpVectorArray[i]);
  }
}

void SampledData::SetInterpVectors(int newIndex) {
  int order = m_interpolator->GetInterpolationSize();

  if(m_interpVectorsGood && (m_lastInterpIndex == newIndex)) {
    return;
  }

  if(m_numberOfSamples < order) {
    Log(L"ERROR: SampledData::SetInterpVectors(d,v): Number of times stored less than order of interpolation");
    throw std::exception();
  }

  for(int i=0; i<m_numberOfDependent; i++) {
    for(int j=0; j<order; j++) {
      m_interpVectorArray[i](j) = (*m_yArray[newIndex+j])(i);
    }
  }

  m_lastInterpIndex = newIndex;
}

void SampledData::Reset() {
  m_numberOfSamples = 0;
  m_interpVectorsGood = false;
  m_lastInterpIndex = 0;
}

void SampledData::Resize(int newSize) {
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

void SampledData::SetArrayColumnNames(const std::vector<std::string>& names) {
  m_arrayColumnNames.clear();
  for(int i=0; i<names.size(); i++) {
    m_arrayColumnNames.push_back(names[i]);
  }
}

void SampledData::SetArrayColumnUnits(const std::vector<std::string>& units) {
  m_arrayColumnUnits.clear();
  for(int i=0; i<units.size(); i++) {
    m_arrayColumnUnits.push_back(units[i]);
  }
}

int SampledData::Max(int nstate) const {
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

int SampledData::Min(int nstate) const {
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

std::string SampledData::AsJson() {

  std::string json = "{\n \"independent\": { \"name\": \""+m_independentName+"\", \"units\": \""+m_independentUnits+"\", \"values\": [ ";
  char buffer[256];
  for(long i=0; i<m_numberOfSamples; i++) {
    if(i != 0) { json += ","; }
    std::sprintf(buffer, "%10.10e", m_x(i));
    json += buffer;
  }

  json += "]\n},\n \"dependent\": [\n ";

  for(long j=0; j<m_numberOfDependent; j++) {
    if(j != 0) { json += ","; }
    json += "{\n \"name\": \""+m_arrayColumnNames[j]+"\", \"units\": \""+m_arrayColumnUnits[j]+"\", \"values\": [ ";
    for(long i=0; i<m_numberOfSamples; i++) {
      if(i != 0) { json += ","; }
      double value = (*m_yArray[i])(j);
      std::sprintf(buffer, "%10.10e", value);
      json += buffer;
    }
    json += "]\n}\n";
  }


  json += "]\n}\n";

  return json;
}

void SampledData::WriteToLog() {
  wchar_t buffer[256];
  long num = m_yArray[0]->size();

  for(long j=0; j<m_yArray.size() && m_yArray[j]; j++) {

    std::swprintf(buffer, 256, L"x: %5.5e  y:", m_x(j));
    std::wstring output(buffer);

    for(long i=0; i<num; i++) {
      double value = (*m_yArray[j])(i);
      std::swprintf(buffer, 256, L"  %5.5e", value);
      output.append(buffer);
    }
    LogPlain(output.c_str());
  }
}

