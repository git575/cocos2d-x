package org.cocos2dx.cpp;

public class PublicSdkInitialState {
    static boolean bIsAdMobSdkInitialed = false;

    static void setAdMobSdkInitialed(){
        bIsAdMobSdkInitialed = true;
    }

    static boolean isAdMobSdkInitialed(){
        return bIsAdMobSdkInitialed;
    }
}
