/**********************************************************************

File     : TwoElectronRelativityHandler.h
Project  : Bach Simulation
Purpose  : Header file for two electron simulation with reltiave observer simulations.
Revisions: Original definition by Lawrence Gunn.
           2015/01/10

Copyright (c) 2015 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_TWO_ELECTRON_RELATIVITY_HANDLER_H__
#define __BACH_TWO_ELECTRON_RELATIVITY_HANDLER_H__

#include "BetatronEquations.h"
#include "json.h"

namespace Bach {

  class BetatronEquations;
  class BetatronFieldController;
  class BaderDeuflhardOde;
  class OdeData;

  //********************************
  //* TwoElectronRelativityHandler *
  //********************************

  class TwoElectronRelativityHandler {
  public:
  
    static boost::shared_ptr<TwoElectronRelativityHandler> CreateInstance();

    virtual ~TwoElectronRelativityHandler();
    
    std::string HandleRequest(Json::Value request);

  protected:
    TwoElectronRelativityHandler();
    
    boost::weak_ptr<TwoElectronRelativityHandler> m_weakThis;
  };
};

#endif // __BACH_TWO_ELECTRON_RELATIVITY_HANDLER_H__
