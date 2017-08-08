package org.cocos2dx.cpp.update;

/**
 * <b>手动更新后回调监听器</b></br>
 * {@link com.diting.xcloud.services.impl.VersionUpdater#showDefaultDialog(android.content.Context, OnUpdateEndListener)}
 * {@link com.diting.xcloud.services.impl.VersionUpdater#showUpdateDialog(android.content.Context, OnUpdateEndListener)}
 * 
 * @author wangYueLin
 * @12-7-20
 * @下午3:59
 * @since xCloud1.0
 */
public interface OnUpdateEndListener {
	/**
	 * 手动更新完成回调方法
	 * 
	 * @param isSuccess
	 *            更新是否成功
	 */
	void onUpdateEnd(boolean isSuccess);
}
