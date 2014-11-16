/**********************************************************************

File     : Bond.cpp
Project  : Bach Simulation
Purpose  : Source file for a sigma bond.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "Bond.h"
#include "BondElectrons.h"
#include "Atom.h"
#include "BondForce.h"
#include "Molecule.h"
#include "ForceVectors.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

  //********
  //* Bond *
  //********

boost::shared_ptr<Bond> Bond::CreateInstance(boost::shared_ptr<Atom> first, boost::shared_ptr<Atom> second, int id) {
  boost::shared_ptr<Bond> instance(new Bond(first, second, id));
  instance->m_weakThis = instance;
  instance->Initialize();
  return instance;
}

Bond::Bond(boost::shared_ptr<Atom> first, boost::shared_ptr<Atom> second, int id) :
  Element(Element::BondElement),
  m_first(first),
  m_second(second),
  m_id(id)
{
  std::wstringstream nameStream;
  nameStream << L"BD-";
  nameStream << m_first->GetMolecule()->GetType() << L":" << m_first->GetShortName() << L">" << m_second->GetShortName() << L"-";
  nameStream << first->GetMolecule()->GetId() << ":" << m_first->GetId() << L">" << m_second->GetId();
  SetName(nameStream.str());
}

void Bond::Initialize() {

  Eigen::Vector3d first = m_first->GetPosition();
  Eigen::Vector3d second = m_second->GetPosition();
  
  Eigen::Vector3d diff = second.array()-first.array();
  m_bondLength = sqrt(diff.squaredNorm());
  m_bondDirection.array() = diff.array()/m_bondLength;

#if (__BOND_FORCE_VERSION__ == 1)

  m_bondForce = BondForce::CreateInstance(m_weakThis.lock());
  m_bondForce->ConstructCurves();

#endif // __BOND_FORCE_VERSION__

  m_electrons = BondElectrons::CreateInstance(m_weakThis.lock(), m_id);
}

Bond::~Bond() {
}

Real Bond::GetAccelEquationRemainder(shared_ptr<ForceVectors> forceVectorStorage) {
  Eigen::Vector3d force = GetAccelEquationVector(forceVectorStorage);
  Real remainder = force.dot(m_bondDirection);
  return remainder;
}

Real Bond::GetEquilibriumEquationRemainder(shared_ptr<ForceVectors> forceVectorStorage) {
  Eigen::Vector3d force = GetEquilibriumEquationVector(forceVectorStorage);
  Real remainder = force.dot(m_bondDirection);
  return remainder;
}

Eigen::Vector3d Bond::GetAccelEquationVector(shared_ptr<ForceVectors> forceVectorStorage) {

  // Get the static forces. We've already written this piece of code...
  Eigen::Vector3d staticForces = GetEquilibriumEquationVector(forceVectorStorage);

  // Now calculate the inductive forces.
  
  // 1) Calculate dB/dt for the acceleration component. This is done by
  //    taking the derivative of
  // 2) Calculate dB/dt for the velocity component with respect to field movement.
  // 3) Calculate dB/dt for the velocity component with respect to field intensity change
  //    due to field change.

  Eigen::Vector3d forces;
  forces.fill(0.0);

  for(Integer i=0; i<m_first->GetMolecule()->GetNumBonds(); i++) {
    shared_ptr<Bond> bond = m_first->GetMolecule()->GetBondByIndex(i);
    if(bond->GetId() != m_id) {
      shared_ptr<BondElectrons> electrons = bond->GetBondElectrons();
      Eigen::Vector3d inducedForce = electrons->CalculateMagneticInducedForce(m_electrons->GetCharge(), m_electrons->GetPosition(), m_electrons->GetVelocity(), forceVectorStorage);
      forces.array() += inducedForce.array();
    }
  }
  
  // Add together the static and dynamic forces and return the value.
  forces.array() += staticForces.array();
  
  return forces;
}

Eigen::Vector3d Bond::GetEquilibriumEquationVector(shared_ptr<ForceVectors> forceVectorStorage) {

  // First get the bond aligning force. This will be inline between the two bond parent atoms.
  Real linPos = m_electrons->GetLinearPosition();
  BondForce::PositionType posType;
  Real bondForce = m_bondForce->GetForce(linPos, posType);
  if(posType != BondForce::BondUnbroken) {
    Log(L"ERROR: Bond::GetEquilibriumEquationVector: bond broken");
  }
  
  Eigen::Vector3d forces = m_bondDirection.array()*bondForce;
  if(forceVectorStorage) {
    Log(L"LinPos (pm): %g", linPos*METERS_TO_PICOMETERS);
    Log(L"LinForce (nN): %g", bondForce*NEWTONS_TO_NANONEWTONS);
    forceVectorStorage->AddForceVector(GetName(), forces);
  }
  
  Eigen::Vector3d bondElectronsPosition = m_electrons->GetPosition();
  Real bondElectronsCharge = m_electrons->GetCharge();

  shared_ptr<Molecule> molecule = m_first->GetMolecule();

  // Now go through all electrons in the molecule.
  // LAG TODO: Make this to be for all molecules in the system.
  Integer numBonds = molecule->GetNumBonds();
  for(Integer i=0; i<numBonds; i++) {
    shared_ptr<Bond> bond = molecule->GetBondByIndex(i);
    if(bond->GetId() != m_id) {
      shared_ptr<BondElectrons> electrons = bond->GetBondElectrons();
      Eigen::Vector3d electronForce = electrons->CalculateElectricalForce(bondElectronsCharge, bondElectronsPosition, forceVectorStorage);
      forces.array() += electronForce.array();
    }
  }
  
  // Now go through all nuclei/protons in the molecule.
  // LAG TODO: Make this to be for all molecules in the system.
  Integer firstId = m_first->GetId();
  Integer secondId = m_second->GetId();
  
  Integer numAtoms = molecule->GetNumAtoms();
  for(Integer i=0; i<numAtoms; i++) {
    shared_ptr<Atom> atom = molecule->GetAtomByIndex(i);
 #if (__BOND_FORCE_VERSION__ == 1)
    Eigen::Vector3d atomForce = atom->CalculateElectricalForce(bondElectronsPosition, bondElectronsCharge, forceVectorStorage);
    forces.array() += atomForce.array();
 #elif (__BOND_FORCE_VERSION__ == 2)
    Integer atomId = atom->GetId();
    if(atomId != firstId && atomId != secondId) {
      Eigen::Vector3d atomForce = atom->CalculateElectricalForce(bondElectronsPosition, bondElectronsCharge, forceVectorStorage);
      forces.array() += atomForce.array();
    }
#endif // __BOND_FORCE_VERSION__
  }
  
  return forces;
}

