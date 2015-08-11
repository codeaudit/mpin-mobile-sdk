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
 * File:   MpinClient.h
 * Author: mony
 *
 * Created on February 11, 2015, 2:42 PM
 */

#ifndef MPINCLIENT_H
#define	MPINCLIENT_H

#include "mpin_sdk.h"

#include "CvThread.h"
#include "CvQueue.h"

typedef MPinSDK::String String;
typedef MPinSDK::StringMap StringMap;
	
class CStorage;
class CPinPad;
class CContext;

class CMpinClient
{
public:

	CMpinClient( int aClientId, const String& aBackendUrl, const String& aUserId, const String& aPinGood, const String& aPinBad );
	CMpinClient( int aClientId, const String& aBackendUrl, const String& aUserId );

	virtual ~CMpinClient();
	
	uint32_t		GetId() const { return m_id; }
	const String&	GetUserId() const { return m_userId; }
	
	void Register()			{ m_queue.Push( enEvent_Register ); }
	void AuthenticateGood()	{ m_queue.Push( enEvent_AuthenticateGood ); }
	void AuthenticateBad()	{ m_queue.Push( enEvent_AuthenticateBad ); }
	bool Done() const		{ return m_bIdle; }
	
	struct sStats_t
	{
		sStats_t() :
			m_numOfReg(0), m_minRegMsec(0), m_maxRegMsec(0), m_avgRegMsec(0),
			m_numOfAuth(0), m_minAuthMsec(0), m_maxAuthMsec(0), m_avgAuthMsec(0),
			m_numOfErrors(0) {}
		
		uint32_t	m_numOfAuth;
		uint32_t	m_numOfReg;
		uint32_t	m_minRegMsec;
		uint32_t	m_maxRegMsec;
		uint32_t	m_avgRegMsec;
		uint32_t	m_minAuthMsec;
		uint32_t	m_maxAuthMsec;
		uint32_t	m_avgAuthMsec;
		int			m_numOfErrors;
	};
	
	void EnableStats(bool abEnable = true) { m_bStatsEnabled = abEnable; }
	const sStats_t& GetStats() const	{ return m_stats; }
	
private:
	friend class CThread;
	
	class CStorage : public MPinSDK::IStorage
	{
	public:
		CStorage(const String& aFileNameSuffix);
		virtual ~CStorage() {}
		
		virtual bool SetData(const String& data);
		virtual bool GetData(OUT String &data);
		virtual const String& GetErrorMessage() const { return m_errorMsg; }
	private:
		String	m_fileName;
		String	m_errorMsg;
	};

	class CPinPad : public MPinSDK::IPinPad
	{
	public:
		CPinPad()	{}
		virtual ~CPinPad()	{}
		virtual String Show(Mode mode) { return m_pin; }

		void SetPin( const String& aPin )	{ m_pin = aPin; }

	private:
		String	m_pin;
	};

	class CContext : public MPinSDK::IContext
	{
	public:
		CContext( const String& aId, CStorage* apStorageSecure, CStorage* apStorageNonSecure, CPinPad* apPinPad ) :
			m_id(aId), m_pStorageSecure(apStorageSecure), m_pStorageNonSecure(apStorageNonSecure), m_pPinPad(apPinPad)
		{}		

		virtual ~CContext() {}

		virtual MPinSDK::IHttpRequest* CreateHttpRequest() const;
		virtual void ReleaseHttpRequest( IN MPinSDK::IHttpRequest *request ) const	{ delete request; }
		virtual MPinSDK::IStorage* GetStorage( MPinSDK::IStorage::Type type ) const	{ return (type == MPinSDK::IStorage::SECURE) ? m_pStorageSecure : m_pStorageNonSecure; }
		virtual MPinSDK::IPinPad* GetPinPad() const									{ return m_pPinPad; }
		virtual MPinSDK::CryptoType GetMPinCryptoType() const						{ return MPinSDK::CRYPTO_NON_TEE; }

	private:
		String		m_id;
		CStorage*	m_pStorageSecure;
		CStorage*	m_pStorageNonSecure;
		CPinPad*	m_pPinPad;
	};

	CMpinClient(const CMpinClient& orig);
	bool _Init(const String& aBackendUrl);
	bool _Authenticate( const String& aPin );
	bool _Register();
	bool _AuthenticateGood();
	bool _AuthenticateBad();
	
	uint32_t	m_id;
	
	MPinSDK		m_sdk;
	CStorage	m_storageSecure;
	CStorage	m_storageNonSecure;
	CPinPad		m_pinPad;
	CContext	m_context;
	
	bool		m_bInitialized;
	
	String		m_userId;
	String		m_pinGood;
	String		m_pinBad;
	
	enum enEvent_t
	{
		enEvent_Register,
		enEvent_AuthenticateGood,
		enEvent_AuthenticateBad,
		enEvent_Exit
	};

	typedef CvShared::CvThread				CvThread;
	typedef CvShared::CvQueue<enEvent_t>	CQueueEvents;
	
	class CThread : public CvThread
	{
	public:
		CThread( const String& aName ) : CvThread(aName.c_str()) {}
	private:
		virtual long Body( void* apArgs );
	};
		
	CThread			m_thread;
	CQueueEvents	m_queue;
	bool			m_bIdle;
	
	sStats_t		m_stats;
	bool			m_bStatsEnabled;
};

#endif	/* MPINCLIENT_H */

