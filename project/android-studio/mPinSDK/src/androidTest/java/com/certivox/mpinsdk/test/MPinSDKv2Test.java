package com.certivox.mpinsdk.test;

import android.test.InstrumentationTestCase;

import com.certivox.models.Status;
import com.certivox.models.User;
import com.certivox.mpinsdk.MPinSDKv2;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;

public class MPinSDKv2Test extends InstrumentationTestCase {

	static {
		System.loadLibrary("AndroidMpinSDK");
	}
	
	private MPinSDKv2 m_sdk = null;
	private User m_user = null;
	
	private static final String USER_ID = "testUser";
	private static final String BACKEND = "http://ec2-52-28-120-46.eu-central-1.compute.amazonaws.com";
	
	@Before
	public void setUp() {
		// Init the sdk
		m_sdk = new MPinSDKv2();
		
		HashMap<String, String> config = new HashMap<String, String>();
		config.put(MPinSDKv2.CONFIG_BACKEND, BACKEND);
		
		Status s = m_sdk.Init(config, getInstrumentation().getTargetContext());
		assertEquals("MPinSDKv2::Init failed: '" + s.getErrorMessage() + "'.", Status.Code.OK, s.getStatusCode());

		// Delete the USER_ID user if it was leftover in sdk for some reason (probably from previous test run) 
		LinkedList<User> users = new LinkedList<User>();
		m_sdk.ListUsers(users);
		Iterator<User> i = users.iterator(); 
		while(i.hasNext()) {
			User user = i.next();
			if(user.getId().equals(USER_ID)) {
				m_sdk.DeleteUser(user);
			}
		}
		
		m_user = null;
	}
	
	@After
	public void tearDown() {
		if(m_user != null) {
			m_sdk.DeleteUser(m_user);
			m_user = null;
		}
		
		m_sdk.close();
		m_sdk = null;
	}

	@Test
	public void testUserShouldRegisterAndAuthenticate() {
		m_user = m_sdk.MakeNewUser(USER_ID);
		
		Status s = m_sdk.StartRegistration(m_user);
		assertEquals("MPinSDKv2::StartRegistration failed: '" + s.getErrorMessage() + "'.", Status.Code.OK, s.getStatusCode());
		assertEquals("Unexpected user state after MPinSDKv2::StartRegistration (should be force activated).", User.State.ACTIVATED, m_user.getState());
		
		s = m_sdk.ConfirmRegistration(m_user);
		assertEquals("MPinSDKv2::ConfirmRegistration failed: '" + s.getErrorMessage() + "'.", Status.Code.OK, s.getStatusCode());
		
		s = m_sdk.FinishRegistration(m_user, "1234");
		assertEquals("MPinSDKv2::FinishRegistration failed: '" + s.getErrorMessage() + "'.", Status.Code.OK, s.getStatusCode());
		assertEquals("Unexpected user state after MPinSDKv2::FinishRegistration.", User.State.REGISTERED, m_user.getState());
		
		s = m_sdk.StartAuthentication(m_user);
		assertEquals("MPinSDKv2::StartAuthentication failed: '" + s.getErrorMessage() + "'.", Status.Code.OK, s.getStatusCode());
		
		StringBuilder authResultData = new StringBuilder();
		s = m_sdk.FinishAuthentication(m_user, "1234", authResultData);
		assertEquals("MPinSDKv2::FinishAuthentication failed: '" + s.getErrorMessage() + "'.", Status.Code.OK, s.getStatusCode());
	}
}
