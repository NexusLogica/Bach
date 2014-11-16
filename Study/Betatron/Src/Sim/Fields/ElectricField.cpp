/**********************************************************************

File     : ElectricField.cpp
Project  : Bach Simulation
Purpose  : Source file for an electric field produced by a charged particle.
Revisions: Original definition by Lawrence Gunn.
           2013/03/08


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "ElectricField.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

  //*****************
  //* ElectricField *
  //*****************

void ElectricField::GetFieldVectors(
  double charge, // 1.0 for one proton, -2.0 for two electrons, and so on.
  const Eigen::Vector3d& pos,
  const Eigen::Vector3d& vel,
  Eigen::Vector3d& E,
  Eigen::Vector3d& dEdt)
{
  Eigen::Vector3d relativePos = pos-m_position;
  //Eigen::Vector3d relativeVel = vel-m_velocity;
  double rPow2 = relativePos.squaredNorm(); // distance squared.
  double r = sqrt(rPow2);

  Eigen::Vector3d unitRelPos = relativePos.array()*(1.0/r);

  double coeff = PERMIABILITY_OVER_4PI*charge/rPow2;
  Eigen::Vector3d vXp = m_velocity.cross(unitRelPos);
  coeff += 0.0; // suppress warnings
  vXp.array(); // suppress warnings
/*
  B = vXp.array()*PERMIABILITY_OVER_4PI;

  // dB(v)/dt
  Eigen::Vector3d dBvdt = m_acceleration.cross(unitRelPos);

  // dB(r^)/dt first
  Eigen::Vector3d dBrhdt1 = m_velocity.cross(relativeVel).array()*(1.0/r);

  // dB(r^)/dt second
  double vRelPow2 = relativeVel.squaredNorm(); // relative velocity squared.
  double vRel = sqrt(vRelPow2);
  Eigen::Vector3d dBrhdt2 = vXp.array()*(vRel/r/r);

  // dB(r^-2)/dt
  Eigen::Vector3d dinvrrdt = (m_acceleration.cross(unitRelPos)).array()*(3.0/r);

  dBdt = (dBvdt.array()+dBrhdt1.array()-dBrhdt2.array()-dinvrrdt.array())*(PERMIABILITY_OVER_4PI*charge/rPow2);
*/
}
