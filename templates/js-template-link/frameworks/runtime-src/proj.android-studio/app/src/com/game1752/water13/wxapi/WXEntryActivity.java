
package com.game1752.water13.wxapi;

import android.content.Intent;
import android.os.Bundle;

import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.umeng.socialize.weixin.view.WXCallbackActivity;

public class WXEntryActivity extends WXCallbackActivity {

//    private IWXAPI api;
//
//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        // TODO Auto-generated method stub
//        super.onCreate(savedInstanceState);
//
//        System.out.println("aaaaaaaaaaaabbbbb:onCreate:");
//
//        if(api==null){
//            //wx30a9402cd2a87cf3
//            api= WXAPIFactory.createWXAPI(this, "wxc1651208a490959d", false);
//            api.registerApp("wxc1651208a490959d");
//        }
//
//        api.handleIntent(getIntent(), this);
//        handleIntent(getIntent());
//    }
//    private void handleIntent(Intent intent) {
//        SendAuth.Resp resp = new SendAuth.Resp(intent.getExtras());
//        if (resp.errCode == BaseResp.ErrCode.ERR_OK) {
//            //用户同意
//        }
//    }
//    @Override
//    public void onReq(BaseReq req) {
//        super.onReq(req);
//        System.out.println("aaaaaaaaaaaabbbbb:onReq:" + req);
//    }
//
//    @Override
//    public void onResp(BaseResp resp) {
//        super.onResp(resp);
//        System.out.println("aaaaaaaaaaaabbbbb:onResp:" + resp);
//    }



}
