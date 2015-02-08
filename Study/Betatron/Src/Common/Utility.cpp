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
#include <string>
#include <sstream>

using namespace Bach;
using namespace Bach::Utility;
using namespace boost;
using namespace boost::property_tree;
using namespace Eigen;

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

std::wstring Bach::VecXdToString(const Eigen::VectorXd& vec) {
  std::wstringstream vecStream;
  for(int i=0; i < vec.size(); i++) {
    if(i != 0) {
      vecStream << ",";
    }
    vecStream << vec(i);
  }
  return vecStream.str();
}

std::wstring Bach::ToString(const Eigen::MatrixXd& mat) {

  std::stringstream matStream;
  if(mat.cols() == 1) {
    IOFormat fmt(StreamPrecision, DontAlignCols, ", ", ", ", "", "", "[", "]");
    matStream << mat.format(fmt);
  }
  else {
    IOFormat fmt(StreamPrecision, 0, ", ", ";\n", "", "", "[", "]");
    matStream << mat.format(fmt);
  }
  return WideStringFromUTF8(matStream.str());
}

std::wstring Bach::Vec3dToString(const Eigen::Vector3d& vec) {

  std::stringstream vecStream;
  IOFormat fmt(StreamPrecision, DontAlignCols, ", ", ", ", "", "", "[", "]");
  vecStream << vec.format(fmt);
  return WideStringFromUTF8(vecStream.str());
}

std::wstring Bach::Vec3dToStringFixed(const Eigen::Vector3d& vec) {
  wchar_t buffer[2*256];
  swprintf(buffer, 2*256, L"[% #-03.3f % #-03.3f % #-03.3f]", vec(0), vec(1), vec(2));
  return std::wstring(buffer);
}

bool Bach::IsApproxEqual(const Eigen::VectorXd& vec1, const Eigen::VectorXd& vec2, double tolerance) {
  if(vec1.rows() != vec2.rows()) {
    Log(L"ERROR: Bach::IsApproxEqual: vector sizes not equal: %d vs %d", vec1.rows(), vec2.rows());
    return false;
  }

  for(int i=0; i<vec1.rows(); i++) {
    if(fabs(vec1(i)-vec2(i)) > tolerance) {
      Log(L"ERROR: Bach::IsApproxEqual: values at index %d not equal: %g vs %g", i, vec1(i), vec2(i));
      Log(L"  vec1 = %s", ToString(vec1).c_str());
      Log(L"  vec2 = %s", ToString(vec2).c_str());
      return false;
    }
  }
  return true;
}

bool Bach::IsApproxEqual(const Eigen::Vector4d& vec1, const Eigen::Vector4d& vec2, double tolerance) {
  if(vec1.rows() != vec2.rows()) {
    Log(L"ERROR: Bach::IsApproxEqual: vector sizes not equal: %d vs %d", vec1.rows(), vec2.rows());
    return false;
  }

  for(int i=0; i<vec1.rows(); i++) {
    if(fabs(vec1(i)-vec2(i)) > tolerance) {
      Log(L"ERROR: Bach::IsApproxEqual: values at index %d not equal: %g vs %g", i, vec1(i), vec2(i));
      Log(L"  vec1 = %s", ToString(vec1).c_str());
      Log(L"  vec2 = %s", ToString(vec2).c_str());
      return false;
    }
  }
  return true;
}

Eigen::Vector3d Bach::Vec4dTo3d(const Eigen::Vector4d& vec) {
  return Eigen::Vector3d(vec(0), vec(1), vec(2));
}

Eigen::Vector4d Bach::Vec3dTo4d(const Eigen::Vector3d& vec, double element4) {
  return Eigen::Vector4d(vec(0), vec(1), vec(2), element4);
}

