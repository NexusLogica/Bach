/**********************************************************************

File     : ChargeCircuitFieldCalculator.h
Project  : Bach Simulation
Purpose  : Header file for flow through a planar circuit.
Revisions: Original definition by Lawrence Gunn.
           2015/02/07
 
Copyright (c) 2015 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_CHARGE_CIRCUIT_FIELD_CALCULATOR_H__
#define __BACH_CHARGE_CIRCUIT_FIELD_CALCULATOR_H__

#include "RootSolverEquations.h"
#include "Element.h"

namespace Bach {
  
  //******************************
  //* MagneticFieldVectorAtPoint *
  //******************************

  class MagneticFieldVectorAtPoint {
  public:
    Eigen::Vector4d point;
    Eigen::Vector4d magneticField;
  };

  class ChargeMagneticField;

  //********************************
  //* ChargeCircuitFieldCalculator *
  //********************************
  
  class ChargeCircuitFieldCalculator {
  public:
  
    static boost::shared_ptr<ChargeCircuitFieldCalculator> CreateInstance();

    ~ChargeCircuitFieldCalculator();

    void SetCharge(double chargeMagnitude, const Eigen::Vector4d& position, const Eigen::Vector4d& velocity);

    void SetCircuit(
                    const Eigen::Matrix4d& centerTransform,
                    double width,    // in centerTransform x direction
                    double length);  // in centerTransform y direction

    MagneticFieldVectorAtPoint GetMagneticFieldAtCircuitCoord(double widthCoord, double lengthCoord);
    
    std::vector<MagneticFieldVectorAtPoint> GetFieldVectorsOnGrid(int samplesWidthwise, int samplesLengthwise);

  protected:
    ChargeCircuitFieldCalculator();
    
    Eigen::Vector4d GetPointFromCircuitCoord(double widthCoord, double lengthCoord);
    
    boost::shared_ptr<ChargeMagneticField> m_magneticField;
    Eigen::Matrix4d m_centerTransform;
    double m_width;    // in centerTransform x direction
    double m_length;  // in centerTransform y direction
  };
};

#endif // __BACH_CHARGE_CIRCUIT_FIELD_CALCULATOR_H__
