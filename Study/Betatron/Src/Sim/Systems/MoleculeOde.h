/**********************************************************************

File     : MoleculeOde.h
Project  : Bach Simulation
Purpose  : Header file for an ordinary differential wrapper class around a molecule object.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_MOLECULE_ODE_H__
#define __BACH_MOLECULE_ODE_H__

#include "OdeEquations.h"
#include <vector>

namespace Bach {

  class Molecule;

  //***************
  //* MoleculeOde *
  //***************
  
  class MoleculeOde : public Bach::OdeEquations {
  public:
  
    static boost::shared_ptr<MoleculeOde> CreateInstance(boost::shared_ptr<Molecule> molecule);

    ~MoleculeOde();
    
    virtual void Initialize(boost::shared_ptr<Bach::OdeData> odeData);
    virtual void Evaluate(double time, const Eigen::VectorXd& y, Eigen::VectorXd& dydt, boost::shared_ptr<Bach::OdeData> odeData);

    boost::shared_ptr<Molecule> GetMolecule() { return m_molecule; }

  protected:
    MoleculeOde(boost::shared_ptr<Molecule> molecule);
    void InitializeFromMolecule();
    
    boost::weak_ptr<MoleculeOde> m_weakThis;
    boost::shared_ptr<Molecule> m_molecule;
  };
};

#endif // __BACH_MOLECULE_ODE_H__
