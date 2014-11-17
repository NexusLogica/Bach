/**********************************************************************

File     : BondForce.cpp
Project  : Bach Simulation
Purpose  : Source file a sigma bond force curve.
Revisions: Original definition by Lawrence Gunn.
           2013/06/29

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "BondForce.h"
#include "Bond.h"
#include "HermiteInterp.h"
#include <float.h>

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*************
  //* BondForce *
  //*************

boost::shared_ptr<BondForce> BondForce::CreateInstance(shared_ptr<Bond> bond) {
  boost::shared_ptr<BondForce> instance(new BondForce(bond));
#if (__BOND_FORCE_VERSION__ == 1)
  instance->Initialize();
#endif // __BOND_FORCE_VERSION__
  return instance;
}

BondForce::BondForce(boost::shared_ptr<Bond> bond) :
  m_bond(bond),
  m_firstLimitPosition(FLT_MAX),
  m_firstLimitSlope(FLT_MAX),
  m_firstMaxPosition(FLT_MAX),
  m_firstMax(FLT_MAX),
  m_centerPosition(FLT_MAX),
  m_centerSlope(FLT_MAX),
  m_secondMaxPosition(FLT_MAX),
  m_secondMax(FLT_MAX),
  m_secondLimitPosition(FLT_MAX),
  m_secondLimitSlope(FLT_MAX),
  m_bondLength(FLT_MAX)
{
}

BondForce::~BondForce() {
}

namespace BachBond {
#if (__BOND_FORCE_VERSION__ == 1)
  const Real DisplacementDueToOxygen = 0.05;
  const Real MidBondRatio   = 0.5;
  const Real MaxBondRatio   = 0.3333;
  const Real LimitBondRatio = 0.25;
  
  const Real EstimateForMaxFromSlope = 0.5;
#endif // __BOND_FORCE_VERSION__
};

#if (__BOND_FORCE_VERSION__ == 1)
void BondForce::Initialize() {
  m_bondLength = m_bond->GetLength();

  // Set default positions for center, max, and limit.
  m_centerPosition      = BachBond::MidBondRatio*m_bondLength;
  m_firstLimitPosition  = BachBond::LimitBondRatio*m_bondLength;
  m_firstMaxPosition    = BachBond::MaxBondRatio*m_bondLength;
  m_secondMaxPosition   = (1.0-BachBond::MaxBondRatio)*m_bondLength;
  m_secondLimitPosition = (1.0-BachBond::LimitBondRatio)*m_bondLength;
  
  // Set default value for center slope.
  // Real electronCharge = 2.0;
  // Real oxygenCharge = 1.24;
  // Real hydrogenBondLen = A2m(2.037);
  // Real bondOffset = 0.55*m_bondLength;
  // Real r = hydrogenBondLen+bondOffset;
  // Real fDueToHydrogenBond = K_VACUUM*electronCharge*oxygenCharge*Square(ELECTRIC_CHARGE)/Square(r);
  // m_centerSlope = -fDueToHydrogenBond/(BachBond::DisplacementDueToOxygen*m_bondLength);

  // Second way of estimating the maximum bond force - pick a number out of the air.
  Real maxBondForce = 1000.0*NANONEWTONS_TO_NEWTONS;
  m_centerSlope = -maxBondForce/(BachBond::DisplacementDueToOxygen*m_bondLength);
  
  m_firstMax = -BachBond::EstimateForMaxFromSlope*m_centerSlope*(BachBond::MidBondRatio-BachBond::MaxBondRatio)*m_bondLength;
  m_firstLimitSlope = -m_firstMax/(BachBond::LimitBondRatio-BachBond::MaxBondRatio)/m_bondLength/BachBond::EstimateForMaxFromSlope;
  m_secondMax = -m_firstMax;
  m_secondLimitSlope = m_firstLimitSlope;
}

#elif (__BOND_FORCE_VERSION__ == 2)

void BondForce::Configure(
  Real centerXFrac,
  Real centerSlopeFrac,    // This means the input is centerSlopeNominal/bondLengthNominal
  Real max1XFrac,
  Real max1Value,
  Real limit1XFrac,
  Real limit1SlopeFrac,    // Same as with centerSlopeFrac
  Real max2XFrac,
  Real max2Value,
  Real limit2XFrac,
  Real limit2SlopeFrac) {  // Same as with centerSlopeFrac

  m_bondLength = m_bond->GetLength();

  // Set default positions for center, max, and limit.
  m_centerPosition      = centerXFrac*m_bondLength;
  m_firstLimitPosition  = limit1XFrac*m_bondLength;
  m_firstMaxPosition    = max1XFrac*m_bondLength;
  m_secondLimitPosition = limit2XFrac*m_bondLength;
  m_secondMaxPosition   = max2XFrac*m_bondLength;
  
  m_centerSlope = centerSlopeFrac*m_bondLength;
  
  m_firstMax = max1Value;
  m_firstLimitSlope = limit1SlopeFrac*m_bondLength;
  m_secondMax = max2Value;
  m_secondLimitSlope = limit2SlopeFrac*m_bondLength;
  
  ConstructCurves();
}

#endif // __BOND_FORCE_VERSION__

void BondForce::ConstructCurves() {
  m_firstLimitToMax  = HermiteInterp1D::CreateInstance(m_firstLimitPosition, m_firstMaxPosition, 0.0, m_firstMax, m_firstLimitSlope, 0.0);
  m_firstCenter      = HermiteInterp1D::CreateInstance(m_firstMaxPosition, m_centerPosition, m_firstMax, 0.0, 0.0, m_centerSlope);
  m_secondCenter     = HermiteInterp1D::CreateInstance(m_centerPosition, m_secondMaxPosition, 0.0, m_secondMax, m_centerSlope, 0.0);
  m_secondLimitToMax = HermiteInterp1D::CreateInstance(m_secondMaxPosition, m_secondLimitPosition, m_secondMax, 0.0, 0.0, m_secondLimitSlope);
}

Real BondForce::GetForce(Real x, PositionType& positionType) {
  Real returnValue = 0.0;
  positionType = BondUnbroken;

  if(x > m_centerPosition) {
    if(x < m_secondMaxPosition) {
      returnValue = m_secondCenter->Interpolate(x);
    }
    else if(x < m_secondLimitPosition) {
      returnValue = m_secondLimitToMax->Interpolate(x);
      // Log(L"WWW: after second max");
    }
    else {
      positionType = BondBrokenTowardsSecond;
    }
  }
  else {
    if(x > m_firstMaxPosition) {
      returnValue = m_firstCenter->Interpolate(x);
    }
    else if(x > m_firstLimitPosition) {
      returnValue = m_firstLimitToMax->Interpolate(x);
      // Log(L"WWW: after first max");
    }
    else {
      positionType = BondBrokenTowardsFirst;
    }
  }
  return returnValue;
}
