package org.cocos2dx.cpp;

public class PublicStatusNotifier {
    static {
        System.loadLibrary("MyGame");
    }

    /**
     *
     * @param status
     * 199 调用show方法
     * 200 视频播放完成，可以获取奖励
     * 201 视频开始播放
     * 202 视频播放失败
     * 203 视频关闭
     */
    public static void setRewardVideoStatus(int status){
        nativeNotifyStatus("rewardVideoPlayStatus", status);
    }

    private static native void nativeNotifyStatus(String key, int value);
}
