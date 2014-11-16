/**********************************************************************

File     : OdeNumericalDerivatives.h
Project  : Bach Simulation
Purpose  : Header file for the base ODE system of equation's derivative's class.
Revisions: Original definition by Lawrence Gunn.
           92/11/25.
           This is a class for returning numerically approximated derivatives of an
           ordinary system of differential equations.

           Updated by Lawrence Gunn.
           2013/01/27


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ODE_NUMERICAL_DERIVATIVES_H__
#define __BACH_ODE_NUMERICAL_DERIVATIVES_H__

#include "OdeDerivatives.h"

namespace Bach {

  //***************************
  //* OdeNumericalDerivatives *
  //***************************

  class OdeNumericalDerivatives : public OdeDerivatives {
  public:
    OdeNumericalDerivatives(int size,double tol = 0.0001);
    OdeNumericalDerivatives(boost::shared_ptr<MultivariateDerivatives> derivatives, boost::shared_ptr<NDimAccuracySpec> accuracySpec);
    virtual ~OdeNumericalDerivatives() {}

    virtual void GetDerivatives(double x,const Eigen::VectorXd& y, Eigen::VectorXd& dfdx, Eigen::MatrixXd& dfdy, boost::shared_ptr<OdeData> odeData);

    void SetStepSize(Eigen::VectorXd& ss) {  m_stepSize = ss;  }
    void SetXStepSize(double ss)    {  m_xStepSize = ss;  }
    void SetTolerance(double tol);

    boost::shared_ptr<MultivariateDerivatives> GetMVDerivative() { return m_derivatives; }
    boost::shared_ptr<NDimAccuracySpec> GetAccuracySpec() { return m_accuracySpec; }

  protected:
    virtual void GetTimeDeriv(double x, const Eigen::VectorXd& y, boost::shared_ptr<OdeData> odeData);
    virtual void GetJacobian(double x, const Eigen::VectorXd& y, boost::shared_ptr<OdeData> odeData);

    boost::shared_ptr<Jacobian> m_jacobian;
    boost::shared_ptr<MultivariateDerivatives> m_derivatives;
    boost::shared_ptr<NDimAccuracySpec> m_accuracySpec;

    double m_xStepSize;
    Eigen::VectorXd m_stepSize;
    Eigen::VectorXd m_tempValues;
  };
};

#endif // __BACH_ODE_NUMERICAL_DERIVATIVES_H__
