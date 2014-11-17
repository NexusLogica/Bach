/**********************************************************************

File     : BaderDeuflhardOde.cpp
Project  : Bach Simulation
Purpose  : Source file for the Bader-Deurflhard stiff system solver based on the Burlirsch-Stoer solver.
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

#include "BaderDeuflhardOde.h"
#include "OdeDerivatives.h"
#include "OdeData.h"
#include "OdeAccuracySpec.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

namespace {
  const int KMAXX=7;
  const int IMAXX=8;
  const double SAFE1 = 0.25;
  const double SAFE2 = 0.7;
  const double REDMAX = 1.0e-5;
  const double REDMIN = 0.7;
  const double TINY=1.0e-30;
  const double SCALMX=0.1;

  double min(double a, double b) { return (a < b ? a : b); }
  double max(double a, double b) { return (a > b ? a : b); }
}

   //*********************
   //* BaderDeuflhardOde *
   //*********************

int BaderDeuflhardOde::s_defaultMaxStepIncrements = 8;
int BaderDeuflhardOde::s_defaultUsedStepIncrements = 7;
double BaderDeuflhardOde::s_defaultMakeSmallerFactor = 0.95;
double BaderDeuflhardOde::s_defaultMakeBiggerFactor  = 1.20;

BaderDeuflhardOde::BaderDeuflhardOde() :
  m_initialized(false),
  m_first(1),
  m_xInterpTable(KMAXX),
  m_fxInterpTable(s_defaultUsedStepIncrements),
  m_stepSequence(s_defaultMaxStepIncrements)
{
  m_alf.resize(KMAXX,KMAXX);
  m_alf.fill(0.0);

  m_a.resize(IMAXX);
  m_a.fill(0.0);

  m_err.resize(KMAXX);

  m_maxStepIncrements  = s_defaultMaxStepIncrements;
  m_usedStepIncrements = s_defaultUsedStepIncrements;
  m_makeSmallerFactor  = s_defaultMakeSmallerFactor;
  m_makeBiggerFactor   = s_defaultMakeBiggerFactor;

  m_stepSequence << 2, 6, 10, 14, 22, 34, 50, 70;

  m_initialStepSize = 0.001;
  m_stepSize = m_initialStepSize;
  m_minStepSize = 0.0;
  m_maxStepSize = 1.0e32;

  m_numberGood = 0;
  m_numberRetried = 0;
  m_numberAtMinimum = 0;
  m_numberAtMaximum = 0;

  m_nvold = -1;
  m_epsold = -1.0;
}

BaderDeuflhardOde::~BaderDeuflhardOde()
{
}

boost::shared_ptr<BaderDeuflhardOde> BaderDeuflhardOde::CreateInstance() {
  boost::shared_ptr<BaderDeuflhardOde> instance(new BaderDeuflhardOde());
  instance->m_weakThis = instance;
  return instance;
}

void BaderDeuflhardOde::Solve(boost::shared_ptr<OdeData> odeData, bool reset) {
LogPlain(L"**********************************************************************************************");
  int size = odeData->GetStateLength();
  if(m_y.rows() != size) {
    m_interpTable.resize(size, KMAXX);

    m_dfdx.resize(size);
    m_dfdy.resize(size, size);
    m_aSimStep.resize(size, size);

    m_y.resize(size);
    m_ySav.resize(size);
    m_dydx.resize(size);
    m_yResult.resize(size);
    m_yError.resize(size);
    m_yTemp.resize(size);
    m_yTemp2.resize(size);
    m_ySum.resize(size);
    m_delta.resize(size);
    m_extrapC.resize(size);
    m_yScale.resize(size);
    m_yScale.fill(1.0);
  }

  if(reset || !m_initialized) {
    m_initialized = true;
    odeData->ResetStorage();
    m_ascending = (odeData->GetEndTime()-odeData->GetStartTime() > 0.0 ? true : false);

    double direction = m_ascending ? 1.0 : -1.0;
    m_stepSize = direction*fabs(m_initialStepSize);
    m_minStepSize  = direction*fabs(m_minStepSize);
    m_maxStepSize  = direction*fabs(m_maxStepSize);
    m_nextStepSize = direction*fabs(m_nextStepSize);

    m_numberGood = m_numberRetried = m_numberAtMinimum = m_numberAtMaximum = 0;
  }

  InitializeAccuracySpec(odeData);
  m_eps = GetAccuracySpec()->GetTolerance();

  // Initialize (or create) the OdeDerivatives object.
  InitializeDifferentiator(odeData);

  double x = odeData->GetStartTime();
  m_y = odeData->GetInitialConditions();

  boost::shared_ptr<OdeEquations> system = odeData->GetOdeSystem();

  for(;;) {
    odeData->SetStoringThisCall(true);
    system->Evaluate(x, m_y, m_dydx, odeData);
    odeData->SetStoringThisCall(false);

    SolveStep(x, odeData);
    m_stepSize = m_nextStepSize;
    x += m_lastStepSize;
    if(x > odeData->GetEndTime()) {
      break;
    }
  }
}

void BaderDeuflhardOde::SolveStep(double x, boost::shared_ptr<OdeData> odeData) {
  boost::shared_ptr<OdeEquations> system = odeData->GetOdeSystem();
  bool exitFlag = false;

  double red;
  double fact;
  double errmax;
  double eps1;
  double scale;
  double work;
  int reduct;
  int km;
  int k;

  int nv = (int) m_y.size();

  if(m_eps != m_epsold || nv != m_nvold) {
    m_nextStepSize = m_xnew = -1.0e29;
    eps1 = SAFE1*m_eps;
    m_a(0) = m_stepSequence(0)+1;
    for(k=0;k<KMAXX;k++) {
      m_a(k+1) = m_a(k)+m_stepSequence(k+1);
    }
LogPlain(L"BH m_a = %s", ToString(m_a).c_str());
    for(int iq=1;iq<KMAXX;iq++) {
      for(k=0;k<iq;k++) {
        m_alf(k, iq) = pow(eps1,(m_a(k+1)-m_a(iq+1))/((m_a(iq+1)-m_a(0)+1.0)*(2*k+3)));
      }
    }
    m_epsold = m_eps;
    m_nvold = nv;
    m_a(0) += nv;
    for(int k=0;k<KMAXX;k++) {
      m_a(k+1) = m_a(k)+m_stepSequence(k+1);
    }
    for(m_kopt=1;m_kopt<KMAXX-1;m_kopt++) {
      if(m_a(m_kopt+1) > m_a(m_kopt)*m_alf(m_kopt-1, m_kopt)) {
        break;
      }
    }
    kmax = m_kopt;
LogPlain(L"BH alf/m_alf = %s", ToString(m_alf).c_str());
LogPlain(L"BH a/m_a = %s", ToString(m_a).c_str());
  }

  for(int i=0;i<m_y.rows();i++) {
    m_ySav(i) = m_y(i);
  }

  // was jacobn_s(x,m_y,dfdx,dfdy);
  m_differentiator->GetDerivatives(x, m_y, m_dfdx, m_dfdy, odeData);
LogPlain(L"BH JACOBIAN dfdx/m_dfdx = %s", ToString(m_dfdx).c_str());
LogPlain(L"BH JACOBIAN dfdy/m_dfdy = %s", ToString(m_dfdy).c_str());

  if(x != m_xnew || m_stepSize != m_nextStepSize) {
    m_first=1;
    m_kopt=kmax;
  }
  reduct=0;
  for(;;) {
    for(k=0;k<=kmax;k++) {
      m_xnew=x+m_stepSize;
      if(m_xnew == x) {
        LogPlain(L"ERROR: BaderDeuflhardOde::SolveStep: Step size underflow");
        throw std::exception();
      }

      // was simpr(m_ysav, m_dydx, dfdx,dfdy,x,m_stepSize,m_stepSequence[k],m_yResult, derivs);
LogPlain(L"BH Before simpr ******");
LogPlain(L"  step no = %d",m_stepSequence(k));
LogPlain(L"  x = %f",x);
LogPlain(L"  m_ySav = %s", ToString(m_ySav).c_str());
LogPlain(L"  m_dydx = %s", ToString(m_dydx).c_str());
LogPlain(L"  m_dfdx = %s", ToString(m_dfdx).c_str());

      TakeSemiImplicitStep(m_stepSequence(k), x, m_ySav, m_dydx, m_yResult, odeData);

LogPlain(L"BH After simpr ******");
LogPlain(L"  m_yResult = %s", ToString(m_yResult).c_str());

      xest = m_stepSize/m_stepSequence(k);
      xest *= xest;
      // was pzextr(k,xest,m_yResult,m_y,m_yError);
      Extrapolate(k, xest, m_yResult, m_y, m_yError);
      if(k != 0) {
        errmax=TINY;
        for(int i=0;i<nv;i++) {
          errmax = max(errmax,fabs(m_yError(i)/m_yScale(i)));
        }
        errmax /= m_eps;
        km = k-1;
        m_err(km) = pow(errmax/SAFE1,1.0/(2*km+3));
      }
      if(k != 0 && (k >= m_kopt-1 || m_first)) {
        if(errmax < 1.0) {
          exitFlag=true;
          break;
        }
        if(k == kmax || k == m_kopt+1) {
          red=SAFE2/m_err(km);
          break;
        }
        else if(k == m_kopt && m_alf(m_kopt-1, m_kopt) < m_err(km)) {
          red=1.0/m_err(km);
          break;
        }
        else if(m_kopt == kmax && m_alf(km, kmax-1) < m_err(km)) {
          red=m_alf(km,kmax-1)*SAFE2/m_err(km);
          break;
        }
        else if(m_alf(km, m_kopt) < m_err(km)) {
          red=m_alf(km, m_kopt-1)/m_err(km);
          break;
        }
      }
    }
    if(exitFlag) break;
    red = min(red,REDMIN);
    red = max(red,REDMAX);
    m_stepSize *= red;
    reduct = 1;
  }

  x = m_xnew;
  m_lastStepSize = m_stepSize;
  m_first = 0;
  double wrkmin=1.0e35;
  for(int kk=0;kk<=km;kk++) {
    fact = max(m_err(kk),SCALMX);
    work = fact*m_a(kk+1);
    if(work < wrkmin) {
      scale=fact;
      wrkmin=work;
      m_kopt=kk+1;
    }
  }
  m_nextStepSize = m_stepSize/scale;
LogPlain(L"BH hnext/m_stepSize = %f", m_nextStepSize);
  if(m_kopt >= k && m_kopt != kmax && !reduct) {
    fact = max(scale/m_alf(m_kopt-1, m_kopt), SCALMX);
    if(m_a(m_kopt+1)*fact <= wrkmin) {
      m_nextStepSize = m_stepSize/fact;
      m_kopt++;
    }
  }
LogPlain(L"BH hnext/m_stepSize = %f", m_nextStepSize);
  LogPlain(L"BH END STEP Y Out = %s (actual = [%f, %f] -------------------------------------------------------------", ToString(m_y).c_str(), sin(x), sin(x));
}

void BaderDeuflhardOde::TakeSemiImplicitStep(
  int numberOfSteps,
  double xStart,
  const VectorXd& yIn,
  const VectorXd& dyIn,
  VectorXd& yToReturn,
  shared_ptr<OdeData> odeData)
{
  shared_ptr<OdeEquations> system = odeData->GetOdeSystem();

  double subStepSize = m_stepSize/numberOfSteps;

  m_aSimStep = -subStepSize*m_dfdy;
  // LogPlain(L"Diagonal +1 before = %s", ToString(m_aSimStep).c_str());
  m_aSimStep.diagonal().array() += 1.0;
  // LogPlain(L"Diagonal +1 after = %s", ToString(m_aSimStep).c_str());

LogPlain(L"BH a/m_aSimStep = %s", ToString(m_aSimStep).c_str());

  // was using this... FloatGenFact decomp(m_aSimStep);
LogPlain(L"BH h/subStepSize = %f", subStepSize);
LogPlain(L"   dydx/dyIn = %s", ToString(dyIn).c_str());
LogPlain(L"   dfdx/m_dfdx = %s", ToString(m_dfdx).c_str());
  m_yTemp2 = (dyIn.array()+subStepSize*m_dfdx.array()).array()*subStepSize;
LogPlain(L"BH yout/m_yTemp2 = %s", ToString(m_yTemp2).c_str());
  ColPivHouseholderQR<MatrixXd> aDecomposition = m_aSimStep.colPivHouseholderQr();
  m_yTemp = aDecomposition.solve(m_yTemp2);
  m_delta = m_yTemp;
  m_ySum = yIn+m_delta;
LogPlain(L"BH del/m_delta = %s", ToString(m_delta).c_str());
LogPlain(L"BH ytemp/m_ySum = %s", ToString(m_ySum).c_str());

  // This is the first step.
  double x = xStart+subStepSize;
LogPlain(L"BH  x/x = %f", x);
  system->Evaluate(x, m_ySum, m_yTemp, odeData);

  // The remainder of the steps.
  for(int j=1;j<numberOfSteps;j++) {
    m_yTemp *= subStepSize;
    m_yTemp -= m_delta;
    m_yTemp2 = m_yTemp;
    m_yTemp = aDecomposition.solve(m_yTemp2);
    m_delta += 2.0*m_yTemp;
    m_ySum += m_delta;
    x += subStepSize;

    system->Evaluate(x, m_ySum, m_yTemp, odeData);
  }

  // The last step.
  m_yTemp *= subStepSize;
  m_yTemp -= m_delta;
  yToReturn = aDecomposition.solve(m_yTemp);
  yToReturn += m_ySum;
}

void BaderDeuflhardOde::Extrapolate(int iFromStep, double xFromStep, const VectorXd& yFromStep, VectorXd& yToReturn, VectorXd& yErrorEstimate) {

  Bach::LogPlain(L"BH EXTRAPOLATE:");
  Bach::LogPlain(L"BH  iest = %d", iFromStep);
  Bach::LogPlain(L"BH  xest = %d", xFromStep);
  Bach::LogPlain(L"BH  yest = %s", ToString(yFromStep).c_str());

  double q;
  double f2;
  double f1;
  double delta;

  int nv = (int) yToReturn.rows();
  Bach::LogPlain(L"BH nv/nv = %d", nv);

  m_xInterpTable(iFromStep) = xFromStep;

  int j;
  for (j=0;j<nv;j++) {
    yErrorEstimate(j) = yToReturn(j) = yFromStep(j);
  }

  if(iFromStep == 0) {
    for(j=0; j<nv; j++) {
      m_interpTable(j, 0) = yFromStep(j);
    }
    Bach::LogPlain(L"BH 0 d/m_interpTable = %s", ToString(m_interpTable).c_str());
  }
  else {
    for(j=0;j<nv;j++) {
      m_extrapC(j) = yFromStep(j);
    }
    Bach::LogPlain(L"BH c/m_extrapC = %s", ToString(m_extrapC).c_str());
    for(int k1=0;k1<iFromStep;k1++) {
      delta=1.0/(m_xInterpTable(iFromStep-k1-1)-xFromStep);
      Bach::LogPlain(L"BH del/delta = %f", delta);
      Bach::LogPlain(L"BH x/m_xInterpTable = %s", ToString(m_xInterpTable).c_str());
      Bach::LogPlain(L"BH iFromStep-k1-1 = %d", iFromStep-k1-1);
      Bach::LogPlain(L"BH xest/xFromStep = %f", xFromStep);
      f1 = xFromStep*delta;
      f2 = m_xInterpTable(iFromStep-k1-1)*delta;
      for(j=0;j<nv;j++) {
        q = m_interpTable(j, k1);
        m_interpTable(j, k1)=yErrorEstimate(j);
        delta = m_extrapC(j)-q;
        yErrorEstimate(j) = f1*delta;
        m_extrapC(j) = f2*delta;
        yToReturn(j) += yErrorEstimate(j);
      }
    }
    for (j=0;j<nv;j++) {
      m_interpTable(j, iFromStep) = yErrorEstimate[j];
    }
  }

  Bach::LogPlain(L"BH OUT yz = %s", ToString(yToReturn).c_str());
  Bach::LogPlain(L"BH OUT dy = %s", ToString(yErrorEstimate).c_str());
}
