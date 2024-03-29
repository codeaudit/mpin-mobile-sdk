/*******************************************************************************
 * Copyright (c) 2012-2015, Certivox All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * For full details regarding our CertiVox terms of service please refer to the following links:
 * 
 * * Our Terms and Conditions - http://www.certivox.com/about-certivox/terms-and-conditions/
 * 
 * * Our Security and Privacy - http://www.certivox.com/about-certivox/security-privacy/
 * 
 * * Our Statement of Position and Our Promise on Software Patents - http://www.certivox.com/about-certivox/patents/
 ******************************************************************************/
package com.certivox.net;


import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;

import android.net.Uri;
import android.text.TextUtils;


public class HTTPConnector implements IHTTPRequest {

    private Hashtable<String, String> requestHeaders;
    private Hashtable<String, String> queryParams;
    private String                    requestBody;
    private int                       timeout = DEFAULT_TIMEOUT;
    private String                    errorMessage;
    private int                       statusCode;
    private Hashtable<String, String> responseHeaders;
    private String                    responseData;

    private final static String OS_CLASS_HEADER = "X-MIRACL-OS-Class";
    private final static String OS_CLASS_VALUE = "android";

    // / only for test !!!!
    public String getContent() {

        return requestBody;
    }


    public Hashtable<String, String> RequestHeaders() {
        return requestHeaders;
    }


    public HTTPConnector() {
        super();
    }


    protected HttpURLConnection getConnection(String serviceURL, boolean output)
            throws MalformedURLException, IOException {
        if (serviceURL.startsWith("/")) {
            serviceURL = "http://ec2-54-77-232-113.eu-west-1.compute.amazonaws.com" + serviceURL;
        }

        HttpURLConnection httpConnection = (HttpURLConnection) new URL(serviceURL).openConnection();
        httpConnection.setDoInput(true);
        httpConnection.setDoOutput(output);
        return httpConnection;
    }


    protected String HttpMethodMapper(int method) {
        switch (method) {
        case GET:
            return HTTP_GET;
        case POST:
            return HTTP_POST;
        case PUT:
            return HTTP_PUT;
        case DELETE:
            return HTTP_DELETE;
        case OPTIONS:
            return HTTP_OPTIONS;
        default:
            return HTTP_PATCH;

        }
    }


    protected String sendRequest(String serviceURL, String http_method, String requestBody,
            Hashtable<String, String> requestProperties) throws IOException, HTTPErrorException {

        HttpURLConnection connection = null;
        DataOutputStream dos = null;
        String response = "200 OK";

        try {
            connection = getConnection(serviceURL, !TextUtils.isEmpty(requestBody));

            connection.setRequestMethod(http_method);
            connection.setConnectTimeout(timeout);

            if (requestProperties != null) {
                if (!requestProperties.isEmpty()) {
                    Enumeration<String> keyEnum = requestProperties.keys();
                    while (keyEnum.hasMoreElements()) {
                        String key = keyEnum.nextElement();
                        connection.setRequestProperty(key, requestProperties.get(key));
                    }
                }
            }
            
            connection.setRequestProperty(OS_CLASS_HEADER, OS_CLASS_VALUE);

            if (!TextUtils.isEmpty(requestBody)) {
                dos = new DataOutputStream(connection.getOutputStream());
                dos.writeBytes(requestBody);
            }

            // Starts the query
            connection.connect();

            try {
                statusCode = connection.getResponseCode();
            } catch (IOException e) {
                statusCode = connection.getResponseCode();
                if (statusCode != 401) {
                    throw e;
                }
            }

            responseHeaders = new Hashtable<String, String>();
            Map<String, List<String>> map = connection.getHeaderFields();
            for (Map.Entry<String, List<String>> entry : map.entrySet()) {
                List<String> propertyList = entry.getValue();
                String properties = "";
                for (String s : propertyList) {
                    properties += s;
                }
                String key = entry.getKey();
                if (key == null)
                    continue;
                responseHeaders.put(entry.getKey(), properties);
            }
            response = toString(connection.getInputStream());

        } finally {
            if (dos != null) {
                dos.close();
            }
            if (connection != null) {
                connection.disconnect();
            }
        }

        return response;
    }


    protected String sendRequest(String serviceURL, String http_method, String requestBody)
            throws IOException, HTTPErrorException {
        return sendRequest(serviceURL, http_method, requestBody, null);
    }


    protected String sendRequest(String serviceURL, String http_method) throws IOException, HTTPErrorException {
        return sendRequest(serviceURL, http_method, null);
    }


    @Override
    public void SetHeaders(Hashtable<String, String> headers) {
        this.requestHeaders = headers;
    }


    @Override
    public void SetQueryParams(Hashtable<String, String> queryParams) {
        this.queryParams = queryParams;
    }


    @Override
    public void SetContent(String data) {
        this.requestBody = data;
    }


    @Override
    public void SetTimeout(int seconds) {
        if (seconds <= 0)
            throw new IllegalArgumentException();
        this.timeout = seconds;
    }


    @Override
    public boolean Execute(int method, String url) {
        if (TextUtils.isEmpty(url))
            throw new IllegalArgumentException();

        String fullUrl = url;
        if (queryParams != null) {
            if (!queryParams.isEmpty()) {
                Enumeration<String> keyEnum = queryParams.keys();
                fullUrl += "?";
                while (keyEnum.hasMoreElements()) {
                    String key = keyEnum.nextElement();
                    fullUrl = key + "=" + queryParams.get(key) + "&";
                }
                fullUrl = fullUrl.substring(0, fullUrl.length() - 1);
            }
        }

        // TODO temporary hack
        Uri uri = Uri.parse(fullUrl);
        if ("wss".equals(uri.getScheme()))
            fullUrl = uri.buildUpon().scheme("https").build().toString();

        try {
            responseData = sendRequest(fullUrl, HttpMethodMapper(method), requestBody, requestHeaders);
        } catch (FileNotFoundException e) {
            // No data in response
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
            errorMessage = e.getLocalizedMessage();
            return false;
        }

        return true;
    }


    @Override
    public String GetExecuteErrorMessage() {
        return errorMessage;
    }


    @Override
    public int GetHttpStatusCode() {
        return statusCode;
    }


    @Override
    public Hashtable<String, String> GetResponseHeaders() {
        return responseHeaders;
    }


    @Override
    public String GetResponseData() {
        return responseData;
    }

    @SuppressWarnings("serial")
    public class HTTPErrorException extends Exception {

        private int statusCode;


        public HTTPErrorException() {
            // TODO Auto-generated constructor stub
        }


        public HTTPErrorException(String message) {
            super(message);
            // TODO Auto-generated constructor stub
        }


        public HTTPErrorException(String message, int statusCode) {
            super(message);
            setStatusCode(statusCode);
        }


        public HTTPErrorException(Throwable cause) {
            super(cause);
            // TODO Auto-generated constructor stub
        }


        public HTTPErrorException(String message, Throwable cause) {
            super(message, cause);
            // TODO Auto-generated constructor stub
        }


        public int getStatusCode() {
            return statusCode;
        }


        public void setStatusCode(int statusCode) {
            this.statusCode = statusCode;
        }
    }


    private static String toString(InputStream is) throws IOException {
        InputStreamReader isr = null;
        try {
            isr = new InputStreamReader(is, "UTF-8");
            char[] buf = new char[512];
            StringBuilder str = new StringBuilder();
            int i = 0;
            while ((i = isr.read(buf)) != -1)
                str.append(buf, 0, i);
            return str.toString();
        } finally {
            if (isr != null) {
                isr.close();
            }
        }
    }

}
