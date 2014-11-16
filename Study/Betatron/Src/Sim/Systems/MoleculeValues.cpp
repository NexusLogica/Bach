/**********************************************************************

File     : MoleculeValues.cpp
Project  : Bach Simulation
Purpose  : Source file for a utility class for getting and setting bond positions, velocities, and accelerations.
Revisions: Original definition by Lawrence Gunn.
           2013/11/05

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "MoleculeValues.h"
#include "Molecule.h"
#include "Bond.h"
#include "BondElectrons.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

  //******************
  //* MoleculeValues *
  //******************

shared_ptr<MoleculeValues> MoleculeValues::CreateInstance(shared_ptr<Molecule> molecule) {
  shared_ptr<MoleculeValues> instance(new MoleculeValues(molecule));
  return instance;
}

MoleculeValues::MoleculeValues(shared_ptr<Molecule> molecule) : m_molecule(molecule) {
}

MoleculeValues::~MoleculeValues() {
}

Integer MoleculeValues::GetNumBonds() {
  return m_molecule->GetNumBonds();
}

Eigen::VectorXd MoleculeValues::GetLinearPosVel() { // Gets a vector with alternating position and velocity values.
  Integer numBonds = m_molecule->GetNumBonds();
  Eigen::VectorXd values(2*numBonds);

  Integer j = 0;
  for(Integer i=0; i<numBonds; i++, j++) {
    shared_ptr<BondElectrons> electrons = m_molecule->GetBondByIndex(i)->GetBondElectrons();
    values(j) = electrons->GetLinearPosition();
    j++;
    values(j) = electrons->GetLinearVelocity();
    j++;
  }

  return values;
}

Eigen::VectorXd MoleculeValues::GetLinearPos() {
  Integer numBonds = m_molecule->GetNumBonds();
  Eigen::VectorXd values(numBonds);

  for(Integer i=0; i<numBonds; i++) {
    shared_ptr<BondElectrons> electrons = m_molecule->GetBondByIndex(i)->GetBondElectrons();
    values(i) = electrons->GetLinearPosition();
  }

  return values;
}

Eigen::VectorXd MoleculeValues::GetBondLengths() {
  Integer numBonds = m_molecule->GetNumBonds();
  Eigen::VectorXd values(numBonds);

  for(Integer i=0; i<numBonds; i++) {
    shared_ptr<Bond> bond = m_molecule->GetBondByIndex(i);
    values(i) = bond->GetLength();
  }

  return values;
}

Eigen::VectorXd MoleculeValues::GetAccelEquationRemainder() {  // Gets a vector of remainder values when evaluating the equations for L(a,v)+E(x)+C(x).
  Integer numBonds = m_molecule->GetNumBonds();

  Eigen::VectorXd values(numBonds*2);

  for(Integer i=0; i<numBonds; i++) {
    shared_ptr<Bond> bond = m_molecule->GetBondByIndex(i);
    Real remainder = bond->GetAccelEquationRemainder(shared_ptr<ForceVectors>());
    values(i) = remainder;
  }

  return values;
}

void MoleculeValues::SetLinearPosVel(const Eigen::VectorXd& values) { // Sets molecule bonds with alternating position and velocity values.
  Integer numBonds = m_molecule->GetNumBonds();
  if(values.size() != 2*numBonds) {
    Log(L"ERROR: MoleculeValues::SetLinearPosVel: values length of %d incorrect, should be %d", values.size(), 2*numBonds);
    throw std::exception();
  }

  shared_ptr<BondElectrons> electrons;
  
  Integer j = 0;
  for(Integer i=0; i<numBonds; i++) {
    electrons = m_molecule->GetBondByIndex(i)->GetBondElectrons();
    electrons->SetLinearPosition(values(j));
    j++;
    electrons->SetLinearVelocity(values(j));
    j++;
  }
}

void MoleculeValues::SetLinearPos(const Eigen::VectorXd& values) {
  Integer numBonds = m_molecule->GetNumBonds();
  if(values.size() != numBonds) {
    Log(L"ERROR: MoleculeValues::SetLinearPos: values length of %d incorrect, should be %d", values.size(), 2*numBonds);
    throw std::exception();
  }

  shared_ptr<BondElectrons> electrons;
  
  for(Integer i=0; i<numBonds; i++) {
    electrons = m_molecule->GetBondByIndex(i)->GetBondElectrons();
    electrons->SetLinearPosition(values(i));
  }
}

void MoleculeValues::SetLinearVel(const Eigen::VectorXd& values) {
  Integer numBonds = m_molecule->GetNumBonds();
  if(values.size() != numBonds) {
    Log(L"ERROR: MoleculeValues::SetLinearVel: values length of %d incorrect, should be %d", values.size(), 2*numBonds);
    throw std::exception();
  }

  shared_ptr<BondElectrons> electrons;
  
  for(Integer i=0; i<numBonds; i++) {
    electrons = m_molecule->GetBondByIndex(i)->GetBondElectrons();
    electrons->SetLinearVelocity(values(i));
  }
}

void MoleculeValues::SetLinearAccel(const Eigen::VectorXd& values) {
  Integer numBonds = m_molecule->GetNumBonds();
  if(values.size() != numBonds) {
    Log(L"ERROR: MoleculeValues::SetLinearVel: values length of %d incorrect, should be %d", values.size(), 2*numBonds);
    throw std::exception();
  }

  shared_ptr<BondElectrons> electrons;
  
  for(Integer i=0; i<numBonds; i++) {
    electrons = m_molecule->GetBondByIndex(i)->GetBondElectrons();
    electrons->SetLinearAcceleration(values(i));
  }
}
