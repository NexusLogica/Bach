/**********************************************************************

File     : MolecularSimHandler.cpp
Project  : Bach Simulation
Purpose  : Source file for the request handler for molecule simulations.
Revisions: Original definition by Lawrence Gunn.
           2016/03/18

Copyright (c) 2016 by Lawrence Gunn
All Rights Reserved.

*/

#include "MolecularSimHandler.h"
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

  //***********************
  //* MolecularSimHandler *
  //***********************

shared_ptr<MolecularSimHandler> MolecularSimHandler::CreateInstance() {
  shared_ptr<MolecularSimHandler> instance(new MolecularSimHandler());
  instance->m_weakThis = instance;
  return instance;
}

MolecularSimHandler::MolecularSimHandler() {
}

MolecularSimHandler::~MolecularSimHandler() {
}

std::string MolecularSimHandler::HandleRequest(Json::Value request) {
  Json::Value inputs = request["inputs"];
  double radius = inputs.get("radius", "0.0").asDouble();
  double speed  = inputs.get("speed",  "0.0").asDouble();

  shared_ptr<BetatronEquationSolver> solver = BetatronEquationSolver::CreateInstance();

  if(inputs.isMember("fieldIncrease")) {
    double fieldIncrease = inputs.get("fieldIncrease",  "0.0").asDouble();
    solver->SetFieldIncreaseRatePerRotation(fieldIncrease);
  }
  
  solver->SetInitialConditionsFromRadiusAndSpeed(radius, speed*Bach::SPEED_OF_LIGHT);

  solver->Initialize();

  solver->Run();
  std::string json = solver->GetOdeData()->GetCollector()->AsJson();

  return json;
}
