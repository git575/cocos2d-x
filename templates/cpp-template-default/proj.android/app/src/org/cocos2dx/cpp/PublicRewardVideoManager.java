package org.cocos2dx.cpp;

import android.app.Activity;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import androidx.annotation.NonNull;

import com.google.android.gms.ads.AdError;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.FullScreenContentCallback;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.OnUserEarnedRewardListener;
import com.google.android.gms.ads.rewarded.RewardItem;
import com.google.android.gms.ads.rewarded.RewardedAd;
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;

public class PublicRewardVideoManager {
    private static RewardedAd rewardedAd;
    private static final String AD_UNIT_ID = "ca-app-pub-3940256099942544/5224354917";
    private static final long RELOAD_INTERVAL_MS = 58 * 60 * 1000;
    private static final long RETRY_DELAY_MS = 60 * 1000;
    private static final long NULL_ACTIVITY_RETRY_DELAY_MS = 1000;
    private static final Handler handler = new Handler(Looper.getMainLooper());
    private static final String LOG_TAG = "RewardVideo";
    private static final Runnable reloadAdRunnable = new Runnable() {
        @Override
        public void run() {
            Log.i(LOG_TAG, "timer update reward video.");
            preloadVideo();
            handler.postDelayed(this, RELOAD_INTERVAL_MS);
        }
    };

    public static void initialize() {
        preloadVideo();
    }

    public static void preloadVideo() {
        Log.i(LOG_TAG, "preloadVideo");
        if(AppLifecycleTracker.getCurrentActivity() == null
                || !PublicSdkInitialState.isAdMobSdkInitialed()) {
            if(AppLifecycleTracker.getCurrentActivity() == null)
                Log.i(LOG_TAG, "Activity Null.");
            if(!PublicSdkInitialState.isAdMobSdkInitialed())
                Log.i(LOG_TAG, "AdMob Not Init.");
            handler.postDelayed(
                    PublicRewardVideoManager::preloadVideo, NULL_ACTIVITY_RETRY_DELAY_MS);
            return;
        }
        AdRequest adRequest = new AdRequest.Builder().build();
        RewardedAd.load(AppLifecycleTracker.getCurrentActivity(), AD_UNIT_ID,
                adRequest, new RewardedAdLoadCallback() {
                    @Override
                    public void onAdFailedToLoad(@NonNull LoadAdError loadAdError) {
                        Log.i(LOG_TAG, loadAdError.toString());
                        rewardedAd = null;
                        handler.postDelayed(PublicRewardVideoManager::preloadVideo, RETRY_DELAY_MS);
                    }

                    @Override
                    public void onAdLoaded(@NonNull RewardedAd ad) {
                        Log.i(LOG_TAG, "Ad was loaded.");
                        rewardedAd = ad;
                        setVideoCallback();
                        handler.removeCallbacks(reloadAdRunnable);
                        handler.postDelayed(reloadAdRunnable, RELOAD_INTERVAL_MS);
                    }
                });
    }

    public static void showVideo() {
        Log.i(LOG_TAG, "showVideo");
        Activity activity = AppLifecycleTracker.getCurrentActivity();
        if (rewardedAd != null && activity != null) {
            Log.i(LOG_TAG, "showVideo run");
            PublicStatusNotifier.setRewardVideoStatus(199);
            activity.runOnUiThread(()-> rewardedAd.show(
                    activity,
                    new OnUserEarnedRewardListener() {
                        @Override
                        public void onUserEarnedReward(@NonNull RewardItem rewardItem) {
                            PublicStatusNotifier.setRewardVideoStatus(200);
                            Log.d(LOG_TAG, "The user earned the reward.");
                            preloadVideo();
                        }
                    })
            );
        }
    }

    public static boolean isVideoReady() {
        return rewardedAd != null;
    }

    public static void setVideoCallback(){
        if (rewardedAd == null) return;
        Log.i(LOG_TAG, "setVideoCallback");
        rewardedAd.setFullScreenContentCallback(new FullScreenContentCallback() {
            @Override
            public void onAdClicked() {
                // 点击广告触发
                Log.d(LOG_TAG, "Ad was clicked.");
            }

            @Override
            public void onAdDismissedFullScreenContent() {
                // 关闭视频触发
                PublicStatusNotifier.setRewardVideoStatus(203);
                Log.d(LOG_TAG, "Ad dismissed fullscreen content.");
                rewardedAd = null;
                preloadVideo();
            }

            @Override
            public void onAdFailedToShowFullScreenContent(@NonNull AdError adError) {
                // 播放失败触发
                Log.e(LOG_TAG, "Ad failed to show fullscreen content.");
                PublicStatusNotifier.setRewardVideoStatus(202);
                rewardedAd = null;
                preloadVideo();
            }

            @Override
            public void onAdImpression() {
                // 有效曝光触发
                Log.d(LOG_TAG, "Ad recorded an impression.");
            }

            @Override
            public void onAdShowedFullScreenContent() {
                // 视频开始展示触发
                Log.d(LOG_TAG, "Ad showed fullscreen content.");
                PublicStatusNotifier.setRewardVideoStatus(201);
            }
        });
    }
}
