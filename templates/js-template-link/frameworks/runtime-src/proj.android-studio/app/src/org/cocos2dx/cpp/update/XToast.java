package org.cocos2dx.cpp.update;

import org.cocos2dx.cpp.AppActivity;

import android.content.Context;
import android.provider.Settings.Global;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

/**
 * <b>自定义toast</b></br>
 * 
 * @author Steven
 * @2013-4-16
 * @下午04:58:05
 * @since XCloud 4.0.0
 */
public class XToast {
	private static Toast toast;

	private static String preMessage = "";

	private static int layoutId = -1;

	private static int txvId = -1;

	public static void init(int layoutId, int txvId) {
		XToast.layoutId = layoutId;
		XToast.txvId = txvId;
	}

	/**
	 * 创建自定义Toast
	 * 
	 * @param context
	 * @param message
	 * @param duration
	 */
	private static synchronized Toast makeText(Context context, String message,
			int duration) {
		try {

			if (preMessage.equals(message) && null != toast) {
				TextView tv = null;
				if (layoutId != -1 && txvId != -1) {
					tv = (TextView) toast.getView().findViewById(txvId);
				} else {
				}
				if (null != tv) {
					tv.setText(message);
				} else {
					toast.setText(message);
				}
				toast.setDuration(duration);
			} else {

				if (layoutId != -1 && txvId != -1) {
					toast = new Toast(context.getApplicationContext());
					LayoutInflater inflate = (LayoutInflater) context
							.getApplicationContext().getSystemService(
									Context.LAYOUT_INFLATER_SERVICE);
					View v;
					TextView tv = null;
					v = inflate.inflate(layoutId, null);
					tv = (TextView) v.findViewById(txvId);
					tv.setText(message);
					toast.setView(v);

				} else {
					toast = Toast.makeText(context, message, duration);
				}
				toast.setDuration(duration);
			}

			preMessage = message;
		} catch (Exception e) {
			e.printStackTrace();
		}

		return toast;
	}

	/**
	 * 显示Toast
	 * 
	 * @param message
	 * @param duration
	 */
	public static void showToast(Context context, String message, int duration) {
		Toast toast = makeText(context.getApplicationContext(), message,
				duration);
		if (null != toast
				&& SystemUtil.isTopActivity(context.getApplicationContext())) {
			toast.show();
		}
	}

	/**
	 * 显示Toast
	 * 
	 * @param resId
	 * @param duration
	 */
	public static void showToast(Context context, int resId, int duration) {
		showToast(context, context.getApplicationContext().getString(resId),
				duration);
	}

}
