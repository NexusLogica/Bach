/**********************************************************************

File     : ForceVectors.h
Project  : Bach Simulation
Purpose  : Header file for an ForceVectors.
Revisions: Original definition by Lawrence Gunn.
           2013/06/21


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_FORCE_VECTORS_H__
#define __BACH_FORCE_VECTORS_H__

#include "Element.h"
#include <vector>

namespace Bach {

  using namespace PS;
  
  //****************
  //* ForceVectors *
  //****************
  
  class ForceVectors {
  public:

    static boost::shared_ptr<ForceVectors> CreateInstance();
    ~ForceVectors() {}
    
    void SetTargetName(const std::wstring& name) { m_targetName = name; }
    void SetTargetTravelConstraint(const Eigen::Vector3d& vec) { m_targetTravelConstraint = vec; m_targetTravelContraintSet = true; }
    void AddForceVector(const std::wstring& name, const Eigen::Vector3d& force);

    std::wstring GetTargetName() { return m_targetName; }
    Eigen::Vector3d GetTargetTravelConstraint() { return m_targetTravelConstraint; }
    
    Integer GetNum() { return m_forceVectors.size(); }
    std::wstring GetForceName(Integer index);
    Eigen::Vector3d GetForceVector(Integer index);
    
    void WriteToLog();

  protected:
    ForceVectors() : m_targetTravelContraintSet(false) {}

    std::wstring m_targetName;
    Eigen::Vector3d m_targetTravelConstraint;
    bool m_targetTravelContraintSet;
    std::vector< std::wstring > m_forceNames;
    std::vector< Eigen::Vector3d > m_forceVectors;
  };

  //********************
  //* ForceVectorsList *
  //********************
  
  class ForceVectorsList {
  public:

    static boost::shared_ptr<ForceVectorsList> CreateInstance();
    ~ForceVectorsList() {}
    
    void AddForceVector(boost::shared_ptr<ForceVectors> vectors);

    Integer GetNum() { return m_forceVectorList.size(); }
    boost::shared_ptr<ForceVectors> GetByIndex(Integer index);

    void WriteToLog();

  protected:
    ForceVectorsList() {}

    std::vector< boost::shared_ptr<ForceVectors> > m_forceVectorList;
  };
};

#endif // __BACH_FORCE_VECTORS_H__
