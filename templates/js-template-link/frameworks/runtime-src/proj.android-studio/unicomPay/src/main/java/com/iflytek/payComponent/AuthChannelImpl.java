package com.iflytek.payComponent;

import com.iflytek.unipay.AuthCallBack;
import com.iflytek.unipay.AuthChannel;
import com.iflytek.unipay.AuthResultEntity;
import com.sdk.commplatform.Commplatform;
import com.sdk.commplatform.entry.AuthResult;
import com.sdk.commplatform.entry.ErrorCode;
import com.sdk.commplatform.listener.CallbackListener;

/**
 * Created by Administrator on 2017/2/6.
 */

public class AuthChannelImpl implements AuthChannel {

    @Override
    public void checkAuthPermission(String productId, final AuthCallBack callBack) {
        final AuthResultEntity authResultEntity = new AuthResultEntity();

        Commplatform.getInstance().authPermission(productId, "2", productId, new CallbackListener<AuthResult>() {
            @Override
            public void callback(int i, AuthResult authResult) {
                if (i == ErrorCode.COM_PLATFORM_SUCCESS) {
                    authResultEntity.result = true;
                } else {
                    authResultEntity.result = false;
                }
                if (authResult != null) {
                    if (authResult.description != null) {
                        authResultEntity.description = authResult.description;
                    }
                    if (authResult.expiredTime != null) {
                        authResultEntity.expiredTime = authResult.expiredTime;
                    }
                    if (authResult.isTest != null) {
                        authResultEntity.isTest = authResult.isTest;
                    }
                }
                if(callBack != null){
                    callBack.checkReuslt(authResultEntity);
                }
            }
        });
    }
}
