package com.certivox.mpinsdk.test;

import java.util.HashMap;

import android.test.InstrumentationTestCase;
import android.test.suitebuilder.annotation.SmallTest;
import android.util.Log;

import com.certivox.mpinsdk.MPinSDKv2;
import com.certivox.models.User;
import com.certivox.models.Status;

public class MPinSDKv2Test extends InstrumentationTestCase {

	static {
		System.loadLibrary("AndroidMpinSDK");
	}

	@SmallTest
	public void test() {
		HashMap<String, String> config = new HashMap<String, String>();
		config.put("backend", "http://ec2-52-28-120-46.eu-central-1.compute.amazonaws.com");
		
		MPinSDKv2 sdk = new MPinSDKv2(getInstrumentation().getTargetContext(), config);
		Log.w("MPinSDKv2Test", "MPinSDKv2 object successfuly created (version " + sdk.GetVersion() + ")");
		
		User user = sdk.MakeNewUser("testUser");
		
		Status s = sdk.StartRegistration(user);
		assertTrue(s.getStatusCode() == Status.Code.OK);
		assertTrue(user.getState() == User.State.ACTIVATED);
		
		s = sdk.ConfirmRegistration(user);
		assertTrue(s.getStatusCode() == Status.Code.OK);
		
		s = sdk.FinishRegistration(user, "1234");
		assertTrue(s.getStatusCode() == Status.Code.OK);
		assertTrue(user.getState() == User.State.REGISTERED);
		
		Log.w("MPinSDKv2Test", "User registered and force activated");
		Log.w("MPinSDKv2Test", "Trying to authenticate user");
		
		s = sdk.StartAuthentication(user);
		assertTrue(s.getStatusCode() == Status.Code.OK);
		
		StringBuilder authResultData = new StringBuilder();
		s = sdk.FinishAuthentication(user, "1234", authResultData);
		assertTrue(s.getStatusCode() == Status.Code.OK);
		
		Log.w("MPinSDKv2Test", "User successfuly authenticated! Auth result data: " + authResultData);
		
		sdk.DeleteUser(user);
		Log.w("MPinSDKv2Test", "User deleted");
		sdk.close();
		Log.w("MPinSDKv2Test", "MPinSDKv2 object successfuly closed");
	}

}
