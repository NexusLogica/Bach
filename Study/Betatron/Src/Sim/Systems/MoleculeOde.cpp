/**********************************************************************

File     : MoleculeOde.cpp
Project  : Bach Simulation
Purpose  : Source file for an ordinary differential wrapper class around a molecule object.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "MoleculeOde.h"
#include "Molecule.h"
#include "Bond.h"
#include "BondElectrons.h"
#include "OdeData.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //***************
  //* MoleculeOde *
  //***************

shared_ptr<MoleculeOde> MoleculeOde::CreateInstance(shared_ptr<Molecule> molecule) {
  shared_ptr<MoleculeOde> instance(new MoleculeOde(molecule));
  instance->m_weakThis = instance;
  instance->InitializeFromMolecule();
  return instance;
}

MoleculeOde::MoleculeOde(shared_ptr<Molecule> molecule) : m_molecule(molecule) {
}

MoleculeOde::~MoleculeOde() {
}

void MoleculeOde::InitializeFromMolecule() {
  int numBonds = m_molecule->GetNumBonds();
  SetStateLength(2*numBonds);
  SetInternalLength(2*numBonds); // forces due to electrical fields and magnetic fields
}

void MoleculeOde::Initialize(shared_ptr<Bach::OdeData> odeData) {
}

void MoleculeOde::Evaluate(double time, const Eigen::VectorXd& y, Eigen::VectorXd& dydt, shared_ptr<Bach::OdeData> odeData) {
/*
  count++;
  LogPlain(L"Iteration %d", count);
  shared_ptr<Bond> bond;
  shared_ptr<BondElectrons> bondElectrons;
  int i = 0;
  int j = 0;
  for(; i<y.size(); i += 2, j++) {
    bond = m_molecule->GetBondByIndex(j);
    bondElectrons = bond->GetBondElectrons();
    LogPlain(L"Pos[%d]: %e", j, y[i]);
    LogPlain(L"Vel%d]: %e", j, y[i+1]);
    bondElectrons->SetLinearPosition(y[i]); // Set the position relative to the first atom.
    bondElectrons->SetLinearVelocity(y[i+1]);
// LAG - all wrong...    bondElectrons->CalculatePositionAndVelocity();
  }

  Eigen::VectorXd internalValues(odeData->GetInternalLength());
  PSReal acceleration;
  i = 0;
  j = 0;
  for(; i<y.size(); i += 2, j++) {
    bondElectrons = m_molecule->GetBondByIndex(j)->GetBondElectrons();
    acceleration = bondElectrons->CalculateAcceleration();

    dydt[i] = y[i+1];
    dydt[i+1] = acceleration;

    LogPlain(L"dxdt[%d]: %e", j, dydt[i]);
    LogPlain(L"dvdt[%d]: %e", j, dydt[i+1]);
    
    internalValues[i] = bondElectrons->GetElectricalInducedForce();
    internalValues[i+1] = bondElectrons->GetMagneticInducedForce();
  }
  
  odeData->SetInternalValues(internalValues);
*/
}
