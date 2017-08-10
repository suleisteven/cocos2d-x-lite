package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileInputStream;
import java.security.MessageDigest;
import java.util.Locale;

import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Build;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.widget.Toast;

import com.game1752.water13.R;

public class Util {
	public static void rateUs(Context context) {

		// StringBuilder localStringBuilder = new StringBuilder()
		// .append("https://market.android.com/details?id=");
		// 市场分享链接
		// https://market.android.com/details?id=[packagename]
		// 亚马逊分享链接
		// http://www.amazon.com/gp/mas/dl/android?p=[packagename]

		// 打个好评
		try {
			try {
				StringBuilder localStringBuilder = new StringBuilder()
						.append("market://details?id=");


				String packageName = context.getPackageName();
				localStringBuilder.append(packageName);
				Uri marketUri = Uri.parse(localStringBuilder.toString());


				Intent marketIntent = new Intent(Intent.ACTION_VIEW, marketUri);
				marketIntent.setPackage("com.android.vending");
				context.startActivity(marketIntent);
			} catch (Exception e) {

// 使用百度应用市场
				StringBuilder localStringBuilder = new StringBuilder()
						.append("market://details?id=");

				String packageName = context.getPackageName();
				localStringBuilder.append(packageName);
				Uri marketUri = Uri.parse(localStringBuilder.toString());


				Intent marketIntent = new Intent(Intent.ACTION_VIEW, marketUri);
				marketIntent.setPackage("com.baidu.appsearch");
				context.startActivity(marketIntent);


				// 没有安装应用市场,使用浏览器
//				StringBuilder localStringBuilder = new StringBuilder()
//						.append("https://market.android.com/details?id=");
//
//				String packageName = context.getPackageName();
//				localStringBuilder.append(packageName);
//				Uri marketUri = Uri.parse(localStringBuilder.toString());
//				Intent marketIntent = new Intent(Intent.ACTION_VIEW, marketUri);
//				context.startActivity(marketIntent);
			}

		} catch (Exception e) {
			e.printStackTrace();

			Toast.makeText(context, "没有市场", Toast.LENGTH_SHORT)
					.show();

//			// 应用市场和浏览器都没有，给出提示
//			Toast.makeText(context, R.string.rate_us_no_app, Toast.LENGTH_SHORT)
//					.show();
		}
	}

	private static int versionCode = 0;

	/**
	 * 获取当前版本号
	 * 
	 * @param context
	 * @return
	 */
	public static int getVersionCode(Context context) {
		if (versionCode != 0) {
			return versionCode;
		}
		PackageManager packageManager = context.getPackageManager();
		PackageInfo packageInfo = null;

		try {
			packageInfo = packageManager.getPackageInfo(
					context.getPackageName(), 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		versionCode = packageInfo.versionCode;
		return versionCode;
	}

	private static String versionName = "";

	/**
	 * 获取当前版本名称
	 * 
	 * @param context
	 * @return
	 */
	public static String getVersionName(Context context) {
		if (!TextUtils.isEmpty(versionName)) {
			return versionName;
		}
		PackageManager packageManager = context.getPackageManager();
		PackageInfo packageInfo = null;

		try {
			packageInfo = packageManager.getPackageInfo(
					context.getPackageName(), 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
		versionName = packageInfo.versionName;
		return versionName;
	}

	private static String channelKey = "";

	public static String getChannelKey(Context context) {

		if (!TextUtils.isEmpty(channelKey)) {
			return channelKey;
		}
		try {
			PackageManager packageManager = context.getPackageManager();

			ApplicationInfo applicationInfo;
			applicationInfo = packageManager.getApplicationInfo(
					context.getPackageName(), PackageManager.GET_META_DATA);
			if (applicationInfo != null && applicationInfo.metaData != null) {
				channelKey = applicationInfo.metaData
						.getString("UMENG_CHANNEL");
			}
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return channelKey;
	}

	/**
	 * 获取系统当前语言类型
	 * 
	 * @param
	 * @return
	 */
	public static Language getSystemLanguage() {

		// 检测语言环境
		Locale locale = Locale.getDefault();

		Language resultLanguage = Language.getLanguageByLocal(locale);
		if (null == resultLanguage) {
			return Language.EN;
		}
		return resultLanguage;
	}

	/**
	 * 根据值获取语言对象
	 * 
	 * @param locale
	 * @return 默认返回为英文
	 */
	public static Language getLanguageByLocal(Locale locale) {
		if (locale == null) {
			return Language.EN;
		}

		for (Language language : Language.values()) {
			Locale tempLocal = language.locale;

			if (tempLocal.equals(locale)) {
				return language;
			}
		}

		return getLanguageByValue(locale);
	}

	/**
	 * 通过Local 中的language获取语言类型
	 * 
	 * @param locale
	 * @return
	 */
	private static Language getLanguageByValue(Locale locale) {
		for (Language language : Language.values()) {
			if (language.getLocale().getLanguage().equals(locale.getLanguage())) {
				return language;
			}
		}

		return Language.EN;
	}

	/**
	 * 语言类型
	 * 
	 * @author steven
	 */
	public enum Language {
		/**
		 * 中文
		 */
		ZH(Locale.SIMPLIFIED_CHINESE),
		/**
		 * 英文
		 */
		EN(Locale.ENGLISH),
		/**
		 * 台湾繁体
		 */
		TW(Locale.TRADITIONAL_CHINESE);

		private Locale locale;

		private Language(Locale locale) {
			this.locale = locale;
		}

		public Locale getLocale() {
			return locale;
		}

		public void setLocale(Locale locale) {
			this.locale = locale;
		}

		/**
		 * 根据值获取语言对象
		 * 
		 * @param locale
		 * @return 默认返回为英文
		 */
		public static Language getLanguageByLocal(Locale locale) {
			if (locale == null) {
				return EN;
			}

			for (Language language : Language.values()) {
				Locale tempLocal = language.locale;

				if (tempLocal.equals(locale)) {
					return language;
				}
			}

			return getLanguageByValue(locale);
		}

		/**
		 * 通过Local 中的language获取语言类型
		 * 
		 * @param locale
		 * @return
		 */
		private static Language getLanguageByValue(Locale locale) {
			for (Language language : Language.values()) {
				if (language.getLocale().getLanguage()
						.equals(locale.getLanguage())) {
					return language;
				}
			}

			return EN;
		}
	}

	public static final int NO_NETWORK = -1;
	public static final int WIFI = 1;
	public static final int CMWAP = 2;
	public static final int CMNET = 3;

	public static int getAPNType(Context context) {
		int netType = NO_NETWORK;
		ConnectivityManager connectivity = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (connectivity == null) {
			return netType;
		}
		NetworkInfo networkInfo = connectivity.getActiveNetworkInfo();
		if (networkInfo == null) {
			return netType;
		}
		int nType = networkInfo.getType();
		if (nType == ConnectivityManager.TYPE_MOBILE) {
			if (networkInfo.getExtraInfo().toLowerCase().equals("cmnet")) {
				netType = CMNET;
			} else if (networkInfo.getExtraInfo().toLowerCase().equals("cmwap")) {
				netType = CMWAP;
			}
		} else if (nType == ConnectivityManager.TYPE_WIFI) {
			netType = WIFI;
		}
		return netType;
	}

	/**
	 * 判断MOBILE网络是否可用 及是否有信号
	 * 
	 * @Description: TODO
	 * @param
	 * @return boolean
	 * @throws
	 */
	public static boolean isMobileConnected(Context context) {
		ConnectivityManager connectivity = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (connectivity == null) {
			return false;
		}
		NetworkInfo mMobileNetworkInfo = connectivity.getActiveNetworkInfo();
		if (mMobileNetworkInfo != null && mMobileNetworkInfo.isAvailable()) {
			int type = mMobileNetworkInfo.getType();
			if (type == ConnectivityManager.TYPE_MOBILE) {
				return true;
			}
		}
		return false;
	}

	public static final int MOBILE_NET_UNKNOW = -1;
	public static final int MOBILE_NET_2G = 2;
	public static final int MOBILE_NET_3G = 3;
	public static final int MOBILE_NET_4G = 4;

	/**
	 * 获取移动网络类型
	 * 
	 * @param context
	 * @return
	 */
	public static int getMobileSubType(Context context) {
		int result = MOBILE_NET_UNKNOW;
		if (isMobileConnected(context)) {
			ConnectivityManager connectivity = (ConnectivityManager) context
					.getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo mMobileNetworkInfo = connectivity
					.getActiveNetworkInfo();

			int type = mMobileNetworkInfo.getSubtype();

			// GPRS 2G(2.5) General Packet Radia Service 114kbps
			// EDGE 2G(2.75G) Enhanced Data Rate for GSM Evolution 384kbps
			// UMTS 3G WCDMA 联通3G Universal Mobile Telecommunication System
			// 完整的3G移动通信技术标准
			// CDMA 2G 电信 Code Division Multiple Access 码分多址
			// EVDO_0 3G (EVDO 全程 CDMA2000 1xEV-DO) Evolution - Data Only (Data
			// Optimized) 153.6kps - 2.4mbps 属于3G
			// EVDO_A 3G 1.8mbps - 3.1mbps 属于3G过渡，3.5G
			// 1xRTT 2G CDMA2000 1xRTT (RTT - 无线电传输技术) 144kbps 2G的过渡,
			// HSDPA 3.5G 高速下行分组接入 3.5G WCDMA High Speed Downlink Packet Access
			// 14.4mbps
			// HSUPA 3.5G High Speed Uplink Packet Access 高速上行链路分组接入 1.4 - 5.8
			// mbps
			// HSPA 3G (分HSDPA,HSUPA) High Speed Packet Access
			// IDEN 2G Integrated Dispatch Enhanced Networks 集成数字增强型网络
			// （属于2G，来自维基百科）
			// EVDO_B 3G EV-DO Rev.B 14.7Mbps 下行 3.5G
			// LTE 4G Long Term Evolution FDD-LTE 和 TDD-LTE , 3G过渡，升级版 LTE
			// Advanced 才是4G
			// EHRPD 3G CDMA2000向LTE 4G的中间产物 Evolved High Rate Packet Data
			// HRPD的升级
			// HSPAP 3G HSPAP 比 HSDPA 快些

			switch (type) {
			case TelephonyManager.NETWORK_TYPE_GPRS:
			case TelephonyManager.NETWORK_TYPE_EDGE:
			case TelephonyManager.NETWORK_TYPE_CDMA:
			case TelephonyManager.NETWORK_TYPE_1xRTT:
			case TelephonyManager.NETWORK_TYPE_IDEN:
			case 16: // TelephonyManager.NETWORK_TYPE_GSM:
				result = MOBILE_NET_2G;
				break;

			case TelephonyManager.NETWORK_TYPE_UMTS:
			case TelephonyManager.NETWORK_TYPE_EVDO_0:
			case TelephonyManager.NETWORK_TYPE_EVDO_A:
			case TelephonyManager.NETWORK_TYPE_HSDPA:
			case TelephonyManager.NETWORK_TYPE_HSUPA:
			case TelephonyManager.NETWORK_TYPE_HSPA:
			case TelephonyManager.NETWORK_TYPE_EVDO_B:
			case TelephonyManager.NETWORK_TYPE_EHRPD:
			case TelephonyManager.NETWORK_TYPE_HSPAP:
				result = MOBILE_NET_3G;
				break;

			case TelephonyManager.NETWORK_TYPE_LTE:
				result = MOBILE_NET_4G;
				break;
			default:
				break;
			}
		}
		return result;
	}

	/**
	 * 重启app
	 * 
	 * @param context
	 * @param delayTime
	 *            延迟时间，单位：毫秒
	 */
	public static void restartApp(Context context, long delayTime) {

		if (delayTime < 0) {
			delayTime = 0;
		}
		Context ctx = context;
		Intent mStartActivity = context.getPackageManager()
				.getLaunchIntentForPackage(context.getPackageName());
		mStartActivity.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		mStartActivity.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB)
			mStartActivity.addFlags(0x8000); // 相当于Intent.FLAG_ACTIVITY_CLEAR_TASK
		int mPendingIntentId = 123456;
		PendingIntent mPendingIntent = PendingIntent.getActivity(ctx,
				mPendingIntentId, mStartActivity,
				PendingIntent.FLAG_CANCEL_CURRENT);
		AlarmManager mgr = (AlarmManager) ctx
				.getSystemService(Context.ALARM_SERVICE);
		mgr.set(AlarmManager.RTC, System.currentTimeMillis() + delayTime,
				mPendingIntent);

		if (ctx instanceof Activity) {
			((Activity) ctx).moveTaskToBack(true); // 切换到后台
		}
		android.os.Process.killProcess(android.os.Process.myPid());
		System.exit(0);
	}

	/**
	 * 获取单个文件的MD5值
	 * 
	 * @param file
	 * @return
	 */
	public static String getFileMD5(File file) {
		if (!file.isFile()) {
			return "";
		}
		MessageDigest digest = null;
		FileInputStream in = null;
		byte buffer[] = new byte[1024];
		int len;
		try {
			digest = MessageDigest.getInstance("MD5");
			in = new FileInputStream(file);
			while ((len = in.read(buffer, 0, 1024)) != -1) {
				digest.update(buffer, 0, len);
			}
			in.close();
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
		byte[] rs = digest.digest();
		StringBuffer digestHexStr = new StringBuffer();
		for (int i = 0; i < rs.length; i++) {
			digestHexStr.append(byteHEX(rs[i]));
		}

//		BigInteger bigInt = new BigInteger(1, digest.digest());
//		return bigInt.toString(16);
		return digestHexStr.toString();
	}
	public static String byteHEX(byte ib) {
		char[] Digit = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
		char[] ob = new char[2];
		ob[0] = Digit[(ib >>> 4) & 0X0F];
		ob[1] = Digit[ib & 0X0F];
		String s = new String(ob);
		return s;
	}
}
