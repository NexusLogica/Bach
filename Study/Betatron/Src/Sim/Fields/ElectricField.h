/**********************************************************************

File     : ElectricField.h
Project  : Bach Simulation
Purpose  : Header file for an electric field produced by a charged particle.
Revisions: Original definition by Lawrence Gunn.
           2013/03/08


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ELECTRIC_FIELD_H__
#define __BACH_ELECTRIC_FIELD_H__

#include "BachDefs.h"

namespace Bach {

  //*****************
  //* ElectricField *
  //*****************

  class ElectricField {
  public:
    ElectricField() {}
    ~ElectricField() {}

    // All of these are in global coordinates. These values are maintained between calls to GetFieldVectors.
    void SetCharge(double charge) { m_charge = charge; }
    void SetPosition(const Eigen::Vector3d& pos) { m_position = pos; }
    void SetVelocity(const Eigen::Vector3d& vel) { m_velocity = vel; }

    void GetFieldVectors(
      double charge, // 1.0 for one proton, -2.0 for two electrons, and so on.
      const Eigen::Vector3d& pos,
      const Eigen::Vector3d& vel,
      Eigen::Vector3d& E,
      Eigen::Vector3d& dEdt);

  protected:
    Eigen::Vector3d m_position;
    Eigen::Vector3d m_velocity;
    double m_charge; // This is usually an integer value, and +ve for protons, -ve for electrons. For one electron m_charge is -1.0.
  };
};

#endif // __BACH_ELECTRIC_FIELD_H__
