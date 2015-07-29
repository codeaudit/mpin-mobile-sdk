package com.certivox.net;


import java.util.Hashtable;


public interface IHTTPRequest {

    public static final int    GET             = 0;
    public static final int    POST            = 1;
    public static final int    PUT             = 2;
    public static final int    DELETE          = 3;
    public static final int    OPTIONS         = 4;

    public static final String HTTP_GET        = "GET";
    public static final String HTTP_POST       = "POST";
    public static final String HTTP_PUT        = "PUT";
    public static final String HTTP_DELETE     = "DELETE";
    public static final String HTTP_OPTIONS    = "OPTIONS";
    public static final String HTTP_PATCH      = "PATCH";

    public static final int    DEFAULT_TIMEOUT = 10 * 1000;


    public void SetHeaders(Hashtable<String, String> headers);


    public void SetQueryParams(Hashtable<String, String> queryParams);


    public void SetContent(String data);


    public void SetTimeout(int seconds);


    public boolean Execute(int method, String url);


    public String GetExecuteErrorMessage();


    public int GetHttpStatusCode();


    public Hashtable<String, String> GetResponseHeaders();


    public String GetResponseData();
}
