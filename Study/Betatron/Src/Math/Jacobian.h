/**********************************************************************

File     : Jacobian.h
Project  : Bach Simulation
Purpose  : Header file for a Jacobian approximation object.
Revisions: Original definition by Lawrence Gunn.
           92/11/17.

           Updated by Lawrence Gunn.
           2013/01/31

Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_JACOBIAN_H__
#define __BACH_JACOBIAN_H__

#include "BetatronDefs.h"

namespace Bach {

  //************
  //* Jacobian *
  //************

  class Jacobian {
  public:
    Jacobian() : m_complete(true), m_numberOfColumns(0) {}
    ~Jacobian() {}

    void SetDerivativeFinder(boost::shared_ptr<MultivariateDerivatives> derivativeFinder) { m_derivativeFinder = derivativeFinder; }
    void SetAccuracySpec(boost::shared_ptr<NDimAccuracySpec> accuracySpec) { m_accuracySpec = accuracySpec; }

    void Start(const Eigen::VectorXd& target, const Eigen::VectorXd& step);

    const Eigen::VectorXd& GetNewValue();
    void SetFunctionValues(const Eigen::VectorXd& y);

    const Eigen::MatrixXd& GetDerivatives() { return m_jacobian; } // was GetDerivative
    const Eigen::MatrixXd& GetError()       { return m_errorMatrix; }

    bool GetIsFinished();

  protected:
    boost::shared_ptr<MultivariateDerivatives> m_derivativeFinder;
    boost::shared_ptr<NDimAccuracySpec> m_accuracySpec;
    Eigen::MatrixXd m_jacobian;
    Eigen::MatrixXd m_errorMatrix;
    Eigen::VectorXd m_target;
    Eigen::VectorXd m_stepSize;

    int    m_column;
    int    m_numberOfColumns;
    bool   m_complete;
    double m_tempTarget;
  };
};

#endif // __BACH_JACOBIAN_H__
