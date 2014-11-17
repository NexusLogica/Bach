/**********************************************************************

File     : Molecule.cpp
Project  : Bach Simulation
Purpose  : Source file for a sigma Molecule.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "Molecule.h"
#include "Bond.h"
#include <sstream>

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //************
  //* Molecule *
  //************

shared_ptr<Molecule> Molecule::CreateInstance(const std::wstring& moleculeType, Integer id) {
  shared_ptr<Molecule> instance(new Molecule(moleculeType, id));
  instance->m_weakThis = instance;
  instance->Initialize();
  return instance;
}

Molecule::Molecule(const std::wstring& moleculeType, Integer id) : Element(Element::MoleculeElement), m_moleculeType(moleculeType), m_id(id) {
}

Molecule::~Molecule() {
}

void Molecule::Initialize() {
  std::wstringstream nameStream;
  nameStream << L"MO-" << m_moleculeType.c_str() << L"-" << m_id;
  SetName(nameStream.str());
}

shared_ptr<Atom> Molecule::AddAtom(Atom::AtomType atomType, Integer id, const Eigen::Vector3d& position, double charge, double effectiveNuclearCharge) {
  shared_ptr<Atom> atom = Atom::CreateInstance(m_weakThis.lock(), atomType, id, position, charge, effectiveNuclearCharge);
  m_atoms.push_back(atom);
  m_atomsById.insert(std::pair<  int, boost::shared_ptr<Atom> >(id, atom));
  return atom;
}

shared_ptr<Bond> Molecule::BondAtoms(int idFirst, int idSecond, int id) {
  shared_ptr<Atom> first = GetAtomById(idFirst);
  shared_ptr<Atom> second = GetAtomById(idSecond);
  if(!first) {
    Log(L"ERROR: Molecule::BondAtoms: idFirst of %d incorrect", id);
    throw std::exception();
  }
  if(!second) {
    Log(L"ERROR: Molecule::BondAtoms: idSecond of %d incorrect", id);
    throw std::exception();
  }
  
  shared_ptr<Bond> bond = Bond::CreateInstance(first, second, id);
  bond->SetId(id);
  m_bonds.push_back(bond);
  
  m_bondsById.insert(std::pair<  int, boost::shared_ptr<Bond> >(id, bond));
  return bond;
}

shared_ptr<Atom> Molecule::GetAtomById(int id) {
  std::map< int, boost::shared_ptr<Atom> >::iterator iter = m_atomsById.find(id);
  if(iter != m_atomsById.end()) {
    return iter->second;
  }
  return shared_ptr<Atom>();
}

shared_ptr<Bond> Molecule::GetBondById(int id) {
  std::map< int, boost::shared_ptr<Bond> >::iterator iter = m_bondsById.find(id);
  if(iter != m_bondsById.end()) {
    return iter->second;
  }
  return shared_ptr<Bond>();
}
