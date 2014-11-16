/**********************************************************************

File     : RiddersExtrapolation.h
Project  : Bach Simulation
Purpose  : Header file for multivariate derivatives using Ridder's extrapolation.
Revisions: Original definition by Lawrence Gunn.
           92/11/17.

           Updated by Lawrence Gunn.
           2013/01/28


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_RIDDERS_EXTRAPOLATION_H__
#define __BACH_RIDDERS_EXTRAPOLATION_H__

#include "MultivariateDerivatives.h"
#include <vector>

namespace Bach {

  //************************
  //* RiddersExtrapolation *
  //************************

  class RiddersExtrapolation : public MultivariateDerivatives {
  public:
    RiddersExtrapolation(int size);
    virtual ~RiddersExtrapolation();

    virtual void Start(double target,double step);
    virtual double GetNewValue();
    virtual void SetFunctionValues(const Eigen::VectorXd& values);

    virtual bool GetIsFinished(boost::shared_ptr<NDimAccuracySpec> spec);
    bool AccuracySpecTest(boost::shared_ptr<NDimAccuracySpec> spec);

  protected:
    std::vector<Eigen::MatrixXd> m_tables;
    Eigen::VectorXd m_firstHalfVector;
    Eigen::VectorXi m_isFinished;

    int    m_tableSize;
    int    m_numberOfTables;
    double m_xTarget;
    double m_initialStepSize;
    double m_stepSize;
    double m_decreaseFactor;
    double m_decreaseFactorSquared;
    bool   m_firstHalf;
    int    m_iteration;
    int    m_leftToFinish;
  };
  };

#endif // __BACH_RIDDERS_EXTRAPOLATION_H__
