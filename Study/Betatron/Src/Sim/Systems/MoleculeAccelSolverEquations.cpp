/**********************************************************************

File     : MoleculeAccelSolverEquations.cpp
Project  : Bach Simulation
Purpose  : Source file for a sigma MoleculeAccelSolverEquations.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "MoleculeAccelSolverEquations.h"
#include "Molecule.h"
#include "MoleculeValues.h"
#include "Bond.h"
#include "BondElectrons.h"
#include "NDimAccuracySpec.h"
#include "RiddersExtrapolation.h"
#include "Jacobian.h"

using namespace Bach;
using namespace PS;
using namespace boost;
using namespace Eigen;

namespace BachConst {
  const Real DefaultMoleculeSystemSolverTolerance = 0.00001;
}

  //********************************
  //* MoleculeAccelSolverEquations *
  //********************************

shared_ptr<MoleculeAccelSolverEquations> MoleculeAccelSolverEquations::CreateInstance(shared_ptr<Molecule> molecule) {
  shared_ptr<MoleculeAccelSolverEquations> instance(new MoleculeAccelSolverEquations(molecule));
  instance->m_weakThis = instance;
  instance->Initialize();
  return instance;
}

MoleculeAccelSolverEquations::MoleculeAccelSolverEquations(shared_ptr<Molecule> molecule) : m_molecule(molecule), m_tolerance(BachConst::DefaultMoleculeSystemSolverTolerance) {
}

MoleculeAccelSolverEquations::~MoleculeAccelSolverEquations() {
}

void MoleculeAccelSolverEquations::Initialize() {
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
  m_stepSizes.fill(1.0);

}

Eigen::VectorXd MoleculeAccelSolverEquations::GetEquationValues(const Eigen::VectorXd& accel) {
  Eigen::VectorXd remainder = FindFunctionValues(accel);
  return remainder;
}

Eigen::MatrixXd MoleculeAccelSolverEquations::GetJacobian(const Eigen::VectorXd& accel) { // Output: d Remainder/d Accel

  m_jacobian->Start(accel, m_stepSizes);

  Eigen::VectorXd tempRemainders(m_molecule->GetNumBonds());
  
  while(!m_jacobian->GetIsFinished()) {
    tempRemainders = FindFunctionValues(m_jacobian->GetNewValue());
    m_jacobian->SetFunctionValues(tempRemainders);
  }
  
  return m_jacobian->GetDerivatives();
}

// The equations for the forces on a pair of sigma bond electrons is of the form:
//
//   0 = inductance(pos, vel, accel)+electric(pos)+bond(pos)
//
// where
//    inductance is the force due to inductance
//    electric is force due to electrical forces from protons/nuclei and other electrons
//    bond is the centering force of the bond

Eigen::VectorXd MoleculeAccelSolverEquations::FindFunctionValues(const Eigen::VectorXd& accelerations) {
  m_moleculeValues->SetLinearAccel(accelerations);

  // For each sigma bond there is one equation to balance.
  Integer numBonds = m_molecule->GetNumBonds();
  Eigen::VectorXd roots;
  roots.resize(numBonds);
  roots.fill(0.0);
  
  for(Integer i=0; i<numBonds; i++) {
    shared_ptr<Bond> bond = m_molecule->GetBondByIndex(i);
    Real remainder = bond->GetAccelEquationRemainder(shared_ptr<ForceVectors>());
    Log(L"*** FindFunctionValues: Remainder[%d] = %e", i, remainder);
    roots(i) = remainder;
  }
  
  return roots;
}