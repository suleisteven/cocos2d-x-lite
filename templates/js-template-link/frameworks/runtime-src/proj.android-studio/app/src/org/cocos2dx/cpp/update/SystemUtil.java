package org.cocos2dx.cpp.update;

import java.util.List;
import java.util.Locale;
import java.util.UUID;

import org.cocos2dx.cpp.Util.Language;

import android.accounts.Account;
import android.accounts.AccountManager;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.drawable.Drawable;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

/**
 * <b>系统工具类</b></br>
 * 
 * @author wangYueLin
 * @12-7-31
 * @下午1:42
 * @since xCloud1.0
 */
public final class SystemUtil {

	public static int dip2px(Context context, float dipValue) {
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (dipValue * scale + 0.5f);
	}

	public static int px2dip(Context context, float pxValue) {
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (pxValue / scale + 0.5f);
	}

	public static int px2sp(Context context, float pxValue) {
		final float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
		return (int) (pxValue / fontScale + 0.5f);
	}

	public static int sp2px(Context context, float spValue) {
		final float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
		return (int) (spValue * fontScale + 0.5f);
	}

	/**
	 * 跳转到HOME
	 * 
	 * @param context
	 */
	public static void toSysHome(Context context) {
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_HOME);
		context.startActivity(intent);
	}

	/**
	 * 获取系统当前语言类型
	 * 
	 * @param defaultLanguage
	 * @return
	 */
	public static Language getSystemLanguage(Language defaultLanguage) {
		// 检测语言环境
		Locale locale = Locale.getDefault();

		Language resultLanguage = Language.getLanguageByLocal(locale);
		if (null == resultLanguage) {
			return defaultLanguage;
		}
		return resultLanguage;
	}

	private static String mac = "";

	private static String spaceMarkPre = "";

	/**
	 * 获取设备Mac地址
	 * 
	 * @return
	 */
	public static String getDeviceMac(Context context) {
		return getDeviceMac("-", context);
	}

	/**
	 * 获取设备mac地址
	 * 
	 * @param spaceMark
	 *            mac地址间隔符（如：-）
	 * @return
	 */
	public static String getDeviceMac(String spaceMark, Context context) {
		if (!TextUtils.isEmpty(mac) && spaceMarkPre.equals(spaceMark)) {
			return mac;
		}

		if (null != context) {
			// 取得WifiManager对象
			WifiManager wifiManager = (WifiManager) context
					.getSystemService(Context.WIFI_SERVICE);

			// 取得WifiInfo对象
			WifiInfo wifiInfo = wifiManager.getConnectionInfo();
			if (null != wifiInfo) {
				mac = wifiInfo.getMacAddress();

				spaceMarkPre = spaceMark;

				// 替换间隔符号
				if (!TextUtils.isEmpty(mac)) {
					mac = mac.replaceAll(":", spaceMark);
				}
			}
		}
		return mac;
	}

	/**
	 * 获取设备id
	 * 
	 * @return
	 */
	public static String getDeviceSerNum(Context context) {

		String deviceSerNum = "";
		if (null != context) {
			TelephonyManager mTelephonyManager = (TelephonyManager) context
					.getSystemService(Context.TELEPHONY_SERVICE);

			if (null != mTelephonyManager) {
				deviceSerNum = mTelephonyManager.getDeviceId();
			}
		}

		if (TextUtils.isEmpty(deviceSerNum)) {
			deviceSerNum = android.os.Build.ID;
		}

		if (TextUtils.isEmpty(deviceSerNum)) {
			deviceSerNum = UUID.randomUUID().toString().substring(0, 30);
		}

		return deviceSerNum;
	}

	/**
	 * 获取系统版本值
	 * 
	 * @return
	 */
	public static int getSystemVersionCode() {
		int sysVersionCode = android.os.Build.VERSION.SDK_INT;
		return sysVersionCode;
	}

	/**
	 * 获取本机googleplay账号
	 * 
	 * @param context
	 * @return
	 */
	public static String getGooglePlayAccount(Context context) {
		AccountManager accountManager = AccountManager.get(context);
		Account[] accounts = accountManager.getAccountsByType("com.google");
		Account account = null;
		if (accounts.length > 0) {
			account = accounts[0];
			return account.name;
		}
		return null;
	}

	/**
	 * 程序是否正在运行
	 * 
	 * @param context
	 * @return
	 */
	public static final boolean isLauncherRunnig(Context context) {
		boolean result = false;

		ActivityManager mActivityManager = (ActivityManager) context
				.getSystemService(Context.ACTIVITY_SERVICE);
		String packageName = context.getPackageName();
		if (null == packageName) {
			return result;
		}
		List<ActivityManager.RunningAppProcessInfo> appList = mActivityManager
				.getRunningAppProcesses();
		for (RunningAppProcessInfo running : appList) {
			if (running.importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
				if (packageName.equals(running.processName)) {
					result = true;
					break;
				}
			}
		}
		return result;
	}

	/**
	 * 判断当前应用是否在前台运行
	 * 
	 * @param context
	 * @return
	 */
	public static boolean isTopActivity(Context context) {
		if (context == null) {
			return false;
		}

		String packageName = context.getPackageName();

		ActivityManager activityManager = (ActivityManager) context
				.getSystemService(Context.ACTIVITY_SERVICE);

		List<ActivityManager.RunningTaskInfo> tasksInfo = activityManager
				.getRunningTasks(1);

		if (tasksInfo.size() > 0) {
			// 应用程序位于堆栈的顶层
			if (packageName.equals(tasksInfo.get(0).topActivity
					.getPackageName())) {
				return true;
			}
		}
		return false;
	}

	/**
	 * 获取未安装APK的图标
	 * 
	 * @param ctx
	 * @param archiveFilePath
	 * @return
	 */
	public static Drawable getUninstallAPKIcon(Context ctx,
			String archiveFilePath) {
		Drawable icon = null;
		try {
			PackageManager pm = ctx.getPackageManager();
			PackageInfo pakinfo = pm.getPackageArchiveInfo(archiveFilePath,
					PackageManager.GET_ACTIVITIES);

			if (pakinfo != null) {
				ApplicationInfo appinfo = pakinfo.applicationInfo;

				// 设置包资源路径
				appinfo.publicSourceDir = archiveFilePath;
				icon = pm.getApplicationIcon(appinfo);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return icon;
	}

	/**
	 * 获取APK的信息
	 * 
	 * @param ctx
	 * @param apkPath
	 * @return
	 */
	public static PackageInfo getAPKPackageInfo(Context ctx, String apkPath) {
		PackageInfo pakinfo = null;
		try {
			PackageManager pm = ctx.getPackageManager();
			pakinfo = pm.getPackageArchiveInfo(apkPath,
					PackageManager.GET_ACTIVITIES);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return pakinfo;
	}

	/**
	 * 是否为有效的APK文件
	 * 
	 * @param ctx
	 * @param apkPath
	 * @return
	 */
	public static boolean isAvailablyAPK(Context ctx, String apkPath) {
		try {
			PackageManager pm = ctx.getPackageManager();
			PackageInfo pakinfo = pm.getPackageArchiveInfo(apkPath,
					PackageManager.GET_ACTIVITIES);

			if (pakinfo != null) {
				return true;
			} else {
				return false;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return false;
	}

	/**
	 * 关闭软键盘，输入法
	 * 
	 * @param context
	 */
	public static void hideSoftInputMethod(Activity context) {
		try {
			InputMethodManager inputMethodManager = (InputMethodManager) context
					.getSystemService(Context.INPUT_METHOD_SERVICE);
			inputMethodManager.hideSoftInputFromWindow(context
					.getCurrentFocus().getWindowToken(),
					InputMethodManager.HIDE_NOT_ALWAYS);
		} catch (Exception e) {
		}
	}

	/**
	 * 打开软键盘，输入法
	 * 
	 * @param context
	 * @param view
	 */
	public static void showSoftInputMethod(Activity context, EditText view) {
		InputMethodManager inputMethodManager = (InputMethodManager) context
				.getSystemService(Context.INPUT_METHOD_SERVICE);

		// 接受软键盘输入的编辑文本或其它视图
		inputMethodManager.showSoftInput(view, InputMethodManager.SHOW_FORCED);
	}
}
