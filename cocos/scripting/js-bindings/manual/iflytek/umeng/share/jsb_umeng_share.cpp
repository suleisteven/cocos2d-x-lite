#include "jsb_umeng_share.h"


#include "base/ccUTF8.h"
#include "base/CCDirector.h"
#include "platform/CCPlatformMacros.h"
#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/manual/spidermonkey_specifics.h"
#include "cocos2d.h"
#include <string>
#include <map>

using namespace std;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Cocos2dx/Common/CCUMSocialSDK.h"

using namespace umeng::social;

static CCUMSocialSDK *sdk = nullptr;
#endif

USING_NS_CC;
JSClass  *js_cocos2dx_umeng_share_class;
JSObject *js_cocos2dx_umeng_share_prototype;



class JSB_UmengShareNative
{
public:
	JSB_UmengShareNative()
	{
		JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
		_JSDelegate.construct(cx);
	}

	~JSB_UmengShareNative()
	{
		_JSDelegate.destroyIfConstructed();
	}

	void setJSDelegate(JS::HandleObject pJSDelegate)
	{
		_JSDelegate.ref() = pJSDelegate;
	}
protected:
public:

	mozilla::Maybe<JS::PersistentRootedObject> _JSDelegate;
};

static JSB_UmengShareNative* curLoginDelegate = nullptr;
static JSB_UmengShareNative* curShareToDelegate = nullptr;

bool js_cocos2dx_umeng_share_init(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("##js_cocos2dx_umeng_share_init");
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	if (argc == 5)
	{
		if (argv[0].isString() && argv[1].isString() && argv[2].isString() && argv[3].isString() && argv[4].isString())
		{
			std::string umengKey;
			jsval_to_std_string(cx, argv[0], &umengKey);

			std::string qqOpenId;
			jsval_to_std_string(cx, argv[1], &qqOpenId);

			std::string qqKey;
			jsval_to_std_string(cx, argv[2], &qqKey);

			std::string weixinAppId;
			jsval_to_std_string(cx, argv[3], &weixinAppId);

			std::string weixinAppKey;
			jsval_to_std_string(cx, argv[4], &weixinAppKey);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
			CCLOG("##init 111");
			static bool _isInitedUmengSocial = false;

			//////////////////////////////////// 初始化umeng第三方登录设置 ////////////////////////////////////
			sdk = CCUMSocialSDK::create(umengKey.c_str());
			CCLOG("##init 2xxx");
			//sdk->setQQAppIdAndAppKey("1103185283", "YTfduIwhHmCPwjzs");

			if (!_isInitedUmengSocial)
			{
				CCLOG("##init 3xxxxx:%s", qqOpenId.c_str());
				sdk->setQQAppIdAndAppKey(qqOpenId.c_str(), qqKey.c_str());
				CCLOG("##init 4");
				sdk->setWeiXinAppInfo(weixinAppId.c_str(), weixinAppKey.c_str());
				CCLOG("##init 6");

				// 设置友盟SDK支持的平台, 记得加上QQ或者QQ空间
				vector<int>* platforms = new vector<int>();
				CCLOG("##init 6");
				platforms->push_back(QQ); // 支持qq
				platforms->push_back(QZONE); // 支持qq空间
				platforms->push_back(WEIXIN); // 支持微信
				platforms->push_back(WEIXIN_CIRCLE); // 支持微信朋友圈
				CCLOG("##init 7");

				// 将这些平台添加到SDK中
				sdk->setPlatforms(platforms);
				_isInitedUmengSocial = true;
				CCLOG("##init 8");
			}

			CCLOG("##init 9");
			sdk->deleteAuthorization(Platform::WEIXIN, nullptr);
			CCLOG("##init 10");
			bool resultQQ = sdk->isAuthorized(QQ);
			CCLOG("##init 11");
			if (resultQQ)
			{
				CCLOG("##init 12");
				sdk->deleteAuthorization(QQ, nullptr);
			}
			CCLOG("##init 13");

			/*bool resultWeiXin = sdk->isAuthorized(Platform::WEIXIN);
			if(resultWeiXin)
			{
			sdk->deleteAuthorization(Platform::WEIXIN, nullptr);
			}*/

			////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
		}
		else
		{
			JS_ReportError(cx, "wrong params js_cocos2dx_umeng_share_init, need 5 string");
			return false;
		}
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 5);
		return false;
	}
	return true;
}


void authCallback(int platform, int stCode, std::map<string, string>& data)
{

#if CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	bool isError =false;
	if (stCode == 100)
	{
		CCLOG("##authStart");
	}
	else if (stCode == 200)
	{
		CCLOG("##authFinish");
		string openid = "";
		string token = "";
		

		bool isDeleteOauth = false;
		// 输入授权数据, 如果授权失败,则会输出错误信息
		map<string, string>::iterator it = data.begin();
		for (; it != data.end(); ++it) {
			CCLOG("#### data  %s -> %s.", it->first.c_str(), it->second.c_str());

			if (it->first == "uid")
			{
				openid = it->second;
			}
			else if (it->first == "token")
			{
				token = it->second;
			}
			else if (it->second.compare("deleteOauth") == 0)
			{
				isDeleteOauth = true;
			}
		}


		if (!token.empty())
		{
			string accountType = "";
			switch (platform) {
			case Platform::QQ:
			{
				accountType = "qq";
				
			}
				break;
			case Platform::WEIXIN:
				accountType = "weixin";
				
				break;
			default:
				break;
			}

			if(curLoginDelegate)
			{
				Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){

					if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
						return;

					JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET

					JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
					JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));

					int32_t status = stCode;

					JS::RootedValue statusCodeJS(cx);
					statusCodeJS = INT_TO_JSVAL(status);
					JS_SetProperty(cx, jsobj, "status", statusCodeJS);

					JS::RootedValue openidJS(cx);
					openidJS = c_string_to_jsval(cx,openid.c_str());
					JS_SetProperty(cx, jsobj, "openid", openidJS);


					JS::RootedValue tokenJS(cx);
					tokenJS = c_string_to_jsval(cx, token.c_str());
					JS_SetProperty(cx, jsobj, "token", tokenJS);


					JS::RootedValue args(cx, OBJECT_TO_JSVAL(jsobj));

					ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(curLoginDelegate->_JSDelegate.ref()), "onLoginResult", 1, args.address());
				});
			}
			
		}
		else
		{
			if (!isDeleteOauth)
			{
				isError = true;
			}
		}


	}
	else if (stCode == 0)
	{
		CCLOG("##authError");

		isError = true;
		
	}
	else if (stCode == -1)
	{
		CCLOG("##authCancel");

		if(curLoginDelegate)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){

				if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
					return;

				JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET


					JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
				JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));

				int32_t status = stCode;

				JS::RootedValue statusCodeJS(cx);
				statusCodeJS = INT_TO_JSVAL(status);
				JS_SetProperty(cx, jsobj, "status", statusCodeJS);

				JS::RootedValue openidJS(cx);
				openidJS = c_string_to_jsval(cx, "");
				JS_SetProperty(cx, jsobj, "openid", openidJS);


				JS::RootedValue tokenJS(cx);
				tokenJS = c_string_to_jsval(cx, "");
				JS_SetProperty(cx, jsobj, "token", tokenJS);


				JS::RootedValue args(cx, OBJECT_TO_JSVAL(jsobj));

				ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(curLoginDelegate->_JSDelegate.ref()), "onLoginResult", 1, args.address());
			});
		}
	}


	if(isError)
	{
		if(curLoginDelegate)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){

				if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
					return;

				JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET


					JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
				JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));

				int32_t status = -1;

				JS::RootedValue statusCodeJS(cx);
				statusCodeJS = INT_TO_JSVAL(status);
				JS_SetProperty(cx, jsobj, "status", statusCodeJS);

				JS::RootedValue openidJS(cx);
				openidJS = c_string_to_jsval(cx, "");
				JS_SetProperty(cx, jsobj, "openid", openidJS);


				JS::RootedValue tokenJS(cx);
				tokenJS = c_string_to_jsval(cx, "");
				JS_SetProperty(cx, jsobj, "token", tokenJS);


				JS::RootedValue args(cx, OBJECT_TO_JSVAL(jsobj));

				ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(curLoginDelegate->_JSDelegate.ref()), "onLoginResult", 1, args.address());
			});
		}
	}
	

	// 输入授权数据, 如果授权失败,则会输出错误信息
	map<string, string>::iterator it = data.begin();
	for (; it != data.end(); ++it) {
		CCLOG("#### data  %s -> %s.", it->first.c_str(), it->second.c_str());
	}
#endif
}


void shareCallbackDelegate(int platform, int stCode,
	const string& errorMsg)
{
	CCLOG("share callback:%d, %d, %s", platform, stCode, errorMsg.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if (curShareToDelegate)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){

			if (cocos2d::Director::getInstance() == nullptr || cocos2d::ScriptEngineManager::getInstance() == nullptr)
				return;

			JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET


				JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
			JS::RootedObject jsobj(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));

			int32_t status = 0;

			JS::RootedValue statusCodeJS(cx);
			statusCodeJS = INT_TO_JSVAL(stCode);
			JS_SetProperty(cx, jsobj, "status", statusCodeJS);

			string platformStr = "";
			if (platform == Platform::QQ)
			{
				platformStr = "qq";
			}
			else if (platform == Platform::WEIXIN)
			{
				platformStr = "weixin";
			}

			JS::RootedValue platformJS(cx);
			platformJS = c_string_to_jsval(cx, platformStr.c_str());
			JS_SetProperty(cx, jsobj, "platform", platformJS);


			JS::RootedValue errorMsgJS(cx);
			errorMsgJS = c_string_to_jsval(cx, errorMsg.c_str());
			JS_SetProperty(cx, jsobj, "errorMsg", errorMsgJS);


			JS::RootedValue args(cx, OBJECT_TO_JSVAL(jsobj));

			ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(curShareToDelegate->_JSDelegate.ref()), "onShareResult", 1, args.address());
		});
	}
#endif
	
}

bool js_cocos2dx_umeng_share_share_to(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	JSB_UmengShareNative* cobj = (JSB_UmengShareNative *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	if (argc == 5)
	{
		if (argv[0].isString())
		{
			std::string accountType;
			jsval_to_std_string(cx, argv[0], &accountType);

			std::string content;
			jsval_to_std_string(cx, argv[1], &content);

			std::string img;
			jsval_to_std_string(cx, argv[2], &img);

			std::string title;
			jsval_to_std_string(cx, argv[3], &title);

			std::string url;
			jsval_to_std_string(cx, argv[4], &url);

			
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
			
			curShareToDelegate = cobj;
			
			Platform platform = Platform::WEIXIN;

			if (accountType == "qq")
			{
				platform = Platform::QQ;
			}
			else if (accountType == "weixin")
			{
				platform = Platform::WEIXIN;
			}

			sdk->setPlatformShareContent(platform, content.c_str(), img.c_str(), title.c_str(), url.c_str());
			sdk->directShare(platform, "", "", share_selector(shareCallbackDelegate));
#endif
		}
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 5);
		return false;
	}
	return true;
}

bool js_cocos2dx_umeng_share_login(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	JSB_UmengShareNative* cobj = (JSB_UmengShareNative *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	if (argc == 1)
	{
		if (argv[0].isString())
		{
			std::string accountType;
			jsval_to_std_string(cx, argv[0], &accountType);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
			curLoginDelegate = cobj;
			if (accountType == "qq")
			{
				// 对某个平台授权, 参数二为授权回调
				sdk->authorize(Platform::QQ, auth_selector(authCallback));

			}
			else if (accountType == "weixin")
			{
				sdk->authorize(Platform::WEIXIN, auth_selector(authCallback));
			}
#endif
		}
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}

bool js_cocos2dx_umeng_share_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("jsbindings: constructor JS object (UmnegShareNative)");
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

	JS::RootedObject proto(cx, js_cocos2dx_umeng_share_prototype);
	JS::RootedObject obj(cx, JS_NewObject(cx, js_cocos2dx_umeng_share_class, proto, JS::NullPtr()));

	JSB_UmengShareNative* delegate = new JSB_UmengShareNative();
	delegate->setJSDelegate(obj);

	js_proxy_t *p = jsb_new_proxy(delegate, obj);
	JS::AddNamedObjectRoot(cx, &p->obj, "UmengShareNative");

	args.rval().set(OBJECT_TO_JSVAL(obj));

	return true;
}


void js_cocos2dx_umeng_share_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOG("jsbindings: finalizing JS object %p (UmnegShareNative)", obj);

	JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
	JS::RootedObject jsObj(cx, obj);
	js_proxy_t *proxy = jsb_get_js_proxy(jsObj);
	JSB_UmengShareNative* cobj = (JSB_UmengShareNative *)(proxy ? proxy->ptr : NULL);
	if (cobj)
	{
		CC_SAFE_DELETE(cobj);
	}
	curLoginDelegate = nullptr;
	curShareToDelegate = nullptr;
}


void register_jsb_umeng_share_native(JSContext* cx, JS::HandleObject global)
{


	js_cocos2dx_umeng_share_class = (JSClass *)calloc(1, sizeof(JSClass));
	js_cocos2dx_umeng_share_class->name = "UmengShareNative";
	js_cocos2dx_umeng_share_class->addProperty = JS_PropertyStub;
	js_cocos2dx_umeng_share_class->delProperty = JS_DeletePropertyStub;
	js_cocos2dx_umeng_share_class->getProperty = JS_PropertyStub;
	js_cocos2dx_umeng_share_class->setProperty = JS_StrictPropertyStub;
	js_cocos2dx_umeng_share_class->enumerate = JS_EnumerateStub;
	js_cocos2dx_umeng_share_class->resolve = JS_ResolveStub;
	js_cocos2dx_umeng_share_class->convert = JS_ConvertStub;
	js_cocos2dx_umeng_share_class->finalize = js_cocos2dx_umeng_share_finalize;
	js_cocos2dx_umeng_share_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		JS_PS_END
	};
	/*static void beginLogPageView(const char *pageName);
	static void endLogPageView(const char *pageName);*/
	static JSFunctionSpec funcs[] = {
		JS_FN("login", js_cocos2dx_umeng_share_login, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("init", js_cocos2dx_umeng_share_init, 5, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("shareTo", js_cocos2dx_umeng_share_share_to, 5, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		
		JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FS_END
	};

	js_cocos2dx_umeng_share_prototype = JS_InitClass(
		cx, global,
		JS::NullPtr(),
		js_cocos2dx_umeng_share_class,
		js_cocos2dx_umeng_share_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs);
}
