/**********************************************************************

File     : Utility.h
Project  : Bach Betatron Library
Purpose  : Header file for the Betatron utility library.
Revisions: Original definition by Lawrence Gunn.
           2012/02/10
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

#ifndef __BACH_UTILITY_H__
#define __BACH_UTILITY_H__

#include "BachDefs.h"

namespace Bach {
  namespace Utility {

    //*****************
    //* Bach::Utility *
    //*****************

    std::wstring LoadFileToString(const std::wstring& filePath); // throws std::exception on error
    bool FileExists(const std::wstring& filePath);
    bool DeleteFile(const std::wstring& filePath);
  };
};

#endif // __BACH_UTILITY_H__
