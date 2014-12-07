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

  shared_ptr<BetatronEquationSolver> solver = BetatronEquationSolver::CreateInstance();
  solver->SetInitialConditionsFromRadiusAndSpeed(0.1, 0.01*Bach::SPEED_OF_LIGHT);
  solver->Initialize();

  shared_ptr<OdeDataCollector> collector = shared_ptr<OdeDataCollector>(new OdeDataCollector());
  shared_ptr<OdeData> odeData = solver->GetOdeData();
  odeData->SetCollector(collector);

  solver->Run();
  std::string internalJson =  odeData->GetCollector()->GetInternalData()->AsJson();

  return internalJson;
}
