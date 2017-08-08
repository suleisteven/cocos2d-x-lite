package org.cocos2dx.cpp;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.net.Uri;
import android.os.Build;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.widget.Toast;

import org.cocos2dx.cpp.update.VersionUpdateManager;
import org.cocos2dx.cpp.update.VersionUpdater.UpdateInfo;

import java.security.MessageDigest;
import java.util.HashMap;

/**
 * 与native代码交互类
 * 
 * @author Steven
 */
public class NativeInteractive {

	private static Activity mContext;

	private static int mBatteryPercent = 0;
	private static BroadcastReceiver mBatteryLevelReceiver;

	public static void init(Activity context) {
		mContext = context;
		mBatteryLevelReceiver = new BroadcastReceiver() {
			public void onReceive(Context context, Intent intent) {
				int rawlevel = intent.getIntExtra("level", -1);// 获得当前电量
				int scale = intent.getIntExtra("scale", -1);// 获得总电量

				int level = -1;
				if (rawlevel >= 0 && scale > 0) {
					level = (rawlevel * 100) / scale;
				}
				mBatteryPercent = level;
			}
		};
		IntentFilter batteryLevelFilter = new IntentFilter(
				Intent.ACTION_BATTERY_CHANGED);
		mContext.registerReceiver(mBatteryLevelReceiver, batteryLevelFilter);
	}

	public static void uninit() {
		if (null != mContext && null != mBatteryLevelReceiver) {
			mContext.unregisterReceiver(mBatteryLevelReceiver);
		}
	}

	public static Context getContext() {
		return mContext;
	}

	UpdateInfo updateInfo = new UpdateInfo();

	// updateInfo.updateType = 2;
	// updateInfo.newestVersionCode = 300;
	// updateInfo.newestVersionName = "default";
	// updateInfo.updateMD5 = "51a362940f2a04865fadd46c213dc426";
	// updateInfo.updateInstruction = "阿杜刀功不错";
	// updateInfo.updateUrl =
	// "http://120.25.153.194/down/Landlord_20160706.apk";
	// updateInfo.installTtype = 1;
	// updateInfo.updateSize = 12842621;
	// /**
	// * 升级类型，0 不升级, 1 建议升级, 2 强制升级
	// */
	// public int updateType;
	// /**
	// * 安装类型，0-全量更新、1-部分更新
	// */
	// public int installTtype;
	// /**
	// * 最新版本号
	// */
	// public int newestVersionCode;
	// /**
	// * 最新版本名称
	// */
	// public String newestVersionName;
	// /**
	// * 渠道名称
	// */
	// public String channelName = "";
	// /**
	// * 升级地址
	// */
	// public String updateUrl = "";
	// /**
	// * 升级包大小，单位：字节
	// */
	// public long updateSize;
	// /**
	// * 升级包md5值
	// */
	// public String updateMD5 = "";
	// /**
	// * 升级简介
	// */
	// public String updateInstruction = "";
	/**
	 * 开始更新app
	 * 
	 * @param updateType
	 *            升级类型，0 不升级, 1 建议升级, 2 强制升级
	 * @param newestVersionCode
	 *            最新版本号
	 * @param newestVersionName
	 *            最新版本名称
	 * @param channelName
	 *            渠道名称
	 * @param updateMD5
	 *            升级包md5值
	 * @param updateInstruction
	 *            升级简介
	 * @param updateUrl
	 *            升级地址
	 * @param installTtype
	 *            安装类型，0-全量更新、1-部分更新
	 * @param updateSize
	 *            升级包大小，单位：字节
	 */
	public static void updateApp(int updateType, int newestVersionCode,
			String newestVersionName, String channelName, String updateMD5,
			String updateInstruction, String updateUrl, int installTtype,
			long updateSize) {
		UpdateInfo updateInfo = new UpdateInfo();
		updateInfo.updateType = updateType;
		updateInfo.newestVersionCode = newestVersionCode;
		updateInfo.newestVersionName = newestVersionName;
		updateInfo.channelName = channelName;
		updateInfo.updateMD5 = updateMD5;
		updateInfo.updateInstruction = updateInstruction;
		updateInfo.updateUrl = updateUrl;
		updateInfo.installTtype = installTtype;
		updateInfo.updateSize = updateSize;

		if (null == sVersionUpdateManager) {
			sVersionUpdateManager = new VersionUpdateManager(mContext,
					updateInfo);
		} else {
			sVersionUpdateManager.setUpdateInfo(updateInfo);
		}
		sVersionUpdateManager.defaultUpdateApp();
	}

	static VersionUpdateManager sVersionUpdateManager;

	/**
	 * 给我们评分功能
	 */
	public static void rateUs() {
		mContext.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Util.rateUs(mContext);
			}
		});
	}

	/**
	 * 获取电量信息
	 * 
	 * @return 0-100的值
	 */
	public static int getBatteryPercent() {
		return mBatteryPercent;
	}

	/**
	 * 获取程序版本号
	 * 
	 * @return
	 */
	public static int getAppVersionCode() {
		return Util.getVersionCode(mContext);
	}

	/**
	 * 获取程序版本名称
	 * 
	 * @return
	 */
	public static String getAppVersionName() {
		return Util.getVersionName(mContext);
	}

	/**
	 * 获取程序打包渠道
	 * 
	 * @return
	 */
	public static String getAppChannel() {
		return Util.getChannelKey(mContext);
	}

	/**
	 * 显示toast
	 * 
	 * @param content
	 *            内容
	 * @param isShowLongTime
	 *            是否长时间显示，true-大概显示4秒，false-大概显示2秒
	 */
	public static void showToast(final String content,
			final boolean isShowLongTime) {
		if (TextUtils.isEmpty(content)) {
			return;
		}

		mContext.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				CustomToast.showToast(mContext.getApplicationContext(),
						content, isShowLongTime ? Toast.LENGTH_LONG
								: Toast.LENGTH_SHORT);
			}
		});
	}

	private static String deviceId;

	/**
	 * 获取设备唯一id
	 * 
	 * @return
	 */
	public static String getDeviceId() {
		if (TextUtils.isEmpty(deviceId)) {
			final TelephonyManager tm = (TelephonyManager) mContext
					.getSystemService(Service.TELEPHONY_SERVICE);

			final String tmDevice, androidId;

			tmDevice = tm.getDeviceId();

			androidId = android.provider.Settings.Secure.getString(
					mContext.getContentResolver(),
					android.provider.Settings.Secure.ANDROID_ID);

			String m_szLongID = tmDevice + androidId;

			// compute md5
			deviceId = digest(m_szLongID);
		}
		return deviceId;
	}

	/**
	 * 获取网络类型
	 * 
	 * @return 1-wifi；2-2g；3-3g；4-4g；99-未连接
	 */
	public static int getNetType() {
		int result = 99;

		int apnType = Util.getAPNType(mContext);
		switch (apnType) {
		case Util.NO_NETWORK:
			break;
		case Util.WIFI: {
			result = 1;
		}
			break;
		case Util.CMWAP:
		case Util.CMNET: {
			result = Util.getMobileSubType(mContext);
		}
			break;
		default:
			break;
		}
		return result;
	}

	/**
	 * 获取系统版本号
	 * 
	 * @return
	 */
	public static String getOSVersion() {
		return Build.VERSION.RELEASE;
	}

	/**
	 * 获取设备型号
	 * 
	 * @return
	 */
	public static String getDeviceModel() {
		return Build.MODEL;
	}

	/**
	 * 重启app
	 * 
	 * @param delayTime
	 *            延迟时间，单位：毫秒
	 */
	public static void restartApp(long delayTime) {
		Util.restartApp(mContext, delayTime);
	}

	/**
	 * 拷贝内容到剪贴板
	 * @param content
     */
	public static void copyStringToClipboard(final String content)
	{
		if(null != mContext && null != content)
		{

			mContext.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					ClipboardManager myClipboard = (ClipboardManager)mContext.getSystemService(Context.CLIPBOARD_SERVICE);

					ClipData myClip;
					if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
						myClip = ClipData.newPlainText("text", content);
						myClipboard.setPrimaryClip(myClip);
					}
				}
			});
		}
	}

	private static boolean isGetFromClipboardOver = false;
	private static String stringGetFromClipboard = "";
	/**
	 * 从剪贴板获取内容
	 */
	public static String getStringFromClipboard()
	{
		String result = "";

		isGetFromClipboardOver = false;
		stringGetFromClipboard = "";
		if(null != mContext)
		{
			mContext.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					ClipboardManager clipboardManager = (ClipboardManager)mContext.getSystemService(Context.CLIPBOARD_SERVICE);
					if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
						if (clipboardManager.hasPrimaryClip()){
							try
							{
								stringGetFromClipboard = clipboardManager.getPrimaryClip().getItemAt(0).getText().toString();  // 获取内容
							}
							catch (Exception e)
							{
								e.printStackTrace();
							}
						}
					}
					isGetFromClipboardOver = true;
				}
			});
		}else
		{
			isGetFromClipboardOver = true;
		}

		int maxTime = 30000;
		int curTime = 0;
		int step =10;
		while(!isGetFromClipboardOver)
		{
			try {
				Thread.sleep(step);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			curTime += step;

			if(curTime>=maxTime)
			{
				break;
			}
		}
		result = stringGetFromClipboard;
		if(null == result)
		{
			result = "";
		}
		return result;
	}


	/**
	 * @TODO 还未实现 获取将要被加载的动态库
	 * @return
	 */
	public static native String[] getToBeLoadLibNames();

	/**
	 * 获取签名信息
	 * 
	 * @return
	 */
	@SuppressLint("DefaultLocale")
	public static String getSignInfo() {
		String result = "";
		try {
			PackageInfo packageInfo = mContext.getPackageManager()
					.getPackageInfo(mContext.getPackageName(),
							PackageManager.GET_SIGNATURES);
			Signature[] signatures = packageInfo.signatures;
			Signature signature = signatures[0];
			// CertificateFactory certFactory =
			// CertificateFactory.getInstance("X.509");
			// X509Certificate cert = (X509Certificate)
			// certFactory.generateCertificate(new
			// ByteArrayInputStream(signature.toByteArray()));
			// byte[] buffer =cert.getEncoded();
			// result = signature.toCharsString();
			byte[] arrayOfByte = signature.toByteArray();
			result = digest(arrayOfByte);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

	private static final char hexDigits[] = { '0', '1', '2', '3', '4', '5',
			'6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	public static final String digest(String message) {
		try {
			byte[] strTemp = message.getBytes();
			MessageDigest mdTemp = MessageDigest.getInstance("MD5");
			mdTemp.update(strTemp);
			byte[] md = mdTemp.digest();
			int j = md.length;
			char str[] = new char[j * 2];
			int k = 0;
			for (int i = 0; i < j; i++) {
				byte byte0 = md[i];
				str[k++] = hexDigits[byte0 >>> 4 & 0xf];
				str[k++] = hexDigits[byte0 & 0xf];
			}
			return new String(str);
		} catch (Exception e) {
			return null;
		}
	}

	public static final String digest(byte[] strTemp) {
		try {
			// byte[] strTemp = message.getBytes();
			MessageDigest mdTemp = MessageDigest.getInstance("MD5");
			mdTemp.update(strTemp);
			byte[] md = mdTemp.digest();
			int j = md.length;
			char str[] = new char[j * 2];
			int k = 0;
			for (int i = 0; i < j; i++) {
				byte byte0 = md[i];
				str[k++] = hexDigits[byte0 >>> 4 & 0xf];
				str[k++] = hexDigits[byte0 & 0xf];
			}
			return new String(str);
		} catch (Exception e) {
			return null;
		}
	}

	static boolean isOpenWithOther = false;

	/**
	 * 是否是从其他应用打开
	 * @return
     */
	public static final boolean isOpenWithOther()
	{
		return isOpenWithOther;
	}

	public static void setOpenWithOther(boolean value)
	{
		if(!value)
		{
			if(!isLastExternalParamsConsume) // 最近的一次外部参数还未消耗，不能设置为false
			{

			}else
			{
				isOpenWithOther = value;
			}
		}
		else
		{
			isOpenWithOther = value;
		}

	}

	static HashMap<String, String> externalParams;
	static boolean isLastExternalParamsConsume = false; // 最近的一次外部数据是否已经消耗
	/**
	 * 获取外部传入参数
	 * @return
     */
	public static HashMap<String, String> getExternalParams()
	{
		return externalParams;
	}

	public static void setExternalParams(HashMap<String,String> params)
	{
		if(null != params)
		{
			isLastExternalParamsConsume = false;
		}
		else
		{
			isLastExternalParamsConsume = true;
		}

		externalParams = params;
	}

	public static String getExternalParam(String key)
	{
		isLastExternalParamsConsume = true;
		if(null != externalParams)
		{
			return externalParams.get(key);
		}
		return "";
	}


	/****************
	 *
	 * 发起添加群流程。群号：四人斗地主找朋友官方(209991901) 的 key 为： ww_QHnfusLX5Ei7R4lg2r0WS6Qq-L25e
	 * 调用 joinQQGroup(ww_QHnfusLX5Ei7R4lg2r0WS6Qq-L25e) 即可发起手Q客户端申请加群 四人斗地主找朋友官方(209991901)
	 *
	 * @param key 由官网生成的key
	 * @return 返回true表示呼起手Q成功，返回fals表示呼起失败
	 ******************/
	public static boolean joinQQGroup(String key) {
		Intent intent = new Intent();
		intent.setData(Uri.parse("mqqopensdkapi://bizAgent/qm/qr?url=http%3A%2F%2Fqm.qq.com%2Fcgi-bin%2Fqm%2Fqr%3Ffrom%3Dapp%26p%3Dandroid%26k%3D" + key));
		// 此Flag可根据具体产品需要自定义，如设置，则在加群界面按返回，返回手Q主界面，不设置，按返回会返回到呼起产品界面    //intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
		try {
			mContext.startActivity(intent);
			return true;
		} catch (Exception e) {
			// 未安装手Q或安装的版本不支持
			return false;
		}
	}



	public static final void showBanner(int bannerPosition) {

		try {
			Class adUtilCls = Class.forName("com.tomatocloud.adproj.AdUtil");
			Object adUtilInstance = Reflection.invokeStaticMethod(adUtilCls, "getInstance", new Class[]{}, new Object[]{});
			Reflection.invokeMethod(adUtilInstance, "showBanner", new Class[]{int.class}, new Object[]{bannerPosition});
		} catch (Exception e) {
			e.printStackTrace();
		}

//		AdPosition adPosition = AdUtil.AdPosition.getObject(bannerPosition);
//		if (null == adPosition) {
//			adPosition = AdPosition.POSITION_BOTTOM_CENTER;
//		}
//		AdUtil.getInstance().showBanner(adPosition);
	}

	public static final void hideBanner() {

		try {
			Class adUtilCls = Class.forName("com.tomatocloud.adproj.AdUtil");
			Object adUtilInstance = Reflection.invokeStaticMethod(adUtilCls, "getInstance", new Class[]{}, new Object[]{});
			Reflection.invokeMethod(adUtilInstance, "hideBanner", new Class[]{}, new Object[]{});
		} catch (Exception e) {
			e.printStackTrace();
		}

//		AdUtil.getInstance().hideBanner();
	}

	public static final void showInterstitial() {
		try {
			Class adUtilCls = Class.forName("com.tomatocloud.adproj.AdUtil");
			Object adUtilInstance = Reflection.invokeStaticMethod(adUtilCls, "getInstance", new Class[]{}, new Object[]{});
			Reflection.invokeMethod(adUtilInstance, "showInterstitial", new Class[]{}, new Object[]{});
		} catch (Exception e) {
			e.printStackTrace();
		}
//		AdUtil.getInstance().showInterstitial();
	}

	public static final void closeInterstitial() {
		try {
			Class adUtilCls = Class.forName("com.tomatocloud.adproj.AdUtil");
			Object adUtilInstance = Reflection.invokeStaticMethod(adUtilCls, "getInstance", new Class[]{}, new Object[]{});
			Reflection.invokeMethod(adUtilInstance, "closeInterstitial", new Class[]{}, new Object[]{});
		} catch (Exception e) {
			e.printStackTrace();
		}
//		AdUtil.getInstance().closeInterstitial();
	}


	/**
	 * 通知底层收到一条透传消息
	 * @param msg
     */
	public static native void onReceiveMsg(String msg);

	/**
	 * 处理离线消息，底层进入主页时才会调用此方法，将未处理的离线消息
	 */
	public static void handleOfflineMsg()
	{
		// 处理离线消息
		ApplicationExp.handleOfflineMsg();
	}


	/**
	 * 支付
	 * @param goodsName 商品名称
	 * @param desc 商品描述
	 * @param price 商品价格
     * @param payType 支付类型 1-微信，2-支付宝
     */
	public static void pay(String goodsName, String desc, float price, int payType)
	{
		if(payType == 1)
		{
			PayManager.getInstance().pay(goodsName, desc, price, false);
		}
		else if(payType == 2)
		{
			PayManager.getInstance().pay(goodsName, desc, price, true);
		}
	}

	/**
	 * 支付回调
	 * @param resultCode 支付结果码
	 * @param orderId 订单号
	 * @param desc 支付结果描述
     */
	public static native void onPayResult(int resultCode, String orderId, String desc);

	/**
	 * 查询支付订单号
	 * @param orderId 订单号
     */
	public static void queryPayOrderId(String orderId)
	{
		PayManager.getInstance().queryOrder(orderId);
	}

	/**
	 * 查询订单回调
	 * @param resultCode  结果码
	 * @param orderId 订单号
     */
	public static native void onQueryPayOrderIdResult(int resultCode, String orderId);
}
