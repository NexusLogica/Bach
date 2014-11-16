/**********************************************************************

File     : BondElectrons.h
Project  : Bach Simulation
Purpose  : Header file for a the two electrons of a sigma bond.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_BOND_ELECTRONS_H__
#define __BACH_BOND_ELECTRONS_H__

#include "Bond.h"

namespace Bach {

  class ChargeMagneticField;

  //*****************
  //* BondElectrons *
  //*****************
  
  class BondElectrons : public Element {
  public:

    static boost::shared_ptr<BondElectrons> CreateInstance(boost::shared_ptr<Bond> bond, int bondId);

    ~BondElectrons() {}
    
    void SetLinearPosition(Real position);
    void SetLinearVelocity(Real velocity);
    void SetLinearAcceleration(Real accel);

    boost::shared_ptr<Bond> GetBond() { return m_bond; }
    int GetBondId() { return m_bondId; }
    
    Real CalculateAcceleration();
    
    // Calculate the static electrical forces on a point charge with 'charge' (-1.0 for one electron) at position 'chargePosition'.
    Eigen::Vector3d CalculateElectricalForce(
      Real charge,
      const Eigen::Vector3d& chargePosition,
      boost::shared_ptr<ForceVectors> forceVectorStorage);

    // Calculate the magnetically induced forces on a point charge with a position and velocity.
    Eigen::Vector3d CalculateMagneticInducedForce(
      Real charge,
      const Eigen::Vector3d& chargePosition,
      const Eigen::Vector3d& chargeVelocity,
      boost::shared_ptr<ForceVectors> forceVectorStorage);

    Real GetLinearPosition() { return m_linearPosition; }
    Real GetLinearVelocity() { return m_linearVelocity; }
    
    Real GetMagneticInducedForce()   { return m_magneticInducedForce; }
    Real GetBondForce()              { return m_bondForce; }

    Eigen::Vector3d GetBondDirectionUnitVector() { return m_bond->GetBondDirection(); }
    Eigen::Vector3d GetPosition()  { return m_position; }
    Eigen::Vector3d GetVelocity()  { return m_velocity; }
    Real GetCharge() { return m_charge; }

  protected:
    BondElectrons(boost::shared_ptr<Bond> bond, int bondId);

    // This is the sigma bond in these electrons are located.
    boost::shared_ptr<Bond> m_bond;
    int m_bondId;

    // The position is the distance along the bond as measured from the bond's "first" atom.
    Real m_linearPosition;
    Real m_linearVelocity;
    Real m_linearAcceleration;
    
    // Position relative to global observer
    Eigen::Vector3d m_position;
    Eigen::Vector3d m_velocity;
    Eigen::Vector3d m_acceleration;
    Real m_magneticInducedForce;
    Real m_bondForce;
    Real m_charge;
    
    boost::shared_ptr<ChargeMagneticField> m_chargeMagneticField;
  };
};

#endif // __BACH_BOND_ELECTRONS_H__
