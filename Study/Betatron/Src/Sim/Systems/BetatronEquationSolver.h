/**********************************************************************

File     : BetatronEquationSolver.h
Project  : Bach Simulation
Purpose  : Header file for the ODE solver for a charge in a betatron.
Revisions: Original definition by Lawrence Gunn.
           2014/11/06


Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_BETATRON_EQUATION_SOLVER_H__
#define __BACH_BETATRON_EQUATION_SOLVER_H__

#include "BetatronEquations.h"

namespace Bach {

  class BetatronEquations;
  class BetatronFieldController;
  class BaderDeuflhardOde;
  class OdeData;

  //**************************
  //* BetatronEquationSolver *
  //**************************
  
  class BetatronEquationSolver {
  public:
  
    static boost::shared_ptr<BetatronEquationSolver> CreateInstance();

    virtual ~BetatronEquationSolver();
    
    void SetEndTime(double endTime) { m_endTime = endTime; }
    void SetInitialConditionsFromRadiusAndSpeed(double radius, double speed);
    void SetFieldIncreaseRatePerRotation(double fieldIncreaseRatePerRotation) { m_fieldIncreaseRatePerRotation = fieldIncreaseRatePerRotation; }
    void SetNumRotations(double numRotations) { m_numRotations = numRotations; }
    
    void Initialize();
    void Run();

    boost::shared_ptr<OdeData> GetOdeData() { return m_odeData; }

  protected:
    BetatronEquationSolver();
    
    boost::weak_ptr<BetatronEquationSolver> m_weakThis;

    boost::shared_ptr<BetatronEquations> m_equations;
    boost::shared_ptr<BetatronFieldController> m_fieldController;
    boost::shared_ptr<BaderDeuflhardOde> m_solver;
    boost::shared_ptr<OdeData> m_odeData;
    double m_startTime;
    double m_endTime;
    double m_magneticFieldMagnitude;
    double m_fieldIncreaseRatePerRotation;
    double m_fieldIncreaseRate;
    double m_numRotations;
    Eigen::VectorXd m_initialPosition;
    Eigen::VectorXd m_initialVelocity;
    double m_stepSize;
    int m_iterationCount;
  };
};

#endif // __BACH_BETATRON_EQUATION_SOLVER_H__
