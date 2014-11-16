/**********************************************************************

File     : AppleStringUtilities.h
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

#import <Foundation/Foundation.h>
#include <string>

NSString* NSStringFromWideString(const std::wstring& in);
NSString* NSStringFromUTF8(const std::string& in);
std::wstring WideStringFromNSString(NSString* in);
std::string UTF8FromNSString(NSString* in);
