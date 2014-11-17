/**********************************************************************

File     : Atom.h
Project  : Bach Simulation
Purpose  : Header file for an atom.
Revisions: Original definition by Lawrence Gunn.
           2013/06/21


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ATOM_H__
#define __BACH_ATOM_H__

#include "Element.h"
#include <vector>

namespace Bach {

  class Molecule;
  class SpOrbital;
  class ForceVectors;
  
  //********
  //* Atom *
  //********
  
  class Atom : public Element {
  public:
    enum AtomType {
      Hydrogen  = 1,
      Helium    = 2,
      Lithium   = 3,
      Beryllium = 4,
      Boron     = 5,
      Carbon    = 6,
      Nitrogen  = 7,
      Oxygen    = 8,
      Fluorine  = 9,
      Neon      = 10
    };

    static boost::shared_ptr<Atom> CreateInstance(
      boost::shared_ptr<Molecule> molecule,
      AtomType atomType,
      Integer id,
      const Eigen::Vector3d& position,
      Real charge,
      Real effectiveNuclearCharge);
    ~Atom() {}
    
    boost::shared_ptr<SpOrbital> AddSpOrbital(const Eigen::Vector3d& pos, Real charge, int orbitalId);

    boost::shared_ptr<SpOrbital> GetSpOrbitalByIndex(int index) { return m_spOrbitals[index]; }
    boost::shared_ptr<SpOrbital> GetSpOrbitalById(int id);
    
    Integer GetNumSpOrbitals() { return (Integer) m_spOrbitals.size(); }

    Integer GetId() { return m_id; }
    std::wstring GetShortName() { return m_shortName; }

    Eigen::Vector3d GetPosition() { return m_position; }
    Real GetCharge() { return m_charge; }
    Real GetEffectiveNuclearCharge() { return m_effectiveNuclearCharge; }
    AtomType GetAtomType() { return m_atomType; }
    boost::shared_ptr<Molecule> GetMolecule() { return m_molecule; }

    Eigen::Vector3d CalculateElectricalForce(const Eigen::Vector3d& chargePosition, Real charge, boost::shared_ptr<ForceVectors> forceVectorStorage);
    Real CalculateElectricalForceInField(Real fieldIntensityInCoulombs);

  protected:
    Atom(
      boost::shared_ptr<Molecule> molecule,
      AtomType atomType,
      Integer id,
      const Eigen::Vector3d& position,
      Real charge,
      Real effectiveNuclearCharge);

    boost::weak_ptr<Atom> m_weakThis;
    boost::shared_ptr<Molecule> m_molecule;
    Eigen::Vector3d m_position;
    Real m_charge;
    Real m_effectiveNuclearCharge;
    AtomType m_atomType;
    Integer m_id;
    std::wstring m_shortName;
    std::wstring m_name;
    std::vector< boost::shared_ptr<SpOrbital> > m_spOrbitals;
  };
};

#endif // __BACH_ATOM_H__
