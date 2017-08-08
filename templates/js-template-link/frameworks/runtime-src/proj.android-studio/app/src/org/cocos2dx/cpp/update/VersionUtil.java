package org.cocos2dx.cpp.update;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

/**
 * <b>版本工具类</b><br>
 * 
 * @author Steven
 * @time 2013-12-3 上午11:12:17
 * @since XCloud 5.0.0
 */
public class VersionUtil {

	/**
	 * 获取当前版本号
	 * 
	 * @param context
	 * @return
	 */
	public static int getVersionCode(Context context) {
		PackageManager packageManager = context.getPackageManager();
		PackageInfo packageInfo = null;

		try {
			packageInfo = packageManager.getPackageInfo(
					context.getPackageName(), 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		return packageInfo.versionCode;
	}

	/**
	 * 获取当前版本名称
	 * 
	 * @param context
	 * @return
	 */
	public static String getVersionName(Context context) {
		PackageManager packageManager = context.getPackageManager();
		PackageInfo packageInfo = null;

		try {
			packageInfo = packageManager.getPackageInfo(
					context.getPackageName(), 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}

		return packageInfo.versionName;
	}

	/**
	 * 版本长度
	 */
	public static final int VERSION_LENGTH = 4;

	/**
	 * 格式化版本号
	 * 
	 * @param version
	 *            版本号
	 * @return
	 */
	public static final String formatVersion(String version) {
		return formatVersion(version, VERSION_LENGTH);
	}

	/**
	 * 格式化版本号
	 * 
	 * @param version
	 *            版本号
	 * @param minLength
	 *            最小长度
	 * @return
	 */
	public static final String formatVersion(String version, int minLength) {
		if (null != version) {

			try {
				int result = Integer.parseInt(version);
				if (result != 0 && version.length() != 2) {
					return formatVersion(result, minLength);
				}
			} catch (Exception e) {
			}

			StringBuilder result = new StringBuilder();

			String[] splitResult = version.split("\\.");
			int forLength = splitResult.length < minLength ? minLength
					: splitResult.length;
			for (int i = 0; i < forLength; i++) {
				String s = "";
				if (i < splitResult.length) {
					s = String.valueOf(Integer.parseInt(splitResult[i]));
				} else {
					s = "0";
				}

				result.append(s);

				if (i != forLength - 1) {
					result.append(".");
				}
			}
			return result.toString();
		} else {
			return null;
		}
	}

	/**
	 * 格式化版本号
	 * 
	 * @param version
	 *            版本号
	 * @return
	 */
	public static final String formatVersion(int version) {
		return formatVersion(version, VERSION_LENGTH);
	}

	/**
	 * 格式化版本号
	 * 
	 * @param version
	 *            版本号
	 * @param minLength
	 *            最小长度
	 * @return
	 */
	public static final String formatVersion(int version, int minLength) {
		if (version >= 0) {

			String versionStr = String.valueOf(version);
			if (versionStr.length() % 2 != 0) {
				versionStr = "0" + versionStr;
			}

			StringBuilder sb = new StringBuilder();

			for (int i = 0; i < versionStr.length(); i += 2) {
				sb.append(versionStr.substring(i, i + 2));
				if (i + 2 < versionStr.length()) {
					sb.append(".");
				}
			}

			return formatVersion(sb.toString(), minLength);
		} else {
			return null;
		}
	}

	/**
	 * 版本比较，第一个参数与第二个参数进行比较，当第一个大于第二个时返回MORE_THAN，小于第二个时返回LESS_THAN,相等时返回EQUALS
	 * ，错误时返回NONE
	 * 
	 * @param formattedVerion1
	 *            格式化之后的版本号
	 * @param formattedVerion2
	 *            格式化之后的版本号
	 * @return
	 */
	public static final CompareResult compareVersion(String formattedVerion1,
			String formattedVerion2) {
		CompareResult compareResult = CompareResult.NONE;
		if (null != formattedVerion1 && null != formattedVerion2) {
			String[] result1 = formattedVerion1.split("\\.");
			String[] result2 = formattedVerion2.split("\\.");

			if (result1.length < result2.length) {
				formattedVerion1 = formatVersion(formattedVerion1,
						result2.length);
				result1 = formattedVerion1.split("\\.");
			} else if (result1.length > result2.length) {
				formattedVerion2 = formatVersion(formattedVerion2,
						result1.length);
				result2 = formattedVerion2.split("\\.");
			}

			int versionLength = result1.length;
			for (int i = 0; i < versionLength; i++) {
				int int1 = Integer.parseInt(result1[i]);
				int int2 = Integer.parseInt(result2[i]);
				if (int1 > int2) {
					compareResult = CompareResult.MORE_THAN;
					break;
				} else if (int1 < int2) {
					compareResult = CompareResult.LESS_THAN;
					break;
				} else {
					if (i == versionLength - 1) {
						compareResult = CompareResult.EQUALS;
					}
					continue;
				}
			}
		}
		return compareResult;
	}

}
