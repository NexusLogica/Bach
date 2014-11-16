/**********************************************************************

File     : MoleculeEquilibriumEquations.h
Project  : Bach Simulation
Purpose  : Header file for a system of equations for solver for equilibrium position of a molecule.
Revisions: Original definition by Lawrence Gunn.
           2013/11/09


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_MOLECULE_EQUILIBRIUM_EQUATIONS_H__
#define __BACH_MOLECULE_EQUILIBRIUM_EQUATIONS_H__

#include "RootSolverEquations.h"

namespace Bach {

  class Molecule;
  class MoleculeValues;
  class Jacobian;
  class ForceVectorsList;

  //********************************
  //* MoleculeEquilibriumEquations *
  //********************************
  
  class MoleculeEquilibriumEquations : public Bach::RootSolverEquations {
  public:
  
    static boost::shared_ptr<MoleculeEquilibriumEquations> CreateInstance(boost::shared_ptr<Molecule> molecule);

    ~MoleculeEquilibriumEquations();

    boost::shared_ptr<Molecule> GetMolecule() { return m_molecule; }
    
    Eigen::VectorXd FindFunctionValues(const Eigen::VectorXd& positions, boost::shared_ptr<ForceVectorsList> forceVectorsStorageList);
    
    // *** RootSolverEquations functions ***
    virtual void Initialize();

    virtual Eigen::VectorXd GetEquationValues(  // Output: The values of the functions at 'y'
      const Eigen::VectorXd& y);                // Input:  The independent variable

    virtual Eigen::MatrixXd GetJacobian(        // Output: The derivatives of the functions with respect to 'y' about the input 'y' value
      const Eigen::VectorXd& y);                // Input:  The independent variable
    
  protected:
    MoleculeEquilibriumEquations(boost::shared_ptr<Molecule> molecule);
    
    boost::weak_ptr<MoleculeEquilibriumEquations> m_weakThis;
    boost::shared_ptr<Molecule> m_molecule;
    boost::shared_ptr<MoleculeValues> m_moleculeValues;

    boost::shared_ptr<Jacobian> m_jacobian;
    boost::shared_ptr<MultivariateDerivatives> m_derivatives;
    boost::shared_ptr<NDimAccuracySpec> m_accuracySpec;

    Eigen::VectorXd m_stepSizes;
    PS::Real m_tolerance;
  };
};

#endif // __BACH_MOLECULE_EQUILIBRIUM_EQUATIONS_H__
