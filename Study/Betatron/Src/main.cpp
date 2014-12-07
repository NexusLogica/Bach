/**********************************************************************

File     : main.cpp
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
#include "json.h"
#include "BetatronHandler.h"

using namespace Bach;
using namespace boost;
using namespace Eigen;
/*
int main(int argc, const char * argv[]) {

  std::cin >> std::noskipws;

  // use stream iterators to copy the stream to a string
  std::istream_iterator<char> it(std::cin);
  std::istream_iterator<char> end;
  std::string input(it, end);

  std::string output;

  Json::Value root;
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(input, root);
  if(!parsingSuccessful) {
    output = "{ \"error\": \"Parse error\", \"errorMessage\": \""+reader.getFormattedErrorMessages()+"\" }";
  }
  else {
    std::string system = root.get("system", "betatron").asString();
    if(system == "Betatron") {
      shared_ptr<BetatronHandler> handler = BetatronHandler::CreateInstance();
      output = handler->HandleRequest(root);

    }
    output = "{ \"status\": \"success\" }";
  }

  std::cout << output;
  return 0;
}
*/