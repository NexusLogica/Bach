/**********************************************************************

File     : SpOrbital.h
Project  : Bach Simulation
Purpose  : Header file for a the two electrons of a sigma bond.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_SP_ORBITAL_H__
#define __BACH_SP_ORBITAL_H__

#include "Atom.h"

namespace Bach {

  //*************
  //* SpOrbital *
  //*************
  
  class SpOrbital : public Element {
  public:

    static boost::shared_ptr<SpOrbital> CreateInstance(boost::shared_ptr<Atom> atom, const Eigen::Vector3d& position, int orbitalId);

    ~SpOrbital() {}
    
    boost::shared_ptr<Atom> GetAtom() { return m_atom; }
    int GetOrbitalId() { return m_orbitalId; }
    
    // Calculate the static electrical forces on a point charge with 'charge' (-1.0 for one electron) at position 'chargePosition'.
    Eigen::Vector3d CalculateElectricalForce(const Eigen::Vector3d& chargePosition, Real charge, boost::shared_ptr<ForceVectors> forceVectorStorage);

    Eigen::Vector3d GetPosition()  { return m_position; }
    Real GetCharge() { return m_charge; }

  protected:
    SpOrbital(boost::shared_ptr<Atom> atom, const Eigen::Vector3d& position, int orbitalId);

    // This is the atom in which the orbital located.
    boost::shared_ptr<Atom> m_atom;
    int m_orbitalId;

    Eigen::Vector3d m_position;
    Real m_charge;
  };
};

#endif // __BACH_SP_ORBITAL_H__
