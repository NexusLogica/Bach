/**********************************************************************

File     : MoleculeEquilibriumSolver.h
Project  : Bach Simulation
Purpose  : Header file for a molecule equilibrium solver - solve for x where v,a are zero.
Revisions: Original definition by Lawrence Gunn.
           2013/11/07


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_MOLECULE_EQUILIBRIUM_SOLVER_H__
#define __BACH_MOLECULE_EQUILIBRIUM_SOLVER_H__

#include "RootSolverEquations.h"

namespace Bach {

  class Molecule;
  class MoleculeEquilibriumEquations;
  class NDimNewtonRaphson;

  //*****************************
  //* MoleculeEquilibriumSolver *
  //*****************************
  
  class MoleculeEquilibriumSolver {
  public:
  
    static boost::shared_ptr<MoleculeEquilibriumSolver> CreateInstance(boost::shared_ptr<Molecule> molecule);

    ~MoleculeEquilibriumSolver();

    boost::shared_ptr<Molecule> GetMolecule() { return m_molecule; }
    boost::shared_ptr<MoleculeEquilibriumEquations> GetEquilibriumEquations() { return m_equilibriumEquations; }
    
    // *** RootSolverEquations functions ***
    Eigen::VectorXd PositionAndSolve(); // Ensure that all electrons are situated at the middle of all of the bonds.
    Eigen::VectorXd Solve();
    
  protected:
    MoleculeEquilibriumSolver(boost::shared_ptr<Molecule> molecule);
    
    void Initialize();
    
    boost::weak_ptr<MoleculeEquilibriumSolver> m_weakThis;
    boost::shared_ptr<Molecule> m_molecule;
    boost::shared_ptr<MoleculeEquilibriumEquations> m_equilibriumEquations;

    boost::shared_ptr<NDimNewtonRaphson> m_newtonRaphson;
    boost::shared_ptr<NDimAccuracySpec> m_rootValueAccuracySpec;
    boost::shared_ptr<NDimAccuracySpec> m_derivativesAccuracySpec;
    
    Eigen::VectorXd m_stepSizes;
    Real m_rootValueTolerance;
    Real m_derivativesTolerance;
  };
};

#endif // __BACH_MOLECULE_EQUILIBRIUM_SOLVER_H__
