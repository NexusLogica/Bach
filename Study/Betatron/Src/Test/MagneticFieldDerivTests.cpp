/**********************************************************************

File     : MagneticFieldDerivTests.cpp
Project  : Bach Simulation
Purpose  : Source file for a molecule equilibrium solver - solve for x where v,a are zero.
Revisions: Original definition by Lawrence Gunn.
           2013/11/07

Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#include "MagneticFieldDerivTests.h"
#include "RiddersExtrapolation.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

namespace {
  const Integer NUM_INDUCTANCE_EQUATIONS = 3;
  const Integer NUM_INDUCTANCE_VARIABLES = 6;
  const Integer RAND_GENERATOR_SEED = 42;
}

  //***************************
  //* MagneticFieldDerivTests *
  //***************************

shared_ptr<MagneticFieldDerivTests> MagneticFieldDerivTests::CreateInstance() {
  shared_ptr<MagneticFieldDerivTests> instance(new MagneticFieldDerivTests);
  instance->Initialize();
  return instance;
}

MagneticFieldDerivTests::MagneticFieldDerivTests() :
  m_randUniform(RandomUniform::CreateInstance(RandomGenerator::CreateInstance(RAND_GENERATOR_SEED))),
  m_numIterations(10),
  m_toleranceScaleFactor(1.0e-7),
  m_tolerance(0.01),
  m_success(false),
  m_logAll(false)
{
}

MagneticFieldDerivTests::~MagneticFieldDerivTests() {
}

void MagneticFieldDerivTests::Initialize() {
  m_variables.push_back(shared_ptr<MagneticFieldDerivTestVariable>(new MagneticFieldDerivTestVariable(L"Vx", 0, -10.0, 10.0, -1.0, 1.0, 0.01)));
  m_variables.push_back(shared_ptr<MagneticFieldDerivTestVariable>(new MagneticFieldDerivTestVariable(L"Vy", 0, -10.0, 10.0, -1.0, 1.0, 0.01)));
  m_variables.push_back(shared_ptr<MagneticFieldDerivTestVariable>(new MagneticFieldDerivTestVariable(L"Vz", 0, -10.0, 10.0, -1.0, 1.0, 0.01)));
  m_variables.push_back(shared_ptr<MagneticFieldDerivTestVariable>(new MagneticFieldDerivTestVariable(L"rx", 0, -2.0, 2.0, -0.4, 0.4, 0.001)));
  m_variables.push_back(shared_ptr<MagneticFieldDerivTestVariable>(new MagneticFieldDerivTestVariable(L"ry", 0, -2.0, 2.0, -0.4, 0.4, 0.001)));
  m_variables.push_back(shared_ptr<MagneticFieldDerivTestVariable>(new MagneticFieldDerivTestVariable(L"rz", 0, -2.0, 2.0, -0.4, 0.4, 0.001)));
  
  m_chargeMagneticField = ChargeMagneticField::CreateInstance(1.0);
}

bool MagneticFieldDerivTests::RunTests() {
  m_success = true;
  Eigen::VectorXd values(NUM_INDUCTANCE_VARIABLES);
  Eigen::VectorXd derivs(NUM_INDUCTANCE_VARIABLES);

  for(Integer iter=0; iter<m_numIterations; iter++) {

    for(Integer j=0; j<NUM_INDUCTANCE_VARIABLES; j++) {
      Real value = m_variables[j]->GetNewValue(m_randUniform);
      values(j) = value;

      Real deriv = m_variables[j]->GetNewDerivativeValue(m_randUniform);
      derivs(j) = deriv;
    }
    Eigen::VectorXd approx = GetApproximatedDerivative(values, derivs);
    if(m_logAll) {
      Log(L"Approx derivative: %s", VecXdToString(approx).c_str());
    }

    Eigen::VectorXd exact = GetExactDerivative(values, derivs);
    if(m_logAll) {
      Log(L"Exact  derivative: %s", VecXdToString(exact).c_str());
    }

    Eigen::VectorXd scaledDiff = (exact.array()-approx.array()).array()/m_toleranceScaleFactor;
    Real maxDiff = scaledDiff.array().abs().maxCoeff();
    if(maxDiff > m_tolerance || m_logAll) {
      Log(L"Iteration %d", iter);
      Log(L"%s[%e]: %e", (maxDiff > m_tolerance ? L"FAILED" : L"Success"), maxDiff, maxDiff);
      if(maxDiff > m_tolerance) { m_success = false; }
    }
  }
  return m_success;
}

Eigen::VectorXd MagneticFieldDerivTests::GetExactDerivative(const Eigen::VectorXd& values, const Eigen::VectorXd& derivatives) {
  // d(V x r)/dt = V x dr/dt + dV/dt x r
  Eigen::Vector3d V;
  V(0) = values(0);
  V(1) = values(1);
  V(2) = values(2);
  Eigen::Vector3d r;
  r(0) = values(3);
  r(1) = values(4);
  r(2) = values(5);

  Real dVxdt = derivatives(0);
  Real dVydt = derivatives(1);
  Real dVzdt = derivatives(2);

  Real drxdt = derivatives(3);
  Real drydt = derivatives(4);
  Real drzdt = derivatives(5);

  Real rLength = r.norm();
  Real rLengthCubed = rLength*rLength*rLength;
  Real rLengthCubedInv = 1.0/rLengthCubed;
  Real rLengthFifth = rLengthCubed*rLength*rLength;
  Real rLengthFifthInv = 1.0/rLengthFifth;
  
  Real crossX = V(1)*r(2)-V(2)*r(1);
  Real crossY = V(2)*r(0)-V(0)*r(2);
  Real crossZ = V(0)*r(1)-V(1)*r(0);

  Eigen::VectorXd dBdt(NUM_INDUCTANCE_EQUATIONS);
  dBdt(0) =
    dVydt*(r(2)*rLengthCubedInv) +
    dVzdt*(-r(1)*rLengthCubedInv) +
    drxdt*(-3.0*r(0) * crossX*rLengthFifthInv) +
    drydt*(-V(2)*rLengthCubedInv - 3.0*r(1)*crossX*rLengthFifthInv) +
    drzdt*(V(1)*rLengthCubedInv - 3.0*r(2)*crossX*rLengthFifthInv);

  dBdt(1) =
    dVxdt*(-r(2)*rLengthCubedInv) +
    dVzdt*(r(0)*rLengthCubedInv) +
    drxdt*(V(2)*rLengthCubedInv - 3.0*r(0)*crossY*rLengthFifthInv) +
    drydt*(-3.0*r(1) * crossY*rLengthFifthInv) +
    drzdt*(-V(0)*rLengthCubedInv - 3.0*r(2)*crossY*rLengthFifthInv);

  dBdt(2) =
    dVxdt*(r(1)*rLengthCubedInv) +
    dVydt*(-r(0)*rLengthCubedInv) +
    drxdt*(-V(1)*rLengthCubedInv - 3.0*r(0)*crossZ*rLengthFifthInv) +
    drydt*(V(0)*rLengthCubedInv - 3.0*r(1)*crossZ*rLengthFifthInv) +
    drzdt*(-3.0*r(2) * crossZ*rLengthFifthInv);

  dBdt.array() *= PERMIABILITY_OVER_4PI;
  
  Eigen::Vector3d dVdt;
  dVdt(0) = dVxdt;
  dVdt(1) = dVydt;
  dVdt(2) = dVzdt;
  Eigen::Vector3d drdt;
  drdt(0) = drxdt;
  drdt(1) = drydt;
  drdt(2) = drzdt;
  m_chargeMagneticField->SetVelocity(V);
  m_chargeMagneticField->SetAcceleration(dVdt);
  Eigen::Vector3d exactExternal = m_chargeMagneticField->GetMagneticFieldChangeVector(r, drdt);

  if(m_logAll) {
    Log(L"GetExactDerivative: Internal value: %s", VecXdToString(dBdt).c_str());
    Log(L"GetExactDerivative: External value: %s", VecXdToString(exactExternal).c_str());
  }
  
  Eigen::VectorXd scaledDiff = (dBdt.array()-exactExternal.array()).array()/m_toleranceScaleFactor;
  Real maxDiff = scaledDiff.array().abs().maxCoeff();
  if(maxDiff > m_tolerance) {
    Log(L"GetFunctionValue: %s[%e]: %e", (maxDiff > m_tolerance ? L"FAILED" : L"Success"), maxDiff, maxDiff);
    m_success = false;
  }

  if(m_logAll) {
    for(Integer i=0; i<NUM_INDUCTANCE_EQUATIONS; i++) {
      Log(L"*** Exact [%d]: %e", i, dBdt(i));
    }
  }
  return dBdt;
}

Eigen::VectorXd MagneticFieldDerivTests::GetExactPartialDerivative(Integer index, const Eigen::VectorXd& values) {
  // d(V x r)/dt = (1/len) * ( V x dr/dt + dV/dt x r + (V x r) )
  Eigen::Vector3d V(3);
  V(0) = values(0);
  V(1) = values(1);
  V(2) = values(2);
  Eigen::Vector3d r(3);
  r(0) = values(3);
  r(1) = values(4);
  r(2) = values(5);
  
  Real rLength = r.norm();
  Real rLengthCubed = rLength*rLength*rLength;
  Real rLengthCubedInv = 1.0/rLengthCubed;
  Real rLengthFifth = rLengthCubed*rLength*rLength;
  Real rLengthFifthInv = 1.0/rLengthFifth;
  
  Real crossX = V(1)*r(2)-V(2)*r(1);
  Real crossY = V(2)*r(0)-V(0)*r(2);
  Real crossZ = V(0)*r(1)-V(1)*r(0);
  
  Eigen::VectorXd dBdvar(NUM_INDUCTANCE_EQUATIONS);
  dBdvar.fill(0.0);
  switch(index) {
    case 0:              // dB/dVx
      dBdvar(1) = -r(2)*rLengthCubedInv;
      dBdvar(2) =  r(1)*rLengthCubedInv;
      break;
    case 1:              // dB/dVy
      dBdvar(0) =  r(2)*rLengthCubedInv;
      dBdvar(2) = -r(0)*rLengthCubedInv;
      break;
    case 2:              // dB/dVz
      dBdvar(0) = -r(1)*rLengthCubedInv;
      dBdvar(1) =  r(0)*rLengthCubedInv;
      break;
    case 3:              // dB/drx
      dBdvar(0) = -3.0*r(0) * crossX*rLengthFifthInv;
      dBdvar(1) =  V(2)*rLengthCubedInv - 3.0*r(0)*crossY*rLengthFifthInv;
      dBdvar(2) = -V(1)*rLengthCubedInv - 3.0*r(0)*crossZ*rLengthFifthInv;
      break;
    case 4:              // dB/dry
      dBdvar(0) = -V(2)*rLengthCubedInv - 3.0*r(1)*crossX*rLengthFifthInv;
      dBdvar(1) = -3.0*r(1) * crossY*rLengthFifthInv;
      dBdvar(2) =  V(0)*rLengthCubedInv - 3.0*r(1)*crossZ*rLengthFifthInv;
      break;
    case 5:              // dB/drz
      dBdvar(0) =  V(1)*rLengthCubedInv - 3.0*r(2)*crossX*rLengthFifthInv;
      dBdvar(1) = -V(0)*rLengthCubedInv - 3.0*r(2)*crossY*rLengthFifthInv;
      dBdvar(2) = -3.0*r(2) * crossZ*rLengthFifthInv;
      break;
  }

  dBdvar.array() *= PERMIABILITY_OVER_4PI;
  return dBdvar;
}

Eigen::VectorXd MagneticFieldDerivTests::GetApproximatedDerivative(const Eigen::VectorXd& values, const Eigen::VectorXd& derivatives) {
  shared_ptr<NDimAccuracySpec> accSpec(new NDimAccuracySpec(NUM_INDUCTANCE_EQUATIONS, 0.00001));

  // A list of derivative vector, so for variable 2, the values are approxDerivsList[2](0), (1), (2)
  std::vector< Eigen::VectorXd > approxDerivsList;
  approxDerivsList.resize(NUM_INDUCTANCE_VARIABLES);
  
  Eigen::VectorXd derivsTemp(NUM_INDUCTANCE_EQUATIONS);

  for(Integer i=0; i<NUM_INDUCTANCE_VARIABLES; i++) {
    shared_ptr<RiddersExtrapolation> derivFinder(new RiddersExtrapolation(NUM_INDUCTANCE_EQUATIONS));
    
    Eigen::VectorXd valuesCopy = values;
    
    derivFinder->Start(valuesCopy(i), m_variables[i]->GetDerivStep());
    for(int j=0; j<200; j++) {
      Real newVal = derivFinder->GetNewValue();
      valuesCopy(i) = newVal;
      Eigen::VectorXd funcValues = GetFunctionValue(valuesCopy);
      derivFinder->SetFunctionValues(funcValues);

      if(derivFinder->GetIsFinished(accSpec)) {
        break;
      }
    }
    derivsTemp = derivFinder->GetDerivatives();
    approxDerivsList[i] = derivsTemp;

    Eigen::VectorXd exactDeriv = GetExactPartialDerivative(i, values);
  }
  
  // Fill these with the approximated partial derivatives.
  Eigen::VectorXd approximatedPartialDerivatives(NUM_INDUCTANCE_EQUATIONS);
  
  for(Integer i=0; i<NUM_INDUCTANCE_EQUATIONS; i++) {
    Real sum = 0.0;
    for(Integer j=0; j<NUM_INDUCTANCE_VARIABLES; j++) {
      Eigen::VectorXd t = approxDerivsList.at(j);
      sum += t(i) * derivatives(j);
    }
    approximatedPartialDerivatives(i) = sum;
  }

  return approximatedPartialDerivatives;
}

Eigen::Vector3d MagneticFieldDerivTests::GetFunctionValue(const Eigen::VectorXd& values) {
  // V x ^r
  //   which is equal to
  // V x r/len(r)
  Eigen::Vector3d V(3);
  V(0) = values(0);
  V(1) = values(1);
  V(2) = values(2);
  Eigen::Vector3d r(3);
  r(0) = values(3);
  r(1) = values(4);
  r(2) = values(5);

  m_chargeMagneticField->SetVelocity(V);
  Eigen::Vector3d externalValue = m_chargeMagneticField->GetMagneticFieldVector(r);
/*
  Real rLenSquared = r.squaredNorm();

//  Log(L"GFV: r      = %s", VecXdToString(r).c_str());

  r.normalize();

  Eigen::Vector3d vrCross = V.cross(r);

//  Log(L"GFV: V      = %s", VecXdToString(V).c_str());
//  Log(L"GFV: r norm = %s", VecXdToString(r).c_str());
//  Log(L"GFV: V x r  = %s", VecXdToString(vrCross).c_str());
//  Log(L"GFV: r len  = %e", sqrt(rLenSquared));
  
  vrCross.array() *= (PERMIABILITY_OVER_4PI/rLenSquared);

//  Log(L"GetFunctionValue: Internal value: %s", VecXdToString(vrCross).c_str());
//  Log(L"GetFunctionValue: External value: %s", VecXdToString(externalValue).c_str());
  
  Eigen::VectorXd scaledDiff = (vrCross.array()-externalValue.array()).array()/m_toleranceScaleFactor;
  Real maxDiff = scaledDiff.array().abs().maxCoeff();
  if(maxDiff > m_tolerance) {
    Log(L"GetFunctionValue: %s[%e]: %e", (maxDiff > m_tolerance ? L"FAILED" : L"Success"), maxDiff, maxDiff);
  }
*/
  return externalValue;
}
