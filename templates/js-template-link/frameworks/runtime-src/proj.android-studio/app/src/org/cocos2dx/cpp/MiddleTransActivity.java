package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;

/**
 * Created by Administrator on 2017-4-15.
 */
public class MiddleTransActivity extends Activity
{
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        gotoMainActivity();
    }

    private void gotoMainActivity()
    {
        Intent intent = new Intent();
        intent.setComponent(new ComponentName(this.getPackageName(), AppActivity.class.getName()));
        startActivity(intent);
        System.out.println("pay goto game2");
        this.finish();
    }

}
