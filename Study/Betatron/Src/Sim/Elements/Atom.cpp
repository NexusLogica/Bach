/**********************************************************************

File     : Atom.cpp
Project  : Bach Simulation
Purpose  : Source file for an atom.
Revisions: Original definition by Lawrence Gunn.
           2013/06/21

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "Atom.h"
#include "Molecule.h"
#include "SpOrbital.h"
#include "ForceVectors.h"
#include <sstream>

using namespace Bach;
using namespace boost;
using namespace Eigen;

namespace {
/*
  const wchar_t* AtomNameArray[] = {
    L"",
    L"Hydrogen",
    L"Helium",
    L"Lithium",
    L"Beryllium",
    L"Boron",
    L"Carbon",
    L"Nitrogen",
    L"Oxygen",
    L"Fluorine",
    L"Neon"
  };
*/
  const wchar_t* AtomShortNameArray[] = {
    L"",
    L"Hy",
    L"He",
    L"Li",
    L"Be",
    L"Bo",
    L"C",
    L"N",
    L"Oxy",
    L"Fl",
    L"Neon"
  };
}
  //********
  //* Atom *
  //********

shared_ptr<Atom> Atom::CreateInstance(
  shared_ptr<Molecule> molecule,
  AtomType atomType,
  Integer id,
  const Eigen::Vector3d& position,
  double charge,
  double effectiveNuclearCharge
) {
  boost::shared_ptr<Atom> instance(new Atom(molecule, atomType, id, position, charge, effectiveNuclearCharge));
  instance->m_weakThis = instance;
  return instance;
}

Atom::Atom(
  shared_ptr<Molecule> molecule,
  AtomType atomType,
  Integer id,
  const Eigen::Vector3d& position,
  double charge,
  double effectiveNuclearCharge
) :
  Element(Element::AtomElement),
  m_molecule(molecule),
  m_atomType(atomType),
  m_id(id),
  m_position(position),
  m_charge(charge),
  m_effectiveNuclearCharge(effectiveNuclearCharge)
{
  m_shortName = AtomShortNameArray[atomType];
  std::wstringstream nameStream;
  nameStream << L"AT-" << m_molecule->GetType() << L":" << m_shortName << L"-" << m_molecule->GetId() << L":" << m_id;
  SetName(nameStream.str());
}

shared_ptr<SpOrbital> Atom::AddSpOrbital(const Eigen::Vector3d& pos, double charge, int orbitalId) {
  shared_ptr<SpOrbital> orbital = SpOrbital::CreateInstance(m_weakThis.lock(), pos, orbitalId);
  m_spOrbitals.push_back(orbital);
  return orbital;
}

shared_ptr<SpOrbital> Atom::GetSpOrbitalById(int id) {
  for(int i=0; i<m_spOrbitals.size(); i++) {
    shared_ptr<SpOrbital> orbital = m_spOrbitals[i];
    if(orbital->GetOrbitalId() == id) {
      return orbital;
    }
  }
  return shared_ptr<SpOrbital>();
}

Eigen::Vector3d Atom::CalculateElectricalForce(const Eigen::Vector3d& chargePosition, Real charge, shared_ptr<ForceVectors> forceVectorStorage) {
  Eigen::Vector3d forceDirection = chargePosition.array()-m_position.array();
  Real rSquared = forceDirection.squaredNorm();
  
  Real forceMagnitude = K_VACUUM*charge*m_effectiveNuclearCharge*Square(ELECTRIC_CHARGE)/rSquared;
  
  // The force is in the direction of the charge.
  forceDirection.normalize();
  Eigen::Vector3d forceVector = forceDirection.array()*forceMagnitude;
  
  if(forceVectorStorage) {
    forceVectorStorage->AddForceVector(GetName(), forceVector);
  }
  
  // Add in forces due to sp orbitals.
  for(int i=0; i<m_spOrbitals.size(); i++) {
    shared_ptr<SpOrbital> orbital = m_spOrbitals[i];
    Eigen::Vector3d spForce = orbital->CalculateElectricalForce(chargePosition, charge, forceVectorStorage);
    forceVector.array() += spForce.array();
  }
  
  return forceVector;
}

Real Atom::CalculateElectricalForceInField(Real fieldIntensityInCoulombs) {
  Real forceMagnitude = K_VACUUM*m_effectiveNuclearCharge*ELECTRIC_CHARGE*fieldIntensityInCoulombs;
  return forceMagnitude;
}
