/**********************************************************************

File     : MoleculeAccelerationSolver.cpp
Project  : Bach Simulation
Purpose  : Source file for a sigma MoleculeAccelerationSolver.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "MoleculeAccelerationSolver.h"
#include "Molecule.h"
#include "MoleculeAccelSolverEquations.h"
#include "NDimNewtonRaphson.h"
#include "NDimAccuracySpec.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

namespace BachConst {
  const Real DefaultMoleculeAccelerationSolverRootValueTolerance = 0.00001;
  const Real DefaultMoleculeAccelerationSolverDerivativesTolerance = 0.00001;
}

  //******************************
  //* MoleculeAccelerationSolver *
  //******************************

shared_ptr<MoleculeAccelerationSolver> MoleculeAccelerationSolver::CreateInstance(shared_ptr<Molecule> molecule) {
  shared_ptr<MoleculeAccelerationSolver> instance(new MoleculeAccelerationSolver(molecule));
  instance->m_weakThis = instance;
  instance->Initialize();
  return instance;
}

MoleculeAccelerationSolver::MoleculeAccelerationSolver(shared_ptr<Molecule> molecule) :
  m_molecule(molecule),
  m_rootValueTolerance(BachConst::DefaultMoleculeAccelerationSolverRootValueTolerance),
  m_derivativesTolerance(BachConst::DefaultMoleculeAccelerationSolverDerivativesTolerance) {
}

MoleculeAccelerationSolver::~MoleculeAccelerationSolver() {
}

void MoleculeAccelerationSolver::Initialize() {
  int numBonds = m_molecule->GetNumBonds();

  m_rootValueAccuracySpec = shared_ptr<NDimAccuracySpec>(new NDimAccuracySpec(numBonds));
  m_rootValueAccuracySpec->SetTolerance(m_rootValueTolerance);

  m_derivativesAccuracySpec = shared_ptr<NDimAccuracySpec>(new NDimAccuracySpec(numBonds));
  m_derivativesAccuracySpec->SetTolerance(m_derivativesTolerance);

  m_newtonRaphson = shared_ptr<NDimNewtonRaphson>(new NDimNewtonRaphson);
  m_newtonRaphson->SetRootValueAccuracySpec(m_rootValueAccuracySpec);
  m_newtonRaphson->SetDerivativesAccuracySpec(m_derivativesAccuracySpec);
  
  m_systemOfAccelEquations = MoleculeAccelSolverEquations::CreateInstance(m_molecule);
}

Eigen::VectorXd MoleculeAccelerationSolver::SolveForAccelerations(const Eigen::VectorXd& initialAccelerations) {
  m_newtonRaphson->Solve(m_systemOfAccelEquations, initialAccelerations);
  Eigen::VectorXd accelerations = m_newtonRaphson->GetRoots();
  return accelerations;
}
