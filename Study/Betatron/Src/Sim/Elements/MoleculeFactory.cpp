/**********************************************************************

File     : MoleculeFactory.cpp
Project  : Bach Simulation
Purpose  : Source file for a sigma MoleculeFactory.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "MoleculeFactory.h"
#include "Molecule.h"
#include "BondElectrons.h"
#include "BondForce.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*******************
  //* MoleculeFactory *
  //*******************

shared_ptr<MoleculeFactory> MoleculeFactory::CreateInstance() {
  shared_ptr<MoleculeFactory> instance(new MoleculeFactory);
  instance->m_weakThis = instance;
  return instance;
}

shared_ptr<Molecule> MoleculeFactory::CreateInstance(StandardMolecule moleculeType, Integer moleculeId) {
  shared_ptr<MoleculeFactory> factory = CreateInstance();
  switch(moleculeType) {
    case Water :
      return factory->CreateWater(moleculeId);
    default:
      return shared_ptr<Molecule>();
  }
}

MoleculeFactory::MoleculeFactory() {
}

MoleculeFactory::~MoleculeFactory() {
}

void MoleculeFactory::Initialize() {
}

// Add an atom using the MOL2 format.
shared_ptr<Atom> MoleculeFactory::AddAtom(
  Atom::AtomType atomType,
  int id,
  double x, // The positions are input in angstroms
  double y,
  double z,
  double charge,
  double effectiveNuclearCharge,
  const std::wstring& substructureName,
  int substructureId,
  const std::wstring& statusBit)
{
  shared_ptr<Atom> atom = m_molecule->AddAtom(atomType, id, Eigen::Vector3d(x, y, z), charge, effectiveNuclearCharge);
  return atom;
}

shared_ptr<Bond> MoleculeFactory::BondAtoms(
  int idFirst,
  int idSecond,
  int bondId,
  const std::wstring& bondType,
  const std::wstring& statusBits)
{
  shared_ptr<Bond> bond = m_molecule->BondAtoms(idFirst, idSecond, bondId);
  return bond;
}

shared_ptr<Molecule> MoleculeFactory::CreateWater(Integer moleculeId) {
  m_molecule = Molecule::CreateInstance(L"Water", moleculeId);
  //  @<TRIPOS>MOLECULE
  //  Water                                                                           
  //      3     2    0     0     0
  //  SMALL
  //  NO_CHARGES
  //  ****
  //
  //  @<TRIPOS>ATOM
  //  1     O1               0.0000   0.0645   0.0000 O.3       1 ABCD  -0.3736
  //  2     H2               0.7513  -0.5120   0.0000 H         1 ABCD   0.1868
  //  3     H3              -0.7513  -0.5120   0.0000 H         1 ABCD   0.1868
  //  @<TRIPOS>BOND
  //  1     1     2     1
  //  2     1     3     1
  
  // Effective nuclear charges taken as 1.0 for hydrogen and 6.0 for oxygen.

  Real yOffset = 0.0;
  shared_ptr<Atom> a;
  a = AddAtom(Atom::Oxygen, 1, A2m( 0.0000), A2m( 0.0645+yOffset), A2m(0.0000), -0.3736, 6.0);

  Real theta = NXGR_DEG_TO_RAD*109.471;
  // Real spLen = A2m(0.20); // LAT TODO: Absolute guess. Find correct value.
  Real spLen = A2m(0.10); // LAT TODO: Absolute guess. Find correct value.
  Real spY = spLen*cos(0.5*theta)+A2m( 0.0645+yOffset);
  Real spZ = spLen*sin(0.5*theta);
  a->AddSpOrbital(Eigen::Vector3d(A2m( 0.0000), spY,  spZ), -2.0, 1);
  a->AddSpOrbital(Eigen::Vector3d(A2m( 0.0000), spY, -spZ), -2.0, 2);

  AddAtom(Atom::Hydrogen, 2, A2m( 0.7513), A2m(-0.5120+yOffset), A2m(0.0000),  0.1868, 1.0);
  AddAtom(Atom::Hydrogen, 3, A2m(-0.7513), A2m(-0.5120+yOffset), A2m(0.0000),  0.1868, 1.0);

  shared_ptr<Bond> b;
  b = BondAtoms(1, 2, 1);
  CreateOxHydBondForce(b);
  b = BondAtoms(1, 3, 2);
  CreateOxHydBondForce(b);
  
  return m_molecule;
}

// This assumes that the first/second atoms are oxygen/hydrogen respectively.
void MoleculeFactory::CreateOxHydBondForce(shared_ptr<Bond> bond) {
  shared_ptr<BondForce> bondForce = BondForce::CreateInstance(bond);
  Real len = bond->GetLength();
  Log(L"LENGTH=%e", len);
  Real nominalBondLength = 94.69973e-12; // pm

 #if (__BOND_FORCE_VERSION__ == 2)
  
  bondForce->Configure(
     0.420,                          // centerXFrac
    -1.0e5/nominalBondLength,        // centerSlopeFrac N/m
     0.33,                           // max1XFrac
     500.0*NANONEWTONS_TO_NEWTONS,   // max1Value
     0.25,                           // limit1XFrac
     2.0e5/nominalBondLength,        // limit1SlopeFrac
     0.67,                           // max2XFrac
    -700.0*NANONEWTONS_TO_NEWTONS,   // max2Value
     0.80,                           // limit2XFrac
     1.20e5/nominalBondLength);      // limit2SlopeFrac
  bond->SetBondForce(bondForce);

#endif // __BOND_FORCE_VERSION__
}

