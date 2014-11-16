/**********************************************************************

File     : BondForce.h
Project  : Bach Simulation
Purpose  : Header file for a sigma bond force curve.
Revisions: Original definition by Lawrence Gunn.
           2013/06/29


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_BOND_FORCE_H__
#define __BACH_BOND_FORCE_H__

#include "BetatronDefs.h"
#include "vector"

namespace Bach {

  class HermiteInterp1D;
  class Bond;

  //*************
  //* BondForce *
  //*************
  
  class BondForce {
  public:
  
    enum PositionType {
      BondBrokenTowardsFirst  = 1,
      BondUnbroken            = 2,
      BondBrokenTowardsSecond = 3
    };
  
    static boost::shared_ptr<BondForce> CreateInstance(boost::shared_ptr<Bond> bond);

    ~BondForce();

 #if (__BOND_FORCE_VERSION__ == 1)

    void SetFirstLimitPos( Real fraction) { m_firstLimitPosition  = fraction*m_bondLength; }
    void SetFirstMaxPos(   Real fraction) { m_firstMaxPosition    = fraction*m_bondLength; }
    void SetSecondMaxPos(  Real fraction) { m_secondMaxPosition   = fraction*m_bondLength; }
    void SetSecondLimitPos(Real fraction) { m_secondLimitPosition = fraction*m_bondLength; }

    void SetCenterSlope(     Real slope   ) { m_centerSlope      = slope; }
    void SetFirstLimitSlope( Real slope   ) { m_firstLimitSlope  = slope; }
    void SetFirstMax(        Real maxValue) { m_firstMax         = maxValue; }
    void SetSecondMax(       Real maxValue) { m_secondMax        = maxValue; }
    void SetSecondLimitSlope(Real slope   ) { m_secondLimitSlope = slope; }

    // This must be called after the above curve parameters are set.
    void ConstructCurves();

#elif (__BOND_FORCE_VERSION__ == 2)

    void Configure(
      Real centerXFrac,
      Real centerSlopeFrac,  // This means the input is centerSlopeNominal/bondLengthNominal
      Real max1XFrac,
      Real max1Value,
      Real limit1XFrac,
      Real limit1SlopeFrac,  // Same as with centerSlopeFrac
      Real max2XFrac,
      Real max2Value,
      Real limit2XFrac,
      Real limit2SlopeFrac); // Same as with centerSlopeFrac

#endif // __BOND_FORCE_VERSION__


    Real GetForce(Real x, PositionType& positionType);

  protected:
    BondForce(boost::shared_ptr<Bond> bond);

 #if (__BOND_FORCE_VERSION__ == 1)
    void Initialize();
#elif (__BOND_FORCE_VERSION__ == 2)
    void ConstructCurves();
#endif // __BOND_FORCE_VERSION__

    // Positions, slopes, and values in Newtons, for the force/position curve.
    //
    // Positions are from 0.0 to 1.0, the former being closes to the first bond.
    // Force slopes are in Netwons per position change, in the above positional measure.
    // Forces are in Newtons.
    //
    Real m_firstLimitPosition;
    Real m_firstLimitSlope;

    Real m_firstMaxPosition;
    Real m_firstMax;

    Real m_centerPosition;
    Real m_centerSlope;

    Real m_secondMaxPosition;
    Real m_secondMax;

    Real m_secondLimitPosition;
    Real m_secondLimitSlope;

    Real m_bondLength;
    
    boost::shared_ptr<Bond> m_bond;
    boost::shared_ptr<HermiteInterp1D> m_firstLimitToMax;
    boost::shared_ptr<HermiteInterp1D> m_firstCenter;
    boost::shared_ptr<HermiteInterp1D> m_secondCenter;
    boost::shared_ptr<HermiteInterp1D> m_secondLimitToMax;
  };
};

#endif // __BACH_BOND_FORCE_H__
