/**********************************************************************

File     : DipoleMomentFinder.h
Project  : Bach Simulation
Purpose  : Header file for a molecule equilibrium solver - solve for x where v,a are zero.
Revisions: Original definition by Lawrence Gunn.
           2013/11/07


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_DIPOLE_MOMENT_FINDER_H__
#define __BACH_DIPOLE_MOMENT_FINDER_H__

#include "RootSolverEquations.h"
#include "Element.h"

namespace Bach {

  using namespace PS;
  class Molecule;

  //**********************
  //* DipoleMomentFinder *
  //**********************
  
  class DipoleMomentFinder {
  public:
  
    enum Axis {
      X = 0,
      Y = 1
    };
  
    static boost::shared_ptr<DipoleMomentFinder> CreateInstance(boost::shared_ptr<Molecule> molecule);

    ~DipoleMomentFinder();

    void Solve(Real yOffset);
    Real GetMomentAboutXDueToZ() { return m_momentAboutXDueToZ; }  // The dipole moment, in Coulomb-meter's (C m).
    Real GetMomentAboutYDueToZ() { return m_momentAboutYDueToZ; }  // The dipole moment, in Coulomb-meter's (C m).
    Real GetForceZ() { return m_forceZ; }      // Force in Coulombs (C).
    
    Real GetMoment(Axis axis, Element::ElementType elementType, Integer id) { return 0.0; }

  protected:
    DipoleMomentFinder(boost::shared_ptr<Molecule> molecule);
    
    boost::shared_ptr<Molecule> m_molecule;
    Real m_momentAboutXDueToZ;       // The dipole moment about the X axis, in Coulomb-meter's (C m).
    Real m_momentAboutYDueToZ;       // The dipole moment about the Y axis, in Coulomb-meter's (C m).
    Real m_forceZ;                   // Unbalanced force in Coulomb's (C).
  };
};

#endif // __BACH_DIPOLE_MOMENT_FINDER_H__
