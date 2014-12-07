/**********************************************************************

File     : BetatronHandler.cpp
Project  : Bach Simulation
Purpose  : Source file for the request handler for betatron simulations.
Revisions: Original definition by Lawrence Gunn.
           2014/12/06

Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#include "BetatronHandler.h"
#include "BetatronEquations.h"
#include "BetatronFieldController.h"
#include "NDimAccuracySpec.h"
#include "SampledData.h"
#include "SampledDerivedData.h"
#include "BaderDeuflhardOde.h"
#include "OdeData.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //*******************
  //* BetatronHandler *
  //*******************

shared_ptr<BetatronHandler> BetatronHandler::CreateInstance() {
  shared_ptr<BetatronHandler> instance(new BetatronHandler());
  instance->m_weakThis = instance;
  return instance;
}

BetatronHandler::BetatronHandler() {
}

BetatronHandler::~BetatronHandler() {
}

void BetatronHandler::HandleRequest(Json::Value request) {
  
}
