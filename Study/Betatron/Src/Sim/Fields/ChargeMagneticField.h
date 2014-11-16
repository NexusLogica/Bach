/**********************************************************************

File     : ChargeMagneticField.h
Project  : Bach Simulation
Purpose  : Header file for magnetic field produced by charged particles.
Revisions: Original definition by Lawrence Gunn.
           2013/02/20


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_CHARGE_MAGNETIC_FIELD_H__
#define __BACH_CHARGE_MAGNETIC_FIELD_H__

#include "BetatronDefs.h"

namespace Bach {

  //***********************
  //* ChargeMagneticField *
  //***********************

  class ChargeMagneticField {
  public:
    static boost::shared_ptr<ChargeMagneticField> CreateInstance(double charge); // 1.0 for one proton, -2.0 for two electrons, and so on.
    ~ChargeMagneticField() {}

    // All of these are in global coordinates. These values are maintained between calls to GetFieldVectors.
    void SetPosition(const Eigen::Vector3d& pos)       { m_position     = pos;   }
    void SetVelocity(const Eigen::Vector3d& vel)       { m_velocity     = vel;   }
    void SetAcceleration(const Eigen::Vector3d& accel) { m_acceleration = accel; }

    // Calculate the magnetic field vector, B.
    Eigen::Vector3d GetMagneticFieldVector(
      const Eigen::Vector3d& pos);             // position of other charge

    // Calculate the direction of the magnetic field vector, ^B. This returns a unit vector.
    //
    // Note that if the velocity is zero length but there is a non-zero acceleration then the
    // acceleration direction is used for the calculation. If both are zero length then
    // isValid is set to false.
    //
    Eigen::Vector3d GetMagneticFieldDirectionVector(
      const Eigen::Vector3d& pos,              // position of other charge
      bool& isValid);                          // true if either velocity or acceleration vectors were non-zero, false if zero length.

    // Calculate the change of magnetic field, dBdt.
    Eigen::Vector3d GetMagneticFieldChangeVector(
      const Eigen::Vector3d& pos,              // position of other charge
      const Eigen::Vector3d& vel);

  protected:
    ChargeMagneticField(double charge);        // 1.0 for one proton, -2.0 for two electrons, and so on.

    Eigen::Vector3d m_position;
    Eigen::Vector3d m_velocity;
    Eigen::Vector3d m_acceleration;
    double m_charge;
  };
};

#endif // __BACH_CHARGE_MAGNETIC_FIELD_H__
