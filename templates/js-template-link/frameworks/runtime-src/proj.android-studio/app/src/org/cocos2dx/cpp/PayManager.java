package org.cocos2dx.cpp;

import android.Manifest;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Environment;
import android.os.Looper;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.text.TextUtils;
import android.util.Config;
import android.util.Log;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Handler;

import c.b.BP;
import c.b.PListener;
import c.b.QListener;

/**
 * Created by Administrator on 2017-4-9.
 */
public class PayManager {
    String APPID = "1b16ac6770d446e538fcbdbe30a9cd45";

    // 此为微信支付插件的官方最新版本号,请在更新时留意更新说明
    int PLUGINVERSION = 7;
    private static PayManager instance;

    public static synchronized PayManager getInstance() {
        if (instance == null) {
            instance = new PayManager();
        }
        return instance;
    }

    private Activity mActivity;

    public void init(Activity activity) {
        this.mActivity = activity;
        BP.init(APPID);
    }

    public void queryOrder(final String orderId) {
        BP.query(orderId, new QListener() {

            @Override
            public void succeed(String status) {
                NativeInteractive.onQueryPayOrderIdResult(0, orderId);
            }

            @Override
            public void fail(int code, String reason) {
                int resultCode = code;
                if (code == 9010) // 网络异常
                {
                    resultCode = 98;
                }

                NativeInteractive.onQueryPayOrderIdResult(resultCode, orderId);
            }
        });
    }

    private String curPayOrderId;

    public void pay(final String goodsName, final String desc, final double price, final boolean aliOrWechat) {
        mActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (aliOrWechat) {
                    if (!checkPackageInstalled("com.eg.android.AlipayGphone",
                            "https://www.alipay.com")) { // 支付宝支付要求用户已经安装支付宝客户端
                        Toast.makeText(mActivity, "请安装支付宝客户端", Toast.LENGTH_SHORT)
                                .show();

                        NativeInteractive.onPayResult(97, "", ""); // 回调，需要安装插件
                        return;
                    }
                } else {
                    if (checkPackageInstalled("com.tencent.mm", "http://weixin.qq.com")) {// 需要用微信支付时，要安装微信客户端，然后需要插件
                        // 有微信客户端，看看有无微信支付插件
                        int pluginVersion = BP.getPluginVersion(mActivity.getApplicationContext());
                        if (pluginVersion < PLUGINVERSION) {// 为0说明未安装支付插件,
                            // 否则就是支付插件的版本低于官方最新版
                            Toast.makeText(
                                    mActivity,
                                    pluginVersion == 0 ? "尚未安装支付插件,无法进行支付,请先安装插件(无流量消耗)"
                                            : "支付插件需要进行更新,请先更新插件(无流量消耗)",
                                    Toast.LENGTH_LONG).show();
//					installBmobPayPlugin("bp.db");
                            installApk("bp.db");

                            NativeInteractive.onPayResult(97, "", ""); // 回调，需要安装插件
                            return;
                        }
                    } else {// 没有安装微信
                        Toast.makeText(mActivity, "请安装微信客户端", Toast.LENGTH_SHORT).show();

                        NativeInteractive.onPayResult(97, "", ""); // 回调，需要安装插件
                        return;
                    }
                }

                if (!aliOrWechat) // 微信支付，先打开插件
                {
                    try {
                        Intent intent = new Intent(Intent.ACTION_MAIN);
                        intent.addCategory(Intent.CATEGORY_LAUNCHER);
                        ComponentName cn = new ComponentName("com.bmob.app.sport",
                                "com.bmob.app.sport.wxapi.BmobActivity");
                        intent.setComponent(cn);
                        mActivity.startActivity(intent);
                    } catch (Throwable e) {
                        e.printStackTrace();
                    }
                }

                if(!TextUtils.isEmpty(curPayOrderId))
                {
                    stopWatchOrder(curPayOrderId); // 停止之前的订单监听
                }

                curPayOrderId = "";

                LogUtil.log("pay start, type:" + aliOrWechat);
                BP.pay(goodsName, desc, price, aliOrWechat, new PListener() {
                    @Override
                    public void orderId(String s) {
                        LogUtil.log("pay orderId:" + s);
                        curPayOrderId = s;

                        startWatchOrder(s, aliOrWechat); // 开始当前订单的监听
                        NativeInteractive.onPayResult(2, curPayOrderId, "orderId");
                    }

                    @Override
                    public void succeed() {
                        LogUtil.log("pay succeed");

                        boolean hasOrder = hasSuccessOrder(curPayOrderId);
                        addSuccessOrder(curPayOrderId);
                        stopWatchOrder(curPayOrderId); // 停止当前订单监听
                        gotoGameActivity(2000);

                        // 通知底层
                        if(!hasOrder)
                        {
                            NativeInteractive.onPayResult(0, curPayOrderId, "success");
                        }
                    }

                    @Override
                    public void fail(int i, String s) {
                        LogUtil.log("pay fail:" + i);
                        stopWatchOrder(curPayOrderId); // 停止当前订单监听

                        int resultCode = i;
                        if (i == 6001) // 用户取消
                        {
                            resultCode = 99;
                        } else if (i == 9010) // 网络异常
                        {
                            resultCode = 98;
                        }

                        NativeInteractive.onPayResult(resultCode, curPayOrderId, s);
                    }

                    @Override
                    public void unknow() {
                        LogUtil.log("pay unknow");
                        stopWatchOrder(curPayOrderId); // 停止当前订单监听
                        NativeInteractive.onPayResult(-99, curPayOrderId, "unknow");
                    }
                });
            }
        });
    }

    // 已成功的订单，避免重复回调底层
    Map<String, String> successOrderMap = new HashMap<String,String>();

    private synchronized boolean hasSuccessOrder(final String orderId)
    {
        return successOrderMap.get(orderId) != null;
    }

    private synchronized void addSuccessOrder(final String orderId)
    {
        successOrderMap.put(orderId, orderId);
    }

    Map<String, WatchOrderThread> threadMap = new HashMap<String,WatchOrderThread>();
    /**
     * 开始监听订单
     *
     * @param orderId
     */
    private synchronized void startWatchOrder(final String orderId, final boolean isAli)
    {
        WatchOrderThread wot = threadMap.get(orderId);
        if(null == wot)
        {
            wot = new WatchOrderThread(orderId, isAli);
            threadMap.put(orderId, wot);
            wot.start();
        }
    }

    /**
     * 停止监听订单
     * @param orderId
     */
    private synchronized void stopWatchOrder(final String orderId) {
        WatchOrderThread wot = threadMap.get(orderId);
        if(null != wot)
        {
            wot.cancel();
            threadMap.remove(orderId);
        }
    }


    private android.os.Handler mHandler = new android.os.Handler(Looper.getMainLooper());


    private void gotoGameActivity(final int delayTime)
    {
        System.out.println("pay goto game");
//        mHandler.postDelayed(new Runnable() {
//            @Override
//            public void run() {
//                System.out.println("pay goto game do");
//                Intent intent = new Intent();
//                intent.addCategory(Intent.CATEGORY_LAUNCHER);
//                intent.setComponent(new ComponentName(mActivity.getPackageName(), MiddleTransActivity.class.getName()));
//                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
//                mActivity.startActivity(intent);
//            }
//        },delayTime);

        new Thread()
        {
            @Override
            public void run() {
                try {
                    Thread.sleep(delayTime);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("pay goto game do");
                Intent intent = new Intent();
                intent.addCategory(Intent.CATEGORY_LAUNCHER);
                intent.setComponent(new ComponentName(mActivity.getPackageName(), MiddleTransActivity.class.getName()));
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
                mActivity.startActivity(intent);
            }
        }.start();
    }

    /**
     * 监听订单线程
     */
    class WatchOrderThread extends Thread {
        private String orderId = "";
        private boolean isActive = true;
        private boolean isAli = true;

        private int curTimes = 0;
        private static final int INTERVAL = 2000;
        private static final int MAX_TIMES = 50;

        WatchOrderThread(String orderId, boolean isAli) {
            this.orderId = orderId;
            this.isAli = isAli;
        }

        public void cancel() {
            this.isActive = false;
            this.interrupt();
        }

        private boolean isDoNext = false;

        @Override
        public void run() {

            LogUtil.log("pay watch start");
            boolean isFirst = true;
            while (isActive) {
                try {

                    if(isFirst)
                    {
                        Thread.sleep(6000); // 第一次等待8秒才开始处理，给玩家10秒种的时间进行支付
                        isFirst = false;
                    }
                    BP.query(orderId, new QListener() {

                        @Override
                        public void succeed(String status) {
                            System.out.println("pay watch status:" + status);
                            if("SUCCESS".equalsIgnoreCase(status))
                            {
                                int delayTime = 0;
                                if(isAli)
                                {
                                    delayTime = 2000;
                                }
                                gotoGameActivity(delayTime);
                                boolean hasOrder = hasSuccessOrder(orderId);
                                addSuccessOrder(orderId);

                                // 通知底层
                                if(!hasOrder)
                                {
                                    NativeInteractive.onPayResult(0, orderId, "success");
                                }

                                stopWatchOrder(orderId); // 停止订单监测
                            }
                            else
                            {
                                isDoNext = true;
                            }

                        }

                        @Override
                        public void fail(int code, String reason) {
                            isDoNext = true;
                        }
                    });

                    isDoNext = false;
                    while(!isDoNext)
                    {
                        Thread.sleep(1000);
                    }

                    curTimes++;

                    if(curTimes >= MAX_TIMES)
                    {
                        break;
                    }
                    else
                    {
                        Thread.sleep(INTERVAL); // 休息间隔时间,进行下一次轮询
                    }
                }
                catch (Exception e) {
                }
            }
            LogUtil.log("pay watch over:" + curTimes);
            synchronized (this)
            {
                WatchOrderThread wot = threadMap.get(orderId);
                if(null != wot)
                {
                    threadMap.remove(orderId);
                }
            }

        }
    }

    /**
     * 检查某包名应用是否已经安装
     *
     * @param packageName 包名
     * @param browserUrl  如果没有应用市场，去官网下载
     * @return
     */
    private boolean checkPackageInstalled(String packageName, String browserUrl) {
        try {

            // 检查是否有支付宝客户端
            mActivity.getPackageManager().getPackageInfo(packageName, 0);
            return true;
        } catch (PackageManager.NameNotFoundException e) {

            // 没有安装支付宝，跳转到应用市场
            try {
                Intent intent = new Intent(Intent.ACTION_VIEW);
                intent.setData(Uri.parse("market://details?id=" + packageName));
                mActivity.startActivity(intent);
            } catch (Exception ee) {// 连应用市场都没有，用浏览器去支付宝官网下载
                try {
                    Intent intent = new Intent(Intent.ACTION_VIEW);
                    intent.setData(Uri.parse(browserUrl));
                    mActivity.startActivity(intent);
                } catch (Exception eee) {
                    Toast.makeText(mActivity,
                            "请先安装支付宝或者微信再继续支付",
                            Toast.LENGTH_SHORT).show();
                }
            }

        }
        return false;
    }

    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {

        if (requestCode == REQUESTPERMISSION) {
            if (permissions[0].equals(Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    installBmobPayPlugin("bp.db");
                } else {

                    //提示没有权限，安装不了
                    Toast.makeText(mActivity, "您拒绝了权限，这样无法安装支付插件", Toast.LENGTH_LONG).show();
                }
            }
        }
    }

    private static final int REQUESTPERMISSION = 101;

    private void installApk(String s) {

        if (ContextCompat.checkSelfPermission(mActivity, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {

            //申请权限
            ActivityCompat.requestPermissions(mActivity, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, REQUESTPERMISSION);
        } else {
            installBmobPayPlugin(s);
        }
    }

    /**
     * 安装assets里的apk文件
     *
     * @param fileName
     */
    void installBmobPayPlugin(String fileName) {
        try {
            InputStream is = mActivity.getAssets().open(fileName);
            File file = new File(Environment.getExternalStorageDirectory()
                    + File.separator + fileName + ".apk");
            if (file.exists())
                file.delete();
            file.createNewFile();
            FileOutputStream fos = new FileOutputStream(file);
            byte[] temp = new byte[1024];
            int i = 0;
            while ((i = is.read(temp)) > 0) {
                fos.write(temp, 0, i);
            }
            fos.close();
            is.close();

            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.setDataAndType(Uri.parse("file://" + file),
                    "application/vnd.android.package-archive");
            mActivity.startActivity(intent);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
