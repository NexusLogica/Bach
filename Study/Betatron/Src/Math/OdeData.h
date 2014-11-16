/**********************************************************************

File     : OdeData.h
Project  : Bach Simulation
Purpose  : Header file for the ODE data class.
Revisions: Original definition by Lawrence Gunn.
           90/05/04.
           Parts of this are based on routines from Numerical Recipes in C, the Harwell libraries and other sources.

           Updated by Lawrence Gunn.
           2013/01/26


Copyright (c) 1990-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ODE_DATA_H__
#define __BACH_ODE_DATA_H__

#include "OdeEquations.h"

namespace Bach {

  //***********
  //* OdeData *
  //***********

  class OdeData {
  public:
    static boost::shared_ptr<OdeData> CreateInstance(boost::shared_ptr<OdeEquations> system);
    ~OdeData() {}

    void SetStartTime(double value) { m_start = value; }
    void SetEndTime(double value)   { m_end   = value; }

    void SetInitialConditions(const Eigen::VectorXd& ic)  {  m_initialValues = ic;  }
    void SetFinalConditions(const Eigen::VectorXd& fc)    {  m_finalValues   = fc;  }
    void SetInternalValues(const Eigen::VectorXd& iv)     {  m_internalValues = iv; }

    unsigned int GetStateLength()    { return m_system->GetStateLength(); }
    unsigned int GetInternalLength() { return m_system->GetInternalLength(); }

    double GetStartTime() { return m_start; }
    double GetEndTime()   { return m_end;   }

    boost::shared_ptr<OdeEquations> GetOdeSystem() { return m_system; }
    Eigen::VectorXd GetInitialConditions() { return m_initialValues;  }
    Eigen::VectorXd GetFinalConditions()   { return m_finalValues;    }
    Eigen::VectorXd GetInternalValues()    { return m_internalValues; }
    Eigen::VectorXd& GetInternalValueRef() { return m_internalValues; }

    void SetCollector(boost::shared_ptr<OdeDataCollector> c)  {  m_collector = c;  }
    boost::shared_ptr<OdeDataCollector> GetCollector()  {  return m_collector;  }

    bool GetStopFlag() { return m_stopFlag; }
    void SetStopFlag(bool flag) { m_stopFlag = flag; }

    void ResetStorage();
    void StoreData(double x,const Eigen::VectorXd& y,const Eigen::VectorXd& dy);
    void SetStoringThisCall(bool f) { m_storingThisCallFlag = f; }
    bool StoringThisCall() { return m_storingThisCallFlag; }

  private:
    OdeData(boost::shared_ptr<OdeEquations> system);

    boost::weak_ptr<OdeData> m_weakThis;
    boost::shared_ptr<OdeEquations> m_system;
    boost::shared_ptr<OdeDataCollector> m_collector;

    Eigen::VectorXd m_initialValues;
    Eigen::VectorXd m_finalValues;
    Eigen::VectorXd m_internalValues;

    double m_start;
    double m_end;
    bool m_stopFlag;
    bool m_storingThisCallFlag;
  };
};

#endif // __BACH_ODE_DATA_H__
