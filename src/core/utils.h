/*
Copyright (c) 2012-2015, Certivox
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

For full details regarding our CertiVox terms of service please refer to
the following links:
 * Our Terms and Conditions -
   http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
   http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
   http://www.certivox.com/about-certivox/patents/
*/

/*
 * Utility classes and functions
 */

#ifndef _MPIN_SDK_UTILS_H_
#define _MPIN_SDK_UTILS_H_

#include <map>
#include "json/reader.h"
#include "json/elements.h"
#include "json/writer.h"
#include "CvString.h"


namespace util
{

class String : public CvString
{
public:
    String() : CvString() {}
    String(const std::string& str) : CvString(str) {}
    String(const char *str) : CvString(str) {}
    String(const String& str) : CvString(str) {}
    String(const std::string& str, size_t pos, size_t size = npos) : CvString(str, pos, size) {}
    String(const char *str, size_t size) : CvString(str, size) {}
    String(size_t size, char c) : CvString(size, c) {}
    ~String();
    String& Trim(const std::string& chars = " \t\f\v\n\r");
    void Overwrite(char c = ' ');
    int GetHash();
};

void OverwriteString(std::string& str, char c = ' ');


class JsonObject : public json::Object
{
public:
    JsonObject();
    JsonObject(const json::Object& other);
    JsonObject& operator = (const json::Object& other);
    ~JsonObject();
    std::string ToString() const;
    bool Parse(const char *str);
    const char * GetStringParam(const char *name, const char *defaultValue = "") const;
    int GetIntParam(const char *name, int defaultValue = 0) const;
    int64_t GetInt64Param(const char *name, int64_t defaultValue = 0) const;
    bool GetBoolParam(const char *name, bool defaultValue = false) const;
    std::string GetParseError() const;
    
private:
    void Copy(const json::Object& other);

private:
    String m_parseError;
};

void OverwriteJsonValues(json::Object& object);
void OverwriteJsonValues(json::Array& array);
void OverwriteJsonValues(json::UnknownElement& element);

class StringMap : public std::map<String, String>
{
public:
    bool Put(const String& key, const String& value);
    const char * Get(const String& key) const;
};


std::string HexEncode(const char *str, size_t len);
std::string HexEncode(const std::string& str);
std::string HexDecode(const std::string& str);

}


#endif // _MPIN_SDK_UTILS_H_
