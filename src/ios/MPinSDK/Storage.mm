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


#include "Storage.h"
#include "def.h"

#define SECURE_STORE    "/secure_store.txt"
#define STORE           "/store.txt"

namespace store {
    
static String inMemoryStore = "";
static String secureInMemoryStore = "";
    
Storage::Storage(bool isMpinType) : m_isMpinType(isMpinType), store((isMpinType)? (secureInMemoryStore):(inMemoryStore)) {
    if(m_isMpinType)  readStringFromFile(SECURE_STORE, secureInMemoryStore);
    else readStringFromFile(STORE, inMemoryStore);
}
    
void Storage::readStringFromFile(const String & aFileName, OUT String & aData) {
    NSString *filePath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *fileName = [NSString stringWithUTF8String:aFileName.c_str()];
    NSString *fileAtPath = [filePath stringByAppendingString:fileName];
    if (![[NSFileManager defaultManager] fileExistsAtPath:fileAtPath]) return;
    NSError * error = nil;
    NSString * readData = [NSString stringWithContentsOfFile:fileAtPath encoding:NSUTF8StringEncoding error:&error];
    if(error != nil)    m_errorMessage = [error.localizedDescription UTF8String];
    else  aData = [readData UTF8String];
}

void Storage::writeStringToFile(const String & aFileName, const IN String & aData) {
    NSString *filePath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *fileName = [NSString stringWithUTF8String:aFileName.c_str()];
    NSString *fileAtPath = [filePath stringByAppendingString:[NSString stringWithFormat:@"/%@",fileName]];
    if (![[NSFileManager defaultManager] fileExistsAtPath:fileAtPath]) {
        [[NSFileManager defaultManager] createFileAtPath:fileAtPath contents:nil attributes:nil];
    }
    [[[NSString stringWithUTF8String:aData.c_str()] dataUsingEncoding:NSUTF8StringEncoding] writeToFile:fileAtPath atomically:NO];
}

bool Storage::SetData(const String& data) {
    store = data;
    Save();
    return TRUE;
}

bool Storage::GetData(String &data) {
    if(!m_errorMessage.empty()) return FALSE;
    data = store;
    return TRUE;
}

const String& Storage::GetErrorMessage() const { return m_errorMessage; }

    
    void Storage::Save() {
        if(m_isMpinType)  writeStringToFile(SECURE_STORE, secureInMemoryStore);
        else writeStringToFile(STORE, inMemoryStore);
    }
    
    
Storage::~Storage() {
    Save();
    }

}
