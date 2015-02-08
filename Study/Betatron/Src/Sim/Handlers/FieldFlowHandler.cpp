/**********************************************************************

File     : FieldFlowHandler.cpp
Project  : Bach Simulation
Purpose  : Source file for two electron simulation with reltiave observer simulations.
Revisions: Original definition by Lawrence Gunn.
           2015/01/10
 
Copyright (c) 2015 by Lawrence Gunn
All Rights Reserved.

*/

#include "FieldFlowHandler.h"
#include "ChargeCircuitFieldCalculator.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //********************
  //* FieldFlowHandler *
  //********************

shared_ptr<FieldFlowHandler> FieldFlowHandler::CreateInstance() {
  shared_ptr<FieldFlowHandler> instance(new FieldFlowHandler());
  instance->m_weakThis = instance;
  return instance;
}

FieldFlowHandler::FieldFlowHandler() {
}

FieldFlowHandler::~FieldFlowHandler() {
}

std::string FieldFlowHandler::HandleRequest(Json::Value request) {
  Json::Value inputs = request["inputs"];
  double width = inputs.get("circuitWidth", "0.1").asDouble();
  double length  = inputs.get("circultLength",  "0.1").asDouble();
  Eigen::Matrix4d transform = GetMatrix4dfromJSON(inputs.get("circuitCenter", 0));
  
  shared_ptr<ChargeCircuitFieldCalculator> calculator = ChargeCircuitFieldCalculator::CreateInstance();
  calculator->SetCircuit(transform, width, length);

  std::string json = "{}";

  return json;
}

Eigen::Matrix4d FieldFlowHandler::GetMatrix4dfromJSON(Json::Value arrayValues) {

  Eigen::Matrix4d matrix;
  BACH_ASSERT(arrayValues.size() == 16);

  for(int c=0; c<4; c++) {
    for(int r=0; r<4; r++) {
      matrix(r, c) = arrayValues[r+c*4].asDouble();
    }
  }
  return matrix;
}
