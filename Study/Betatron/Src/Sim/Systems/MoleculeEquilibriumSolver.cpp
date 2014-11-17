/**********************************************************************

File     : MoleculeEquilibriumSolver.cpp
Project  : Bach Simulation
Purpose  : Source file for a molecule equilibrium solver - solve for x where v,a are zero.
Revisions: Original definition by Lawrence Gunn.
           2013/11/07

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "MoleculeEquilibriumSolver.h"
#include "Molecule.h"
#include "MoleculeValues.h"
#include "MoleculeEquilibriumEquations.h"
#include "NDimNewtonRaphson.h"
#include "NDimAccuracySpec.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

namespace BachConst {
  const Real DefaultMoleculeEquilibriumSolverRootValueTolerance = 0.000001;
  const Real DefaultMoleculeEquilibriumSolverDerivativesTolerance = 0.000001;
}

  //*****************************
  //* MoleculeEquilibriumSolver *
  //*****************************

shared_ptr<MoleculeEquilibriumSolver> MoleculeEquilibriumSolver::CreateInstance(shared_ptr<Molecule> molecule) {
  shared_ptr<MoleculeEquilibriumSolver> instance(new MoleculeEquilibriumSolver(molecule));
  instance->m_weakThis = instance;
  instance->Initialize();
  return instance;
}

MoleculeEquilibriumSolver::MoleculeEquilibriumSolver(shared_ptr<Molecule> molecule) :
  m_molecule(molecule),
  m_rootValueTolerance(BachConst::DefaultMoleculeEquilibriumSolverRootValueTolerance),
  m_derivativesTolerance(BachConst::DefaultMoleculeEquilibriumSolverDerivativesTolerance) {
}

MoleculeEquilibriumSolver::~MoleculeEquilibriumSolver() {
}

void MoleculeEquilibriumSolver::Initialize() {
  int numBonds = m_molecule->GetNumBonds();

  m_rootValueAccuracySpec = shared_ptr<NDimAccuracySpec>(new NDimAccuracySpec(numBonds));
  m_rootValueAccuracySpec->SetTolerance(m_rootValueTolerance);

  m_derivativesAccuracySpec = shared_ptr<NDimAccuracySpec>(new NDimAccuracySpec(numBonds));
  m_derivativesAccuracySpec->SetTolerance(m_derivativesTolerance);

  m_newtonRaphson = shared_ptr<NDimNewtonRaphson>(new NDimNewtonRaphson);
  m_newtonRaphson->SetRootValueAccuracySpec(m_rootValueAccuracySpec);
  m_newtonRaphson->SetDerivativesAccuracySpec(m_derivativesAccuracySpec);
  
  m_equilibriumEquations = MoleculeEquilibriumEquations::CreateInstance(m_molecule);
}

Eigen::VectorXd MoleculeEquilibriumSolver::PositionAndSolve() {
  shared_ptr<MoleculeValues> values = MoleculeValues::CreateInstance(m_molecule);

  Eigen::VectorXd bondPositions = values->GetBondLengths();
  bondPositions.array() *= 0.5;
  values->SetLinearPos(bondPositions);

  return Solve();
}

Eigen::VectorXd MoleculeEquilibriumSolver::Solve() {
  shared_ptr<MoleculeValues> values = MoleculeValues::CreateInstance(m_molecule);

  Eigen::VectorXd velocities(2);
  velocities.fill(0.0);
  values->SetLinearVel(velocities);

  Eigen::VectorXd linearPositions = values->GetLinearPos();

  m_newtonRaphson->Solve(m_equilibriumEquations, linearPositions);
  Eigen::VectorXd rootPositions = m_newtonRaphson->GetRoots();
  values->SetLinearPos(rootPositions);
  
  return rootPositions;
}
