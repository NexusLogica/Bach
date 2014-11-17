/**********************************************************************

File     : Interpolator.h
Project  : Bach Simulation
Purpose  : Header file for the base Interpolator class.
Revisions: Original definition by Lawrence Gunn.
           1990/05/08
           The interpolation index classes provide means of choosing an interpolation
           index based upon an index in a table.

           Revised for RW library and better readability.
           1992/07/29.

           Table search functions and interpolation index class separated into
           two different classes.
           1992/09/08.

           Updated by Lawrence Gunn.
           2013/01/29

Copyright (c) 1990-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_INTERPOLATOR_H__
#define __BACH_INTERPOLATOR_H__

#include "BetatronDefs.h"
#include <vector>

namespace Bach {

  //****************
  //* Interpolator *
  //****************

  class Interpolator {
  public:
    Interpolator() : m_checkExtrapFlag(false) {}
    virtual ~Interpolator() {}

    virtual double Interpolate(double xTarget, const Eigen::VectorXd& x,const Eigen::VectorXd& y, int index = 0) = 0;

    void ArrayInterpolation(double xTarget, const Eigen::VectorXd& x, std::vector<Eigen::VectorXd>& y, Eigen::VectorXd& results, int baseIndex = 0) {
      int numY = (int) y.size();
      if(m_errorEstimateVector.rows() != numY) {
        m_errorEstimateVector.resize(numY);
      }

      for(int i=0;i<numY;i++) {
        results(i) = Interpolate(xTarget, x.block(baseIndex, 0, m_interpSize, 1), y[i].block(baseIndex, 0, m_interpSize, 1), 0);
        m_errorEstimateVector(i) = m_errorEstimateSingle;
      }
    }

    int GetInterpolationSize() const { return m_interpSize; }
    virtual void SetInterpolationSize(int n) { m_interpSize = n; }

    double GetErrorEstimate() const {  return m_errorEstimateSingle;  }
    void GetErrorEstimate(Eigen::VectorXd& e) const {
      e = m_errorEstimateVector;
    }

    virtual void CheckForExtrapolation(double xTarget, const Eigen::VectorXd& x) {
      bool ascending = x(0) < x(m_interpSize-1);

      if((ascending && (x(m_interpSize-1) < xTarget && xTarget < x(0))) || (!ascending && (x(0) < xTarget && xTarget <= x(m_interpSize-1)))) {
        Bach::Log(L"WARNING: Interpolator::CheckForExtrapolation: Attempted extrapolation to %f (range %f to %f)", xTarget, x(0), x(m_interpSize-1));
      }
    }
    void SetCheckExtrapolationFlag(bool f) { m_checkExtrapFlag = f; }

  protected:
    int m_interpSize;
    double m_errorEstimateSingle;
    Eigen::VectorXd m_errorEstimateVector;
    bool m_checkExtrapFlag;
  };
};

#endif // __BACH_INTERPOLATOR_H__
