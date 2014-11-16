/**********************************************************************

File     : MoleculeAccelSolverEquations.h
Project  : Bach Simulation
Purpose  : Header file for a sigma MoleculeAccelSolverEquations.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_MOLECULE_ACCEL_SOLVER_EQUATIONS_H__
#define __BACH_MOLECULE_ACCEL_SOLVER_EQUATIONS_H__

#include "RootSolverEquations.h"

namespace Bach {

  class Molecule;
  class MoleculeValues;
  class Jacobian;

  //********************************
  //* MoleculeAccelSolverEquations *
  //********************************
  
  class MoleculeAccelSolverEquations : public Bach::RootSolverEquations {
  public:
  
    static boost::shared_ptr<MoleculeAccelSolverEquations> CreateInstance(boost::shared_ptr<Molecule> molecule);

    ~MoleculeAccelSolverEquations();

    boost::shared_ptr<Molecule> GetMolecule() { return m_molecule; }
    
    // *** RootSolverEquations functions ***
    virtual void Initialize();

    virtual Eigen::VectorXd GetEquationValues(  // Output: The values of the functions at 'y'
      const Eigen::VectorXd& accel);            // Input:  The independent variable

    virtual Eigen::MatrixXd GetJacobian(        // Output: The derivatives of the functions with respect to 'y' about the input 'y' value
      const Eigen::VectorXd& accel);            // Input:  The independent variable
    
  protected:
    MoleculeAccelSolverEquations(boost::shared_ptr<Molecule> molecule);
    
    Eigen::VectorXd FindFunctionValues(const Eigen::VectorXd& accel);
    
    boost::weak_ptr<MoleculeAccelSolverEquations> m_weakThis;
    boost::shared_ptr<Molecule> m_molecule;
    boost::shared_ptr<MoleculeValues> m_moleculeValues;

    boost::shared_ptr<Jacobian> m_jacobian;
    boost::shared_ptr<MultivariateDerivatives> m_derivatives;
    boost::shared_ptr<NDimAccuracySpec> m_accuracySpec;

    Eigen::VectorXd m_stepSizes;
    PS::Real m_tolerance;
  };
};

#endif // __BACH_MOLECULE_ACCEL_SOLVER_EQUATIONS_H__
