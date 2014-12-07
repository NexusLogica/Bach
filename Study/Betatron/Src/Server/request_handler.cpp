//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"
#include <boost/lexical_cast.hpp>
#include "json.h"
#include "BetatronHandler.h"

using namespace Bach;
using namespace Eigen;

namespace http {
namespace server {

request_handler::request_handler(const std::string& doc_root)
  : doc_root_(doc_root)
{
}

void request_handler::handle_request(const request& req, reply& rep)
{
  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  std::string origin = "";
  for(long i=0; i<req.headers.size(); i++) {
    std::string header = req.headers[i].name;
    if(header == "Origin") {
      origin = req.headers[i].value;
    }
  }

  std::string output = "{}";

  size_t doPos = request_path.find("do=");
  if(doPos == std::string::npos) {
    output = "{ \"error\": \"Request error\", \"errorMessage\": \"The 'do' parameter was not found in the URL.\" }";
  }
  else {
    std::string input = request_path.substr(doPos+3);

    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(input, root);
    if(!parsingSuccessful) {
      output = "{ \"error\": \"Parse error\", \"errorMessage\": \""+reader.getFormattedErrorMessages()+"\" }";
    }
    else {
      std::string system = root.get("system", "betatron").asString();
      if(system == "Betatron") {
        boost::shared_ptr<BetatronHandler> handler = BetatronHandler::CreateInstance();
        output = handler->HandleRequest(root);

      }
      else {
        output = "{ \"error\": \"Request command error\", \"errorMessage\": \"The system '"+system+"' was not found.\" }";
      }
    }
  }

  // Fill out the reply to be sent to the client.
  rep.status = reply::ok;
  rep.content = output;
  rep.headers.resize(4);
  rep.headers[0].name = "Content-Type";
  rep.headers[0].value = "application/json; charset=utf-8";
  rep.headers[1].name = "Content-Length";
  rep.headers[1].value = boost::lexical_cast<std::string>(rep.content.size());
  rep.headers[2].name = "Access-Control-Allow-Origin";
  rep.headers[2].value = origin;
  rep.headers[3].name = "Access-Control-Allow-Credentials";
  rep.headers[3].value = "true";
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace server
} // namespace http
