/*
 * HTTPConnector.cpp
 *
 *  Created on: Oct 20, 2014
 *      Author: georgi.georgiev
 */

#include "HTTPConnector.h"
#import "MPin.h"


namespace net {
    static const String HTTP_GET = "GET";
    static const String HTTP_POST = "POST";
    static const String HTTP_PUT = "PUT";
    static const String HTTP_DELETE = "DELETE";
    static const String HTTP_OPTIONS = "OPTIONS";
    static const String HTTP_PATHCH = "PATCH";
    
    const String&  getHTTPMethod(Method method) {
        if(method == 0)
            return HTTP_GET;
        else if(method == 1)
            return HTTP_POST;
        else if (method == 2)
            return HTTP_PUT;
        else if (method == 3)
            return HTTP_DELETE;
        else if (method == 4)
            return HTTP_OPTIONS;
        else
            return HTTP_PATHCH;

    }
    
	void HTTPConnector::SetHeaders(const StringMap& headers) {
		m_requestHeaders = headers;
	}

	void HTTPConnector::SetQueryParams(const StringMap& queryParams){
        m_queryParams = queryParams;
	}

	void HTTPConnector::SetContent(const String& data) {
        m_bodyData = data;
	}

	void HTTPConnector::SetTimeout(int seconds) {
        if(seconds <=0) throw IllegalArgumentException("Timeout is negative or 0");
        timeout = seconds;
	}

	bool HTTPConnector::Execute(Method method, const String& url){
        NSString * strURL = [NSString stringWithUTF8String:url.c_str()];
        strURL = [strURL stringByReplacingOccurrencesOfString:@"wss://" withString:@"https://"];
        strURL = [strURL stringByReplacingOccurrencesOfString:@"ws://" withString:@"http://"];
    
        if ( [strURL hasPrefix:@"/"] ) {
             strURL = [[MPin getRPSUrl] stringByAppendingString:strURL];
        }
        
        if(!m_queryParams.empty()) {
            NSString *queryString = @"";
            for (StringMap::const_iterator it=m_queryParams.begin(); it!=m_queryParams.end(); ++it) {
                queryString = [queryString stringByAppendingString:[NSString stringWithUTF8String:it->first.c_str()]];
                queryString = [queryString stringByAppendingString:@"="];
                queryString = [queryString stringByAppendingString:[NSString stringWithUTF8String:it->second.c_str()]];
                queryString = [queryString stringByAppendingString:@"&"];
            }
            
            queryString = [queryString substringToIndex:[queryString length] -1];
            strURL = [strURL stringByAppendingString:@"?"];
            strURL = [strURL stringByAppendingString:queryString];
        }
        
        NSURL * theUrl = [NSURL URLWithString:strURL];
        NSMutableURLRequest * request = [NSMutableURLRequest requestWithURL:theUrl cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:timeout];
        request.HTTPMethod = [NSString stringWithUTF8String:(getHTTPMethod(method)).c_str()];
        
        if(!m_requestHeaders.empty()) {
            for (StringMap::const_iterator it=m_requestHeaders.begin(); it!=m_requestHeaders.end(); ++it) {
                [request addValue:[NSString stringWithUTF8String:it->second.c_str()] forHTTPHeaderField:[NSString stringWithUTF8String:it->first.c_str()]];
            }
        }
        
        if(!m_bodyData.empty()) {
            request.HTTPBody =  [[NSString stringWithUTF8String:m_bodyData.c_str()] dataUsingEncoding:NSUTF8StringEncoding];
        }
        
        NSHTTPURLResponse * response = nil;
        NSError * error = nil;
        NSData * data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
        
        if(error != nil) {
            /// TODO :: IMPORTANT FIX THIS IN LATER COMMITS
            if(error.code == -1012) {
                m_statusCode = 401;
                m_errorMessage += "Unauthorized Access! Please check your e-mail and confirm the activation link!";
                return true;
            }
            m_errorMessage += [error.localizedDescription UTF8String];
            return false;
        }
        
        if(response != nil) {
            m_statusCode = (int)response.statusCode;
            for(NSString * key in response.allHeaderFields) {
                NSString * value = [response.allHeaderFields objectForKey:key];
                m_responseHeaders[([key UTF8String])] = [value UTF8String];
            }
        }
        
        if(data != nil) {
            NSString * dataStr = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            m_response += [dataStr UTF8String];
        }

		return true;
	}

	const String& HTTPConnector::GetExecuteErrorMessage() const { return m_errorMessage; }

	int HTTPConnector::GetHttpStatusCode() const { return m_statusCode; }

	const StringMap& HTTPConnector::GetResponseHeaders() const { return m_responseHeaders; }

	const String& HTTPConnector::GetResponseData() const {	return m_response; }

	HTTPConnector :: ~HTTPConnector () { }

}
