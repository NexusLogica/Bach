/**********************************************************************

File     : OdeSolverWithDerivs.h
Project  : Bach Simulation
Purpose  : Header file for the OdeSolver derived class for using supplied derivatives.
Revisions: Original definition by Lawrence Gunn.
           92/12/12.
           This class is the base class for ordinary differential equation solvers
           which require the derivatives of the system equations (i.e. Jacobian and
           time dependent derivatives)

           Updated by Lawrence Gunn.
           2013/01/26


Copyright (c) 1992-2012 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ODE_SOLVER_WITH_DERIVS_H__
#define __BACH_ODE_SOLVER_WITH_DERIVS_H__

#include "OdeSolver.h"

namespace Bach {

  //***********************
  //* OdeSolverWithDerivs *
  //***********************

  class OdeSolverWithDerivs : public OdeSolver {
  public:
    OdeSolverWithDerivs() {}
    virtual ~OdeSolverWithDerivs() {}

    virtual bool DerivativesRequired()  {  return true;  }
    virtual void InitializeDifferentiator(boost::shared_ptr<OdeData> odeData);

    void SetDifferentiator(boost::shared_ptr<OdeDerivatives> differentiator) { m_differentiator = differentiator; }
    boost::shared_ptr<OdeDerivatives> GetDifferentiator() { return m_differentiator; }

  protected:
    boost::shared_ptr<OdeDerivatives> m_differentiator;
  };
};

#endif // __BACH_ODE_SOLVER_WITH_DERIVS_H__
