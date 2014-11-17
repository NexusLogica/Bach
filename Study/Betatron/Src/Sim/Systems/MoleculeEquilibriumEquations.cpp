/**********************************************************************

File     : MoleculeEquilibriumEquations.cpp
Project  : Bach Simulation
Purpose  : Source file for a system of equations for solver for equilibrium position of a molecule.
Revisions: Original definition by Lawrence Gunn.
           2013/11/09

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "MoleculeEquilibriumEquations.h"
#include "Molecule.h"
#include "MoleculeValues.h"
#include "Bond.h"
#include "BondElectrons.h"
#include "NDimAccuracySpec.h"
#include "RiddersExtrapolation.h"
#include "Jacobian.h"
#include "ForceVectors.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

namespace BachConst {
  const Real DefaultMoleculeSystemSolverTolerance = 0.00001;
}

  //********************************
  //* MoleculeEquilibriumEquations *
  //********************************

shared_ptr<MoleculeEquilibriumEquations> MoleculeEquilibriumEquations::CreateInstance(shared_ptr<Molecule> molecule) {
  shared_ptr<MoleculeEquilibriumEquations> instance(new MoleculeEquilibriumEquations(molecule));
  instance->m_weakThis = instance;
  instance->Initialize();
  return instance;
}

MoleculeEquilibriumEquations::MoleculeEquilibriumEquations(shared_ptr<Molecule> molecule) : m_molecule(molecule), m_tolerance(BachConst::DefaultMoleculeSystemSolverTolerance) {
}

MoleculeEquilibriumEquations::~MoleculeEquilibriumEquations() {
}

void MoleculeEquilibriumEquations::Initialize() {
  m_moleculeValues = MoleculeValues::CreateInstance(m_molecule);

  int numBonds = m_molecule->GetNumBonds();
  SetStateLength(numBonds);
  SetInternalLength(2*numBonds); // forces due to electrical fields and magnetic fields

  m_derivatives = shared_ptr<MultivariateDerivatives>(new RiddersExtrapolation(numBonds));
  m_accuracySpec = shared_ptr<NDimAccuracySpec>(new NDimAccuracySpec(numBonds));
  m_accuracySpec->SetTolerance(m_tolerance);

  m_jacobian = shared_ptr<Jacobian>(new Jacobian());
  m_jacobian->SetDerivativeFinder(m_derivatives);
  m_jacobian->SetAccuracySpec(m_accuracySpec);

  m_stepSizes.resize(numBonds);
  m_stepSizes.fill(Bach::CHARACTERISTIC_BOND_DISTANCE_UNIT);
}

Eigen::VectorXd MoleculeEquilibriumEquations::GetEquationValues(const Eigen::VectorXd& position) {
Log(L"GETTING FUNCTION VALUES*****");
  Eigen::VectorXd remainder = FindFunctionValues(position, shared_ptr<ForceVectorsList>());
  return remainder;
}

Eigen::MatrixXd MoleculeEquilibriumEquations::GetJacobian(const Eigen::VectorXd& position) {
Log(L"GETTING JACOBIAN*****");
  m_jacobian->Start(position, m_stepSizes);

  Eigen::VectorXd tempRemainders(m_molecule->GetNumBonds());
  
  while(!m_jacobian->GetIsFinished()) {
    tempRemainders = FindFunctionValues(m_jacobian->GetNewValue(), shared_ptr<ForceVectorsList>());
    m_jacobian->SetFunctionValues(tempRemainders);
  }
  
  Eigen::MatrixXd dRemainderdPosition = m_jacobian->GetDerivatives();
  Log(L"DDD derivs: %s", ToString(dRemainderdPosition.array()*(METERS_TO_PICOMETERS/NEWTONS_TO_NANONEWTONS)).c_str());
  Log(L"EXIT EQUATIONS*****");
  return dRemainderdPosition;
}

// The equations for the forces on a pair of sigma bond electrons is of the form:
//
//   0 = inductance(pos, vel, accel)+electric(pos)+bond(pos)
//
// where
//    inductance is the force due to inductance
//    electric is force due to electrical forces from protons/nuclei and other electrons
//    bond is the centering force of the bond

Eigen::VectorXd MoleculeEquilibriumEquations::FindFunctionValues(const Eigen::VectorXd& positions, shared_ptr<ForceVectorsList> forceVectorStorageList) {
LogPlain(L"* Positions to Try (pm): %s", ToString(positions.array()*METERS_TO_PICOMETERS).c_str());

  m_moleculeValues->SetLinearPos(positions);

  // For each sigma bond there is one equation to balance.
  Integer numBonds = m_molecule->GetNumBonds();
  Eigen::VectorXd roots;
  roots.resize(numBonds);
  roots.fill(0.0);
  
  shared_ptr<ForceVectors> forceVectorStorage;
  
  for(Integer i=0; i<numBonds; i++) {
    shared_ptr<Bond> bond = m_molecule->GetBondByIndex(i);

    if(forceVectorStorageList) {
      forceVectorStorage = ForceVectors::CreateInstance();
      forceVectorStorage->SetTargetName(bond->GetName());
      forceVectorStorage->SetTargetTravelConstraint(bond->GetBondDirection());
      forceVectorStorageList->AddForceVector(forceVectorStorage);
    }

    Real remainder = bond->GetEquilibriumEquationRemainder(forceVectorStorage);
    Log(L"Remainder[%d] (nN): %e", i, remainder*NEWTONS_TO_NANONEWTONS);
    roots(i) = remainder;
  }
  
  return roots;
}