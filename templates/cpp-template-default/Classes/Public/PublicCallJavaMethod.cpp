#include "PublicCallJavaMethod.h"
#include "cocos2d.h"
USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include <memory>
#endif


PublicJavaStatusManager* PublicJavaStatusManager::getInstance()
{
    static PublicJavaStatusManager ins;
    return &ins;
}

int PublicJavaStatusManager::getRewardVideoStatus()
{
	if (m_map.find("rewardVideoPlayStatus") == m_map.end()) {
		return 0;
	}
	return m_map["rewardVideoPlayStatus"];
}

void PublicJavaStatusManager::setKeyValue(const std::string& key, int val)
{
	if (key == "") { return; }
	if (m_map.find(key) != m_map.end() && m_map[key] == val) { return; }
	m_map[key] = val;

	emitChange(key, val);
}

PublicJavaStatusManager::PublicJavaStatusManager()
{

}

PublicJavaStatusManager::~PublicJavaStatusManager()
{

}

void PublicCallMethodVoidVoid(const std::string& method)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	CCLOG("CallJavaMethod:%s", method.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo methodInfo;
	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo,
		"org/cocos2dx/cpp/PublicNativeBridge",
		method.c_str(),
		"()V")) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
#endif
}

bool PublicCallMethodBoolVoid(const std::string& method)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    CCLOG("CallJavaMethod:%s", method.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniMethodInfo methodInfo;
    if (cocos2d::JniHelper::getStaticMethodInfo(
        methodInfo,
        "org/cocos2dx/cpp/PublicNativeBridge",
        method.c_str(),
        "()Z")) {
        jboolean result = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        return result == JNI_TRUE;
    }
#endif
    return false;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_PublicStatusNotifier_nativeNotifyStatus(JNIEnv * env, jclass clazz, jstring key_, jint value_)
{
	const char* key_cstr = env->GetStringUTFChars(key_, 0);

	std::string key(key_cstr);

	env->ReleaseStringUTFChars(key_, key_cstr);

	PublicJavaStatusManager::getInstance()->setKeyValue(key, static_cast<int>(value_));

	CCLOG("PublicJavaStatusManager:[%s, %d]", key.c_str(), static_cast<int>(value_));
}
#endif