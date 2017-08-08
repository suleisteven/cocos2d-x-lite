package org.cocos2dx.cpp;

import android.app.Application;
import android.content.Context;
import android.support.multidex.MultiDex;
import android.util.Log;

import java.util.concurrent.CopyOnWriteArrayList;

public class ApplicationExp extends Application
{

	static final int MAX_OFFLINE_MSG  = 10;

	// 离线推送消息，当界面未运行，服务在运行时使用
	private static CopyOnWriteArrayList<String> offlineMsgList = new CopyOnWriteArrayList<>();

	public static CopyOnWriteArrayList<String> getOfflineMsgList()
	{
		return offlineMsgList;
	}

	// 处理离线消息
	public static void handleOfflineMsg()
	{
		final CopyOnWriteArrayList<String> msgList =  ApplicationExp.getOfflineMsgList();
		if(null!=msgList && !msgList.isEmpty())
		{
			new Thread()
			{
				@Override
				public void run() {


					try
					{
						for(String msg: msgList)
						{
							NativeInteractive.onReceiveMsg(msg);
						}
					}
					catch (Exception e)
					{
					}
					msgList.clear(); // 清空离线消息
				}
			}.start();
		}
	}

	// 派发消息
	public static void sendMsg(String msg)
	{
		if(AppActivity.getActivity() != null) // 当前界面在运行
		{
			System.out.println("onReceiveMessage-----1:"+msg);
			NativeInteractive.onReceiveMsg(msg);
		}
		else // 界面未运行，存放到离线消息中
		{
			System.out.println("onReceiveMessage-----2:"+msg);
			try
			{
				if(offlineMsgList.size() < MAX_OFFLINE_MSG)
				{
					offlineMsgList.add(msg);
				}
				else
				{
					offlineMsgList.remove(0);
					offlineMsgList.add(msg);
				}
			}catch (Exception e)
			{
			}


		}
	}

	@Override
	protected void attachBaseContext(Context base) {
		super.attachBaseContext(base);
		MultiDex.install(this);
		try {
			//Reflection.invokeStaticMethod("android.support.multidex.MultiDex", "install", new Class[]{Context.class}, new Object[]{this});
		} catch (Exception e) {
			//  e.printStackTrace();
		}
	}

	@Override
	public void onCreate() {
		super.onCreate();

		UncaughtExceptionHandler xCloudUncaughtExceptionHandler = new UncaughtExceptionHandler(
				this);
		Thread.setDefaultUncaughtExceptionHandler(xCloudUncaughtExceptionHandler);
	}

	public class UncaughtExceptionHandler implements
			Thread.UncaughtExceptionHandler {

		private Context context;

		public UncaughtExceptionHandler(Context context) {
			this.context = context;
		}

		@Override
		public void uncaughtException(Thread thread, Throwable throwable) {

			String classInfo = null;
			try {
				classInfo = String.valueOf(throwable.getStackTrace()[0]);

			} catch (Exception e) {
				e.printStackTrace();
			}
			// 获取异常信息
			String throwables = Log.getStackTraceString(throwable);

			// 截取异常信息
			String packageStr = context.getPackageName();
			int nindex = throwables.indexOf("\n");
			String exception = throwables.substring(
					throwables.indexOf("}:") + 1, nindex);
			String errMessage = throwables.substring(nindex);
			int index = errMessage.indexOf(packageStr);
			if (index != -1)
				errMessage = errMessage.substring(index);
			// 最终要显示的异常信息
			final String err = "Caused by " + exception + "\n" + errMessage;
			System.err.println(err);
			Log.e("ApplicationExp", throwables);
			/*
			 * FlurryAgent.onError(
			 * String.valueOf(VersionUtil.getVersionCode(context)), err,
			 * classInfo);
			 */
			System.exit(1);
		}
	}

}
