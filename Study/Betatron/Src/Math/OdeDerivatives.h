/**********************************************************************

File     : OdeDerivatives.h
Project  : Bach Simulation
Purpose  : Header file for the base ODE system of equation's derivative's class.
Revisions: Original definition by Lawrence Gunn.
           92/11/25.
           This is a base class for returning derivatives of the derivitive
           functions for an ordinary system of differential equations.

           Updated by Lawrence Gunn.
           2013/01/27


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ODE_DERIVATIVES_H__
#define __BACH_ODE_DERIVATIVES_H__

#include "BachDefs.h"

namespace Bach {

  //******************
  //* OdeDerivatives *
  //******************

  class OdeDerivatives {
  public:
    OdeDerivatives() {  }
    virtual ~OdeDerivatives() {  }

    virtual void GetDerivatives(double x, const Eigen::VectorXd& y, Eigen::VectorXd& dfdx, Eigen::MatrixXd& dfdy, boost::shared_ptr<OdeData> odeData) = 0;
  };
};

#endif // __BACH_ODE_DERIVATIVES_H__
