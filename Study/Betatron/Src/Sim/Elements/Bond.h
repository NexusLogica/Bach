/**********************************************************************

File     : Bond.h
Project  : Bach Simulation
Purpose  : Header file for a sigma bond.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_BOND_H__
#define __BACH_BOND_H__

#include "Atom.h"

namespace Bach {

  class BondElectrons;
  class BondForce;
  class ForceVectors;

  //********
  //* Bond *
  //********
  
  class Bond : public Element {
  public:
  
    static boost::shared_ptr<Bond> CreateInstance(boost::shared_ptr<Atom> first, boost::shared_ptr<Atom> second, int id);

    ~Bond();
    
    void SetFirst(boost::shared_ptr<Atom> first) { m_first = first; }
    void SetSecond(boost::shared_ptr<Atom> second) { m_second = second; }
    void SetId(int id) { m_id = id; }
    void SetBondForce(boost::shared_ptr<BondForce> bondForce) { m_bondForce = bondForce; }
    
    Real GetAccelEquationRemainder(boost::shared_ptr<ForceVectors> forceVectorStorage);
    Real GetEquilibriumEquationRemainder(boost::shared_ptr<ForceVectors> forceVectorStorage);

    Eigen::Vector3d GetAccelEquationVector(boost::shared_ptr<ForceVectors> forceVectorStorage); // The vector relative to the coordinate
    Eigen::Vector3d GetEquilibriumEquationVector(boost::shared_ptr<ForceVectors> forceVectorStorage);

    Eigen::Vector3d GetBondDirection() { return m_bondDirection; } // Returns a unit vector pointing from first to second in the molecule's coordinate system.
    Real GetLength() { return m_bondLength; }

    boost::shared_ptr<Atom> GetFirst() { return m_first; }
    boost::shared_ptr<Atom> GetSecond() { return m_second; }
    boost::shared_ptr<BondElectrons> GetBondElectrons() { return m_electrons; }
    boost::shared_ptr<BondForce> GetBondForceObject() { return m_bondForce; }
    Integer GetId() { return m_id; }


  protected:
    Bond(boost::shared_ptr<Atom> first, boost::shared_ptr<Atom> second, int id);
    void Initialize();
    
    boost::weak_ptr<Bond> m_weakThis;

    boost::shared_ptr<Atom> m_first;
    boost::shared_ptr<Atom> m_second;
    Integer m_id;
    
    boost::shared_ptr<BondElectrons> m_electrons;
    boost::shared_ptr<BondForce> m_bondForce;

    Eigen::Vector3d m_bondDirection; // a unit vector pointing from first to second in the molecule's coordinate system.
    Real m_bondLength;
  };
};

#endif // __BACH_BOND_H__
