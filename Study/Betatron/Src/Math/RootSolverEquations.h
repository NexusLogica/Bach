/**********************************************************************

File     : RootSolverEquations.h
Project  : Bach Simulation
Purpose  : Header file for the base root solver equations class.
Revisions: Original definition by Lawrence Gunn.
           90/05/04.
           Parts of this are based on routines from Numerical Recipes in C, the Harwell libraries and other sources.

           Updated by Lawrence Gunn.
           2013/02/09


Copyright (c) 1990-2012 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ROOT_SOLVER_EQUATIONS_H__
#define __BACH_ROOT_SOLVER_EQUATIONS_H__

#include "BetatronDefs.h"

namespace Bach {

  //***********************
  //* RootSolverEquations *
  //***********************

  class RootSolverEquations {
  public:
    RootSolverEquations() : m_numEquations(0), m_numInternals(0) {}
    virtual ~RootSolverEquations() {}

    virtual void Initialize() { }

    virtual Eigen::VectorXd GetEquationValues(  // Output: The values of the functions at 'y'
      const Eigen::VectorXd& y) = 0;            // Input:  The independent variable

    virtual Eigen::MatrixXd GetJacobian(        // Output: The derivatives of the functions with respect to 'y' about the input 'y' value
      const Eigen::VectorXd& y) = 0;            // Input:  The independent variable

    int GetStateLength()    const {  return m_numEquations; }
    int GetInternalLength() const {  return m_numInternals; }

    virtual Eigen::VectorXd GetInternals() const { return m_internals; }

  protected:
    void SetStateLength(unsigned int length)    { m_numEquations = length; }
    void SetInternalLength(unsigned int length) { m_numInternals = length; m_internals.resize(m_numInternals); }
    void SetInternals(const Eigen::VectorXd& internals) { m_internals = internals; }

  private:
    int m_numEquations;
    int m_numInternals;
    int m_error;

    Eigen::VectorXd m_internals;
  };
};

#endif // __BACH_ROOT_SOLVER_EQUATIONS_H__
