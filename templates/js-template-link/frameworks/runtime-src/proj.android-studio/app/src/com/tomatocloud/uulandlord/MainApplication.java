package com.tomatocloud.uulandlord;

import android.content.Context;
import android.content.res.Configuration;

import com.google.gson.Gson;
import com.iflytek.app.BaseApplication;

import java.io.IOException;
import java.io.InputStream;

public class MainApplication extends BaseApplication {

    static class Config {
        String channel;
    }

    public static String channel = "";

    private static Context _context;
    public static final Context geContext()
    {
        return _context;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        _context = this;
    }

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
//        MultiDex.install(this);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        // TODO Auto-generated method stub
        super.onConfigurationChanged(newConfig);

    }

    @Override
    public void onLowMemory() {
        // TODO Auto-generated method stub
        super.onLowMemory();
    }

    private String getChannelId() {
        InputStream is = null;
        try {
            is = getAssets().open("res/raw-assets/resources/config.json");
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);

            Config config = new Gson().fromJson(new String(buffer), Config.class);

            channel = config.channel;
            return channel;
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (is != null) {
                try {
                    is.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return "default";
    }
}
