/**********************************************************************

File     : TwoElectronRelativityHandler.cpp
Project  : Bach Simulation
Purpose  : Source file for two electron simulation with reltiave observer simulations.
Revisions: Original definition by Lawrence Gunn.
           2015/01/10
 
Copyright (c) 2015 by Lawrence Gunn
All Rights Reserved.

*/

#include "TwoElectronRelativityHandler.h"
#include "BetatronEquationSolver.h"
#include "BetatronFieldController.h"
#include "NDimAccuracySpec.h"
#include "SampledData.h"
#include "SampledDerivedData.h"
#include "BaderDeuflhardOde.h"
#include "OdeData.h"
#include "OdeDataCollector.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

  //********************************
  //* TwoElectronRelativityHandler *
  //********************************

shared_ptr<TwoElectronRelativityHandler> TwoElectronRelativityHandler::CreateInstance() {
  shared_ptr<TwoElectronRelativityHandler> instance(new TwoElectronRelativityHandler());
  instance->m_weakThis = instance;
  return instance;
}

TwoElectronRelativityHandler::TwoElectronRelativityHandler() {
}

TwoElectronRelativityHandler::~TwoElectronRelativityHandler() {
}

std::string TwoElectronRelativityHandler::HandleRequest(Json::Value request) {
  Json::Value inputs = request["inputs"];
  double electronSeparation = inputs.get("electronSeparation", "0.01").asDouble();
  double speed  = inputs.get("observerSpeed",  "1000.0").asDouble();

  std::string json = "{}";

  return json;
}
