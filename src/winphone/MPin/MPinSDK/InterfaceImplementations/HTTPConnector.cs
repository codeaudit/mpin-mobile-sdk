﻿using MPinRC;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.Threading;
using Windows.Web.Http;
using Windows.Web.Http.Headers;
using Windows.Storage.Streams;
using Windows.Security.Cryptography;
using Windows.Foundation;
using System.Runtime.InteropServices.WindowsRuntime;

namespace MPinSDK
{
    class HTTPConnector : IHttpRequest
    {
        #region Members
        public const int DEFAULT_TIMEOUT = 30 * 1000;

        private IDictionary<String, String> requestHeaders = new Dictionary<string, string>();
        private IDictionary<String, String> queryParams = new Dictionary<string, string>();
        private String requestBody;
        private int timeout = DEFAULT_TIMEOUT;
        private String errorMessage;
        private int statusCode;
        private IDictionary<String, String> responseHeaders = new Dictionary<string, string>();
        private String responseData;
        #endregion

        #region C'tor
        public HTTPConnector()
            : base()
        { }
        #endregion 
        
        #region IHTTPRequest
        public void SetHeaders(IDictionary<string, string> headers)
        {
            this.requestHeaders = headers;
        }

        public void SetQueryParams(IDictionary<string, string> queryParams)
        {
            this.queryParams = queryParams;
        }

        public void SetContent(string data)
        {
            this.requestBody = data;
        }

        public void SetTimeout(int seconds)
        {
            if (seconds <= 0) throw new ArgumentException("The timeout could not be set to a negative value!");
            this.timeout = seconds;
        }

        public bool Execute(Windows.Web.Http.HttpMethod method, string url)
        {
            bool result = false;
            Task.Run( async () =>
                {
                    result =  await ExecuteAsync(method, url);
                }).Wait();

            return result;            
        }
    
        public string GetExecuteErrorMessage()
        {
            return errorMessage;
        }

        public int GetHttpStatusCode()
        {
            return statusCode;
        }

        public IDictionary<string, string> GetResponseHeaders()
        {
            return responseHeaders;
        }

        public string GetResponseData()
        {
            return responseData;
        }
        #endregion // IHTTPRequest

        #region Methods
        
        private async Task<bool> ExecuteAsync(Windows.Web.Http.HttpMethod method, string url)
        {
            ClearResponseData();

            if (string.IsNullOrEmpty(url)) 
                throw new ArgumentException("The url value is empty!");

            String fullUrl = url;
            if (queryParams != null && queryParams.Count > 0)
            {
                ICollection<String> keyEnum = queryParams.Keys;
                fullUrl += "?";
                foreach (var key in keyEnum)
                {
                    fullUrl = key + "=" + queryParams[key] + "&";
                }

                fullUrl = fullUrl.Substring(0, fullUrl.Length - 1);
            }

            //Uri uri = new Uri(fullUrl);// Uri.parse(fullUrl);
            ////TODO temporary hack -> check
            //if ("wss".Equals(uri.Scheme))
            //    uri.Scheme ="https";
            bool successful = true;
            try
            {
                await this.SendRequest(fullUrl, method, requestBody, requestHeaders);
            }
            catch (Exception e)
            {
                errorMessage = e.Message;
                successful = false;
            }
            finally
            {
                ClearRequestData();
            }

            return successful;
        }

        private void ClearRequestData()
        {
            this.requestHeaders.Clear();
            this.queryParams.Clear();
            this.requestBody = string.Empty;
            this.timeout = 0;
        }

        private void ClearResponseData()
        {
            this.statusCode = 0;
            this.responseHeaders.Clear();
            this.responseData = string.Empty;
            this.errorMessage = string.Empty;
        }
       
        protected async Task SendRequest(String serviceURL, Windows.Web.Http.HttpMethod http_method, String requestBody, IDictionary<String, String> requestProperties)
        {
            // TODO: check if the response is empty, if an exception is thrown
            // empty resonse returned on unsuccessful authentication
            HttpClient httpClient = new HttpClient();
            CancellationTokenSource cts = new CancellationTokenSource();
            try
            {
                Uri resourceAddress = new Uri(serviceURL);
                HttpRequestMessage request = new HttpRequestMessage(http_method, resourceAddress);

                if (!string.IsNullOrEmpty(requestBody))
                {
                    Stream stream = new MemoryStream(Encoding.UTF8.GetBytes(requestBody ?? ""));
                    request.Content = new HttpStreamContent(stream.AsInputStream());
                }

                if (requestProperties != null && requestProperties.Count > 0)
                {
                    foreach (var key in requestProperties.Keys)
                    {
                        request.Properties.Add(new KeyValuePair<string, object>(key, requestProperties[key]));                        
                    }
                }

                HttpResponseMessage response = await httpClient.SendRequestAsync(
                    request,
                    HttpCompletionOption.ResponseHeadersRead).AsTask(cts.Token);

                SetResponseHeaders(response.Headers);
                this.responseData = await response.Content.ReadAsStringAsync();
                this.statusCode = (int)response.StatusCode;

            }
            catch (TaskCanceledException)
            {
                this.responseData = string.Empty;
                this.errorMessage = "Request canceled!";
            }
            catch (Exception ex)
            {
                this.responseData = string.Empty;
                this.errorMessage = "Error: " + ex.Message;
            }
        }
       
        private string ToString(StreamReader sr)
        {
            try
            {
                int i = 0, length = 512;
                char[] buf = new char[length];
                StringBuilder str = new StringBuilder();

                while ((i = sr.Read(buf, 0, length)) != -1)
                {
                    str.Append(buf, 0, i);
                }

                return str.ToString();
            }
            finally
            {
                if (sr != null)
                {
                    sr.Dispose();
                }
            }
        }
        
        private void SetResponseHeaders(HttpResponseHeaderCollection headers)
        {
            if (responseHeaders == null)
            {
                responseHeaders = new Dictionary<String, String>();
            }
            else
            {
                responseHeaders.Clear();
            }

            foreach (var key in headers.Keys)
            {
                String properties = "";
                foreach (var s in headers[key])
                {
                    properties += s;
                }
                responseHeaders.Add(new KeyValuePair<string, string>(key, properties));
            }
        }
        #endregion // Methods

        #region HTTPErrorException
        public class HTTPErrorException : Exception
        {
            public int StatusCode
            {
                get;
                set;
            }

            public HTTPErrorException()
                : base()
            { }

            public HTTPErrorException(String message)
                : base(message)
            { }

            public HTTPErrorException(String message, int statusCode)
                : base(message)
            {
                this.StatusCode = statusCode;
            }
        }
        #endregion // HTTPErrorException
    }
}
