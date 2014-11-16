/**********************************************************************

File     : Circuit.h
Project  : Bach Simulation
Purpose  : Header file for the ordinary differential equations of a charge in a betatron.
Revisions: Original definition by Lawrence Gunn.
           2014/10/04

Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_BETATRON_FIELD_CONTROLLER_H__
#define __BACH_BETATRON_FIELD_CONTROLLER_H__

#include "OdeEquations.h"
#include <vector>

namespace Bach {

  //***********
  //* Circuit *
  //***********
  
  class Circuit {
  public:
  
    static boost::shared_ptr<Circuit> CreateInstance();

    virtual ~Circuit();
    
    Eigen::Vector3d GetField(const Eigen::Vector3d& point);

    Eigen::Vector3d GetFieldFromPoint(
      const Eigen::Vector3d& point,
      const Eigen::Vector3d& segmentCenter,
      const Eigen::Vector3d& flowDirection,
      double segmentCoeff);
  
  protected:
    Circuit();
    
    boost::weak_ptr<Circuit> m_weakThis;
    double m_radius;   //
    double m_current;  // amps
    int    m_segments;
  };
};

#endif // __BACH_BETATRON_FIELD_CONTROLLER_H__
