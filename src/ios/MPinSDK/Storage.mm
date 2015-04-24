/*
 * Storage.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: Georgi Georgiev
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
