/* 
 * File:   CvLdapConnection.h
 * Author: mony
 *
 * Created on August 20, 2012, 2:53 PM
 */

#ifndef CVLDAPCONNECTION_H
#define	CVLDAPCONNECTION_H

#include <ldap.h>

#include "CvCommon.h"
#include "CvTime.h"

#include <list>
#include <map>
#include <string>

using namespace std;
using namespace CvShared;

class CvLdapResult;

enum enLdapScope_t
{
	enLdapScope_Default = LDAP_SCOPE_DEFAULT,
	enLdapScope_Base = LDAP_SCOPE_BASE,
	enLdapScope_OneLevel = LDAP_SCOPE_ONELEVEL,
	enLdapScope_SubTree = LDAP_SCOPE_SUBTREE,
	enLdapScope_Children = LDAP_SCOPE_CHILDREN
};

class CvLdapConnection
{
public:
	
	CvLdapConnection( const string& aHostUri );
	virtual ~CvLdapConnection();
	
	LDAP*	GetHandle() const	{ return m_pLdapConnection; }
	
	bool	Bind( const string& aUser, const string& aPassword, OUT int& aErrCode, OUT string& aErrDesc );
	
	bool	Search( const string& aBaseDn, enLdapScope_t aScope, const string& aFilter, const Millisecs& aTimeout,
					OUT CvLdapResult& aResult, OUT int& aErrCode, OUT string& aErrDesc );
	
	inline bool	Search( const string& aBaseDn, enLdapScope_t aScope, const Millisecs& aTimeout,
					OUT CvLdapResult& aResult, OUT int& aErrCode, OUT string& aErrDesc );
	
private:
	CvLdapConnection(const CvLdapConnection& orig)	{}
	bool	Init( const string& aHostUri );
	bool	Unbind();
	bool	Reconnect();
	
	string	m_hostUri;
	int		m_port;
	string	m_user;
	string	m_password;
	
	LDAP*	m_pLdapConnection;
	bool	m_bBound;
};

bool CvLdapConnection::Search( const string& aBaseDn, enLdapScope_t aScope, const Millisecs& aTimeout, OUT CvLdapResult& aResult, OUT int& aErrCode, OUT string& aErrDesc )
{
	return Search( aBaseDn, aScope, "objectClass=*", aTimeout, aResult, aErrCode, aErrDesc );
}

class CvLdapResult
{
	friend class CvLdapConnection;
	
public:
	
	class CEntry
	{
		friend class CvLdapResult;

	public:
		typedef list<string>				CListValues;
		typedef map<string, CListValues>	CMapAttrs;

		virtual ~CEntry()	{}

		const string&		GetDn() const		{ return m_dn; }
		const CMapAttrs&	GetAttrs() const	{ return m_mapAttrs; }

	protected:
		CEntry( const CvLdapConnection& aConnection, LDAPMessage* apLdapEntry );

		string		m_dn;
		CMapAttrs	m_mapAttrs;
	};
	
	typedef list<CEntry*>	CListEntries;
	
	CvLdapResult()	{}
	virtual ~CvLdapResult();
	
	const CListEntries&	GetEntries() const		{ return m_listEntries; }

protected:
	CvLdapResult( const CvLdapConnection& aConnection, LDAPMessage* apLdapResult );
	
	bool Init( const CvLdapConnection& aConnection, LDAPMessage* apLdapResult );
	void Clear();
	
	CListEntries		m_listEntries;
};

#endif	/* CVLDAPCONNECTION_H */
