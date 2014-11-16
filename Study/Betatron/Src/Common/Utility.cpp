/**********************************************************************

File     : Utility.cpp
Project  : Bach Betatron Library
Purpose  : Source file for the Betatron utility library.
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

#include "Utility.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <exception>
#include <sys/stat.h>

using namespace Bach;
using namespace Bach::Utility;
using namespace boost;
using namespace boost::property_tree;

#ifdef DeleteFile
# undef DeleteFile
#endif // DeleteFile

    //*****************
    //* Bach::Utility *
    //*****************

std::wstring Bach::Utility::LoadFileToString(const std::wstring& filePath) {  // throws std::exception

  std::ifstream file;
  file.open(UTF8FromWideString(filePath).c_str(), std::ios::binary);
  if(!file.is_open()) {
    Log(L"Utility::Setup: Unable to open file %s", filePath.c_str());
    throw std::exception();
  }

  // get length of file:
  file.seekg(0, std::ios::end);
  int length = (int) file.tellg();
  file.seekg (0, std::ios::beg);
  char* buffer = new char[length];
  file.read(buffer, length);

  std::string dataAsUTF8(buffer, length);
  delete[] buffer;
  std::wstring data = WideStringFromUTF8(dataAsUTF8);
  file.close();

  return data;
}

bool Bach::Utility::FileExists(const std::wstring& filePath) {
  struct stat buffer;
  return (stat(UTF8FromWideString(filePath).c_str(), &buffer) == 0);
}

bool Bach::Utility::DeleteFile(const std::wstring& filePath) {
  int status = remove(UTF8FromWideString(filePath).c_str());
  if(status != 0) {
    Log(L"ERROR: Bach::Utility::DeleteFile: Unable to delete file %s (%d)", filePath.c_str(), status);
    return false;
  }
  return true;
}
