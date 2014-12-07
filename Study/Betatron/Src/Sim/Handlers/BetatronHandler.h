/**********************************************************************

File     : BetatronHandler.h
Project  : Bach Simulation
Purpose  : Header file for the request handler for betatron simulations.
Revisions: Original definition by Lawrence Gunn.
           2014/12/06


Copyright (c) 2014 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_BETATRON_HANDLER_H__
#define __BACH_BETATRON_HANDLER_H__

#include "BetatronEquations.h"
#include "json.h"

namespace Bach {

  class BetatronEquations;
  class BetatronFieldController;
  class BaderDeuflhardOde;
  class OdeData;

  //*******************
  //* BetatronHandler *
  //*******************

  class BetatronHandler {
  public:
  
    static boost::shared_ptr<BetatronHandler> CreateInstance();

    virtual ~BetatronHandler();
    
    void HandleRequest(Json::Value request);

  protected:
    BetatronHandler();
    
    boost::weak_ptr<BetatronHandler> m_weakThis;
  };
};

#endif // __BACH_BETATRON_HANDLER_H__
