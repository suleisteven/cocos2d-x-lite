package org.cocos2dx.cpp.update;

import org.cocos2dx.cpp.update.VersionUpdater.UpdateInfo;

import android.app.Activity;
import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.widget.Toast;

import com.tomatocloud.uulandlord.R;

/**
 * <br>
 * 版本更新管理器</br></br>.
 * 
 * @author: wangYueLin 12-9-25 上午10:57
 * @since xCloud3.0.
 */
public class VersionUpdateManager {

	/**
	 * 自动更新 (只能在界面上调用)
	 * 
	 * @param activity
	 */
	public static synchronized void autoUpdateApp(final Activity activity,
			UpdateInfo updateInfo) {
		if (activity == null) {
			return;
		}

		VersionUpdateManager versionUpdateManager = new VersionUpdateManager(
				activity, updateInfo);
		versionUpdateManager.defaultUpdateApp();
	}

	public void setUpdateInfo(UpdateInfo updateInfo) {
		this.mUpdater.setUpdateInfo(updateInfo);
	}

	private Context context;

	private Handler mHandler = new Handler(Looper.getMainLooper());

	public VersionUpdateManager(Activity activity, UpdateInfo updateInfo) {
		this.context = activity;
		mUpdater = new VersionUpdater(activity, updateInfo);
	}

	VersionUpdater mUpdater;

	/**
	 * @param isAutoUpdate
	 *            是否是自动更新
	 */
	public void defaultUpdateApp() {
		if (context == null) {
			return;
		}

		mHandler.post(new Runnable() {

			@Override
			public void run() {
				// 如果apk正在下载中
				if (mUpdater.downloadingApk) {
					XToast.showToast(context,
							R.string.landlord_update_loading_text_tip,
							Toast.LENGTH_SHORT);
					return;
				}
				XToast.showToast(context,
						R.string.landlord_begin_update_notification_text,
						Toast.LENGTH_SHORT);
				mUpdater.startUpdateApp(onUpdateEndListener);
			}
		});

	}

	private OnUpdateEndListener onUpdateEndListener = new OnUpdateEndListener() {
		@Override
		public void onUpdateEnd(final boolean isSuccess) {
			mHandler.post(new Runnable() {
				@Override
				public void run() {
					if (!isSuccess) {
						XToast.showToast(context,
								R.string.landlord_update_failed_text_tip,
								Toast.LENGTH_LONG);
					}

					NotificationUtil.removeNotifycation(context,
							NotificationConstant.NOTIFICATION_TAG_UPDATE_APP);
				}
			});

			mUpdater.downloadingApk = false;
		}

	};
}
