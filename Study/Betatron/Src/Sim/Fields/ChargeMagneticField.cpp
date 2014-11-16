/**********************************************************************

File     : ChargeMagneticField.cpp
Project  : Bach Simulation
Purpose  : Source file for magnetic field produced by charged particles.
Revisions: Original definition by Lawrence Gunn.
           2013/02/20


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "ChargeMagneticField.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

  //***********************
  //* ChargeMagneticField *
  //***********************

shared_ptr<ChargeMagneticField> ChargeMagneticField::CreateInstance(Real charge) {
  shared_ptr<ChargeMagneticField> instance(new ChargeMagneticField(charge));
  return instance;
}

ChargeMagneticField::ChargeMagneticField(double charge) : m_charge(charge) {  // 1.0 for one proton, -2.0 for two electrons, and so on.
  m_position.fill(0.0);
  m_velocity.fill(0.0);
  m_acceleration.fill(0.0);
}

Eigen::Vector3d ChargeMagneticField::GetMagneticFieldVector(const Eigen::Vector3d& pos) {
  Eigen::Vector3d r = pos.array()-m_position.array();
  double rLengthSquared = r.squaredNorm(); // distance squared.
  Eigen::Vector3d rUnit = r.normalized();
  Eigen::Vector3d Vcrossr = m_velocity.cross(rUnit);

  Eigen::Vector3d B = (PERMIABILITY_OVER_4PI*m_charge/rLengthSquared)*Vcrossr.array();
  return B;
}

Eigen::Vector3d ChargeMagneticField::GetMagneticFieldDirectionVector(const Eigen::Vector3d& pos, bool& isValid) {
  isValid = true;
  bool useVelocity = true;
  if(m_velocity.norm() < 1.0e-12) {
    if(m_acceleration.norm() < 1.0e-12) {
      isValid = false;
      return Eigen::Vector3d();
    }
    useVelocity = false;
  }

  Eigen::Vector3d r = pos.array()-m_position.array();
  Eigen::Vector3d Vcrossr = (useVelocity ? m_velocity.cross(r) : m_acceleration.cross(r));

  Eigen::Vector3d directionB = Vcrossr.normalized();
  return directionB;
}

Eigen::Vector3d ChargeMagneticField::GetMagneticFieldChangeVector(const Eigen::Vector3d& pos, const Eigen::Vector3d& vel) {
  // d(V x r)/dt = V x dr/dt + dV/dt x r
  Real Vx = m_velocity(0);
  Real Vy = m_velocity(1);
  Real Vz = m_velocity(2);

  Real rx = pos(0)-m_position(0);
  Real ry = pos(1)-m_position(1);
  Real rz = pos(2)-m_position(2);

  Real dVxdt = m_acceleration(0);
  Real dVydt = m_acceleration(1);
  Real dVzdt = m_acceleration(2);

  Real drxdt = vel(0);
  Real drydt = vel(1);
  Real drzdt = vel(2);

  Real rLength = sqrt(rx*rx+ry*ry+rz*rz);
  Real rLengthCubed = rLength*rLength*rLength;
  Real rLengthCubedInv = 1.0/rLengthCubed;
  Real rLengthFifth = rLengthCubed*rLength*rLength;
  Real rLengthFifthInv = 1.0/rLengthFifth;
  
  Real crossX = Vy*rz-Vz*ry;
  Real crossY = Vz*rx-Vx*rz;
  Real crossZ = Vx*ry-Vy*rx;

  Eigen::Vector3d dBdt;
  dBdt(0) =
    dVydt*(rz*rLengthCubedInv) +
    dVzdt*(-ry*rLengthCubedInv) +
    drxdt*(-3.0*rx * crossX*rLengthFifthInv) +
    drydt*(-Vz*rLengthCubedInv - 3.0*ry*crossX*rLengthFifthInv) +
    drzdt*(Vy*rLengthCubedInv - 3.0*rz*crossX*rLengthFifthInv);

  dBdt(1) =
    dVxdt*(-rz*rLengthCubedInv) +
    dVzdt*(rx*rLengthCubedInv) +
    drxdt*(Vz*rLengthCubedInv - 3.0*rx*crossY*rLengthFifthInv) +
    drydt*(-3.0*ry * crossY*rLengthFifthInv) +
    drzdt*(-Vx*rLengthCubedInv - 3.0*rz*crossY*rLengthFifthInv);

  dBdt(2) =
    dVxdt*(ry*rLengthCubedInv) +
    dVydt*(-rx*rLengthCubedInv) +
    drxdt*(-Vy*rLengthCubedInv - 3.0*rx*crossZ*rLengthFifthInv) +
    drydt*(Vx*rLengthCubedInv - 3.0*ry*crossZ*rLengthFifthInv) +
    drzdt*(-3.0*rz * crossZ*rLengthFifthInv);

  dBdt.array() *= PERMIABILITY_OVER_4PI;

  return dBdt;
}


