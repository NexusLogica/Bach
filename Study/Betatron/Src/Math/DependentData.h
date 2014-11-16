/**********************************************************************

File     : DependentData.h
Project  : Bach Simulation
Purpose  : Header file for the data that depends on a variable.
Revisions: Original definition by Lawrence Gunn.
           92/08/16.
           Created as base class for independent/dependent data.

           Updated by Lawrence Gunn.
           2013/01/29


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_DEPENDENT_DATA_H__
#define __BACH_DEPENDENT_DATA_H__

#include "BetatronDefs.h"

namespace Bach {

  //*****************
  //* DependentData *
  //*****************

  class DependentData {
  public:
    virtual double Retrieve(double xTarget, int yIndex) = 0;
    virtual void   Retrieve(double xTarget, Eigen::VectorXd& yOut) = 0;

    int GetDependentLength() const { return m_numberOfDependent; }

  protected:
    int m_numberOfDependent;
  };
};

#endif // __BACH_DEPENDENT_DATA_H__
