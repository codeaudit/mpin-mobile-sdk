package com.certivox.utils;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * String utility class
 * @author Georgi Georgiev
 *
 */
public class StringUtils {
	
	public static boolean isBlank(String str) {
		return str == null || str.length() == 0;
	}
	
	public static boolean isNotBlank(String str) {
		return str != null && str.length() > 0;
	}
	
	
	public static Date stringToDate(String strDate) {
		DateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss"); 
		 try {
			return (Date)formatter.parse(strDate);
		} catch (ParseException e) {
			e.printStackTrace();
			return null;
		}
	}
	
	
	public static String convertStreamToString(InputStream is) {
		if( is == null ) return "";

		StringBuilder sb = new StringBuilder();
		BufferedReader br = null;
		try {
			br = new BufferedReader (new InputStreamReader((is)));
			String line = null;
			while ((line = br.readLine()) != null) {
				sb.append(line);
			}	
			return sb.toString();
		} catch(Exception e) {
			e.printStackTrace();
			return null;
		} finally {
			if (br != null) {
				try {
					br.close();
				} catch(Exception e) { 
					e.printStackTrace();
				}
			}
		}
	}
	
	public static String convertStreamToString(BufferedReader br) {
		if(br == null ) return "";
		StringBuilder sb = new StringBuilder();
		try {
			String line = null;
			while ((line = br.readLine()) != null) {
				sb.append(line);
			}	
			return sb.toString();
		} catch(Exception e) {
			e.printStackTrace();
			return null;
		} finally {
			if (br != null) {
				try {
					br.close();
				} catch(Exception e) {
					e.printStackTrace();
				}
			}
		}
	}
	
}