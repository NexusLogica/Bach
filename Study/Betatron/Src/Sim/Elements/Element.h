/**********************************************************************

File     : Atom.h
Project  : Bach Simulation
Purpose  : Header file for an atom.
Revisions: Original definition by Lawrence Gunn.
           2013/06/21


Copyright (c) 2013 by Lawrence Gunn
All Rights Reserved.

*/

#ifndef __BACH_ELEMENT_H__
#define __BACH_ELEMENT_H__

#include "BetatronDefs.h"

namespace Bach {
  
  //***********
  //* Element *
  //***********
  
  class Element {
  public:
    enum ElementType {
      MoleculeElement     = 1,
      AtomElement         = 2,
      BondElement         = 3,
      ElectronPairElement = 4,
      SpOrbitalElement    = 5
    };

    ~Element() {}
    
    virtual std::wstring GetName() { return m_name; }
    virtual ElementType GetElementType() { return m_elementType; }

  protected:
    Element(ElementType elementType) : m_elementType(elementType) {}

    void SetName(const std::wstring& name) { m_name = name; Log(L"Name: %s", m_name.c_str()); }
    std::wstring m_name;
    ElementType m_elementType;
  };
};

#endif // __BACH_ELEMENT_H__
