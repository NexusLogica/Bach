/**********************************************************************

File     : FieldFlowHandler.h
Project  : Bach Simulation
Purpose  : Header file for two electron simulation with reltiave observer simulations.
Revisions: Original definition by Lawrence Gunn.
           2015/01/10

Copyright (c) 2015 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_FIELD_FLOW_HANDLER_H__
#define __BACH_FIELD_FLOW_HANDLER_H__

#include "BetatronEquations.h"
#include "json.h"

namespace Bach {

  class BetatronEquations;
  class BetatronFieldController;
  class BaderDeuflhardOde;
  class OdeData;

  //********************
  //* FieldFlowHandler *
  //********************

  class FieldFlowHandler {
  public:
  
    static boost::shared_ptr<FieldFlowHandler> CreateInstance();

    virtual ~FieldFlowHandler();
    
    std::string HandleRequest(Json::Value request);

  protected:
    FieldFlowHandler();
    
    boost::weak_ptr<FieldFlowHandler> m_weakThis;
    Eigen::Matrix4d GetMatrix4dfromJSON(Json::Value arrayValue);

  };
};

#endif // __BACH_FIELD_FLOW_HANDLER_H__
