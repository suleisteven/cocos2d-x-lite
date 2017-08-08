package org.cocos2dx.cpp.update;

/**
 * <b>http操作常量类</b></br>
 * 
 * @author wangYueLin
 * @12-7-13
 * @下午3:00
 * @since xCloud1.0
 */
public class HttpConstant {
	/**
	 * UTF-8
	 */
	public static final String UTF_8 = "UTF-8";

	/**
	 * gzip
	 */
	public static final String GZIP = "gzip";

	/**
	 * http 请求超时时间间隔
	 */
	public static final int CLIENT_GET_TIMOUT = 6000;

	/**
	 * 检查更新时传递给服务器的渠道key
	 */
	public static final String KEY_UPDATE_APK_CHANNEL = "access";

	/**
	 * 获取最新版本号对应URL地址
	 */
	public static final String GET_LATEST_VERSION_URL = "http://update.xcloud.cc/xcloud/update-nan.html";

	/**
	 * 版本更新时对应的data 参数名称
	 */
	public static final String VERSION_DATA_PARAM = "data";

	/**
	 * 版本说明信息参数
	 */
	public static final String VERSION_TEXT_PARAM = "text";

	/**
	 * 版本号参数
	 */
	public static final String VERSION_CODE_PARAM = "version";

	/**
	 * apk下载链接参数
	 */
	public static final String VERSION_DOWN_PARAM = "down";

	/**
	 * 网络错误CODE值
	 */
	public static final String NETWORK_ERROR_CODE = "00001";

	/**
	 * 英文注册协议连接
	 */
	public static final String REGISTER_AGREEMENT_EN = "file:///android_asset/doc/en/copyright.html";

	/**
	 * 简体中文注册协议连接
	 */
	public static final String REGISTER_AGREEMENT_CN = "file:///android_asset/doc/zh/copyright.html";

	/**
	 * 台湾繁文注册协议连接
	 */
	public static final String REGISTER_AGREEMENT_TW = "file:///android_asset/doc/zh_TW/copyright.html";

	/**
	 * 英文了解更多请求连接
	 */
	public static final String KNOW_MORE_URL_EN = "http://www.xcloud.cc/faq_en.html";

	/**
	 * 简体中文了解更多请求连接
	 */
	public static final String KNOW_MORE_URL_CN = "http://www.xcloud.cc/faq_cn.html";

	/**
	 * 台湾繁体了解更多请求连接
	 */
	public static final String KNOW_MORE_URL_TW = "http://www.xcloud.cc/faq_tw.html";

	/**
	 * 英文设备管理下的帮组文档链接
	 */
	public static final String HELP_DOCUMENT_URL_EN = "http://www.xcloud.cc/about.html?page=4&L=en";
	/**
	 * 中文设备管理下的帮组文档链接
	 */
	public static final String HELP_DOCUMENT_URL_CN = "http://www.xcloud.cc/faq.html?L=cn";
	/**
	 * 繁体设备管理下的帮组文档链接
	 */
	public static final String HELP_DOCUMENT_URL_TW = "http://www.xcloud.cc/faq.html?L=tw";

	/**
	 * 检查强制更新链接
	 */
	public static final String CEHCK_FORCE_UPDATE_URL = "http://www.xcloud.cc/xcloud/updateNew.html";
	/**
	 * 检查强制更新参数key：类型，android传nan
	 */
	public static final String PARAMS_KEY_TYPE_CEHCK_FORCE_UPDATE = "type";
	/**
	 * 检查强制更新参数key：版本标识，例如检查3.5.0是否有强制更新内容，则传350
	 */
	public static final String PARAMS_KEY_VERSION_CEHCK_FORCE_UPDATE = "version";
	/**
	 * 检查强制更新参数key：语言类型，中文cn，英文en，繁体tw
	 */
	public static final String PARAMS_KEY_LANGUAGE_CEHCK_FORCE_UPDATE = "L";
	/**
	 * 参数值，语言类型：中文
	 */
	public static final String PARAMS_VALUE_LANGUAGE_CN = "cn";
	/**
	 * 参数值，语言类型：英文
	 */
	public static final String PARAMS_VALUE_LANGUAGE_EN = "en";
	/**
	 * 参数值，语言类型：台湾
	 */
	public static final String PARAMS_VALUE_LANGUAGE_TW = "tw";

	/**
	 * 反馈从官网渠道更新apk成功
	 */
	public static final String UPDATE_APK_REPLY_URL = "http://www.xcloud.cc/xcloud/accessStatistic.html";

	/**
	 * 更新成功反馈，参数名(客服端类型)android传nan
	 */
	public static final String PARAMS_CLIENT_TYPE = "platform";

	/**
	 * 底层通信库检查更新接口
	 */
	public static final String URL_NATIVE_LIB_UPDATE = "http://www.xcloud.cc/xcloud/update-ansup.html";

	/**
	 * 用户头像接口验证地址
	 */
	public static final String HEAD_PORTRAIT_CHECK_URL = "http://r.xcloud.cc/avatar/set.php";

	/**
	 * 用户头像上传接口地址
	 */
	public static final String HEAD_PORTRAIT_UPLOAD_URL = "http://r.xcloud.cc/avatar/set.php";

	/**
	 * 用户头像下载接口地址
	 */
	public static final String HEAD_PORTRAIT_DOWNLOAD_URL = "http://r.xcloud.cc/avatar/get.php";
}
