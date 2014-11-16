/**********************************************************************

File     : MagneticFieldDerivTests.h
Project  : Bach Simulation
Purpose  : Header file for a molecule equilibrium solver - solve for x where v,a are zero.
Revisions: Original definition by Lawrence Gunn.
           2013/11/07


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_MAGNETIC_FIELD_DERIV_TESTS_H__
#define __BACH_MAGNETIC_FIELD_DERIV_TESTS_H__

#include "RootSolverEquations.h"
#include "Element.h"
#include "RandomUniform.h"
#include "ChargeMagneticField.h"
#include <vector>

namespace Bach {

  using namespace PS;

  //**********************************
  //* MagneticFieldDerivTestVariable *
  //**********************************

  class MagneticFieldDerivTestVariable {
  public:
    MagneticFieldDerivTestVariable(
      const std::wstring& id,
      Integer index,
      Real minValue,
      Real maxValue,
      Real minDerivativeValue,
      Real maxDerivativeValue,
      Real derivStep)
      : m_id(id), m_index(index), m_minValue(minValue), m_maxValue(maxValue), m_minDerivativeValue(minDerivativeValue), m_maxDerivativeValue(maxDerivativeValue), m_derivStep(derivStep) {}
    
    Real GetNewValue(boost::shared_ptr<RandomUniform> randUniform) {
      return randUniform->GetRandomDoubleNum(m_minValue, m_maxValue);
    }
    
    Real GetNewDerivativeValue(boost::shared_ptr<RandomUniform> randUniform) {
      return randUniform->GetRandomDoubleNum(m_minDerivativeValue, m_maxDerivativeValue);
    }
    
    std::wstring GetId() { return m_id; }
    Integer GetIndex() { return m_index; }
    Real GetDerivStep() { return m_derivStep; }
  private:
    std::wstring m_id;
    Integer m_index;
    Real m_minValue;
    Real m_maxValue;
    Real m_minDerivativeValue;
    Real m_maxDerivativeValue;
    Real m_derivStep;
  };
  
  //***************************
  //* MagneticFieldDerivTests *
  //***************************
  
  class MagneticFieldDerivTests {
  public:
  
    static boost::shared_ptr<MagneticFieldDerivTests> CreateInstance();

    ~MagneticFieldDerivTests();
    
    void SetNumIterations(Integer numIterations) { m_numIterations = numIterations; }
    
    bool RunTests();

  protected:
    MagneticFieldDerivTests();
    
    void Initialize();
    
    Eigen::VectorXd GetExactDerivative(const Eigen::VectorXd& values, const Eigen::VectorXd& derivatives);
    Eigen::VectorXd GetExactPartialDerivative(Integer index, const Eigen::VectorXd& values);

    Eigen::VectorXd GetApproximatedDerivative(const Eigen::VectorXd& values, const Eigen::VectorXd& derivatives);
    Eigen::Vector3d GetFunctionValue(const Eigen::VectorXd& values);
    
    boost::shared_ptr<RandomUniform> m_randUniform;
    std::vector< boost::shared_ptr<MagneticFieldDerivTestVariable> > m_variables;
    Integer m_numIterations;
    Real m_toleranceScaleFactor;
    Real m_tolerance;
    bool m_success;
    
    boost::shared_ptr<ChargeMagneticField> m_chargeMagneticField;
    bool m_logAll;
  };
};

#endif // __BACH_MAGNETIC_FIELD_DERIV_TESTS_H__
