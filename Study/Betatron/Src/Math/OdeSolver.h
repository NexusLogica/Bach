/**********************************************************************

File     : OdeSolver.h
Project  : Bach Simulation
Purpose  : Header file for the base ODE solver.
Revisions: Original definition by Lawrence Gunn.
           90/05/04.
           Parts of this are based on routines from Numerical Recipes in C, the Harwell libraries and other sources.

           Updated by Lawrence Gunn.
           2013/01/26


Copyright (c) 1990-2012 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ODE_SOLVER_H__
#define __BACH_ODE_SOLVER_H__

#include "BachDefs.h"

namespace Bach {

  //*************
  //* OdeSolver *
  //*************

  class OdeSolver {
  public:
    OdeSolver() : m_stepSize(0.0) {}
    virtual ~OdeSolver() {}

    virtual void Solve(boost::shared_ptr<OdeData> data, bool reset = true) = 0; // reset, meaning clear existing data, or just continue
    virtual void SetStepSize(double stepSize) { m_stepSize = stepSize; }
    virtual bool DerivativesRequired() { return false; }
    virtual bool AccuracySpecRequired() { return true; }

    void SetAccuracySpec(boost::shared_ptr<OdeAccuracySpec> accuracySpec) { m_accuracySpec = accuracySpec; }
    boost::shared_ptr<OdeAccuracySpec> GetAccuracySpec() { return m_accuracySpec; }

  protected:
    boost::shared_ptr<OdeAccuracySpec> m_accuracySpec;

    void InitializeAccuracySpec(boost::shared_ptr<OdeData> odeData);

    double m_stepSize;
  };
}

#endif // __BACH_ODE_SOLVER_H__
