/**********************************************************************

File     : SampledDerivedData.h
Project  : Bach Simulation
Purpose  : Header file for the sampled derived data that depends on a variable.
Revisions: Original definition by Lawrence Gunn.
           1990/05/08

           Minor revisions.
           1990/10/18.

           Major revisions and updating.  Base class of NXDependentData
           created and data class changed to NXSampledData.  Interface
           is simplified.
           1992/08/02.

           Updated by Lawrence Gunn.
           2013/01/29

Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_SAMPLED_DERIVED_DATA_H__
#define __BACH_SAMPLED_DERIVED_DATA_H__

#include "DependentData.h"
#include "InterpolationIndex.h"
#include <vector>

namespace Bach {

  //**********************
  //* SampledDerivedData *
  //**********************

  class SampledDerivedData : public DependentData {
  public:
    SampledDerivedData(int numDependent, int maxNumberOfSamples);
     ~SampledDerivedData();

    void Store(double x, const Eigen::VectorXd& y, const Eigen::VectorXd& dy);

    // Retrieve individual values.
    double operator()(int) const;
    double operator()(int ,int) const;
    double Retrieve(double xTarget, int yIndex);

    // Inherited from NXDependentData
    void Retrieve(int index, double& x, Eigen::VectorXd& y) const;
    void Retrieve(double xTarget, Eigen::VectorXd& y);

      // Retrieve values and derivatives.
    void Retrieve(int index, double&, Eigen::VectorXd& y, Eigen::VectorXd& dy) const;

    void Reset();     // set all of the data to zero
    void Resize(int);

    // return information about the object
    int GetNumberOfSamples() const    {  return m_numberOfSamples;    }; // was GetLength()
    int GetMaxNumberOfSamples() const {  return m_maxNumberOfSamples; }; // was GetMaxLength()

    void SetIndependentName(const std::string& name) { m_independentName = name; }
    void SetIndependentUnit(const std::string& unit) { m_independentUnits = unit; }
    void SetArrayColumnNames(const std::vector<std::string>& names);
    void SetArrayColumnUnits(const std::vector<std::string>& units);

    // the maximums and minimums of the data
    int Max(int yIndex) const;
    int Max() const;
    int Min(int yIndex) const;
    int Min() const;

    std::string AsJson();
    void WriteToLog();

  protected:
    Eigen::VectorXd m_x;
    std::vector<Eigen::VectorXd*> m_yArray;

    std::string m_independentName;
    std::string m_independentUnits;
    std::vector<std::string> m_arrayColumnNames;
    std::vector<std::string> m_arrayColumnUnits;

    int m_numberOfSamples;
    int m_maxNumberOfSamples;
    boost::shared_ptr<TableSearch> m_indexHunter;

    bool CheckBounds(int i) const {
      return (i < m_numberOfSamples ? true : false);
    }

    bool CheckBounds(int i,int j) const {
      return ((i < m_numberOfSamples && j < 2*m_numberOfDependent) ? true : false);
    }
  };
};

#endif // __BACH_SAMPLED_DERIVED_DATA_H__
