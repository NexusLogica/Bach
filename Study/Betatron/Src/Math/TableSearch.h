/**********************************************************************

File     : TableSearch.h
Project  : Bach Simulation
Purpose  : Header file for the base table search class.
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

#ifndef __BACH_TABLE_SEARCH_H__
#define __BACH_TABLE_SEARCH_H__

#include "BachDefs.h"

namespace Bach {

  //***************
  //* TableSearch *
  //***************

  class TableSearch {
  public:
    TableSearch(int i = 0) : m_indexLow(i) {}
    virtual ~TableSearch() {}

    virtual int Find(double x, const Eigen::VectorXd& y) = 0;

    int GetIndexLow() const { return m_indexLow; }
    void SetIndexLow(int i = 0) { m_indexLow = i; }

  protected:
    int  m_indexLow;
    int  m_indexHigh;
    bool m_ascending;
    int  m_size;

    void Bisection(double x, const Eigen::VectorXd& y);
  };
};

#endif // __BACH_TABLE_SEARCH_H__
