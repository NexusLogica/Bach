/**********************************************************************

File     : BondElectrons.cpp
Project  : Bach Simulation
Purpose  : Source file for a the two electrons of a sigma bond.
Revisions: Original definition by Lawrence Gunn.
           2013/06/23

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "BondElectrons.h"
#include "BondForce.h"
#include "ChargeMagneticField.h"
#include "Atom.h"
#include "Molecule.h"
#include "ForceVectors.h"
#include <sstream>

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*****************
  //* BondElectrons *
  //*****************

shared_ptr<BondElectrons> BondElectrons::CreateInstance(boost::shared_ptr<Bond> bond, int bondId) {
  shared_ptr<BondElectrons> instance(new BondElectrons(bond, bondId));
  return instance;
}

BondElectrons::BondElectrons(boost::shared_ptr<Bond> bond, int bondId) :
  Element(Element::ElectronPairElement),
  m_bond(bond),
  m_bondId(bondId),
  m_charge(-2.0)
{
  m_chargeMagneticField = ChargeMagneticField::CreateInstance(m_charge);

  std::wstringstream nameStream;
  nameStream << L"BE-";
  shared_ptr<Atom> first = m_bond->GetFirst();
  shared_ptr<Atom> second = m_bond->GetSecond();
  nameStream << first->GetMolecule()->GetType() << L":" << first->GetShortName() << L">" << second->GetShortName() << L"-";
  nameStream << first->GetMolecule()->GetId() << ":" << first->GetId() << L">" << second->GetId();
  SetName(nameStream.str());
}

void BondElectrons::SetLinearPosition(Real position) {
  m_linearPosition = position;
  m_position = m_bond->GetBondDirection()*m_linearPosition+m_bond->GetFirst()->GetPosition();
  m_chargeMagneticField->SetPosition(m_position);
}

void BondElectrons::SetLinearVelocity(Real velocity) {
  m_linearVelocity = velocity;
  m_velocity = m_bond->GetBondDirection()*m_linearVelocity;
  m_chargeMagneticField->SetVelocity(m_velocity);
}

void BondElectrons::SetLinearAcceleration(Real accel) {
  m_linearAcceleration = accel;
  m_acceleration = m_bond->GetBondDirection()*m_linearAcceleration;
  m_chargeMagneticField->SetAcceleration(m_acceleration);
}

// DEAD - Wrong direction...
// Mass doesn't matter - inductance does.
Real BondElectrons::CalculateAcceleration() {
/*
  // Two electrons moving at relative velocity m_velocity has the effective mass as follows.
  Real mass = 2.0*ELECTRON_MASS*sqrt(1.0-Square(m_linearVelocity)/SPEED_OF_LIGHT_SQUARED);
  
  BondForce::PositionType positionType;
  m_bondForce = m_bond->GetBondForceObject()->GetForce(m_linearPosition, positionType);
  if(positionType != BondForce::BondUnbroken) {
    m_bondForce = 0.0;
  }

  Eigen::Vector3d forceSum;
  forceSum.setZero();
  
  // The force from electical and magnetic forces need to be summed. Start with atom nuclei.
  shared_ptr<Molecule> molecule = m_bond->GetFirst()->GetMolecule();
  int numAtoms = molecule->GetNumAtoms();
  shared_ptr<Atom> atom;
  Eigen::Vector3d forceVector;
  for(int i = 0; i<numAtoms; i++) {
    atom = molecule->GetAtomByIndex(i);
    forceVector = CalculateElectricalForce(atom->GetPosition(), atom->GetCharge());
    forceSum.array() += forceVector.array();
  }
  
  // And now sum forces from other bond electrons.
  int numBonds = molecule->GetNumBonds();
  shared_ptr<BondElectrons> bondElectrons;
  for(int i = 0; i<numBonds; i++) {
    bondElectrons = molecule->GetBondByIndex(i)->GetBondElectrons();
    if(bondElectrons->GetBondId() != m_bondId) {
      forceVector = CalculateElectricalForce(bondElectrons->GetPosition(), m_charge);
    }
    forceSum.array() += forceVector.array();
  }
  
  // Calculate the electric field forces in the direction of the bond.
  m_electricalInducedForce = m_bond->GetBondDirection().dot(forceSum);
  
  // Repeat for magnetic field forces.
  m_magneticInducedForce = 0.0;

  Real force = m_electricalInducedForce+m_magneticInducedForce+m_bondForce;
  Real accel = force/mass;
  return accel;
*/
  return 0.0;
}

Eigen::Vector3d BondElectrons::CalculateElectricalForce(
  Real charge,
  const Eigen::Vector3d& chargePosition,
  shared_ptr<ForceVectors> forceVectorStorage)
{
  Eigen::Vector3d forceDirection = chargePosition.array()-m_position.array();
  Real rSquared = forceDirection.squaredNorm();
  
  // The 2.0 is for the local electron charge.
  Real forceMagnitude = K_VACUUM*charge*m_charge*Square(ELECTRIC_CHARGE)/rSquared;
  
  // The force is in the direction of the charge.
  forceDirection.normalize();
  Eigen::Vector3d forceVector = forceDirection.array()*forceMagnitude;
  if(forceVectorStorage) {
    forceVectorStorage->AddForceVector(GetName(), forceVector);
  }
  return forceVector;
}

// Calculate the magnetically induced forces on a point charge with a position and velocity.
Eigen::Vector3d BondElectrons::CalculateMagneticInducedForce(
  Real charge,
  const Eigen::Vector3d& chargePosition,
  const Eigen::Vector3d& chargeVelocity,
  boost::shared_ptr<ForceVectors> forceVectorStorage)
{
  bool directionBisValid = false;
  Eigen::Vector3d directionB = m_chargeMagneticField->GetMagneticFieldDirectionVector(chargePosition, directionBisValid);
  if(!directionBisValid) {
    Eigen::Vector3d zero;
    zero.fill(0.0f);
    return zero;
  }
  Log(L"***********");
  Log(L"direction B = %s", VecXdToString(directionB).c_str());
  Eigen::Vector3d dBdt = m_chargeMagneticField->GetMagneticFieldChangeVector(chargePosition, chargeVelocity);
  Log(L"dBdt = %s", VecXdToString(dBdt).c_str());
  Eigen::Vector3d emf = directionB.cross(dBdt);
  Log(L"emf = %s", VecXdToString(emf).c_str());
  emf.array() *= charge;

  return emf;
}
