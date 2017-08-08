package org.cocos2dx.cpp.update;

import java.io.File;

import org.cocos2dx.cpp.Util;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.text.TextUtils;
import android.widget.RemoteViews;
import android.widget.Toast;

import com.tomatocloud.uulandlord.R;

/**
 * <b>小云版本检测更新工具类</b></br>
 * 
 * @author wangYueLin
 * @2012-7-11
 * @上午09:34:31
 */
public class VersionUpdater {

	/**
	 * 当期版本检测状态
	 */
	public enum VersionStatus {
		/**
		 * 是最新版本
		 */
		IS_LATEST_STATUS,
		/**
		 * 非最新，需要更新
		 */
		NEED_UPDATE_STATUS,
		/**
		 * 检测时发生网络错误
		 */
		NETWORK_ERROR,
	}

	/**
	 * http请求错误
	 */
	public static final int HTTP_ERROR = 1;

	/**
	 * 下载完成
	 */
	public static final int DOWNLOAD_COMPLETE = 0;

	// update: 2
	// latest_version {
	// version_name: "test version update"
	// version_code: 300
	// channel_name: "default"
	// }
	// app_md5: "d3a7d905b64ceddd78fbcdf25618a168"
	// app_instruction: "阿杜刀功不错"
	// app_url: "http://120.25.153.194/down/android.apk"
	// install_type: 1
	// version_size: 3484856

	public VersionUpdater(Context context, UpdateInfo updateInfo) {
		this.mContext = context;
		this.mUpdateInfo = updateInfo;
	}

	private UpdateInfo mUpdateInfo;
	private Context mContext;

	public void setUpdateInfo(UpdateInfo updateInfo) {
		this.mUpdateInfo = updateInfo;
	}

	/**
	 * 升级信息
	 */
	public static class UpdateInfo {
		/**
		 * 升级类型，0 不升级, 1 建议升级, 2 强制升级
		 */
		public int updateType;
		/**
		 * 安装类型，0-全量更新、1-部分更新
		 */
		public int installTtype;
		/**
		 * 最新版本号
		 */
		public int newestVersionCode;
		/**
		 * 最新版本名称
		 */
		public String newestVersionName;
		/**
		 * 渠道名称
		 */
		public String channelName = "";
		/**
		 * 升级地址
		 */
		public String updateUrl = "";
		/**
		 * 升级包大小，单位：字节
		 */
		public long updateSize;
		/**
		 * 升级包md5值
		 */
		public String updateMD5 = "";
		/**
		 * 升级简介
		 */
		public String updateInstruction = "";
	}

	/**
	 * apk是否在下载中
	 */
	public boolean downloadingApk = false;

	public void showUpdateDialog(final OnUpdateEndListener onUpdateEndListener) {
		if (onUpdateEndListener == null) {
			return;
		}

		AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
		String updateMsg = mUpdateInfo.updateInstruction;

		String version = mUpdateInfo.newestVersionName;

		if (TextUtils.isEmpty(updateMsg)) {
			builder.setMessage(mContext.getResources().getString(
					R.string.app_update_msg, version));
		} else {
			String str = mContext.getResources().getString(
					R.string.app_update_msg, version);
			builder.setMessage(str + "\n" + updateMsg);
		}

		builder.setTitle(R.string.app_update_title);
		builder.setPositiveButton(R.string.app_update_ok_btn,
				new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialogInterface, int i) {
						dialogInterface.dismiss();

						XToast.showToast(mContext,
								R.string.notifycation_update_app_msg,
								Toast.LENGTH_SHORT);
						new Thread() {
							@Override
							public void run() {
								updateApp(onUpdateEndListener);
							}
						}.start();
					}
				});

		builder.setNegativeButton(R.string.app_update_cancel_btn,
				new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialogInterface, int i) {
						dialogInterface.dismiss();
					}
				});

		if (null != mContext && (mContext instanceof Activity)
				&& !((Activity) mContext).isFinishing()) {
			builder.create().show();
		}
	}

	public void showDefaultDialog(final OnUpdateEndListener onUpdateEndListener) {
		if (onUpdateEndListener == null) {
			return;
		}

		AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
		builder.setMessage(R.string.app_is_latest_msg);
		builder.setTitle(R.string.app_update_title);
		builder.setPositiveButton(R.string.global_confirm,
				new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialogInterface, int i) {
						dialogInterface.dismiss();
						onUpdateEndListener.onUpdateEnd(true);
					}
				});

		if (null != mContext && (mContext instanceof Activity)
				&& !((Activity) mContext).isFinishing()) {
			builder.create().show();
		}
	}

	/**
	 * 开始更新
	 * 
	 * @param onUpdateEndListener
	 */
	public void startUpdateApp(final OnUpdateEndListener onUpdateEndListener) {
		new Thread() {
			@Override
			public void run() {
				updateApp(onUpdateEndListener);
			}
		}.start();
	}

	/**
	 * 判断apk文件md5是否相等
	 * 
	 * @param file
	 * @return
	 */
	boolean isAPKFileMd5Ok(File file) {
		String md5 = Util.getFileMD5(file);
		return md5.equalsIgnoreCase(mUpdateInfo.updateMD5);
	}

	/**
	 * 更新应用
	 * 
	 * @param context
	 * @param onUpdateEndListener
	 */
	private void updateApp(final OnUpdateEndListener onUpdateEndListener) {
		// 比对本地包

		// 本地已下载包
		final File fileReal = new File(FileUtil.getSDCardPath()
				+ File.separator + FileConstant.FILE_ROOT_SAVE_PATH
				+ File.separator + FileConstant.FILE_NAME_UPDATE);

		// 是否需要下载
		boolean isNeedDownload = false;

		// 如果文件已经存在,并且是合法的APK文件
		if (SystemUtil.isAvailablyAPK(mContext, fileReal.getAbsolutePath())) {
			// PackageInfo packageInfo = SystemUtil.getAPKPackageInfo(
			// mContext, fileReal.getAbsolutePath());
			//
			// // 获取本地版本号
			// int appVersionCode = packageInfo.versionCode;
			//
			// // 校验文件的正确性
			// if (null != packageInfo
			// && packageInfo.packageName.equals(mContext
			// .getPackageName())
			// && appVersionCode == mUpdateInfo.newestVersionCode)
			if (isAPKFileMd5Ok(fileReal)) {

				installApkFile(fileReal, mContext);
				isNeedDownload = false;
			} else {
				fileReal.delete();
				isNeedDownload = true;
			}
		} else {
			fileReal.delete();
			isNeedDownload = true;
		}

		if (!isNeedDownload) {
			return;
		}

		String title = mContext.getResources().getString(R.string.app_name);
		String msg = mContext.getResources().getString(
				R.string.notifycation_update_app_msg);

		// 通知栏
		final NotificationManager notificationManager = (NotificationManager) mContext
				.getSystemService(Context.NOTIFICATION_SERVICE);

		// 定义Notification的各种属性
		final Notification notification = new Notification(
				R.drawable.icon_notification, title, System.currentTimeMillis());

		// 将此通知放到通知栏的"Ongoing"即"正在运行"组中
		notification.flags |= Notification.FLAG_ONGOING_EVENT;

		notification.contentView = new RemoteViews(mContext.getPackageName(),
				R.layout.update_app_notification_layout);

		notification.contentView.setTextViewText(R.id.text, msg);
		notification.contentView
				.setProgressBar(R.id.progressBar, 100, 0, false);

		Intent intent = new Intent();
		PendingIntent contentIntent = PendingIntent.getActivity(mContext, 0,
				intent, PendingIntent.FLAG_UPDATE_CURRENT);
		notification.contentIntent = contentIntent;

		// 把Notification传递给NotificationManager
		notificationManager.notify(
				NotificationConstant.NOTIFICATION_TAG_UPDATE_APP, notification);

		String url = mUpdateInfo.updateUrl;

		// 正在下载
		downloadingApk = true;

		// 下载文件，临时路径
		final File fileTemp = new File(FileUtil.getSDCardPath()
				+ File.separator + FileConstant.FILE_TEMP_PATH + File.separator
				+ FileConstant.FILE_NAME_UPDATE + ".temp."
				+ mUpdateInfo.newestVersionCode);

		// 通知任务开始
		NotificationUtil
				.showMessageNotify(
						mContext.getApplicationContext(),
						mContext.getResources().getString(R.string.app_name),
						mContext.getString(R.string.landlord_begin_update_notification_text),
						NotificationConstant.NOTIFICATION_TAG_UPDATE_APP,
						false, false, R.drawable.icon_notification);

		// 下载
		HttpUtil.downloadFileAsyn(url, fileTemp.getAbsolutePath(), false, true,
				new HttpUtil.OnDownloadFileListener() {
					long lastNotifyTime = System.currentTimeMillis();

					final long interval = 500;

					@Override
					public void onDownloadFileStart(String downloadUrl,
							File downloadFile, final long fileSize) {
					}

					@Override
					public void onDownloadFileStop(String downloadUrl,
							File downloadFile, long downloadedSize) {
						downloadingApk = false;
						onUpdateEndListener.onUpdateEnd(false);
					}

					@Override
					public void onDownloadingFile(String downloadUrl,
							final long fileSize, final long downloadedSize) {
						long curTime = System.currentTimeMillis();
						if (curTime - lastNotifyTime >= interval) {
							notification.contentView.setProgressBar(
									R.id.progressBar, (int) fileSize,
									(int) (downloadedSize), false);
							notificationManager
									.notify(NotificationConstant.NOTIFICATION_TAG_UPDATE_APP,
											notification);

							lastNotifyTime = curTime;
						}
					}

					@Override
					public void onDownloadFileFinish(String downloadUrl,
							final File downloadFile) {
						downloadingApk = false;
						if (fileReal.exists()) {
							fileReal.delete();
						}

						downloadFile.renameTo(fileReal);

						if (fileReal.exists()) {
							// PackageInfo packageInfo = SystemUtil
							// .getAPKPackageInfo(mContext,
							// fileReal.getAbsolutePath());
							//
							//
							// // 校验文件的正确性
							// if (packageInfo != null
							// && packageInfo.packageName.equals(mContext
							// .getPackageName())
							// && packageInfo.versionCode ==
							// mUpdateInfo.newestVersionCode)
							if (isAPKFileMd5Ok(fileReal)) {
								installApkFile(fileReal, mContext);
								onUpdateEndListener.onUpdateEnd(true);
								return;
							}
						}

						onUpdateEndListener.onUpdateEnd(false);
					}

					@Override
					public void onDownloadError(String downloadUrl) {
						downloadingApk = false;
						onUpdateEndListener.onUpdateEnd(false);
					}
				});
		/*
		 * if (HttpConstant.NETWORK_ERROR_CODE.equals(filePath)) {
		 * onUpdateEndListener.onUpdateEnd(false); } else {
		 * onUpdateEndListener.onUpdateEnd(true); }
		 */
	}

	/**
	 * 安装Apk
	 * 
	 * @param f
	 * @param context
	 */
	private static void installApkFile(File f, Context context) {
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setAction(android.content.Intent.ACTION_VIEW);

		// 设置类型
		intent.setDataAndType(Uri.fromFile(f),
				"application/vnd.android.package-archive");

		// 安装
		context.startActivity(intent);
	}

}
