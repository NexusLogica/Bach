/**********************************************************************

File     : SpOrbital.cpp
Project  : Bach Simulation
Purpose  : Source file for a the two electrons of a sigma bond.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "SpOrbital.h"
#include "BondForce.h"
#include "Atom.h"
#include "Molecule.h"
#include "ForceVectors.h"
#include <sstream>

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

  //*************
  //* SpOrbital *
  //*************

shared_ptr<SpOrbital> SpOrbital::CreateInstance(shared_ptr<Atom> atom, const Eigen::Vector3d& position, int orbitalId) {
  shared_ptr<SpOrbital> instance(new SpOrbital(atom, position, orbitalId));
  return instance;
}

SpOrbital::SpOrbital(shared_ptr<Atom> atom, const Eigen::Vector3d& position, int orbitalId) :
  Element(Element::SpOrbitalElement),
  m_atom(atom),
  m_position(position),
  m_orbitalId(orbitalId),
  m_charge(-2.0)
{
  std::wstringstream nameStream;
  nameStream << L"SP-" << m_atom->GetMolecule()->GetType() << L":" << m_atom->GetShortName() << L"-" << m_atom->GetMolecule()->GetId() << ":" << m_atom->GetId() <<  L":" << m_orbitalId;
  SetName(nameStream.str());
}

Eigen::Vector3d SpOrbital::CalculateElectricalForce(const Eigen::Vector3d& chargePosition, Real charge, shared_ptr<ForceVectors> forceVectorStorage) {
  Eigen::Vector3d forceDirection = chargePosition.array()-m_position.array();
  Real rSquared = forceDirection.squaredNorm();
  
  Real forceMagnitude = K_VACUUM*charge*m_charge*Square(ELECTRIC_CHARGE)/rSquared;
  
  // The force is in the direction of the charge.
  forceDirection.normalize();
  Eigen::Vector3d forceVector = forceDirection.array()*forceMagnitude;
  
  if(forceVectorStorage) {
    forceVectorStorage->AddForceVector(GetName(), forceVector);
  }
  
  return forceVector;
}
