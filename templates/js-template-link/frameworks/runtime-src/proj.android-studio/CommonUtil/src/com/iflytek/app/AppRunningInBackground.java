package com.iflytek.app;

import android.app.ActivityManager;
import android.app.KeyguardManager;
import android.content.ComponentName;
import android.content.Context;

import java.util.List;

public class AppRunningInBackground {
    public static boolean notifyShowing = false;
    public static boolean isSpecialSystem=false;
    private boolean isInstance = false;
    ActivityManager activityManager;
    KeyguardManager keyguardManager;
    private Context context = null;
    private static AppRunningInBackground appRunningInBackground = null;
    public static AppRunningInBackground getInstance(Context context){
        synchronized (AppRunningInBackground.class) {
            if(appRunningInBackground == null){
                appRunningInBackground = new AppRunningInBackground(context);
            }
            return appRunningInBackground;
        }

    }

    private AppRunningInBackground(Context context) {
        this.context = context;
        activityManager = (ActivityManager)context.getSystemService(Context.ACTIVITY_SERVICE);
        keyguardManager = (KeyguardManager)context.getSystemService(Context.KEYGUARD_SERVICE);
    }

    /**
     * 判断程序是否在前台
     * @return true 在前台; false 在后台
     */
    public boolean isAppOnForeground() {
        if(!isSpecialSystem){
            boolean isspecial=true;
            String packageName = context.getPackageName();
            List<ActivityManager.RunningAppProcessInfo> appProcesses = activityManager.getRunningAppProcesses();
            if (appProcesses == null)
                return false;
            for (ActivityManager.RunningAppProcessInfo appProcess : appProcesses) {
                if (appProcess.processName.equals(packageName)) {
                    if (appProcess.importance == ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND||appProcess.importance == ActivityManager.RunningAppProcessInfo.IMPORTANCE_VISIBLE) {
                        return true;
                    }
                    if (keyguardManager.inKeyguardRestrictedInputMode()) return true;
                }
                if(isspecial){
                    if(appProcess.importance == ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND){
                        isspecial=false;
                    }
                }
            }
            if(isspecial){
                isSpecialSystem=true;
                return !isApplicationBroughtToBackgroundByTask();
            }
            return false;
        }else{
            return !isApplicationBroughtToBackgroundByTask();
        }
    }

    /**
     * 判断当前应用程序是否处于后台，通过getRunningTasks的方式
     * @return true 在后台; false 在前台
     */
    public  boolean isApplicationBroughtToBackgroundByTask() {
        List<ActivityManager.RunningTaskInfo> tasks = activityManager.getRunningTasks(1);
        if (!tasks.isEmpty()) {
            ComponentName topActivity = tasks.get(0).topActivity;
            if (!topActivity.getPackageName().equals(context.getPackageName())) {
                return true;
            }
        }
        return false;
    }
}
