#pragma once
#include <string>
#include <unordered_map>
#include "PublicSupport.h"
/*
* 保存java中的状态的类
*/
class PublicJavaStatusManager: public PublicSubject {
public:
	static PublicJavaStatusManager* getInstance();
	int getRewardVideoStatus();
	void setKeyValue(const std::string& key, int val);
private:
	PublicJavaStatusManager();
	~PublicJavaStatusManager();

	PublicJavaStatusManager(const PublicJavaStatusManager&) = delete;
	PublicJavaStatusManager& operator=(const PublicJavaStatusManager&) = delete;
private:
	std::unordered_map<std::string, int> m_map;
};


/*
* 调用java静态方法void(void) 
*/
void PublicCallMethodVoidVoid(const std::string& method);
/*
* 调用java静态方法bool(void)
*/
bool PublicCallMethodBoolVoid(const std::string& method);