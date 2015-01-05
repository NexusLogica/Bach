/**********************************************************************

File     : OdeDataCollector.h
Project  : Bach Simulation
Purpose  : Header file for the ODE data collector class.
Revisions: Original definition by Lawrence Gunn.
           90/05/04.
           Updated 92/10/01.

           Updated by Lawrence Gunn.
           2013/01/29


Copyright (c) 1990-2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ODE_DATA_COLLECTOR_H__
#define __BACH_ODE_DATA_COLLECTOR_H__

#include "BetatronDefs.h"

namespace Bach {

  //********************
  //* OdeDataCollector *
  //********************

  class OdeDataCollector {
  public:
    OdeDataCollector();
    ~OdeDataCollector();

    void InitializeWithSizes(long numStates, long numInternal);

    void StoreData(double time, const Eigen::VectorXd& states, const Eigen::VectorXd& derivs, boost::shared_ptr<OdeData> odeData);

    void SetStateData(boost::shared_ptr<SampledDerivedData> sd) { m_stateData = sd; }
    void SetInternalData(boost::shared_ptr<SampledData> id)   { m_internalData = id; }

    boost::shared_ptr<SampledDerivedData> GetStateData() { return m_stateData; }
    boost::shared_ptr<SampledData> GetInternalData() { return m_internalData; }

    void Restart();

    std::string AsJson();

  protected:
    boost::shared_ptr<SampledDerivedData> m_stateData;
    boost::shared_ptr<SampledData> m_internalData;

    void SetStateStorageIsStopped(bool f) { m_stateStorageIsStopped = f; }
    void SetInternalStorageIsStopped(bool f) { m_internalStorageIsStopped = f; }

    virtual void StoreStates(double x, const Eigen::VectorXd& y, const Eigen::VectorXd& dy, boost::shared_ptr<OdeData> odeData);
    virtual void StoreInternals(double x, boost::shared_ptr<OdeData> odeData);
    virtual void HandleStateDataOverflow(double, boost::shared_ptr<OdeData>);
    virtual void HandlerInternalDataOverflow(double, boost::shared_ptr<OdeData>);

    virtual int GetNewStateSamplesSize(double x, boost::shared_ptr<OdeData> odeData);
    virtual int GetNewInternalSamplesSize(double x, boost::shared_ptr<OdeData> odeData);

  private:
    bool m_stateStorageIsStopped;
    bool m_internalStorageIsStopped;
  };
};

#endif // __BACH_ODE_DATA_COLLECTOR_H__
