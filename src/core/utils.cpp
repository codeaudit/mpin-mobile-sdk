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

#include "utils.h"
#include "CvXcode.h"


namespace util
{

/*
 * String class
 */

void String::Overwrite(char c)
{
    OverwriteString(*this, c);
}

String& String::Trim(const std::string& chars)
{
    TrimLeft(chars);
    TrimRight(chars);

    return *this;
}

int String::GetHash()
{
    // Implements java style hashcode: h(s)=\sum_{i=0}^{n-1}s[i] \cdot 31^{n-1-i}
    int hash = 0;
    for(const_iterator i = begin(); i != end(); ++i)
    {
        hash = ((hash << 5) - hash) + *i; //hash = hash * 31 + *i;
    }

    return hash;
}

String::~String()
{
    Overwrite();
}

void OverwriteString(std::string& str, char c)
{
    for(size_t i = 0; i < str.length(); ++i)
    {
        str[i] = c;
    }
}


/*
 * JsonObject class
 */

JsonObject::JsonObject()
{
}

JsonObject::JsonObject(const json::Object& other)
{
    Copy(other);
}

JsonObject& JsonObject::operator =(const json::Object& other)
{
    Copy(other);
    m_parseError = "";
    return *this;
}
    
JsonObject::~JsonObject()
{
    OverwriteJsonValues(*this);
}

std::string JsonObject::ToString() const
{
    std::stringstream jsonStream;
    json::Writer::Write(*this, jsonStream);
    return jsonStream.str();
}

bool JsonObject::Parse(const char* str)
{
    try
    {
        Clear();

        if(str == NULL || *str == '\0')
        {
            throw json::Exception("Failed to find root element");
        }
        
        std::istringstream istream(str);
        json::Reader::Read(*this, istream);
        
        m_parseError = "";

        return true;
    }
    catch(const json::Exception& e)
    {
        m_parseError.Format("Failed to parse '%s' json. Error='%s'\n", str, e.what());
        return false;
    }
}

const char * JsonObject::GetStringParam(const char *name, const char *defaultValue) const
{
    try
    {
        return ((const json::String&) (*this)[name]).Value().c_str();
    }
    catch(const json::Exception&)
    {
        return defaultValue;
    }
}

int JsonObject::GetIntParam(const char *name, int defaultValue) const
{
    try
    {
        return (int) ((const json::Number&) (*this)[name]).Value();
    }
    catch(const json::Exception&)
    {
        return defaultValue;
    }
}
    
int64_t JsonObject::GetInt64Param(const char *name, int64_t defaultValue) const
{
    try
    {
        return (int64_t)((const json::Number&) (*this)[name]).Value();
    }
    catch(const json::Exception&)
    {
        return defaultValue;
    }
}

bool JsonObject::GetBoolParam(const char *name, bool defaultValue) const
{
    try
    {
        return ((const json::Boolean&) (*this)[name]).Value();
    }
    catch(const json::Exception&)
    {
        return defaultValue;
    }
}

std::string JsonObject::GetParseError() const
{
    return m_parseError;
}

void JsonObject::Copy(const json::Object& other)
{
    for(json::Object::const_iterator i = other.Begin(); i != other.End(); ++i)
    {
        Insert(*i);
    }
}


class JsonVisitor : public json::Visitor
{
public:
    virtual void Visit(json::Array& array)
    {
        OverwriteJsonValues(array);
    }

    virtual void Visit(json::Object& object)
    {
        OverwriteJsonValues(object);
    }

    virtual void Visit(json::Number& number)
    {
        number.Value() = 0.0;
    }

    virtual void Visit(json::String& string)
    {
        OverwriteString(string.Value());
    }

    virtual void Visit(json::Boolean& boolean)
    {
        boolean.Value() = false;
    }

    virtual void Visit(json::Null& null)
    {
    }
};


void OverwriteJsonValues(json::Object& object)
{
    for(json::Object::iterator i = object.Begin(); i != object.End(); ++i)
    {
        OverwriteJsonValues(i->element);
    }
}

void OverwriteJsonValues(json::Array& array)
{
    for(json::Array::iterator i = array.Begin(); i != array.End(); ++i)
    {
        OverwriteJsonValues(*i);
    }
}

void OverwriteJsonValues(json::UnknownElement& element)
{
    JsonVisitor visitor;
    element.Accept(visitor);
}


/*
 * StringMap class
 */

bool StringMap::Put(const String& key, const String& value)
{
    iterator i = find(key);
    if(i != end())
    {
        return false;
    }

    (*this)[key] = value;

    return true;
}

const char * StringMap::Get(const String& key) const
{
    const_iterator i = find(key);
    if(i == end())
    {
        return "";
    }

    return i->second.c_str();
}


/*
 * Hex encoding/decoding
 */

std::string HexEncode(const char* str, size_t len)
{
    std::string hexEncodedStr;
    CvShared::CvHex::Encode((const unsigned char *) str, len, hexEncodedStr);
    return hexEncodedStr;
}

std::string HexEncode(const std::string& str)
{
    return HexEncode(str.c_str(), str.size());
}

std::string HexDecode(const std::string& str)
{
    std::string hexDecodedStr;
    CvShared::CvHex::Decode(str, hexDecodedStr);
    return hexDecodedStr;
}
}
