/**********************************************************************

File     : OdeEquations.h
Project  : Bach Simulation
Purpose  : Header file for the base ODE system of equations class.
Revisions: Original definition by Lawrence Gunn.
           90/05/04.
           Parts of this are based on routines from Numerical Recipes in C, the Harwell libraries and other sources.

           Updated by Lawrence Gunn.
           2013/01/26


Copyright (c) 1990-2012 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ODE_EQUATIONS_H__
#define __BACH_ODE_EQUATIONS_H__

#include "BachDefs.h"

namespace Bach {

  //****************
  //* OdeEquations *
  //****************

  class OdeEquations {
  public:
    OdeEquations() : m_numEquations(0), m_numInternals(0), m_doLog(true) {}
    OdeEquations(OdeEquations const & copy) { }
    
    virtual ~OdeEquations() {}

    virtual void Initialize(boost::shared_ptr<OdeData> odeData)  { }
    virtual void Evaluate(double x, const Eigen::VectorXd& yIn, Eigen::VectorXd& yOut, boost::shared_ptr<OdeData> odeData) = 0;

    int GetStateLength()    const {  return m_numEquations; }
    int GetInternalLength() const {  return m_numInternals; }

    void SetLogging(bool doLog) { m_doLog = doLog; }

  protected:
    void SetStateLength(unsigned int length)    { m_numEquations = length; }
    void SetInternalLength(unsigned int length) { m_numInternals = length; }

    bool m_doLog;

  private:
    int m_numEquations;
    int m_numInternals;
  };
};

#endif // __BACH_ODE_EQUATIONS_H__
