/**********************************************************************

File     : MoleculeAccelerationSolver.h
Project  : Bach Simulation
Purpose  : Header file for a sigma MoleculeAccelerationSolver.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_MOLECULE_ACCELERATION_SOLVER_H__
#define __BACH_MOLECULE_ACCELERATION_SOLVER_H__

#include "RootSolverEquations.h"

namespace Bach {

  class Molecule;
  class MoleculeAccelSolverEquations;
  class NDimNewtonRaphson;

  //******************************
  //* MoleculeAccelerationSolver *
  //******************************
  
  class MoleculeAccelerationSolver {
  public:
  
    static boost::shared_ptr<MoleculeAccelerationSolver> CreateInstance(boost::shared_ptr<Molecule> molecule);

    ~MoleculeAccelerationSolver();

    boost::shared_ptr<Molecule> GetMolecule() { return m_molecule; }
    
    Eigen::VectorXd SolveForAccelerations(const Eigen::VectorXd& initialAccelerations);
    
  protected:
    MoleculeAccelerationSolver(boost::shared_ptr<Molecule> molecule);

    void Initialize();

    boost::weak_ptr<MoleculeAccelerationSolver> m_weakThis;
    boost::shared_ptr<Molecule> m_molecule;
    boost::shared_ptr<MoleculeAccelSolverEquations> m_systemOfAccelEquations;

    boost::shared_ptr<NDimNewtonRaphson> m_newtonRaphson;
    boost::shared_ptr<NDimAccuracySpec> m_rootValueAccuracySpec;
    boost::shared_ptr<NDimAccuracySpec> m_derivativesAccuracySpec;
    
    Eigen::VectorXd m_stepSizes;
    PS::Real m_rootValueTolerance;
    PS::Real m_derivativesTolerance;
  };
};

#endif // __BACH_MOLECULE_ACCELERATION_SOLVER_H__
