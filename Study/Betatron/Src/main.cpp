/**********************************************************************

File     : AppleLog.h
Project  : Bach Betatron Library
Purpose  : Header file for main.
Revisions: Original definition by Lawrence Gunn.
           2014/11/15

Copyright (c) 2014 by Lawrence Gunn

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

*/

#include <iostream>
#include "Utility.h"

#include "BetatronSimulationAgent.h"
#include "BetatronEquationSolver.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;

int main(int argc, const char * argv[]) {

  std::cout << "Hello, World!\n";

  shared_ptr<BetatronEquationSolver> solver = BetatronEquationSolver::CreateInstance();
  solver->SetInitialConditionsFromRadiusAndSpeed(0.1, 0.01*Bach::SPEED_OF_LIGHT);
  solver->Initialize();
  solver->Run();

  return 0;
}
