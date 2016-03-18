/**********************************************************************

File     : MolecularSimHandler.h
Project  : Bach Simulation
Purpose  : Header file for the request handler for molecule simulations.
Revisions: Original definition by Lawrence Gunn.
           2016/03/18

Copyright (c) 2016 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_MOLECULAR_SIM_HANDLER_H__
#define __BACH_MOLECULAR_SIM_HANDLER_H__

#include "BetatronEquations.h"
#include "json.h"

namespace Bach {

  class BetatronEquations;
  class BetatronFieldController;
  class BaderDeuflhardOde;
  class OdeData;

  //***********************
  //* MolecularSimHandler *
  //***********************

  class MolecularSimHandler {
  public:
  
    static boost::shared_ptr<MolecularSimHandler> CreateInstance();

    virtual ~MolecularSimHandler();
    
    std::string HandleRequest(Json::Value request);

  protected:
    MolecularSimHandler();
    
    boost::weak_ptr<MolecularSimHandler> m_weakThis;
  };
};

#endif // __BACH_MOLECULAR_SIM_HANDLER_H__
