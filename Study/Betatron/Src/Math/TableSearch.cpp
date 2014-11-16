/**********************************************************************

File     : TableSearch.cpp
Project  : Bach Simulation
Purpose  : Source file for the base table search class.
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

#include "TableSearch.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

  //***************
  //* TableSearch *
  //***************

void TableSearch::Bisection(double xTarget,const Eigen::VectorXd& x) {
  while(m_indexHigh-m_indexLow != 1) {
    int indexMiddle = (m_indexHigh+m_indexLow) >> 1;

    if((xTarget > x(indexMiddle)) == m_ascending) {
      m_indexLow = indexMiddle;
    }
    else {
      m_indexHigh = indexMiddle;
    }
  }
}
