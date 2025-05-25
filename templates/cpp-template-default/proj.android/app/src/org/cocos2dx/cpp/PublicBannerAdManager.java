package org.cocos2dx.cpp;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import androidx.annotation.NonNull;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.LoadAdError;

public class PublicBannerAdManager {
    private static AdView adView;
    private static boolean bAdLoaded;
    private static boolean bShow;
    private static final long NULL_ACTIVITY_RETRY_DELAY_MS = 1000;
    private static final String LOG_TAG = "Banner";
    private static final Handler handler = new Handler(Looper.getMainLooper());

    public static void initialize(AdView _adView) {
        Log.i(LOG_TAG, "initialize");
        adView = _adView;
        bAdLoaded = false;
        bShow = false;

        adView.setAdListener(new AdListener() {
            @Override
            public void onAdLoaded() {
                Log.i(LOG_TAG, "onAdLoaded");
                bAdLoaded = true;
                showAd();
            }

            @Override
            public void onAdFailedToLoad(@NonNull LoadAdError adError) {
                Log.i(LOG_TAG, "onAdFailedToLoad:" + adError.toString());
                preloadAd();
            }
        });

        preloadAd();
    }

    public static void showBanner() {
        Log.i(LOG_TAG, "showBanner");
        bShow = true;
        showAd();
    }

    public static void hideBanner() {
        Log.i(LOG_TAG, "hideBanner");
        bShow = false;
        showAd();
    }

    private static void preloadAd(){
        Log.i(LOG_TAG, "preloadAd0");
        if(adView == null || bAdLoaded){return;}
        Log.i(LOG_TAG, "preloadAd1");
        if(!PublicSdkInitialState.isAdMobSdkInitialed()
                || AppLifecycleTracker.getCurrentActivity() == null){
            handler.postDelayed(PublicBannerAdManager::preloadAd, NULL_ACTIVITY_RETRY_DELAY_MS);
            return;
        }
        Log.i(LOG_TAG, "preloadAd2");
        AppLifecycleTracker.getCurrentActivity().runOnUiThread(
                ()->adView.loadAd(new AdRequest.Builder().build())
        );
    }

    private static void showAd() {
        if(adView == null || AppLifecycleTracker.getCurrentActivity() == null){return;}
        if(bShow && PublicSdkInitialState.isAdMobSdkInitialed() && bAdLoaded){
            Log.i(LOG_TAG, "showBannerAd");
            AppLifecycleTracker.getCurrentActivity().runOnUiThread(
                    () -> adView.setVisibility(android.view.View.VISIBLE)
            );
        }
        else{
            Log.i(LOG_TAG, "hideBannerAd");
            AppLifecycleTracker.getCurrentActivity().runOnUiThread(
                    () -> adView.setVisibility(android.view.View.GONE)
            );
        }
    }

}
