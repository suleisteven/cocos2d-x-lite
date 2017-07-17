
/****************************************************************************
Copyright (c) 2015 Chukong Technologies Inc.
 
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
package com.uufungame.UUThirteenWater;

import android.content.SharedPreferences;
import android.os.Bundle;

import com.iflytek.utils.common.ApkUtil;
import com.iflytek.utils.common.FileUtil;
import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.mobclick.game.MobClickCppHelper;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import java.io.File;

// For JS and JAVA reflection test, you can delete it if it's your own project

public class AppActivity extends Cocos2dxActivity {

    boolean isAppUpgrade = false; // 程序是否发生过升级

    private static final String KEY_APP_VERSION_CODE = "KEY_APP_VERSION_CODE";
    private void initInfo()
    {
        SharedPreferences sp = getPreferences(MODE_PRIVATE);
        int preCode = sp.getInt(KEY_APP_VERSION_CODE, -1);
        int curCode = ApkUtil.getAppVersionCode();
        if(preCode == -1)
        {
            sp.edit().putInt(KEY_APP_VERSION_CODE, curCode).commit(); // 保存当前程序版本号
        }
        else
        {
           if(preCode != curCode)
           {
               isAppUpgrade = true; // 程序发生过升级
               sp.edit().putInt(KEY_APP_VERSION_CODE, curCode).commit(); // 保存当前程序版本号
           }
        }
        System.out.println("isAppUpgrade:" + isAppUpgrade);

        if(isAppUpgrade)
        {
            File downloadFile = getExternalFilesDir("download");

            if(null != downloadFile)
            {
                String downloadPath = downloadFile.getAbsolutePath();

                // 程序发生过升级，把之前热更新数据全部清空
                File fileDir =  new File(downloadPath);
                if(fileDir.exists())
                {
                    FileUtil.Delete(fileDir);
                }
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        ApkUtil.init(this);
        initInfo();
        super.onCreate(savedInstanceState);


        // 初始化友盟统计
        MobClickCppHelper.init(this,"59506351bbea835a61000f4f", MainApplication.channel);
    }

    /**
     * 上报错误日志到友盟
     * @param errorMsg
     */
    public static void reportError(String errorMsg)
    {
        System.out.println("errorMsg:" + errorMsg);
        MobclickAgent.reportError(MainApplication.geContext(), errorMsg);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void onResume() {
        super.onResume();
        MobClickCppHelper.onResume(this);
    }

    @Override
    protected void onPause() {
        super.onPause();
        MobClickCppHelper.onPause(this);
    }

    @Override
    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        // TestCpp should create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return glSurfaceView;
    }

    @Override
    protected void onLoadNativeLibraries() {
//        super.onLoadNativeLibraries();
        System.loadLibrary("iflyteknet");
        System.loadLibrary("cocos2djs");
    }
}
