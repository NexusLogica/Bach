/**********************************************************************

File     : Molecule.h
Project  : Bach Simulation
Purpose  : Header file for a sigma Molecule.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_MOLECULE_H__
#define __BACH_MOLECULE_H__

#include "Atom.h"
#include <vector>
#include <map>

namespace Bach {

  class Atom;
  class Bond;

  //************
  //* Molecule *
  //************
  
  class Molecule : public Element {
  public:
  
    static boost::shared_ptr<Molecule> CreateInstance(const std::wstring& moleculeType, Integer id);

    ~Molecule();
    
    boost::shared_ptr<Atom> AddAtom(Atom::AtomType atomType, Integer id, const Eigen::Vector3d& pos, double charge, double effectiveNuclearCharge);
    boost::shared_ptr<Bond> BondAtoms(int indexFirst, int indexSecond, Integer id);

    Integer GetId() { return m_id; }
    std::wstring GetType() { return m_moleculeType; }
    boost::shared_ptr<Atom> GetAtomByIndex(int index) { return m_atoms[index]; }
    boost::shared_ptr<Bond> GetBondByIndex(int index) { return m_bonds[index]; }
    boost::shared_ptr<Atom> GetAtomById(int id);
    boost::shared_ptr<Bond> GetBondById(int id);
    
    Integer GetNumAtoms() { return (Integer) m_atoms.size(); }
    Integer GetNumBonds() { return (Integer) m_bonds.size(); }
    
  protected:
    Molecule(const std::wstring& moleculeType, Integer id);
    void Initialize();
    
    boost::weak_ptr<Molecule> m_weakThis;

    std::wstring m_moleculeType;
    Integer m_id;
    
    std::vector< boost::shared_ptr<Atom> > m_atoms;
    std::vector< boost::shared_ptr<Bond> > m_bonds;

    std::map< int, boost::shared_ptr<Atom> > m_atomsById;
    std::map< int, boost::shared_ptr<Bond> > m_bondsById;
  };
};

#endif // __BACH_MOLECULE_H__
