package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import androidx.annotation.NonNull;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.FullScreenContentCallback;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.interstitial.InterstitialAd;
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback;

public class PublicInterstitialAdManager {
    private static InterstitialAd interstitialAd;
    private static final String AD_UNIT_ID = "ca-app-pub-3940256099942544/1033173712";
    private static final long RELOAD_INTERVAL_MS = 58 * 60 * 1000;
    private static final long RETRY_DELAY_MS = 60 * 1000;
    private static final long NULL_ACTIVITY_RETRY_DELAY_MS = 1000;
    private static final String LOG_TAG = "Interstitial";
    private static final Handler handler = new Handler(Looper.getMainLooper());
    private static final Runnable reloadAdRunnable = new Runnable() {
        @Override
        public void run() {
            Log.i(LOG_TAG, "timer update interstitial ad");
            preloadAd();
            handler.postDelayed(this, RELOAD_INTERVAL_MS);
        }
    };



    public static void initialize() {
        preloadAd();
    }

    public static void preloadAd() {
        Log.i(LOG_TAG, "preloadAd");
        if(AppLifecycleTracker.getCurrentActivity() == null
                || !PublicSdkInitialState.isAdMobSdkInitialed()) {
            Log.i(LOG_TAG, "Interstitial Reload.");
            if(AppLifecycleTracker.getCurrentActivity() == null)
                Log.i(LOG_TAG, "Activity Null.");
            if(!PublicSdkInitialState.isAdMobSdkInitialed())
                Log.i(LOG_TAG, "AdMob Not Init.");
            handler.postDelayed(PublicInterstitialAdManager::preloadAd,
                    NULL_ACTIVITY_RETRY_DELAY_MS);
            return;
        }
        AdRequest adRequest = new AdRequest.Builder().build();
        InterstitialAd.load(AppLifecycleTracker.getCurrentActivity(), AD_UNIT_ID, adRequest, new InterstitialAdLoadCallback() {
            @Override
            public void onAdLoaded(@NonNull InterstitialAd ad) {
                Log.i(LOG_TAG, "onAdLoaded");
                interstitialAd = ad;
                setAdCallbacks();
                handler.removeCallbacks(reloadAdRunnable);
                handler.postDelayed(reloadAdRunnable, RELOAD_INTERVAL_MS);
            }

            @Override
            public void onAdFailedToLoad(@NonNull LoadAdError adError) {
                Log.i(LOG_TAG, adError.toString());
                interstitialAd = null;
                handler.postDelayed(PublicInterstitialAdManager::preloadAd, RETRY_DELAY_MS);
            }
        });
    }

    public static void showAd() {
        Log.i(LOG_TAG, "ShowAd");
        Activity activity = AppLifecycleTracker.getCurrentActivity();
        if (interstitialAd != null && activity != null) {
            activity.runOnUiThread(()->interstitialAd.show(activity));
        }
    }

    public static boolean isAdReady() {
        return interstitialAd != null;
    }

    // 设置广告回调
    private static void setAdCallbacks() {
        if (interstitialAd == null) return;
        Log.i(LOG_TAG, "setAdCallbacks");
        interstitialAd.setFullScreenContentCallback(new FullScreenContentCallback() {
            @Override
            public void onAdDismissedFullScreenContent() {
                Log.i(LOG_TAG, "onAdDismissedFullScreenContent");
                interstitialAd = null;
                preloadAd();
            }

            @Override
            public void onAdFailedToShowFullScreenContent(@NonNull com.google.android.gms.ads.AdError adError) {
                Log.i(LOG_TAG, "onAdFailedToShowFullScreenContent");
                interstitialAd = null;
                preloadAd();
            }

            @Override
            public void onAdShowedFullScreenContent() {
                // 广告显示时的逻辑(如记录日志)
            }
        });
    }
}
