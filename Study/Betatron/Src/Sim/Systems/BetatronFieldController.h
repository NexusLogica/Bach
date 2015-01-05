/**********************************************************************

File     : BetatronFieldController.h
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

  class PointMagneticField {
  public:
    void SetB(double newB) { m_B = newB; }
    void SetdBdt(double newdBdt) { m_dBdt = newdBdt; }
    void SetDirection(const Eigen::Vector3d& unitVectorB) { m_unitVectorB = unitVectorB; }
    
    const double B() { return m_B; }
    const double dBdt() { return m_dBdt; }
    const Eigen::Vector3d& UnitVectorB() { return m_unitVectorB; }

  private:
    double m_B;
    double m_dBdt;
    Eigen::Vector3d m_unitVectorB;
  };

  //***************************
  //* BetatronFieldController *
  //***************************
  
  class BetatronFieldController {
  public:
  
    static boost::shared_ptr<BetatronFieldController> CreateInstance();

    virtual ~BetatronFieldController();
    
    double GetConstantB() { return m_constantB; }
    virtual void SetAsConstantB(double B, const Eigen::Vector3d& direction);
    virtual void SetFractionalIncreaseBPerSecond(double fractionalIncreaseBPerSecond) { m_fractionalIncreaseBPerSecond = fractionalIncreaseBPerSecond; }
    virtual void GetField(
                          double t,
                          const Eigen::Vector3d& positionInField,
                          boost::shared_ptr<PointMagneticField>& field);
    
  protected:
    BetatronFieldController();
    
    boost::weak_ptr<BetatronFieldController> m_weakThis;
    enum ControlType {
      ConstantB = 0,
    } m_controlType;

    double m_constantB;
    Eigen::Vector3d m_directionOfConstantB;
    double m_fractionalIncreaseBPerSecond;
  };
};

#endif // __BACH_BETATRON_FIELD_CONTROLLER_H__
