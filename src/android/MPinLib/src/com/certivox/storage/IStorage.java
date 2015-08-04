package com.certivox.storage;


public interface IStorage {

    boolean SetData(String data);


    String GetData();


    String GetErrorMessage();
}
