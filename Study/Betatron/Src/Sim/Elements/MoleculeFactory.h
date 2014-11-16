/**********************************************************************

File     : MoleculeFactory.h
Project  : Bach Simulation
Purpose  : Header file for a sigma MoleculeFactory.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_MOLECULE_FACTORY_H__
#define __BACH_MOLECULE_FACTORY_H__

#include "Atom.h"
#include <vector>

namespace Bach {

  class Molecule;
  class Bond;

  //*******************
  //* MoleculeFactory *
  //*******************
  
  class MoleculeFactory {
  public:
  
    enum StandardMolecule {
      Water = 1
    };
  
    static boost::shared_ptr<MoleculeFactory> CreateInstance();
    static boost::shared_ptr<Molecule> CreateInstance(StandardMolecule moleculeType, Integer moleculeId);

    ~MoleculeFactory();
    
    // Add an atom using the MOL2 format.
    boost::shared_ptr<Atom> AddAtom(
      Atom::AtomType atomType,
      int id,
      double x, // The positions are input in angstroms
      double y,
      double z,
      double charge,
      double effectiveNuclearCharge,
      const std::wstring& substructureName = L"",
      int substructureId = 0,
      const std::wstring& statusBit = L"");

    boost::shared_ptr<Bond> BondAtoms(
      int idFirst,
      int idSecond,
      int bondId,
      const std::wstring& bondType = L"1", // "1", "2", "3", "am" (amide), "ar" (aromatic), "du" (dummy), "un" (unknown), "nc" (not connected)
      const std::wstring& statusBits = L"");
    
  
    boost::shared_ptr<Molecule> GetMolecule() { return m_molecule; }

  protected:
    MoleculeFactory();
    void Initialize();
    boost::shared_ptr<Molecule> CreateWater(Integer moleculeId);
    
    boost::weak_ptr<MoleculeFactory> m_weakThis;
    boost::shared_ptr<Molecule> m_molecule;

    void CreateOxHydBondForce(boost::shared_ptr<Bond> bond);
  };
};

#endif // __BACH_MOLECULE_FACTORY_H__
