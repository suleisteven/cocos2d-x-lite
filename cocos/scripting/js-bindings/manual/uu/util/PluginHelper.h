/*
 * 中文
 *
 *  Created on: 2015-03-14
 *      Author: Steven
 *   Update on:
 *       Since: Lanlord 1.0.0
 */
#ifndef __brain__PluginHelper__
#define __brain__PluginHelper__

#include <iostream>

using namespace std;
class PluginHelper {
    
public:
    
#if CC_TARGET_PLATFORM==CC_PLATFORM_IOS
    
    void openURL(std::string appstoreUrl);
    
    static PluginHelper* getInstance();
    
    void onFacebookSharingButtonTouched(std::string descriptionWords,std::string imagePath, std::string appstoreUrl);
    
   // std::string getVersionCode();
    
    std::string getVersionName();
    
    std::string getChannelName();
    
	/**
	 * 拷贝数据到剪贴板
	 */
	void copyStringToClipboard(std::string content);

	/**
	 * 从剪贴板获取内容
	 */
	string getStringFromClipboard();

    /**
     * 获取系统剩余电量百分比
     * @return 0-100
     */
    int getBatteryPercent();
    
    
    /**
     * 获取设备唯一标示id
     */
    string getDeviceId();
    
    /**
     * 获取网络类型
     * @return 1-wifi；2-2g；3-3g；4-4g；99-未知
     */
    int getNetType();
    
    /**
     * 获取版本类型
     */
    string getOSVersion();
    
    /**
     * 获取设备型号
     */
    string getDeviceModel();
    
	/**
	 * 一键加入qq群
	 * @param qqGroupNum qq群号
	 * @param key qq群key
	 */
	bool joinQQGroup(const string& qqGroupNum, const string& key);
    
    typedef std::function<void(int, string, bool)> PayCallback;
    
    bool payForIAP(const string& productId, PayCallback callback);
#endif
    
};



#endif /* defined(__brain__PluginHelper__) */
