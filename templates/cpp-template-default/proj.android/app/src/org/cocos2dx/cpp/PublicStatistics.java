package org.cocos2dx.cpp;

import android.content.Context;
import com.umeng.analytics.MobclickAgent;
import android.util.Log;

public class PublicStatistics {
    private static Context appContext;

    public static void init(Context ctx) {
        appContext = ctx.getApplicationContext();
    }

    public static void logEvent(String eventId) {
        Log.i("UMENG", String.format("CUSTOM EVENT:%s", eventId) );
        MobclickAgent.onEvent(appContext, eventId);
    }
}
