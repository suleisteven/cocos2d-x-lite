/*
 * 中文
 *
 *  Created on: 2015-03-14
 *      Author: Steven
 *   Update on:
 *       Since: Lanlord 1.0.0
 */
#include "PlatformFunUtil.h"
#include "StringUtils.h"


//===================== android ======================  
#define ANDROID_CLASS_NAME "org/cocos2dx/cpp/NativeInteractive"  

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)   

extern "C"
{
	
}
#endif  



#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
bool PlatformFunUtil::getJNIStaticMethodInfo(cocos2d::JniMethodInfo &methodinfo,
											 const char *methodName,
											 const char *paramCode) 
{
	return cocos2d::JniHelper::getStaticMethodInfo(methodinfo,
		CLASS_NAME,
		methodName,
		paramCode);
}

#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

map<string, string> PlatformFunUtil::_externalParams;

bool PlatformFunUtil::_isOpenWithOther = false;



void PlatformFunUtil::handleExternalUrl(const string& url)
{
	map<string, string> params = StringUtil::urlParamsParse(url);

	PlatformFunUtil::_externalParams = params;

	if (!params.empty())
	{
		PlatformFunUtil::_isOpenWithOther = true;
	}
	else
	{
		PlatformFunUtil::_isOpenWithOther = false;
	}
}


#endif

void PlatformFunUtil::rateUs()
{
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "rateUs", "()V")) {
		return ;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	PluginHelper::getInstance()->openURL(OnlineConfigParams::getAppDetailAppstoreUrl());
#endif


}

int PlatformFunUtil::getAppVersionCode()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "getAppVersionCode", "()I")) {
		return 0;
	}

	int ret = 0;
	jint code = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	ret = (int)code;
	return ret;
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    static int iosVersionCode = 0;
    
    if(iosVersionCode!=0)
    {
        return iosVersionCode;
    }
	string version = PluginHelper::getInstance()->getVersionName();
	//StringUtil::replaceAll(version, ".", "");
    vector<string> versionVec = StringUtil::split(version, '.');
    
    
    int v0 = __String(versionVec.at(0)).intValue();
    int v1 = __String(versionVec.at(1)).intValue();
    int v2 = __String(versionVec.at(2)).intValue();
    string s = StringUtils::format("%02d%02d%02d", v0,v1,v2);
    
	int ret = 0;
	if (StringUtil::isInt(s))
	{
		int code = __String(s).intValue();
        ret = code;
        iosVersionCode = ret;
	}
	return ret;
#endif

	return 0;
}

std::string PlatformFunUtil::getAppVersionName()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "getAppVersionName", "()Ljava/lang/String;")) {

		return "";
	}

	std::string ret("");
	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	ret = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);
	return ret;

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return PluginHelper::getInstance()->getVersionName();
#endif
	return "1.0";
}
std::string PlatformFunUtil::getAppChannel()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "getAppChannel", "()Ljava/lang/String;")) {
		return "";
	}

	std::string ret("");
	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	ret = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);
	return ret;

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	return PluginHelper::getInstance()->getChannelName();
#endif
	return "windows";
}


void PlatformFunUtil::showToast(std::string content, bool isShowLongTime)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "showToast", "(Ljava/lang/String;Z)V")) {
		return;
	}

	jstring stringArg = methodInfo.env->NewStringUTF(content.c_str());
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, isShowLongTime);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	methodInfo.env->DeleteLocalRef(stringArg);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	
#endif
	
}

void PlatformFunUtil::restartApp(const int& delayTime)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "restartApp", "(J)V")) {
		return;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (jlong)delayTime);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#endif
}


void PlatformFunUtil::copyStringToClipboard(const string& content)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "copyStringToClipboard", "(Ljava/lang/String;)V")) {
		return;
	}

	jstring contentJStr = methodInfo.env->NewStringUTF(content.c_str());

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, contentJStr);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	methodInfo.env->DeleteLocalRef(contentJStr);

	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	PluginHelper::getInstance()->copyStringToClipboard(content);
#endif
}

std::string PlatformFunUtil::getStringFromClipboard()
{
	string result = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "getStringFromClipboard", "()Ljava/lang/String;")) {
		return;
	}

	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);

	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	result = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	result = PluginHelper::getInstance()->getStringFromClipboard();
#endif

	return result;
}

bool PlatformFunUtil::joinQQGroup(const string& qqGroupNum, const string& key)
{
	bool result = false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "joinQQGroup", "(Ljava/lang/String;)Z")) {
		return;
	}

	jstring contentJStr = methodInfo.env->NewStringUTF(key.c_str());

	result = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, contentJStr);

	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	methodInfo.env->DeleteLocalRef(contentJStr);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	result = PluginHelper::getInstance()->joinQQGroup(qqGroupNum, key);
#endif

	return result;
}

bool PlatformFunUtil::isOpenWithOther()
{
	bool result = false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "isOpenWithOther", "()Z")) {
		return;
	}

	result = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);

	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	result = PlatformFunUtil::_isOpenWithOther;	
#endif

	return result;
}

void PlatformFunUtil::setOpenWithOther(const bool& value)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "setOpenWithOther", "(Z)")) {
		return;
	}

	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);


#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	PlatformFunUtil::_isOpenWithOther = value;
#endif

	
	
}

string PlatformFunUtil::getExternalParam(const string& key)
{
	string result;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "getExternalParam", "(Ljava/lang/String;)Ljava/lang/String;")) {
		return;
	}
	jstring contentJStr = methodInfo.env->NewStringUTF(key.c_str());
	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID,contentJStr);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	methodInfo.env->DeleteLocalRef(contentJStr);

	result = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

	auto it = PlatformFunUtil::_externalParams.find(key);

	if (it != PlatformFunUtil::_externalParams.end()){
		result =it->second;
	}
#endif

	return result;
}




std::string PlatformFunUtil::getSignInfo()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	//cocos2d::JniMethodInfo methodInfo;

	//if (! getJNIStaticMethodInfo(methodInfo, "getSignInfo", "()Ljava/lang/String;")) {
	//	return "";
	//}

	//std::string ret("");
	//jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	//methodInfo.env->DeleteLocalRef(methodInfo.classID);
	//ret = JniHelper::jstring2string(str);
	//methodInfo.env->DeleteLocalRef(str);
	//return ret;
	
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "getContext", "()Landroid/content/Context;")) 
	{
		DEBUG_LOG("getSignInfo");
		return "";
	}

	
	jobject context_object = (jobject)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);

	jclass context_class = methodInfo.env->GetObjectClass(context_object);  
	//context.getPackageManager()  
	jmethodID methodId = methodInfo.env->GetMethodID(context_class, "getPackageManager", "()Landroid/content/pm/PackageManager;");  
	jobject package_manager_object = methodInfo.env->CallObjectMethod(context_object, methodId);  
	if (package_manager_object == NULL) {  
		DEBUG_LOG("getPackageManager() Failed!");  
		return "";  
	}  

	//context.getPackageName()  
	methodId = methodInfo.env->GetMethodID(context_class, "getPackageName", "()Ljava/lang/String;");  
	jstring package_name_string = (jstring)methodInfo.env->CallObjectMethod(context_object, methodId);

	methodInfo.env->DeleteLocalRef(context_object);
	if (package_name_string == NULL) {  
		DEBUG_LOG("getPackageName() Failed!");  
		return "";  
	}  
	methodInfo.env->DeleteLocalRef(context_class);  

	//PackageManager.getPackageInfo(Sting, int)  
	jclass pack_manager_class = methodInfo.env->GetObjectClass(package_manager_object);  
	methodId = methodInfo.env->GetMethodID(pack_manager_class, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");  
	methodInfo.env->DeleteLocalRef(pack_manager_class);  
	jobject package_info_object = methodInfo.env->CallObjectMethod(package_manager_object, methodId, package_name_string, 64);  

	methodInfo.env->DeleteLocalRef(package_name_string);
	if (package_info_object == NULL) {  
		DEBUG_LOG("getPackageInfo() Failed!");  
		return "";  
	}  

	methodInfo.env->DeleteLocalRef(package_manager_object);  

	//PackageInfo.signatures[0]  
	jclass package_info_class = methodInfo.env->GetObjectClass(package_info_object);  
	jfieldID fieldId = methodInfo.env->GetFieldID(package_info_class, "signatures", "[Landroid/content/pm/Signature;");  
	methodInfo.env->DeleteLocalRef(package_info_class);  
	jobjectArray signature_object_array = (jobjectArray)methodInfo.env->GetObjectField(package_info_object, fieldId);  
	if (signature_object_array == NULL) {  
		DEBUG_LOG("PackageInfo.signatures[] is null");  
		return "";  
	}  
	jobject signature_object = methodInfo.env->GetObjectArrayElement(signature_object_array, 0);  
	methodInfo.env->DeleteLocalRef(signature_object_array);  
	methodInfo.env->DeleteLocalRef(package_info_object);  

	//Signature.toCharsString()  
	jclass signature_class = methodInfo.env->GetObjectClass(signature_object);  
	methodId = methodInfo.env->GetMethodID(signature_class, "toByteArray", "()[B");  
	methodInfo.env->DeleteLocalRef(signature_class);  
	jbyteArray signatureByteArray = (jbyteArray) methodInfo.env->CallObjectMethod(signature_object, methodId);

	char* signatureData = (char*)methodInfo.env->GetByteArrayElements(signatureByteArray, 0);
	jsize signatureDataSize = methodInfo.env->GetArrayLength(signatureByteArray);
	int length = (int)signatureDataSize;
	methodInfo.env->DeleteLocalRef(signatureByteArray);

	std::string result = MD5Util::getStringUseMD5(signatureData,length);
	
	DEBUG_LOG("result:%s", result.c_str());
	return result;

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#endif
	return "";
}

int PlatformFunUtil::getBatteryPercent()
{
	int result = 0;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (! getJNIStaticMethodInfo(methodInfo, "getBatteryPercent", "()I")) {
		return result;
	}

	jint batteryPercent = (jint)methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	result = (int)batteryPercent;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    result = PluginHelper::getInstance()->getBatteryPercent();
#else
	result = 100;
#endif
	return result;
}

int PlatformFunUtil::getNetType()
{
    int result = 99;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    cocos2d::JniMethodInfo methodInfo;
    
    if (! getJNIStaticMethodInfo(methodInfo, "getNetType", "()I")) {
        return result;
    }
    
    jint networktType = (jint)methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    result = (int)networktType;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    result = PluginHelper::getInstance()->getNetType();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    result = 1;
#endif
    return result;
}

std::string PlatformFunUtil::getOSType()
{
	string result = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	result = "android";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	result = "ios";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	result =  "windows";
#endif
	return result;
}

std::string PlatformFunUtil::getOSVersion()
{
	string result = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "getOSVersion", "()Ljava/lang/String;")) {
		return "";
	}

	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	result = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    result =  PluginHelper::getInstance()->getOSVersion();
#else
	result = "7.0";
#endif
	return result;
}

std::string PlatformFunUtil::getNetTypeString()
{
	int type = getNetType();
	string result;
	if (type == 1)
	{
		result = "WIFI";
	}
	else if (type == 2)
	{
		result = "2G";
	}
	else if (type == 3)
	{
		result = "3G";
	}
	else if (type == 4)
	{
		result = "4G";
	}
	else
	{
		result = "UNKNOW";
	}
	return result;
}

std::string PlatformFunUtil::getDeviceModel()
{
	string result = "";

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	
	cocos2d::JniMethodInfo methodInfo;

	if (!getJNIStaticMethodInfo(methodInfo, "getDeviceModel", "()Ljava/lang/String;")) {
		return "";
	}

	jstring str = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	result = JniHelper::jstring2string(str);
	methodInfo.env->DeleteLocalRef(str);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	result =  PluginHelper::getInstance()->getDeviceModel();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	result = "Windows7";
#endif
	return result;
}


