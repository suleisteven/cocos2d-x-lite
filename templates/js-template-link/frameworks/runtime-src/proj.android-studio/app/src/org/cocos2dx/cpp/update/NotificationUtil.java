package org.cocos2dx.cpp.update;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.text.TextUtils;

import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.cpp.Reflection;

import java.lang.reflect.InvocationTargetException;

/**
 * <b>notifycation通知工具类</b></br>
 * 
 * @author wangYueLin
 * @12-7-17
 * @下午1:30
 * @since xCloud1.0
 */
public class NotificationUtil {
	private static final String NOTIFICATION_SERVICE = Context.NOTIFICATION_SERVICE;

	/**
	 * 发送一个无需跳转的通知
	 *
	 * @param context
	 * @param title
	 * @param message
	 * @param tag
	 *            消息标示（UPDATE_APK_MESSAGE）
	 */
	public static void showMessageNotify(Context context, String title,
			String message, int iconID, int tag) {
		if (context == null || null == title || null == message || tag <= 0) {
			return;
		}

		NotificationManager notificationManager = (NotificationManager) context
				.getSystemService(NOTIFICATION_SERVICE);

		// 定义Notification的各种属性
		Notification notification = new Notification(iconID, title,
				System.currentTimeMillis());

		// 将此通知放到通知栏的"Ongoing"即"正在运行"组中
		notification.flags |= Notification.FLAG_ONGOING_EVENT;
		notification.defaults |= Notification.DEFAULT_SOUND;
		notification.flags |= Notification.FLAG_SHOW_LIGHTS;

		// 设置通知的事件消息
		// 通知栏标题
		CharSequence contentTitle = title;

		// 通知栏内容
		CharSequence contentText = message;

		Intent intent = new Intent();
		PendingIntent contentIntent = PendingIntent.getActivity(context, 0,
				intent, PendingIntent.FLAG_UPDATE_CURRENT);
		notification.contentIntent = contentIntent;

		try {
			Reflection.invokeMethod(notification, "setLatestEventInfo", new Class[]{Context.class,
                    CharSequence.class, CharSequence.class, PendingIntent.class}, new Object[]{context, contentTitle, contentText,
					contentIntent});
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		}
//		notification.setLatestEventInfo(context, contentTitle, contentText,
//				contentIntent);

		// 把Notification传递给NotificationManager
		notificationManager.notify(tag, notification);
	}

	/**
	 * 发送一个跳转到当前Activity的通知
	 * 
	 * @param context
	 * @param title
	 *            标题
	 * @param message
	 *            显示消息
	 * @param tag
	 *            代表此通知的id
	 * @param isAutoClear
	 *            点击之后是否自动清除
	 * @param isAutoSound
	 *            是否使用音效
	 */
	public static void showMessageNotify(Context context, String title,
			String message, int tag, boolean isAutoClear, boolean isAutoSound,
			int iconId) {
		if (context == null || null == title || null == message || tag <= 0) {
			return;
		}

		NotificationManager notificationManager = (NotificationManager) context
				.getSystemService(NOTIFICATION_SERVICE);

		// 定义Notification的各种属性
		Notification notification = new Notification(iconId, title,
				System.currentTimeMillis());

		// 将此通知放到通知栏的"Ongoing"即"正在运行"组中
		notification.flags |= Notification.FLAG_ONGOING_EVENT;

		if (isAutoSound) {
			notification.defaults |= Notification.DEFAULT_SOUND;
		}

		notification.flags |= Notification.FLAG_SHOW_LIGHTS;

		if (isAutoClear) {
			notification.flags |= Notification.FLAG_AUTO_CANCEL;
		}

		// 设置通知的事件消息
		// 通知栏标题
		CharSequence contentTitle = title;

		// 通知栏内容
		CharSequence contentText = message;

		// 设置跳转的Intent
		Intent notificationIntent = new Intent(Intent.ACTION_MAIN);
		notificationIntent.addCategory(Intent.CATEGORY_LAUNCHER);
		Class<? extends Activity> clz = null != AppActivity.getActivity() ? AppActivity
				.getActivity().getClass() : null;
		if (null != clz) {
			notificationIntent.setClass(context, clz);
		} else {
			notificationIntent.setClass(context, context.getClass());
		}
		notificationIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK
				| Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
		PendingIntent contentIntent = PendingIntent.getActivity(context, 0,
				notificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);

		try {
			Reflection.invokeMethod(notification, "setLatestEventInfo", new Class[]{Context.class,
					CharSequence.class, CharSequence.class, PendingIntent.class}, new Object[]{context, contentTitle, contentText,
					contentIntent});
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		}
//		notification.setLatestEventInfo(context, contentTitle, contentText,
//				contentIntent);

		// 把Notification传递给NotificationManager
		notificationManager.notify(tag, notification);

	}

	/**
	 * 发送一个需要activity跳转的通知
	 * 
	 * @param context
	 * @param cls
	 *            要跳转的activity
	 * @param title
	 * @param message
	 * @param tag
	 *            消息标示(UPDATE_APK_MESSAGE)
	 */
	public static void showMessageNotify(Context context,
			java.lang.Class<? extends Activity> cls, String title,
			String message, int tag, boolean isAutoSound, int iconId) {
		if (context == null || cls == null || TextUtils.isEmpty(title)
				|| TextUtils.isEmpty(message) || tag <= 0) {
			return;
		}

		NotificationManager notificationManager = (NotificationManager) context
				.getSystemService(NOTIFICATION_SERVICE);

		// 定义Notification的各种属性
		Notification notification = new Notification(iconId, title,
				System.currentTimeMillis());

		// 将此通知放到通知栏的"Ongoing"即"正在运行"组中
		notification.flags |= Notification.FLAG_ONGOING_EVENT;
		if (isAutoSound) {
			notification.defaults |= Notification.DEFAULT_SOUND;
		}
		notification.flags |= Notification.FLAG_SHOW_LIGHTS;
		notification.flags |= Notification.FLAG_AUTO_CANCEL;

		// 设置通知的事件消息
		// 通知栏标题
		CharSequence contentTitle = title;

		// 通知栏内容
		CharSequence contentText = message;

		// 点击该通知后要跳转的Activity
		Intent notificationIntent = new Intent(context, cls);
		notificationIntent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
		PendingIntent contentIntent = PendingIntent.getActivity(context, 0,
				notificationIntent, 0);

//		notification.setLatestEventInfo(context, contentTitle, contentText,
//				contentIntent);

		try {
			Reflection.invokeMethod(notification, "setLatestEventInfo", new Class[]{Context.class,
					CharSequence.class, CharSequence.class, PendingIntent.class}, new Object[]{context, contentTitle, contentText,
					contentIntent});
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		}

		// 把Notification传递给NotificationManager
		notificationManager.notify(tag, notification);

	}

	/**
	 * 取消全部通知
	 * 
	 * @param context
	 */
	public static void removeNotifycation(Context context) {
		NotificationManager notificationManager = (NotificationManager) context
				.getSystemService(NOTIFICATION_SERVICE);
		notificationManager.cancelAll();
	}

	/**
	 * 取消指定通知
	 * 
	 * @param context
	 * @param tag
	 *            UPDATE_APK_MESSAGE
	 */
	public static void removeNotifycation(Context context, int tag) {
		if (tag <= 0) {
			return;
		}

		NotificationManager notificationManager = (NotificationManager) context
				.getSystemService(NOTIFICATION_SERVICE);
		notificationManager.cancel(tag);
	}

}