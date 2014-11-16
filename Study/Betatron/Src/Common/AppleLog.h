/**********************************************************************

File     : AppleLog.h
Project  : Bach Betatron Library
Purpose  : Header file for an Apple Log object.
Revisions: Original definition by Lawrence Gunn.
           2000/05/19
           Added to Bach Betatron library by Lawrence Gunn.
           2014/11/15

Copyright (c) 2012-2014 by Lawrence Gunn

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

*/

#ifndef __PS_APPLE_LOG_H__
#define __PS_APPLE_LOG_H__

#include "BetatronDefs.h"

namespace Bach {

    //******************
    //* Bach::AppleLog *
    //******************

  class AppleLog : public ILog {
  public:
    static boost::shared_ptr<ILog> CreateInstance();

    virtual ~AppleLog();

    virtual void Write(const wchar_t* message);
    virtual void WritePlain(const wchar_t* message);

  protected:
    AppleLog();
    bool OpenFile();
    void WriteToFile(const std::wstring& text);
    void CloseFile();

    static boost::shared_ptr<AppleLog> s_strongThis;
  };
};

#endif // __PS_APPLE_LOG_H__
