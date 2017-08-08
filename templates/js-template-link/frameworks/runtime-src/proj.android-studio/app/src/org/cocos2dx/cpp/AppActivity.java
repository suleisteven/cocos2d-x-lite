/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;

import com.facebook.FacebookSdk;
import com.igexin.sdk.PushManager;
import com.testin.agent.TestinAgent;
import com.umeng.mobclickcpp.MobClickCppHelper;
import com.umeng.social.CCUMSocialController;
import com.umeng.socialize.PlatformConfig;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

import c.b.BP;

public class AppActivity extends Cocos2dxActivity {
	static AppActivity sAppActivity;

	public static Activity getActivity() {
		return sAppActivity;
	}

	private static final String KEY_TEST_IN = "1cef83d3f06d41a8ff780a8248f2dbc5";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		sAppActivity = this;
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		PlatformConfig.setWeixin("wxc1651208a490959d", "d4624c36b6795d1d99dcf0547af5443d");
		PlatformConfig.setQQZone("1104979614", "67jnTCTRMFP4fMtb");
		CCUMSocialController.setQQAndQzoneAppIdWithAppKey("1104979614", "67jnTCTRMFP4fMtb");
		CCUMSocialController.setFacebookAppId("1193985570691152");
		CCUMSocialController.setWeiXinAppId("wxc1651208a490959d");
		CCUMSocialController.setWeiXinAppInfo("wxc1651208a490959d", "d4624c36b6795d1d99dcf0547af5443d");
		FacebookSdk.setApplicationId("1193985570691152");


		// 初始化本地交互功能模块
		NativeInteractive.init(this);

		// 初始化umeng本地交互功能模块
		MobClickCppHelper.init(this);

		// 初始化umeng社交sdk
		CCUMSocialController.initSocialSDK(this, "com.umeng.social.share");

		// 初始化testin
		TestinAgent.init(this, KEY_TEST_IN,
				Util.getChannelKey(getApplicationContext()));

		// 初始化广告
		try {
			Class adUtilCls = Class.forName("com.tomatocloud.adproj.AdUtil");
			Object adUtilInstance = Reflection.invokeStaticMethod(adUtilCls, "getInstance", new Class[]{}, new Object[]{});
			Reflection.invokeMethod(adUtilInstance, "initAd", new Class[]{Activity.class}, new Object[]{this});
		} catch (Exception e) {
			e.printStackTrace();
		}
//		AdUtil.getInstance().initAd(this);

		// 初始化个推
		initGeTui();

		// 初始化bmob
		PayManager.getInstance().init(this);


//		LinearLayout.LayoutParams lp1 = new LinearLayout.LayoutParams(
//				LinearLayout.LayoutParams.WRAP_CONTENT,
//				LinearLayout.LayoutParams.WRAP_CONTENT);
//		LinearLayout layout = new LinearLayout(this);
//		addContentView(layout, lp1);
//
//		Button btn = new Button(this);
//		btn.setText("支付");
//		btn.setOnClickListener(new View.OnClickListener() {
//			@Override
//			public void onClick(View v) {
//				PayManager.getInstance().pay("商品测试", "商品测试描述", 0.01, false);
//			}
//		});
//
//		layout.addView(btn, lp1);
//
////		99adf8014d24ac4132c9b8e272d334fd
//		Button btn2 = new Button(this);
//		btn2.setText("查询");
//		btn2.setOnClickListener(new View.OnClickListener() {
//			@Override
//			public void onClick(View v) {
//
//				PayManager.getInstance().queryOrder("d0b613f3d06706b1d16c4be8630b0366");
////				PayManager.getInstance().queryOrder("9d73d25831a9ba01040343afc65b654b");
////				PayManager.getInstance().queryOrder("58f4b15f266d01cda8f765c754e3f2a9");
//				//PayManager.getInstance().queryOrder("46dab4cbae80d3ce5c659e6f127741a8");
//			}
//		});
//		layout.addView(btn2, lp1);

//
//
//		btn.setOnClickListener(new View.OnClickListener() {
//
//			@Override
//			public void onClick(View v) {
//
//				final EditText inputET = new EditText(AppActivity.this);
//				inputET.setHint("随便输个字符串，后台区分");
//				AlertDialog.Builder builder = new AlertDialog.Builder(
//						AppActivity.this);
//				builder.setTitle("随便输个字符串，后台区分")
//						.setView(inputET)
//						.setNegativeButton("取消",
//								new DialogInterface.OnClickListener() {
//
//									@Override
//									public void onClick(DialogInterface dialog,
//											int which) {
//										dialog.dismiss();
//									}
//								})
//						.setPositiveButton("确定",
//								new DialogInterface.OnClickListener() {
//									public void onClick(DialogInterface dialog,
//											int which) {
//										String content = inputET.getText()
//												.toString();
//										throw new RuntimeException(
//												"exception for upload log:"
//														+ content);
//
//									}
//								});
//				builder.show().setCanceledOnTouchOutside(false);
//
//			}
//		});

		// 处理外部打开参数
		Intent intent = getIntent();
		handleIntent(intent);


	}

	@Override
	protected void onDestroy() {
		NativeInteractive.uninit();
		sAppActivity = null;
		super.onDestroy();

	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();

	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {

		// 回调umeng社交sdk
		CCUMSocialController.onActivityResult(requestCode, resultCode, data);
		super.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	protected void onLoadNativeLibraries() {
		super.onLoadNativeLibraries();

		// TODO 加载其他库
		// String[] libs = NativeInteractive.getToBeLoadLibNames();
		// if(null != libs && libs.length>0)
		// {
		// for(String libName: libs)
		// {
		// System.loadLibrary(libName);
		// }
		// }

	}


	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		handleIntent(intent);
	}

	private void handleIntent(Intent intent)
	{

		if(null == intent)
		{
			NativeInteractive.setOpenWithOther(false);
			NativeInteractive.setExternalParams(null);
			return;
		}

		String action = intent.getAction();
		Uri uri = intent.getData();
		if(uri != null)
		{
			HashMap<String, String> map = new HashMap<String,String>();

			if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.HONEYCOMB) {
				Set<String> keys =  uri.getQueryParameterNames();

				Iterator<String> itKey = keys.iterator();

				while(itKey.hasNext())
				{
					String key = itKey.next();
					String value = uri.getQueryParameter(key);
					map.put(key, value);
				}
			}

			if(!map.isEmpty())
			{
				NativeInteractive.setOpenWithOther(true);
				NativeInteractive.setExternalParams(map);
			}
			else
			{
				NativeInteractive.setOpenWithOther(false);
				NativeInteractive.setExternalParams(null);
			}
		}
		else
		{
			NativeInteractive.setOpenWithOther(false);
//			NativeInteractive.setExternalParams(null);
		}
	}

	@Override
	protected void onStop() {
		super.onStop();

		NativeInteractive.setOpenWithOther(false);
	}

	private void initGeTui()
	{
		PackageManager pkgManager = getPackageManager();

		// 读写 sd card 权限非常重要, android6.0默认禁止的, 建议初始化之前就弹窗让用户赋予该权限
		boolean sdCardWritePermission =
				pkgManager.checkPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, getPackageName()) == PackageManager.PERMISSION_GRANTED;

		// read phone state用于获取 imei 设备信息
		boolean phoneSatePermission =
				pkgManager.checkPermission(Manifest.permission.READ_PHONE_STATE, getPackageName()) == PackageManager.PERMISSION_GRANTED;

		if (Build.VERSION.SDK_INT >= 23 && !sdCardWritePermission || !phoneSatePermission) {
			requestPermission();
		} else {
			PushManager.getInstance().initialize(this.getApplicationContext(), userPushService);
		}

		// 注册 intentService 后 PushDemoReceiver 无效, sdk 会使用 DemoIntentService 传递数据,
		// AndroidManifest 对应保留一个即可(如果注册 DemoIntentService, 可以去掉 PushDemoReceiver, 如果注册了
		// IntentService, 必须在 AndroidManifest 中声明)
		PushManager.getInstance().registerPushIntentService(this.getApplicationContext(), DemoIntentService.class);
	}

	// DemoPushService.class 自定义服务名称, 核心服务
	private Class userPushService = DemoPushService.class;
	private static final int REQUEST_PERMISSION = 0;
	private void requestPermission() {
		ActivityCompat.requestPermissions(this, new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_PHONE_STATE},
				REQUEST_PERMISSION);
	}

	@TargetApi(23)
	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
		if (requestCode == REQUEST_PERMISSION) {
			if ((grantResults.length == 2 && grantResults[0] == PackageManager.PERMISSION_GRANTED
					&& grantResults[1] == PackageManager.PERMISSION_GRANTED)) {
				PushManager.getInstance().initialize(this.getApplicationContext(), userPushService);
			} else {
				PushManager.getInstance().initialize(this.getApplicationContext(), userPushService);
			}
		} else {
			super.onRequestPermissionsResult(requestCode, permissions, grantResults);
			PayManager.getInstance().onRequestPermissionsResult(requestCode, permissions, grantResults);
		}
	}
}
