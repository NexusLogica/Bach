/**********************************************************************

File     : BetatronDefs.h
Project  : Memoria Logica PlaySpace Library
Purpose  : Header file for PlaySpace common definitions.
Revisions: Original definition by L. Gunn.
           2012/01/05

Copyright (c) 2012 by Lawrence Gunn
All Rights Reserved.

*/
#ifndef __BACH_BETATRON_DEFS_H__
#define __BACH_BETATRON_DEFS_H__

#include <boost/smart_ptr.hpp>
#include <vector>
#include "Eigen/Dense"

namespace Bach {
  typedef double Real;

  const Real NXGR_PI            = 3.1415926535897932384626433832795f;
  const Real NXGR_E             = 2.71828182845904523536f;
  const Real NXGR_RAD_TO_DEG    = 57.295779513082320876798154814105f;
  const Real NXGR_DEG_TO_RAD    = 0.017453292519943295769236907684886f;
  const Real DEG_2_RAD_HALVED   = 0.0087266462599716478846184538424431f;

  const Real PICOMETERS_TO_METERS = 1.0e-12;
  const Real METERS_TO_ANGSTROMS = 1.0e10;
  const Real METERS_TO_PICOMETERS = 1.0e12;
  const Real NEWTONS_TO_NANONEWTONS = 1.0e9;
  const Real NANONEWTONS_TO_NEWTONS = 1.0e-9;
  const Real COULOMB_METERS_TO_DEBYES = 2.9979246e29;
  const Real ELECTRIC_CHARGE = 1.602176565e-19; // Coulombs per electron
  const Real K_VACUUM = 8.987551787368176e9; // N m^2 / C^2 Electric permittivity
  const Real ELECTRON_MASS = 9.10938291e-31; // kg
  const Real PERMEABILITY = 1.2566370614e-6; // H/m (Henry's per meter) or N/A/A (Newtons per Ampere)
  const Real PERMIABILITY_OVER_4PI = PERMEABILITY/(4.0*NXGR_PI);
  const Real SPEED_OF_LIGHT = 299792458.0; // m/s
  const Real SPEED_OF_LIGHT_SQUARED = 89875517873681764.0; // m^2/s^2
  
  // These values are for use when setting step sizes for distance or time related algorithms.
  // The characteristic unit is a rough starting point but can be larger or smaller.
  const Real CHARACTERISTIC_BOND_DISTANCE_UNIT = 1.0e-12; // 0.01 Angstroms, so 1/100th of the length of a water bond.

  enum Axis {
    X = 1,
    Y = 2,
    Z = 3
  };

  inline Real Square(Real x) { return x*x; }
  inline Real SignOf(Real x) { return (x >= 0.0f ? 1.0f : -1.0f); }

  class ILog {
  public:
    virtual ~ILog() {}
    virtual void Write(const wchar_t* message) = 0;
    virtual void WritePlain(const wchar_t* message) = 0;
  };

  void Log(const wchar_t* formatString, ...);
  void LogPlain(const wchar_t* formatString, ...);
  void LogVaArgs(const wchar_t* formatString, va_list vaList);

  std::wstring WideStringFromUTF8(const std::string& in);
  std::string UTF8FromWideString(const std::wstring& in);

  class OdeEquations;
  class OdeSolver;
  class OdeDerivatives;
  class OdeAccuracySpec;
  class OdeData;
  class OdeDataCollector;
  class SampledDerivedData;
  class SampledData;
  class TableSearch;
  class Interpolator;
  class OdeNumericalDerivatives;
  class MultivariateDerivatives;
  class NDimAccuracySpec;
  class Jacobian;
  class RootSolverEquations;
  class RootSolverEquationsOdeWrapper;
  class NDimNewtonRaphson;
  
  std::wstring ToString(const Eigen::MatrixXd& mat);
  std::wstring VecXdToString(const Eigen::VectorXd& vec);
  std::wstring Vec3dToString(const Eigen::Vector3d& vec);
  std::wstring Vec3dToStringFixed(const Eigen::Vector3d& vec);
  bool IsApproxEqual(const Eigen::VectorXd& vec1, const Eigen::VectorXd& vec2, double tolerance = 0.0001);
  bool IsApproxEqual(const Eigen::Vector4d& vec1, const Eigen::Vector4d& vec2, double tolerance = 0.0001);

  // Bond force version to use.
  //   Version 1:
  //     A basic symetrical equation with a max force and limit force. All intermediate
  //     values determined Hermitian interpolation.
  //   Version 2:
  //     Similar to the one above except
  //       a) It is treated as including nucleus and sp forces, so they are no longer
  //          included in the force calculations.
  //       b) The curves are not symetrical, nor is the zero force in the middle of the
  //          bond.
  //       c) The configuration is not parametric. The original values were because
  //          the offset in position in polar molecules would come about due to differential
  //          charge on atoms. Now the differential is implicit in the equation.

  #define __BOND_FORCE_VERSION__ 2
}

#include <cstring>

#ifdef _DEBUG
# include <assert.h>
# define BACH_PRECONDITION(test)   assert(test)
# define BACH_POSTCONDITION(test)  assert(test)
# define BACH_ASSERT(test)         assert(test)
#else
# define BACH_PRECONDITION(test)
# define BACH_POSTCONDITION(test)
# define BACH_ASSERT(test)
#endif // _DEBUG

#endif // __BACH_BETATRON_DEFS_H__
