/**********************************************************************

File     : BisectionHunt.h
Project  : Bach Simulation
Purpose  : Header file for the bisection based table search class.
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

#ifndef __BACH_BISECTION_HUNT_H__
#define __BACH_BISECTION_HUNT_H__

#include "TableSearch.h"

namespace Bach {

  //*****************
  //* BisectionHunt *
  //*****************

  class BisectionHunt : public TableSearch {
  public:
    BisectionHunt(int i = 0) : TableSearch(i) {}

    virtual int Find(double x, const Eigen::VectorXd& y);

  protected:
    void HuntUp(double x,const Eigen::VectorXd& y);
    void huntDown(double x,const Eigen::VectorXd& y);
  };
};

#endif // __BACH_BISECTION_HUNT_H__
