/**********************************************************************

File     : BetatronEquations.h
Project  : Bach Simulation
Purpose  : Header file for the ordinary differential equations of a charge in a betatron.
Revisions: Original definition by Lawrence Gunn.
           2014/10/04


Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_BETATRON_EQUATIONS_H__
#define __BACH_BETATRON_EQUATIONS_H__

#include "OdeEquations.h"
#include "BetatronFieldController.h"
#include <vector>

namespace Bach {

  //*********************
  //* BetatronEquations *
  //*********************
  
  class BetatronEquations : public Bach::OdeEquations {
  public:
  
    static boost::shared_ptr<BetatronEquations> CreateInstance();

    virtual ~BetatronEquations();
    
    double GetCharge() { return m_charge; }
    double GetMass()   { return m_mass; }
    
    virtual void SetFieldController(boost::shared_ptr<BetatronFieldController> fieldController) { m_fieldController = fieldController; }
    virtual void SetCharge(double q) { m_charge = q; }
    virtual void SetMass(double m) { m_mass = m; }

    virtual void Initialize(boost::shared_ptr<Bach::OdeData> odeData);
    virtual void Evaluate(double time, const Eigen::VectorXd& y, Eigen::VectorXd& dydt, boost::shared_ptr<Bach::OdeData> odeData);

  protected:
    BetatronEquations();
    
    boost::weak_ptr<BetatronEquations> m_weakThis;

    double m_charge; // Number of electrons or protons, -ve for electrons.
    double m_mass;   // kg's
    boost::shared_ptr<BetatronFieldController> m_fieldController;
    boost::shared_ptr<PointMagneticField> m_magneticField;
    Eigen::Vector3d m_position;
    Eigen::Vector3d m_velocity;
    Eigen::Vector3d m_force;
    Eigen::VectorXd m_internalValues;
    int m_iterationCount;
  };
};

#endif // __BACH_BETATRON_EQUATIONS_H__
