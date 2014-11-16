/**********************************************************************

File     : BaderDeuflhardOde.h
Project  : Bach Simulation
Purpose  : Header file for the Bader-Deurflhard stiff system solver based on the Burlirsch-Stoer solver.
Revisions: Original definition by Lawrence Gunn.
           1992/11/01.
           This class implements an ordinary differential equation solver
           which employs a semi-implicit extrapolation method due to Bader
           and Deuflhard.  It is similar to the Bulirsch-Stoer method 
           except that it excels at stiff systems of equations.
           See Numerical Recipes, 2nd Edition, pp 735-739.

           Updated by Lawrence Gunn.
           2013/01/26


Copyright (c) 1992-2012 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_BADER_DEUFLHARD_ODE_H__
#define __BACH_BADER_DEUFLHARD_ODE_H__

#include "BetatronDefs.h"
#include "OdeSolverWithDerivs.h"

namespace Bach {

   //*********************
   //* BaderDeuflhardOde *
   //*********************

  class BaderDeuflhardOde : public OdeSolverWithDerivs {
  public:
    static boost::shared_ptr<BaderDeuflhardOde> CreateInstance();
    virtual ~BaderDeuflhardOde();

    void Solve(boost::shared_ptr<OdeData> odeData, bool reset = true);

    void SetStepSize(double stepSize)            {  m_initialStepSize = stepSize;  }
    void SetMinimumStepSize(double minStepSize)  {  m_minStepSize = minStepSize;  }
    void SetMaximumStepSize(double maxStepSize)  {  m_maxStepSize = maxStepSize;  }

    int GetNumberGood()       {  return m_numberGood;  }
    int GetNumberRetried()    {  return m_numberRetried;  }
    int GetNumberAtMinimum()  {  return m_numberAtMinimum;  }
    int GetNumberAtMaximum()  {  return m_numberAtMaximum;  }

  protected:

    BaderDeuflhardOde();
    void SolveStep(double x, boost::shared_ptr<OdeData> odeData);
    //void TakeBaderDeuflhardStep(double x,const Eigen::VectorXd& ,const Eigen::VectorXd&, boost::shared_ptr<OdeData> data);
    void TakeSemiImplicitStep(int numberOfSteps, double xStart,const Eigen::VectorXd& yIn,const Eigen::VectorXd& dyIn, Eigen::VectorXd& yToReturn, boost::shared_ptr<OdeData> odeData);
    virtual void Extrapolate(int iFromStep, double xFromStep, const Eigen::VectorXd& yFromStep, Eigen::VectorXd& yToReturn, Eigen::VectorXd& yErrorEstimate);

    boost::weak_ptr<BaderDeuflhardOde> m_weakThis;

    bool m_initialized;

    int m_maxStepIncrements;
    int m_usedStepIncrements;

    double m_makeSmallerFactor;
    double m_makeBiggerFactor;

    double m_minStepSize;
    double m_maxStepSize;
    double m_initialStepSize;
    double m_lastStepSize;
    double m_nextStepSize;
    bool   m_ascending;

    Eigen::MatrixXd m_interpTable;
    Eigen::VectorXd m_xInterpTable;  // used by the rational extrapolation routine
    Eigen::VectorXd m_fxInterpTable;  // used by the rational extrapolation routine

    Eigen::VectorXd m_dfdx;
    Eigen::MatrixXd m_dfdy;
    Eigen::MatrixXd m_alf;
    Eigen::MatrixXd m_aSimStep;
    Eigen::VectorXd m_a;

    Eigen::VectorXd m_y;
    Eigen::VectorXd m_ySav;
    Eigen::VectorXd m_dydx;
    Eigen::VectorXd m_yTemp;
    Eigen::VectorXd m_yTemp2;
    Eigen::VectorXd m_ySum;
    Eigen::VectorXd m_delta;
    Eigen::VectorXd m_yResult;
    Eigen::VectorXd m_yError;
    Eigen::VectorXd m_yScale;
    Eigen::VectorXd m_extrapC;

    int m_numberGood;
    int m_numberRetried;
    int m_numberAtMinimum;
    int m_numberAtMaximum;

    Eigen::VectorXi m_stepSequence;

    int m_first;
    Eigen::VectorXd m_err;
int kmax;
int m_kopt;
int m_nvold;
double xest;
double m_eps;
double m_epsold;
double m_xnew;

    static double s_defaultMakeSmallerFactor;
    static double s_defaultMakeBiggerFactor;
    static int s_defaultMaxStepIncrements;
    static int s_defaultUsedStepIncrements;
  };
};

#endif // __BACH_BADER_DEUFLHARD_ODE_H__
