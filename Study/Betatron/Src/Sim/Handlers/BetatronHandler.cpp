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

std::string BetatronHandler::HandleRequest(Json::Value request) {
  Json::Value inputs = request["inputs"];
  double radius = inputs.get("radius", "0.0").asDouble();
  double speed  = inputs.get("speed",  "0.0").asDouble();

  shared_ptr<BetatronEquationSolver> solver = BetatronEquationSolver::CreateInstance();

  solver->SetInitialConditionsFromRadiusAndSpeed(radius, speed*Bach::SPEED_OF_LIGHT);

  if(inputs.isMember("fieldIncrease")) {
    double fieldIncrease = inputs.get("fieldIncrease",  "0.0").asDouble();
    solver->SetFieldIncreaseRatePerRotation(fieldIncrease);
  }

  solver->Initialize();

  solver->Run();
  std::string json = solver->GetOdeData()->GetCollector()->AsJson();

  return json;
}
