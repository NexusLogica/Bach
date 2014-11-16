/**********************************************************************

File     : RootSolverEquationsOdeWrapper.h
Project  : Bach Simulation
Purpose  : Header file for the wrapper of the base root solver equations for ODE systems.
Revisions: Original definition by Lawrence Gunn.
           90/05/04.
           Parts of this are based on routines from Numerical Recipes in C, the Harwell libraries and other sources.

           Updated by Lawrence Gunn.
           2013/02/13

Copyright (c) 1990-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ROOT_SOLVER_ODE_WRAPPER_EQUATIONS_H__
#define __BACH_ROOT_SOLVER_ODE_WRAPPER_EQUATIONS_H__

#include "RootSolverEquations.h"

namespace Bach {

  //*********************************
  //* RootSolverEquationsOdeWrapper *
  //*********************************

  class RootSolverEquationsOdeWrapper : public RootSolverEquations {
  public:
    static boost::shared_ptr<RootSolverEquationsOdeWrapper> CreateInstance(boost::shared_ptr<OdeData> odeData, double stepSize);

    virtual ~RootSolverEquationsOdeWrapper() {}

    void SetJacobianAccuracySpec(boost::shared_ptr<NDimAccuracySpec> accuracySpec);

    void SetXForEvalutations(double x) { m_x = x; }

    // **** RootSolverEquations ***

    virtual void Initialize();
    
    virtual Eigen::VectorXd GetEquationValues(  // Output: The values of the functions at 'y'
      const Eigen::VectorXd& y);                // Input:  The independent variable

    virtual Eigen::MatrixXd GetJacobian(        // Output: The derivatives of the functions with respect to 'y' about the input 'y' value
      const Eigen::VectorXd& y);                // Input:  The independent variable

  private:
    RootSolverEquationsOdeWrapper(boost::shared_ptr<OdeData> odeData, double stepSize);
    double m_x;
    double m_stepSize;
    Eigen::VectorXd m_stepSizeVector;
    Eigen::VectorXd m_tempValues;
    boost::shared_ptr<OdeData> m_odeData;
    boost::shared_ptr<Jacobian> m_jacobianFinder;
  };
};

#endif // __BACH_ROOT_SOLVER_ODE_WRAPPER_EQUATIONS_H__
