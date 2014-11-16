/**********************************************************************

File     : OdeAccuracySpec.h
Project  : Bach Simulation
Purpose  : Header file for the base ODE accuracy specification class.
Revisions: Original definition by Lawrence Gunn.
           92/10/25.
           This is an extention of the accuracy specification technique in Harwell's DC01AD.

           Updated by Lawrence Gunn.
           2013/01/27


Copyright (c) 1992-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ODE_ACCURACY_SPEC_H__
#define __BACH_ODE_ACCURACY_SPEC_H__

#include "NDimAccuracySpec.h"

namespace Bach {

  //*******************
  //* OdeAccuracySpec *
  //*******************

  class OdeAccuracySpec : public NDimAccuracySpec {
  public:
    OdeAccuracySpec(unsigned int size, double tolerance = 0.0001);
    virtual ~OdeAccuracySpec() {}

    virtual double GetNormalizedError(const Eigen::VectorXd& error, const Eigen::VectorXd& y);
    virtual double GetOdeNormalizedError(const Eigen::VectorXd& error, const Eigen::VectorXd& y, boost::shared_ptr<OdeSolver> solver, boost::shared_ptr<OdeData> odeData);

    void SetRelativeErrorForMaximums(double e)                 { m_relativeErrorForMaximums.fill(fabs(e)); }
    void SetRelativeErrorForMaximums(const Eigen::VectorXd& e) { m_relativeErrorForMaximums = e.array().abs(); }
    void SetRelativeErrorOffsets(double e)                     { m_relativeErrorOffsets.fill(e); }
    void SetRelativeErrorOffsets(const Eigen::VectorXd& e)     { m_relativeErrorOffsets = e; }

    void SetMaximumValues(double e)                            { m_maximumValues.fill(fabs(e)); }
    void SetMaximumValues(const Eigen::VectorXd& e)            { m_maximumValues = e.array().abs(); }
    virtual void CheckForMaximumValues(const Eigen::VectorXd&);

    Eigen::VectorXd GetRelativeErrorForMaximums()              { return m_relativeErrorForMaximums; }
    Eigen::VectorXd GetRelativeErrorOffsets()                  { return m_relativeErrorOffsets; }
    Eigen::VectorXd GetMaximumValues()                         { return m_maximumValues; }

  protected:
    Eigen::VectorXd m_relativeErrorForMaximums;
    Eigen::VectorXd m_relativeErrorOffsets;
    Eigen::VectorXd m_maximumValues;

  };
};

#endif // __BACH_ODE_ACCURACY_SPEC_H__
