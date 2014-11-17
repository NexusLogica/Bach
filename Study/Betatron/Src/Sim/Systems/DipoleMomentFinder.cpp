/**********************************************************************

File     : DipoleMomentFinder.cpp
Project  : Bach Simulation
Purpose  : Source file for a molecule equilibrium solver - solve for x where v,a are zero.
Revisions: Original definition by Lawrence Gunn.
           2013/11/07

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "DipoleMomentFinder.h"
#include "Molecule.h"
#include "SpOrbital.h"
#include "BondElectrons.h"
#include "MoleculeValues.h"
#include "MoleculeEquilibriumEquations.h"
#include "NDimNewtonRaphson.h"
#include "NDimAccuracySpec.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //**********************
  //* DipoleMomentFinder *
  //**********************

shared_ptr<DipoleMomentFinder> DipoleMomentFinder::CreateInstance(shared_ptr<Molecule> molecule) {
  shared_ptr<DipoleMomentFinder> instance(new DipoleMomentFinder(molecule));
  return instance;
}

DipoleMomentFinder::DipoleMomentFinder(shared_ptr<Molecule> molecule) :
  m_molecule(molecule) {
}

DipoleMomentFinder::~DipoleMomentFinder() {
}

void DipoleMomentFinder::Solve(Real yOffset) {

  // For now (water simulation) the moment is only about the X and Y axes in
  Real forceSum = 0.0;
  Real momentSumAboutX = 0.0;
  Real momentSumAboutY = 0.0;
  
  LogPlain(L"Y Offset = %f", yOffset*METERS_TO_PICOMETERS);

  shared_ptr<Atom> atom;
  shared_ptr<SpOrbital> orbital;
  shared_ptr<BondElectrons> bondElectrons;

  for(Integer i=0; i<m_molecule->GetNumAtoms(); i++) {
    atom = m_molecule->GetAtomByIndex(i);
    Real charge = atom->GetEffectiveNuclearCharge();
    Eigen::Vector3d position = atom->GetPosition();

    forceSum += charge;

    Real x = position(0);
    Real y = position(1)+yOffset;
    momentSumAboutX += y*charge;
    momentSumAboutY += x*charge;
    LogPlain(L"Atom[%d] mX=%f (%f pm x %f charge)", i, (y*charge)*ELECTRIC_CHARGE*COULOMB_METERS_TO_DEBYES, y*METERS_TO_PICOMETERS, charge);
    
    for(Integer j=0; j<atom->GetNumSpOrbitals(); j++) {
      orbital = atom->GetSpOrbitalByIndex(j);
      charge = orbital->GetCharge();
      position = orbital->GetPosition();

      forceSum += charge;

      x = position(0);
      y = position(1)+yOffset;
      momentSumAboutX += y*charge;
      momentSumAboutY += x*charge;
      LogPlain(L"SP Orbital[%d,%d] mX=%f (%f pm x %f charge)", i, j, (y*charge)*ELECTRIC_CHARGE*COULOMB_METERS_TO_DEBYES, y*METERS_TO_PICOMETERS, charge);
    }
  }
  
  for(Integer i=0; i<m_molecule->GetNumBonds(); i++) {
    bondElectrons = m_molecule->GetBondByIndex(i)->GetBondElectrons();

    Real charge = bondElectrons->GetCharge();
    Eigen::Vector3d position = bondElectrons->GetPosition();

    forceSum += charge;

    Real x = position(0);
    Real y = position(1)+yOffset;
    momentSumAboutX += y*charge;
    momentSumAboutY += x*charge;
    LogPlain(L"Bond[%d] mX=%f (%f pm x %f charge)", i, (y*charge)*ELECTRIC_CHARGE*COULOMB_METERS_TO_DEBYES, y*METERS_TO_PICOMETERS, charge);
  }

  m_forceZ = forceSum*ELECTRIC_CHARGE;
  m_momentAboutXDueToZ = momentSumAboutX*ELECTRIC_CHARGE;
  m_momentAboutYDueToZ = momentSumAboutY*ELECTRIC_CHARGE;
}
