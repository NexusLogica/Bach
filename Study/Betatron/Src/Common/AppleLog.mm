/**********************************************************************

File     : AppleLog.mm
Project  : Bach Betatron Library
Purpose  : Source file for an Apple Log object.
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

#include "AppleLog.h"
#include "AppleStringUtilities.h"
#include <stdio.h>
#include <wchar.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>


using namespace Bach;
using namespace boost;

#if TARGET_RT_BIG_ENDIAN
const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32BE);
#else
const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
#endif

    //******************
    //* Bach::AppleLog *
    //******************

boost::shared_ptr<AppleLog> AppleLog::s_strongThis;

AppleLog::AppleLog() {

}

AppleLog::~AppleLog() {
}

boost::shared_ptr<ILog> AppleLog::CreateInstance() {
  if(s_strongThis.get() == NULL) {
    s_strongThis = boost::shared_ptr<AppleLog>(new AppleLog);
  }
  return s_strongThis;
}

void AppleLog::Write(const wchar_t* message) {
	struct timeval tv;
	struct timezone tz;
  gettimeofday(&tv, &tz);
	struct tm* currentTime = localtime(&tv.tv_sec);
  
  // Write the date into a buffer.
  wchar_t buffer[2*256];
  swprintf(buffer, 2*256, L"[%d.%02d.%02d %02d:%02d:%02d.%03d] ",
           currentTime->tm_year+1900,
           currentTime->tm_mon+1,
           currentTime->tm_mday,
           currentTime->tm_hour,
           currentTime->tm_min,
           currentTime->tm_sec,
           tv.tv_usec / 1000);
  std::wstring fullMessage = buffer;
  fullMessage.append(message);
  
  NSString* macString = NSStringFromWideString(fullMessage);
  NSLog(@"%@", macString);
}

void AppleLog::WritePlain(const wchar_t* message) {
  std::wstring fullMessage = message;
  
  NSString* macString = NSStringFromWideString(fullMessage);
  NSLog(@"%@", macString);
}

void Bach::Log(const wchar_t* formatString, ...) {
  va_list vaArgs;
  va_start(vaArgs, formatString);
  
  std::wstring formatStringModified(formatString);
  bool lastWasPercent = false;
  for(int i=0; i<formatStringModified.size(); i++) {
    if(lastWasPercent) {
      if(formatStringModified[i] == L's') {
        formatStringModified[i] = L'S';
      }
      else if(formatStringModified[i] == L'S') {
        formatStringModified[i] = L's';
      }
    }
    if(formatStringModified[i] == L'%') {
      lastWasPercent = true;
    }
    else {
      lastWasPercent = false;
    }
  }

  wchar_t buffer[512];
  vswprintf(buffer, 511, formatStringModified.c_str(), vaArgs);

  shared_ptr<ILog> log = AppleLog::CreateInstance();
  log->Write(buffer);
}

void Bach::LogPlain(const wchar_t* formatString, ...) {
  va_list vaArgs;
  va_start(vaArgs, formatString);
  
  std::wstring formatStringModified(formatString);
  bool lastWasPercent = false;
  for(int i=0; i<formatStringModified.size(); i++) {
    if(lastWasPercent && formatStringModified[i] == L's') {
      formatStringModified[i] = L'S';
    }
    if(formatStringModified[i] == L'%') {
      lastWasPercent = true;
    }
    else {
      lastWasPercent = false;
    }
  }
  
  wchar_t buffer[512];
  vswprintf(buffer, 511, formatStringModified.c_str(), vaArgs);
  
  shared_ptr<ILog> log = AppleLog::CreateInstance();
  log->Write(buffer);
}
