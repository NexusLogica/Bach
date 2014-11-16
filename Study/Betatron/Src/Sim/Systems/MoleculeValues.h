/**********************************************************************

File     : MoleculeValues.h
Project  : Bach Simulation
Purpose  : Header file for a utility class for getting and setting bond positions, velocities, and accelerations.
Revisions: Original definition by Lawrence Gunn.
           2013/11/05

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.
*/

#ifndef __BACH_MOLECULE_VALUES_H__
#define __BACH_MOLECULE_VALUES_H__

#include "Atom.h"
#include <vector>
#include <map>

namespace Bach {

  class Molecule;

  //******************
  //* MoleculeValues *
  //******************
  
  class MoleculeValues {
  public:
  
    static boost::shared_ptr<MoleculeValues> CreateInstance(boost::shared_ptr<Molecule> molecule);

    ~MoleculeValues();
    
    Integer         GetNumBonds();
    Eigen::VectorXd GetLinearPosVel(); // Gets a vector with alternating linear position and linear velocity values.
    Eigen::VectorXd GetLinearPos();    // Gets a vector with position values.
    Eigen::VectorXd GetLinearVel();    // Gets a vector with velocity values.
    Eigen::VectorXd GetLinearAccel();  // Gets a vector with acceleration values.
    Eigen::VectorXd GetBondLengths();  // Gets a vector of bond lengths.
    Eigen::VectorXd GetAccelEquationRemainder();  // Gets a vector of remainder values when evaluating the equations for L(a,v)+E(x)+C(x)=0.
    Eigen::VectorXd GetEquilibriumEquationRemainder();  // Gets a vector of remainder values when evaluating the equations for E(x)=0.

    void SetLinearPosVel(const Eigen::VectorXd& values); // Sets molecule bonds with alternating position and velocity values.
    void SetLinearPos(const Eigen::VectorXd& values);    // Gets molecule bonds with position values.
    void SetLinearVel(const Eigen::VectorXd& values);    // Gets molecule bonds with velocity values.
    void SetLinearAccel(const Eigen::VectorXd& values);  // Gets molecule bonds with acceleration values.
    
  protected:
    MoleculeValues(boost::shared_ptr<Molecule> molecule);
    
    boost::shared_ptr<Molecule> m_molecule;
  };
};

#endif // __BACH_MOLECULE_VALUES_H__
