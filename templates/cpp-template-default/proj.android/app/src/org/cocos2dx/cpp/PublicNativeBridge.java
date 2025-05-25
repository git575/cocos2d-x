package org.cocos2dx.cpp;

public class PublicNativeBridge {
    /**
    * 显示banner
    **/
    public static void showBanner() {
        PublicBannerAdManager.showBanner();
    }
    /**
     * 隐藏banner
     **/
    public static void hideBanner() {
        PublicBannerAdManager.hideBanner();
    }
    /**
     * 显示插页式广告
     **/
    public static void showInterstitial() {
        if(PublicInterstitialAdManager.isAdReady()){
            PublicInterstitialAdManager.showAd();
        }
    }
    /**
     * 激励视频是否准备好
     **/
    public static boolean isRewardedVideoReady() {
        return PublicRewardVideoManager.isVideoReady();
    }
    /**
     * 播放激励视频
     **/
    public static void playRewardVideo() {
        PublicRewardVideoManager.showVideo();
    }
    /**
     * 自定义事件上报
     **/
    public static void customEvent(String eventId) {
        PublicStatistics.logEvent(eventId);
    }
}
