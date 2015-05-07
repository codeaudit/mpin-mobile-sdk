/* 
 * File:   MpinClient.cpp
 * Author: mony
 * 
 * Created on February 11, 2015, 2:42 PM
 */

#include "MpinClient.h"

#include "HttpRequest.h"

#include "CvLogger.h"
#include "CvTime.h"

#include <fstream>

using namespace std;
using CvShared::SleepFor;
using CvShared::Millisecs;
using CvShared::Seconds;
using CvShared::TimeSpec;
using CvShared::GetCurrentTime;
using CvShared::LogMessage;

MPinSDK::IHttpRequest* CMpinClient::CContext::CreateHttpRequest() const
{
	return new CHttpRequest();
}
		
CMpinClient::CStorage::CStorage(const String& aFileNameSuffix)
{
	m_fileName = "client-storage-";
	m_fileName += aFileNameSuffix;
}

bool CMpinClient::CStorage::SetData(const String& data)
{
	std::ofstream file( m_fileName.c_str() );
	file << data;
	file.close();
	
//	printf( "Writing data to [%s]:\n%s\n", m_fileName.c_str(), data.c_str() );
	
	return true;
}

bool CMpinClient::CStorage::GetData(OUT String &data)
{
	std::ifstream file( m_fileName.c_str() );
	std::stringstream buffer;
	buffer << file.rdbuf();	
	file.close();
	
	data = buffer.str();
	
//	printf( "Reading data from [%s]:\n%s\n", m_fileName.c_str(), data.c_str() );
	
	return true;
}
		
CMpinClient::CMpinClient( int aClientId, const String& aBackendUrl, const String& aUserId ) :
	m_id(aClientId), m_userId(aUserId),
	m_storageSecure( String().Format("sec-%d", aClientId) ), m_storageNonSecure( String().Format("%d", aClientId) ),
	m_context(&m_storageSecure, &m_storageNonSecure, &m_pinPad),
	m_thread(aUserId), m_queue(aUserId.c_str()), m_bIdle(false), m_bStatsEnabled(true)
{
	std::ifstream filePin( String().Format("pin-%d", m_id).c_str() );
	filePin >> m_pinGood;
	filePin >> m_pinBad;
	
	_Init(aBackendUrl);	
}

CMpinClient::CMpinClient( int aClientId, const String& aBackendUrl, const String& aUserId, const String& aPinGood, const String& aPinBad ) :
	m_id(aClientId), m_userId(aUserId), m_pinGood(aPinGood), m_pinBad(aPinBad),
	m_storageSecure( String().Format("sec-%d", aClientId) ), m_storageNonSecure( String().Format("%d", aClientId) ),
	m_context(&m_storageSecure, &m_storageNonSecure, &m_pinPad),
	m_thread(aUserId), m_queue(aUserId.c_str()), m_bIdle(false), m_bStatsEnabled(true)
{
	std::ofstream filePin( String().Format("pin-%d", m_id).c_str() );
	filePin << m_pinGood << " " << m_pinBad;
	
	_Init(aBackendUrl);
}

CMpinClient::~CMpinClient()
{
	m_queue.PushFront(enEvent_Exit);
	
	SleepFor( Millisecs(100) );
}

bool CMpinClient::_Init(const String& aBackendUrl)
{
	printf( "Initializing client #%d for [%s] with PIN [%s] and BAD PIN [%s]\n", m_id, m_userId.c_str(), m_pinGood.c_str(), m_pinBad.c_str() );
	
	StringMap config;
	config["backend"] = aBackendUrl;
	
	MPinSDK::Status status = m_sdk.Init( config, &m_context );
	
	if ( status != MPinSDK::Status::OK )
	{
		printf( "Client #%d for user [%s] couldn't be initialized: %s", m_id, m_userId.c_str(), status.GetErrorMessage().c_str() );
		return false;
	}
	
	m_thread.Create(this);
	
	return true;
}
	
bool CMpinClient::_AuthenticateGood()
{
	return _Authenticate( m_pinGood );
}

bool CMpinClient::_AuthenticateBad()
{
	return _Authenticate( m_pinBad );
}

bool CMpinClient::_Register()
{
	printf( "Registering user [%s]...\n", m_userId.c_str() );
			
	std::vector<MPinSDK::UserPtr> listUsers;
	m_sdk.ListUsers( listUsers );
	
	std::vector<MPinSDK::UserPtr>::iterator itr = listUsers.begin();
	for ( ;itr != listUsers.end(); ++itr )
	{
		if ( (*itr)->GetId() == m_userId )
		{
			m_sdk.DeleteUser( *itr );
			break;
		}
	}

	MPinSDK::UserPtr user = m_sdk.MakeNewUser( m_userId, String().Format( "M-Pin Test Client #%d", m_id ) );
	
	m_pinPad.SetPin( m_pinGood );
			
	TimeSpec now;
	GetCurrentTime(now);
	Millisecs startTime = now.ToMillisecs();

	MPinSDK::Status status = m_sdk.StartRegistration( user, "{ \"data\": \"test\" }" );
	
	if ( status != MPinSDK::Status::OK )
	{
		printf( "Failed in StartRegistration(): %s [%d]\n", status.GetErrorMessage().c_str(), status.GetStatusCode() );
		if ( m_bStatsEnabled )
		{
			++m_stats.m_numOfErrors;
		}
		return false;
	}
	
	if ( user->GetState() != MPinSDK::User::ACTIVATED )
	{
		while ( user->GetState() != MPinSDK::User::REGISTERED )
		{
			printf( "User [%s] has NOT been activated yet\n", user->GetId().c_str() );

			CvShared::SleepFor( CvShared::Seconds(10) );

			status = m_sdk.FinishRegistration( user );
			
			if ( status == MPinSDK::Status::OK )
			{
				printf( "User [%s] has been activated\n", user->GetId().c_str() );
				continue;
			}
				
			if ( status != MPinSDK::Status::IDENTITY_NOT_VERIFIED )
			{
				printf( "Failed in FinishRegistration(): %s [%d]\n", status.GetErrorMessage().c_str(), status.GetStatusCode() );
				if ( m_bStatsEnabled )
				{
					++m_stats.m_numOfErrors;
				}
				return false;
			}
		}
	}
	else
	{
		printf( "User [%s] has been force-activated\n", user->GetId().c_str() );
		
		status = m_sdk.FinishRegistration( user );

		if ( status != MPinSDK::Status::OK )
		{
			printf( "Failed in FinishRegistration(): %s [%d]\n", status.GetErrorMessage().c_str(), status.GetStatusCode() );
			if ( m_bStatsEnabled )
			{
				++m_stats.m_numOfErrors;
			}
			return false;
		}
	}
	
	GetCurrentTime(now);
	
	if ( m_bStatsEnabled )
	{
		uint32_t currMsec = now.ToMillisecs() - startTime.Value();
		m_stats.m_avgRegMsec = ( m_stats.m_avgRegMsec*m_stats.m_numOfReg + currMsec ) / ( m_stats.m_numOfReg + 1 );
		++m_stats.m_numOfReg;
		
		if ( currMsec < m_stats.m_minRegMsec || m_stats.m_minRegMsec == 0 )
		{
			m_stats.m_minRegMsec = currMsec;
		}
		if ( currMsec > m_stats.m_maxRegMsec )
		{
			m_stats.m_maxRegMsec = currMsec;
		}
	}
	
	return true;
}

bool CMpinClient::_Authenticate( const String& aPin )
{
	std::vector<MPinSDK::UserPtr> listUsers;
	m_sdk.ListUsers( listUsers );
	
	std::vector<MPinSDK::UserPtr>::iterator itr = listUsers.begin();
	for ( ;itr != listUsers.end(); ++itr )
	{
		if ( (*itr)->GetId() == m_userId )
			break;
	}
	
	if ( itr == listUsers.end() )
	{
		printf( "User [%s] not found in the list\n", m_userId.c_str() );
		if ( m_bStatsEnabled )
		{
			++m_stats.m_numOfErrors;
		}
		return false;
	}
	
	MPinSDK::UserPtr user = *itr;
	
	m_pinPad.SetPin( aPin );
	
	if ( aPin == m_pinGood )
	{
		printf( "Authenticating user [%s] with correct PIN...\n", user->GetId().c_str() );
	}
	else
	{
		printf( "Authenticating user [%s] with incorrect PIN...\n", user->GetId().c_str() );		
	}
	
	TimeSpec now;
	GetCurrentTime(now);
	Millisecs startTime = now.ToMillisecs();

	MPinSDK::Status status = m_sdk.Authenticate( user );
	
	if ( aPin == m_pinGood )
	{
		if ( status != MPinSDK::Status::OK )
		{
			printf( "ERROR: Authentication for user [%s] failed: %s [%d]\n", user->GetId().c_str(), status.GetErrorMessage().c_str(), status.GetStatusCode() );
			if ( m_bStatsEnabled )
			{
				++m_stats.m_numOfErrors;
			}
			return false;
		}

		printf( "Authentication for user [%s] succeeded\n", user->GetId().c_str() );
	}
	else
	{
		if ( status == MPinSDK::Status::OK )
		{
			printf( "ERROR: Authentication for user [%s] succeeded ?!\n", user->GetId().c_str() );
			if ( m_bStatsEnabled )
			{
				++m_stats.m_numOfErrors;
			}
			return false;
		}
		
		printf( "Authentication for user [%s] failed (as it should): %s [%d]\n", user->GetId().c_str(), status.GetErrorMessage().c_str(), status.GetStatusCode() );		
	}

	GetCurrentTime(now);
	
	if ( m_bStatsEnabled )
	{
		uint32_t currMsec = now.ToMillisecs() - startTime.Value();
		m_stats.m_avgAuthMsec = ( m_stats.m_avgAuthMsec*m_stats.m_numOfAuth + currMsec ) / ( m_stats.m_numOfAuth + 1 );
		++m_stats.m_numOfAuth;
		
		if ( currMsec < m_stats.m_minAuthMsec || m_stats.m_minAuthMsec == 0 )
		{
			m_stats.m_minAuthMsec = currMsec;
		}
		if ( currMsec > m_stats.m_maxAuthMsec )
		{
			m_stats.m_maxAuthMsec = currMsec;
		}
	}
	
	return true;
}

long CMpinClient::CThread::Body( void* apArgs )
{
	CMpinClient* pClient = (CMpinClient*)apArgs;
	uint32_t id = pClient->m_id;
	
	bool bExit = false;
	
	while (!bExit)
	{
		enEvent_t event;
			
		if ( !pClient->m_queue.Pop( event, 0 ) )
		{
			pClient->m_bIdle = true;
			
			if ( !pClient->m_queue.Pop( event ) )
			{
				LogMessage( CvShared::enLogLevel_Error, "Client #%d: Error popping from the event queue. Thread [%s]", pClient->m_id, m_name.c_str() );
				SleepFor( Millisecs(500) );
				continue;
			}
		}
		
		pClient->m_bIdle = false;
		
		switch (event)
		{
			case enEvent_Register:
				pClient->_Register();
				break;
			case enEvent_AuthenticateGood:
				pClient->_AuthenticateGood();
				break;
			case enEvent_AuthenticateBad:
				pClient->_AuthenticateBad();
				break;
			case enEvent_Exit:
				pClient->m_bIdle = true;				
				bExit = true;
				break;
		}
	}
	
//	printf( "Client thread #%d is exiting...\n", id );
	
	return 0;
}
