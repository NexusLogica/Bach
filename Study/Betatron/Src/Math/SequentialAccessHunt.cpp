/**********************************************************************

File     : SequentialAccessHunt.h
Project  : Bach Simulation
Purpose  : Source file for the sequential access table search class.
Revisions: Original definition by Lawrence Gunn.
           1990/05/08
           The table search classes provide means of finding nearest neighbours
           of a target value in an array of monotonically increasing or decreasing
           values. This is typically for use with interpolation algorithms.

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


#include "SequentialAccessHunt.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

  //************************
  //* SequentialAccessHunt *
  //************************

int SequentialAccessHunt::Find(double xTarget, const Eigen::VectorXd& x) {
  m_size = x.rows();
  m_ascending = (x(m_size-1) > x(0));

  // Due to a poor initial guess try bisection.
  if(m_indexLow <= -1 || m_indexLow > m_size-1)  {
    m_indexLow  = -1;
    m_indexHigh = m_size;
  }
  else {
    if ((xTarget >= x(m_indexLow)) == m_ascending) {
      if(m_indexLow == m_size-1) {
        return m_indexLow;
      }
      HuntUp(xTarget,x);
    }
    else {
      if(m_indexLow == 0)  {
        m_indexLow = -1;
        return m_indexLow;
      }
      HuntDown(xTarget,x);
    }
  }

  Bisection(xTarget,x);
  return m_indexLow;
}

void SequentialAccessHunt::HuntUp(double xTarget, const Eigen::VectorXd& x) {
  int increment = 1;
  m_indexHigh = m_indexLow+1;

  while((xTarget >= x(m_indexHigh)) == m_ascending) {

    // Double the hunting increment.
    m_indexLow = m_indexHigh;
    increment += increment;
    m_indexHigh = m_indexLow+increment;

    // If we are off the high end of the vector, leave.
    if(m_indexHigh > m_size-1) {
      m_indexHigh = m_size;
      break;
    }
  }
}

void SequentialAccessHunt::HuntDown(double xTarget,const Eigen::VectorXd& x) {
  int increment = 1;
  m_indexHigh = m_indexLow;
  m_indexLow -= 1;

  while(xTarget < x(m_indexLow) == m_ascending) {
    // Double the hunting increment.
    m_indexHigh = m_indexLow;
    increment += increment;
    m_indexLow = m_indexHigh-increment;

    // If we are off the low end of the vector, leave.
    if(m_indexLow < 0) {
      m_indexLow = -1;
      break;
    }
  }
}
