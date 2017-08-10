/********************************************************************
	created:	2017/07/08
	created:	8:7:2017   9:19
	file base:	jsb_uu_water13
	file ext:	cpp
	author:		sulei
	
	purpose:	
*********************************************************************/

#include "base/ccUTF8.h"
#include "base/CCDirector.h"
#include "platform/CCPlatformMacros.h"
#include "scripting/js-bindings/manual/ScriptingCore.h"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/manual/spidermonkey_specifics.h"
#include "cocos2d.h"
#include "uu/service/RuleManager.h"
#include "uu/util/PlatformFunUtil.h"

USING_NS_CC;
JSClass  *js_cocos2dx_uu_water13_class;
JSObject *js_cocos2dx_uu_water13_prototype;

class JSB_UUWater13Native
{
public:
	JSB_UUWater13Native()
	{
		JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
		_JSDelegate.construct(cx);
	}

	~JSB_UUWater13Native()
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


///**
//* 获取游戏规则类型
//*/
//virtual RuleType getRuleType() = 0;
//
///**
//* 根据手牌获得推荐牌型
//* @param pokerVector
//*/
//virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getRecommondPokerCombination(vector<shared_ptr<PokerModel>> pokerVector) = 0;
//
///**
//* 根据扑克集合获取牌型
//* @param pokerVector 扑克集合，可以是3张的头道，也可以是5张的中尾道
//*/
//virtual shared_ptr<PokerCombinationModel> getPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector) = 0;

bool js_cocos2dx_uu_water13_getRuleType(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("jsbindings: UUWater13Native getRuleType");
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	RuleManagerAbstract* cobj = (RuleManagerAbstract *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");

	argv.rval().set(INT_TO_JSVAL(cobj->getRuleType()));
	return true;
}

/**
 * 将扑克集合转换成js的扑克数组
 * @param curPokerVector
 * @param outPokerVectorObjJS 输出的js数组
 */
void pokerVectorToJsPokerVector(JSContext *cx, vector<shared_ptr<PokerModel>>& curPokerVector, JS::RootedObject& outPokerVectorObjJS)
{
	
	//JS::RootedObject* curPokerVectorObjJS = new JS::RootedObject(cx, JS_NewArrayObject(cx, curPokerVector.size()));

	for (int32_t k = 0, sizeK = curPokerVector.size(); k < sizeK; ++k)
	{
		shared_ptr<PokerModel> curPokerModel = curPokerVector.at(k);

		JS::RootedObject pokerObjJS(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));

		
		JS::RootedValue valueJS(cx, INT_TO_JSVAL(curPokerModel->getPokerValueType()));
		JS::RootedValue markJS(cx, INT_TO_JSVAL(curPokerModel->getPokerMarkType()));

		JS_SetProperty(cx, pokerObjJS, "pokerValueType", valueJS);
		JS_SetProperty(cx, pokerObjJS, "pokerMarkType", markJS);

		JS_SetElement(cx, outPokerVectorObjJS, k, pokerObjJS);
	}
}

bool js_cocos2dx_uu_water13_getRecommendPokerCombination(JSContext *cx, uint32_t argc, jsval *vp)
{

	CCLOG("jsbindings: UUWater13Native getRecommendPokerCombination");
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	js_proxy_t *proxy = jsb_get_js_proxy(obj);
	RuleManagerAbstract* cobj = (RuleManagerAbstract *)(proxy ? proxy->ptr : NULL);
	JSB_PRECONDITION2(cobj, cx, false, "Invalid Native Object");


	vector<shared_ptr<PokerModel>> pokerVector;

	if (argc == 1)
	{

		JS::RootedObject jsobj(cx);
		bool ok = argv.get(0).isObject() && JS_ValueToObject(cx, argv.get(0), &jsobj);
		JSB_PRECONDITION3(ok, cx, false, "Error converting value to object");
		JSB_PRECONDITION3(jsobj && JS_IsArrayObject(cx, jsobj), cx, false, "argument must be an array");

		
		
		vector<shared_ptr<PokerModel>> pokerVector;

		vector<shared_ptr<PokerModel>> allPokerVector = cobj->getAllPokerVector();


		std::map<int32_t, JS::RootedValue*> oriPokerJSObjMap;

		uint32_t length = 0;
		ok &= JS_GetArrayLength(cx, jsobj, &length);
		for (uint32_t i = 0; i < length; ++i)
		{
//			JS::RootedValue value(cx);
			JS::RootedValue* value = new JS::RootedValue(cx);
			if (JS_GetElement(cx, jsobj, i, value))
			{
				JS::RootedValue pokerValueTypeJS(cx);
				JS::RootedValue pokerMarkTypeJS(cx);

				if (!value->isObject()) {

					JS_ReportError(cx, "data type must be an object");
					return false;
				}
				
				JS::RootedObject tmpObj(cx, value->toObjectOrNull());

				JS_GetProperty(cx, tmpObj, "pokerValueType", &pokerValueTypeJS);
				JS_GetProperty(cx, tmpObj, "pokerMarkType", &pokerMarkTypeJS);

				int32_t pokerValueType = 0;
				int32_t pokerMarkType = 0;
				ok = JS::ToInt32(cx, pokerValueTypeJS, &pokerValueType);
				JSB_PRECONDITION(ok, "Error converting value to pokerValueType failed");

				ok = JS::ToInt32(cx, pokerMarkTypeJS, &pokerMarkType);
				JSB_PRECONDITION(ok, "Error converting value to pokerMarkType failed");

				int32_t key = pokerValueType * 1000 + pokerMarkType;
				oriPokerJSObjMap[key] = value;

				shared_ptr<PokerModel> pokerModel = PokerModel::findPoker(allPokerVector, (PokerValueType)pokerValueType, (PokerMarkType)pokerMarkType);

				if (!pokerModel)
				{
					JS_ReportError(cx, "poker %d-%d not found", pokerValueType, pokerMarkType);
					return false;
				}

				pokerVector.push_back(pokerModel); // 添加当前扑克
			}
			else
			{
				JS_ReportError(cx, "get data %d error", i);
				return false;
			}
		}

		{

			// 计算推荐牌型
			vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> result = cobj->getRecommendPokerCombination(pokerVector);


			JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
			auto obj = JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr());
			JS::RootedObject jsobj(cx, obj);
			jsval args = OBJECT_TO_JSVAL(jsobj);


			JS::RootedObject resultObjJS(cx, JS_NewArrayObject(cx, result.size()));

			// 转换数据类型
			for (int32_t i = 0, sizeI = result.size(); i < sizeI; ++i)
			{
				shared_ptr<vector<shared_ptr<PokerCombinationModel>>> curPCMVector = result.at(i);

				JS::RootedObject curResultObjJS(cx, JS_NewArrayObject(cx, curPCMVector->size()));

				for (int32_t j = 0, sizeJ = curPCMVector->size(); j < sizeJ; ++j)
				{
					shared_ptr<PokerCombinationModel> curPCM = curPCMVector->at(j);
					
					JS::RootedObject resultPCMObjJS(cx, JS_NewObject(cx, NULL, JS::NullPtr(), JS::NullPtr()));
					
					JS::RootedValue valueJS(cx, INT_TO_JSVAL(curPCM->getValue()));
					JS_SetProperty(cx, resultPCMObjJS, "value", valueJS);

					JS::RootedValue pokerCombinationTypeJS(cx, INT_TO_JSVAL(curPCM->getPokerCombinationType()));
					JS_SetProperty(cx, resultPCMObjJS, "pokerCombinationType", pokerCombinationTypeJS);

					vector<shared_ptr<PokerModel>>& curPokerVector =  curPCM->getPokerModelVector();
					JS::RootedObject curPokerVectorObjJS(cx, JS_NewArrayObject(cx, curPokerVector.size()));
					

					for (int32_t k = 0, sizeK = curPokerVector.size(); k < sizeK; ++k)
					{
						shared_ptr<PokerModel> curPoker = curPokerVector.at(k);

						int32_t key = curPoker->getPokerValueType() * 1000 + curPoker->getPokerMarkType();
						

						JS_SetElement(cx, curPokerVectorObjJS, k, *oriPokerJSObjMap[key]);
					}

					//pokerVectorToJsPokerVector(cx, curPokerVector, curPokerVectorObjJS); // 将扑克集合转换到js数组中

					JS::RootedValue curPokerVectorObjJSValue(cx, OBJECT_TO_JSVAL(curPokerVectorObjJS));
					JS_SetProperty(cx, resultPCMObjJS, "pokers", curPokerVectorObjJSValue);

					JS_SetElement(cx, curResultObjJS, j, resultPCMObjJS);
					
				}

				JS_SetElement(cx, resultObjJS, i, curResultObjJS);
			}
			

			for (auto it = oriPokerJSObjMap.begin(); it != oriPokerJSObjMap.end(); ++it)
			{
				auto obj = (it->second);
				delete obj;
			}

			argv.rval().set(OBJECT_TO_JSVAL(resultObjJS));
		}
		
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}

bool js_cocos2dx_uu_water13_getPokerCombination(JSContext *cx, uint32_t argc, jsval *vp)
{

	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	if (argc == 2)
	{
		double cash = 0;
		int32_t coin = 0;
		if (argv[0].isNumber())
		{
			cash = argv[0].toDouble();
		}
		if (argv[1].isNumber())
		{
			coin = argv[1].toInt32();
		}

	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}
	return true;
}


bool js_cocos2dx_uu_water13_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	CCLOG("jsbindings: constructor JS object (UUWater13Native)");
	JS::CallArgs args = JS::CallArgsFromVp(argc, vp);

	JS::RootedObject proto(cx, js_cocos2dx_uu_water13_prototype);
	JS::RootedObject obj(cx, JS_NewObject(cx, js_cocos2dx_uu_water13_class, proto, JS::NullPtr()));
	args.rval().set(OBJECT_TO_JSVAL(obj));

	return true;

	/*if (argc == 1)
	{
		RuleManagerAbstract* cobj = nullptr;
		if (args[0].isInt32())
		{
			int32_t ruleType = args[0].toInt32();

			if (ruleType == 0)
			{
				cobj = new SimpleRuleManager();


				JS::RootedObject proto(cx, js_cocos2dx_uu_water13_prototype);
				JS::RootedObject obj(cx, JS_NewObject(cx, js_cocos2dx_uu_water13_class, proto, JS::NullPtr()));


				JSB_UUWater13Native* delegate = new JSB_UUWater13Native();
				delegate->setJSDelegate(obj);
				cobj->setRefPtr(delegate);
				js_proxy_t *p = jsb_new_proxy(cobj, obj);
				JS::AddNamedObjectRoot(cx, &p->obj, "UUWater13Native");

				args.rval().set(OBJECT_TO_JSVAL(obj));
				return true;
			}
			else
			{
				JS_ReportError(cx, "RuleManager dont support ruleType:%d", ruleType);
				return false;
			}
		}
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}*/
}



bool js_cocos2dx_uu_water13_rateUs(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	PlatformFunUtil::rateUs();
	return true;
}

bool js_cocos2dx_uu_water13_getAppChannel(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	string channel = PlatformFunUtil::getAppChannel();

	argv.rval().set(c_string_to_jsval(cx, channel.c_str()));

	return true;
}

bool js_cocos2dx_uu_water13_getNetType(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	int32_t value = PlatformFunUtil::getNetType();

	argv.rval().set(INT_TO_JSVAL(value));

	return true;
}

bool js_cocos2dx_uu_water13_getOSVersion(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	string value = PlatformFunUtil::getOSVersion();

	argv.rval().set(c_string_to_jsval(cx, value.c_str()));

	return true;
}

bool js_cocos2dx_uu_water13_getNetTypeString(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	string value = PlatformFunUtil::getNetTypeString();

	argv.rval().set(c_string_to_jsval(cx, value.c_str()));

	return true;
}

bool js_cocos2dx_uu_water13_getOSType(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	string value = PlatformFunUtil::getOSType();

	argv.rval().set(c_string_to_jsval(cx, value.c_str()));

	return true;
}

bool js_cocos2dx_uu_water13_getDeviceModel(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	string value = PlatformFunUtil::getDeviceModel();

	argv.rval().set(c_string_to_jsval(cx, value.c_str()));

	return true;
}

bool js_cocos2dx_uu_water13_getBatteryPercent(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	int32_t value = PlatformFunUtil::getBatteryPercent();

	argv.rval().set(INT_TO_JSVAL(value));

	return true;
}

bool js_cocos2dx_uu_water13_restartApp(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	PlatformFunUtil::restartApp(0);
	return true;
}

bool js_cocos2dx_uu_water13_joinQQGroup(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	if (argc == 2)
	{
		string qqGroupNum;
		string qqKey = 0;
		if (argv[0].isString())
		{
			jsval_to_std_string(cx, argv[0], &qqGroupNum);
		}
		if (argv[1].isString())
		{
			jsval_to_std_string(cx, argv[0], &qqKey);
		}

		bool isSuccess = PlatformFunUtil::joinQQGroup(qqGroupNum, qqKey);

		argv.rval().set(BOOLEAN_TO_JSVAL(isSuccess));
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 2);
		argv.rval().set(BOOLEAN_TO_JSVAL(false));
		return false;
	}

	return true;
}

bool js_cocos2dx_uu_water13_isOpenWithOther(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());
	bool value  =PlatformFunUtil::isOpenWithOther();
	argv.rval().set(BOOLEAN_TO_JSVAL(value));
	return true;
}

bool js_cocos2dx_uu_water13_setOpenWithOther(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());


	if (argc == 1)
	{
		
		if (argv[0].isBoolean())
		{
			bool value = argv[0].toBoolean();


			PlatformFunUtil::setOpenWithOther(value);
			
		}
		else
		{
			JS_ReportError(cx, "wrong params");
		}
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}

	return true;
}

bool js_cocos2dx_uu_water13_getExternalParam(JSContext *cx, uint32_t argc, jsval *vp)
{
	JS::CallArgs argv = JS::CallArgsFromVp(argc, vp);
	JS::RootedObject obj(cx, argv.thisv().toObjectOrNull());

	if (argc == 1)
	{

		string key;
		if (argv[0].isString())
		{
			jsval_to_std_string(cx, argv[0], &key);

			string value = PlatformFunUtil::getExternalParam(key);
			argv.rval().set(std_string_to_jsval(cx, value));

		}
		else
		{
			JS_ReportError(cx, "wrong params");
		}
	}
	else
	{
		JS_ReportError(cx, "wrong number of arguments: %d, was expecting %d", argc, 1);
		return false;
	}

	
	return true;
}

void js_cocos2dx_uu_water13_finalize(JSFreeOp *fop, JSObject *obj) {
	CCLOG("jsbindings: finalizing JS object %p (UUWater13Native)", obj);

	JSContext* cx = ScriptingCore::getInstance()->getGlobalContext();
	JS::RootedObject jsObj(cx, obj);
	js_proxy_t *proxy = jsb_get_js_proxy(jsObj);
}

void register_jsb_uu_water13(JSContext* cx, JS::HandleObject global)
{
	js_cocos2dx_uu_water13_class = (JSClass *)calloc(1, sizeof(JSClass));
	js_cocos2dx_uu_water13_class->name = "UUWater13Native";
	js_cocos2dx_uu_water13_class->addProperty = JS_PropertyStub;
	js_cocos2dx_uu_water13_class->delProperty = JS_DeletePropertyStub;
	js_cocos2dx_uu_water13_class->getProperty = JS_PropertyStub;
	js_cocos2dx_uu_water13_class->setProperty = JS_StrictPropertyStub;
	js_cocos2dx_uu_water13_class->enumerate = JS_EnumerateStub;
	js_cocos2dx_uu_water13_class->resolve = JS_ResolveStub;
	js_cocos2dx_uu_water13_class->convert = JS_ConvertStub;
	js_cocos2dx_uu_water13_class->finalize = js_cocos2dx_uu_water13_finalize;
	js_cocos2dx_uu_water13_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
		JS_PS_END
	};
	/*static void beginLogPageView(const char *pageName);
	static void endLogPageView(const char *pageName);*/
	static JSFunctionSpec funcs[] = {
		JS_FN("getRuleType", js_cocos2dx_uu_water13_getRuleType, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getPokerCombination", js_cocos2dx_uu_water13_getPokerCombination, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getRecommendPokerCombination", js_cocos2dx_uu_water13_getRecommendPokerCombination, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),

		JS_FN("rateUs", js_cocos2dx_uu_water13_rateUs, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getAppChannel", js_cocos2dx_uu_water13_getAppChannel, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getNetType", js_cocos2dx_uu_water13_getNetType, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getOSVersion", js_cocos2dx_uu_water13_getOSVersion, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getNetTypeString", js_cocos2dx_uu_water13_getNetTypeString, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getOSType", js_cocos2dx_uu_water13_getOSType, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getDeviceModel", js_cocos2dx_uu_water13_getDeviceModel, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getBatteryPercent", js_cocos2dx_uu_water13_getBatteryPercent, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("restartApp", js_cocos2dx_uu_water13_restartApp, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("joinQQGroup", js_cocos2dx_uu_water13_joinQQGroup, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("isOpenWithOther", js_cocos2dx_uu_water13_isOpenWithOther, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("setOpenWithOther", js_cocos2dx_uu_water13_setOpenWithOther, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
		JS_FN("getExternalParam", js_cocos2dx_uu_water13_getExternalParam, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),

		
		JS_FS_END
	};

	static JSFunctionSpec st_funcs[] = {
		JS_FS_END
	};

	js_cocos2dx_uu_water13_prototype = JS_InitClass(
		cx, global,
		JS::NullPtr(),
		js_cocos2dx_uu_water13_class,
		js_cocos2dx_uu_water13_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs);
}

