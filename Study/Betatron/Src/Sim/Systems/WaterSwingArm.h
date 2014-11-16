/**********************************************************************

File     : WaterSwingArm.h
Project  : Bach Simulation
Purpose  : Header file for an ordinary differential wrapper class around a water bond rotation molecule model.
Revisions: Original definition by Lawrence Gunn.
           2014/06/23

Notes    : There are 3 equations of state.
             0) Rotational position of the arm in radians.
             1) Rotational velocity of the arm in radians per second.
             2) Linear position along arm of the bond electrons.

Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_WATER_SWING_ARM_H__
#define __BACH_WATER_SWING_ARM_H__

#include "OdeEquations.h"
#include <vector>

namespace Bach {

  //*****************
  //* WaterSwingArm *
  //*****************
  
  class WaterSwingArm : public Bach::OdeEquations {
  public:
  
    static boost::shared_ptr<WaterSwingArm> CreateInstance();

    ~WaterSwingArm();
    
    virtual void Initialize(boost::shared_ptr<Bach::OdeData> odeData);
    virtual void Evaluate(double time, const Eigen::VectorXd& y, Eigen::VectorXd& dydt, boost::shared_ptr<Bach::OdeData> odeData);

  protected:
    WaterSwingArm();
    void InitializeModel();
    
    boost::weak_ptr<WaterSwingArm> m_weakThis;
  };
};

#endif // __BACH_WATER_SWING_ARM_H__
