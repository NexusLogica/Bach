/**********************************************************************

File     : InterpolationIndex.h
Project  : Bach Simulation
Purpose  : Header file for an interpolation index selection class.
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

#ifndef __BACH_INTERPOLATION_INDEX_H__
#define __BACH_INTERPOLATION_INDEX_H__

#include "BetatronDefs.h"

namespace Bach {

  //**********************
  //* InterpolationIndex *
  //**********************

  class InterpolationIndex {
  public:
    static int ChooseInterpolationIndex(const Eigen::VectorXd& x, int indexLow, int interpSize) {
      return ChooseInterpolationIndex(x.rows(), indexLow, interpSize);
    }

    static int ChooseInterpolationIndex(int length,int indexLow, int interpSize) {
      int i = indexLow-interpSize/2+1;
      int j = length-interpSize;
      i = (i > 0 ? i : 0);
      i = (i < j ? i : j);
      return i;
    }
  };
};

#endif // __BACH_INTERPOLATION_INDEX_H__
