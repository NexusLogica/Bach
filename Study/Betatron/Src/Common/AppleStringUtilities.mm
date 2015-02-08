/**********************************************************************

File     : AppleStringUtilities.mm
Project  : Bach Betatron Library
Purpose  : Header file for Apple string utilities.
Revisions: Original definition by Lawrence Gunn.
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

#include "AppleStringUtilities.h"
#include "BachDefs.h"

#if TARGET_RT_BIG_ENDIAN
const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32BE);
#else
const NSStringEncoding kEncoding_wchar_t = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
#endif

NSString* NSStringFromWideString(const std::wstring& in) {
  char* data = (char*)in.data();
  unsigned int size = (unsigned int) in.size()*sizeof(wchar_t);
  NSString* macString = [[NSString alloc] initWithBytes:data length:size encoding:kEncoding_wchar_t];
  return macString;
}

NSString* NSStringFromUTF8(const std::string& in) {
  char* data = (char*)in.data();
  unsigned int size = (unsigned int) in.size()*sizeof(char);
  NSString* macString = [[NSString alloc] initWithBytes:data length:size encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF8)];
  return macString;
}

std::wstring WideStringFromNSString(NSString* in) {
  NSData* data = [in dataUsingEncoding : kEncoding_wchar_t];      
  return std::wstring((wchar_t*)[data bytes], [data length]/sizeof(wchar_t));   
}

std::string UTF8FromNSString(NSString* in) {
  std::string s = [in cStringUsingEncoding:(NSUTF8StringEncoding)];
  return s;   
}

std::wstring Bach::WideStringFromUTF8(const std::string& in) {
  NSString* macString = NSStringFromUTF8(in);
  NSData* data = [macString dataUsingEncoding:kEncoding_wchar_t];
  return std::wstring((wchar_t*)[data bytes], [data length] / sizeof(wchar_t));
}

std::string Bach::UTF8FromWideString(const std::wstring& in) {
  NSString* macString = NSStringFromWideString(in);
  std::string s = [macString cStringUsingEncoding:(NSUTF8StringEncoding)];
  return s;
}
