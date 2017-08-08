package org.cocos2dx.cpp.update;

import android.text.TextUtils;
import android.webkit.URLUtil;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.RandomAccessFile;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.UUID;
import java.util.regex.Pattern;
import java.util.zip.GZIPInputStream;

/**
 * @author: wylll
 * @version: [v0.10, 2012-4-28]
 * @create time: 上午11:08:16,2012-4-28
 * @see:
 * @since: [product/module-version]
 * @describe: HTTP 请求操作工具类
 */
public class HttpUtil {

	/**
	 * http get请求
	 * 
	 * @param addressStr
	 * @param params
	 * @return
	 * @throws Exception
	 */
	public static String httpGet(String addressStr, Map<String, String> params)
			throws Exception {
		if (addressStr == null) {
			return null;
		}

		String result = "";

		if (params != null && !params.isEmpty()) {
			Set<String> keys = params.keySet();
			String content = "";
			int count = 0;

			for (String key : keys) {
				if (count == 0) {
					content += URLEncoder.encode(key, HttpConstant.UTF_8)
							+ "="
							+ URLEncoder.encode(params.get(key).toString(),
									HttpConstant.UTF_8);
				} else {
					content += "&"
							+ URLEncoder.encode(key, HttpConstant.UTF_8)
							+ "="
							+ URLEncoder.encode(params.get(key).toString(),
									HttpConstant.UTF_8);
				}

				count++;
			}

			addressStr += "?" + content;
		}

		HttpURLConnection conn = null;

		InputStream is = null;
		try {

			URL url = new URL(addressStr);
			conn = (HttpURLConnection) url.openConnection();
			conn.setRequestProperty("Charset", HttpConstant.UTF_8);
			conn.setConnectTimeout(HttpConstant.CLIENT_GET_TIMOUT);

			if (conn.getResponseCode() != 200) {
				throw new RuntimeException("请求url失败");
			}

			is = conn.getInputStream();

			// 得到网络返回的输入流
			if (HttpConstant.GZIP.equals(conn.getContentEncoding())) {
				result = readDataForZgip(is, HttpConstant.UTF_8);
			} else {
				result = readData(is, HttpConstant.UTF_8);
			}

		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (null != is) {
				try {
					is.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}

			if (null != conn) {
				conn.disconnect();
			}
		}

		return result;
	}

	/**
	 * HttpURLConnection 发送post请求方式
	 * 
	 * @param postUrl
	 * @param params
	 *            请求参数
	 * @return
	 */
	public static String httpPost(String postUrl, Map<String, String> params) {
		if (TextUtils.isEmpty(postUrl) || params == null || params.isEmpty()) {
			return null;
		}

		String httpUrl = postUrl;
		String resultData = "";
		URL url = null;
		try {
			// 创建一个URL对象
			url = new URL(httpUrl);
		} catch (MalformedURLException e) {
			e.printStackTrace();
		}

		// 声明HttpURLConnection对象
		HttpURLConnection urlConn = null;

		// 声明DataOutputStream流
		DataOutputStream out = null;

		if (url != null) {
			try {
				// 使用HttpURLConnection打开连接
				urlConn = (HttpURLConnection) url.openConnection();

				// 因为这个是POST请求所以要设置为true
				urlConn.setDoInput(true);
				urlConn.setDoOutput(true);

				// 设置POST方式
				urlConn.setRequestMethod("POST");

				// POST请求不能设置缓存
				urlConn.setUseCaches(false);
				urlConn.setInstanceFollowRedirects(false);

				// 设置维持长连接
				urlConn.setRequestProperty("Connection", "Keep-Alive");
				// 设置文件字符集
				urlConn.setRequestProperty("Charset", "UTF-8");

				// 配置本次连接的Content-type，配置为application/x-www-form-urlencoded的
				urlConn.setRequestProperty("Content-Type",
						"application/x-www-form-urlencoded");

				// 连接，从postUrl.openConnection()至此的配置必须要在connect之前完成
				// 要注意的是connectio.getOutputStream会隐含的进行connect
				urlConn.connect();

				// DataOutputStream流
				out = new DataOutputStream(urlConn.getOutputStream());

				// 要传的参数
				Set<String> keys = params.keySet();
				String content = "";
				int count = 0;

				for (String key : keys) {
					if (count == 0) {
						content += URLEncoder.encode(key, HttpConstant.UTF_8)
								+ "="
								+ URLEncoder.encode(params.get(key),
										HttpConstant.UTF_8);
					} else {
						content += "&"
								+ URLEncoder.encode(key, HttpConstant.UTF_8)
								+ "="
								+ URLEncoder.encode(params.get(key),
										HttpConstant.UTF_8);
					}

					count++;
				}

				// 将要上传的内容写入流中
				out.writeBytes(content);

				InputStream is = urlConn.getInputStream();

				// 得到网络返回的输入流
				if (HttpConstant.GZIP.equals(urlConn.getContentEncoding())) {
					resultData = readDataForZgip(is, HttpConstant.UTF_8);
				} else {
					resultData = readData(is, HttpConstant.UTF_8);
				}

			} catch (IOException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				try {
					if (null != out) {

						// 刷新DataOutputStream流
						out.flush();
						// 关闭DataOutputStream流
						out.close();
					}

					if (null != urlConn) {

						// 关闭URL连接
						urlConn.disconnect();
					}

				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		return resultData;
	}


	/**
	 * @param inSream
	 *            输入流
	 * @param charsetName
	 *            字符集编码
	 * @return
	 * @throws Exception
	 */
	private static String readData(InputStream inSream, String charsetName)
			throws IOException {
		ByteArrayOutputStream outStream = new ByteArrayOutputStream();
		byte[] buffer = new byte[1024];
		int len = -1;
		while ((len = inSream.read(buffer)) != -1) {
			outStream.write(buffer, 0, len);
		}
		byte[] data = outStream.toByteArray();
		outStream.close();
		inSream.close();
		return new String(data, charsetName);
	}

	/**
	 * @param inStream
	 *            输入流
	 * @param charsetName
	 *            字符集编码
	 * @return
	 * @throws Exception
	 */
	private static String readDataForZgip(InputStream inStream,
			String charsetName) throws IOException {
		GZIPInputStream gzipStream = new GZIPInputStream(inStream);
		ByteArrayOutputStream outStream = new ByteArrayOutputStream();
		byte[] buffer = new byte[1024];
		int len = -1;
		while ((len = gzipStream.read(buffer)) != -1) {
			outStream.write(buffer, 0, len);
		}
		byte[] data = outStream.toByteArray();
		outStream.close();
		gzipStream.close();
		inStream.close();
		return new String(data, charsetName);

	}

	/**
	 * android上传文件到服务器
	 * 
	 * @param file
	 *            需要上传的文件
	 * @param requestURL
	 *            请求的rul
	 * @return 返回服务返回的结果
	 */
	public static String uploadFile(File file, String requestURL) {
		String BOUNDARY = UUID.randomUUID().toString(); // 边界标识 随机生成
		String PREFIX = "--", LINE_END = "\r\n";
		String CONTENT_TYPE = "multipart/form-data"; // 内容类型
		String CHARSET = "utf-8"; // 设置编码
		int TIME_OUT = 10 * 10000000; // 超时时间

		try {
			URL url = new URL(requestURL);
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			conn.setReadTimeout(TIME_OUT);
			conn.setConnectTimeout(TIME_OUT);
			conn.setDoInput(true); // 允许输入流
			conn.setDoOutput(true); // 允许输出流
			conn.setUseCaches(false); // 不允许使用缓存
			conn.setRequestMethod("POST"); // 请求方式
			conn.setRequestProperty("Charset", CHARSET); // 设置编码
			conn.setRequestProperty("connection", "keep-alive");
			conn.setRequestProperty("Content-Type", CONTENT_TYPE + ";boundary="
					+ BOUNDARY);
			if (file != null) {
				/**
				 * 当文件不为空，把文件包装并且上传
				 */
				OutputStream outputSteam = conn.getOutputStream();

				DataOutputStream dos = new DataOutputStream(outputSteam);
				StringBuffer sb = new StringBuffer();
				sb.append(PREFIX);
				sb.append(BOUNDARY);
				sb.append(LINE_END);
				/**
				 * 这里重点注意： name里面的值为服务器端需要key 只有这个key 才可以得到对应的文件
				 * filename是文件的名字，包含后缀名的 比如:abc.png
				 */

				sb.append("Content-Disposition: form-data; name=\"img\"; filename=\""
						+ file.getName() + "\"" + LINE_END);
				sb.append("Content-Type: application/octet-stream; charset="
						+ CHARSET + LINE_END);
				sb.append(LINE_END);
				dos.write(sb.toString().getBytes());
				InputStream is = new FileInputStream(file);
				byte[] bytes = new byte[1024];
				int len = 0;
				while ((len = is.read(bytes)) != -1) {
					dos.write(bytes, 0, len);
				}
				is.close();
				dos.write(LINE_END.getBytes());
				byte[] end_data = (PREFIX + BOUNDARY + PREFIX + LINE_END)
						.getBytes();
				dos.write(end_data);
				dos.flush();
				/**
				 * 获取响应码 200=成功 当响应成功，获取响应的流
				 */
				int res = conn.getResponseCode();

				if (res == 200) {

					String encoding = conn.getContentEncoding();
					is = conn.getInputStream();
					int read = -1;
					ByteArrayOutputStream baos = new ByteArrayOutputStream();
					while ((read = is.read()) != -1) {
						baos.write(read);
					}
					byte[] data = baos.toByteArray();
					baos.close();

					String content = null;
					if (encoding != null) {
						content = new String(data, encoding);
					} else {
						content = new String(data);
					}

					return content;
				}
			}
		} catch (MalformedURLException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * <b>Http文件下载监听器</b></br>
	 * 
	 * @author Steven
	 * @2012-11-23
	 * @下午1:34:59
	 * @since XCloud 2.2.1
	 */
	public interface OnDownloadFileListener {
		/**
		 * 文件下载开始
		 * 
		 * @param downloadUrl
		 *            下载文件的URL
		 * @param downloadFile
		 *            下载完成的文件
		 */
		void onDownloadFileStart(String downloadUrl, File downloadFile,
				long fileSize);

		/**
		 * 文件下载完成
		 * 
		 * @param downloadUrl
		 *            下载文件的URL
		 * @param downloadFile
		 *            下载完成的文件
		 */
		void onDownloadFileFinish(String downloadUrl, File downloadFile);

		/**
		 * 文件正在下载
		 * 
		 * @param downloadUrl
		 *            下载文件的URL
		 * @param fileSize
		 *            下载文件总大小
		 * @param downloadedSize
		 *            已下载文件大小
		 */
		void onDownloadingFile(String downloadUrl, long fileSize,
				long downloadedSize);

		/**
		 * 下载文件停止
		 * 
		 * @param downloadUrl
		 *            下载文件的URL
		 * @param downloadFile
		 *            下载的文件对象
		 * @param downloadedSize
		 *            已完成的下载大小
		 */
		void onDownloadFileStop(String downloadUrl, File downloadFile,
				long downloadedSize);

		/**
		 * 下载文件出错
		 * 
		 * @param downloadUrl
		 *            下载文件的URL
		 */
		void onDownloadError(String downloadUrl);
	}

	private static Map<String, HttpDownloadThread> downloadThreadMap = new HashMap<String, HttpDownloadThread>();

	/**
	 * 异步下载文件
	 * 
	 * @param downloadUrl
	 *            下载文件URL
	 * @param saveAbsPath
	 *            下载文件保存的绝对路径
	 * @param isCover
	 *            是否覆盖
	 * @param isResumeIfExist
	 *            如果文件存在，是否续传,前提是isCover为false
	 * @param onDownloadFileListener
	 *            下载文件监听器
	 */
	public static void downloadFileAsyn(final String downloadUrl,
			String saveAbsPath, final boolean isCover, boolean isResumeIfExist,
			final OnDownloadFileListener onDownloadFileListener) {

		saveAbsPath = FileUtil.formatFilePath(saveAbsPath, File.separator);

		boolean isUseXL = false;

		// 如果使用迅雷加速下载失败，再使用普通任务进行下载
		if (!isUseXL) {

			String key = downloadUrl + saveAbsPath;
			// 不能重复开启同一个任务线程
			if (downloadThreadMap.containsKey(key)
					&& downloadThreadMap.get(key).isAlive()) {
				return;
			}
			HttpDownloadThread downloadThread = new HttpDownloadThread(
					downloadUrl, saveAbsPath, isCover, isResumeIfExist,
					onDownloadFileListener);
			downloadThread.start();
			downloadThreadMap.put(key, downloadThread);
		}
	}

	/**
	 * 停止正在下载的任务
	 * 
	 * @param downloadUrl
	 * @return
	 */
	public static void stopDownloadFile(final String downloadUrl,
			String saveAbsPath) {
		stopDownloadFile(downloadUrl, saveAbsPath, 0);
	}

	/**
	 * 停止正在下载的任务
	 * 
	 * @param downloadUrl
	 * @param saveAbsPath
	 * @param joinTime
	 */
	public static void stopDownloadFile(final String downloadUrl,
			String saveAbsPath, final long joinTime) {

		// 再查找普通任务并停止
		saveAbsPath = FileUtil.formatFilePath(saveAbsPath, File.separator);
		String key = downloadUrl + saveAbsPath;
		if (downloadThreadMap.containsKey(key)) {
			HttpDownloadThread downloadThread = downloadThreadMap.get(key);
			if (null != downloadThread && downloadThread.isAlive()) {
				downloadThread.stopDownload();
				if (joinTime > 0) {

					try {
						downloadThread.join(joinTime);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		}
	}

	/**
	 * <b>Http下载线程</b></br>
	 * 
	 * @author Steven
	 * @2012-11-23
	 * @下午3:07:44
	 * @since XCloud 2.2.1
	 */
	private static class HttpDownloadThread extends Thread {

		private String downloadUrl, saveAbsPath;

		private boolean isCover;

		private boolean isResumeIfExist;

		private OnDownloadFileListener onDownloadFileListener;

		private boolean isStop = false;

		public HttpDownloadThread(final String downloadUrl,
				final String saveAbsPath, final boolean isCover,
				final boolean isResumeIfExist,
				final OnDownloadFileListener onDownloadFileListener) {
			this.downloadUrl = downloadUrl;
			this.saveAbsPath = saveAbsPath;
			this.isCover = isCover;
			this.isResumeIfExist = isResumeIfExist;
			this.onDownloadFileListener = onDownloadFileListener;
		}

		/**
		 * 停止下载
		 */
		public void stopDownload() {
			isStop = true;
		}

		@Override
		public void run() {

			if (TextUtils.isEmpty(downloadUrl)
					|| TextUtils.isEmpty(saveAbsPath)) {
				if (null != onDownloadFileListener) {
					onDownloadFileListener.onDownloadError(downloadUrl);
				}
				return;
			}

			HttpURLConnection conn = null;

			// FileOutputStream fos = null;

			InputStream is = null;

			RandomAccessFile raf = null;

			if (URLUtil.isNetworkUrl(downloadUrl.trim())) {
				try {
					File fileTemp = new File(saveAbsPath);
					long existSize = 0;

					// 是否可以续传
					boolean isCanResume = false;
					if (fileTemp.exists() && !isCover && isResumeIfExist) {
						isCanResume = true;
						existSize = fileTemp.length();
					}

					URL myUrl = new URL(downloadUrl);
					conn = (HttpURLConnection) myUrl.openConnection();

					conn.setRequestMethod("GET");
					// 设置 User-Agent
					conn.setRequestProperty("User-Agent", "NetFox");
					conn.setConnectTimeout(HttpConstant.CLIENT_GET_TIMOUT);
					if (isCanResume) {

						// 设置续传断点
						conn.setRequestProperty("RANGE", "bytes=" + existSize
								+ "-");
					}
					int responseCode = conn.getResponseCode();

					if (responseCode != 200 && responseCode != 206) {
						if (null != onDownloadFileListener) {
							onDownloadFileListener.onDownloadError(downloadUrl);
						}
						return;
					}

					if (responseCode == 206) {
						isCanResume = true;
						existSize = fileTemp.length();
					} else {
						isCanResume = false;
						existSize = 0;
					}

					is = conn.getInputStream();

					File file = new File(saveAbsPath);
					File parentFile = file.getParentFile();

					// 创建目录
					if (!parentFile.exists()) {
						parentFile.mkdirs();
					}

					// 创建文件
					if (!file.exists()) {
						String dirPath = FileUtil.getCatalogPath(saveAbsPath);
						File dir = new File(dirPath);
						dir.mkdirs();
						file.createNewFile();
					} else {
						if (isCover) {

							// 删除原有文件
							boolean isDeleteSuccess = file.delete();

							// 删除失败，文件被占用
							if (!isDeleteSuccess) {
								if (null != onDownloadFileListener) {
									onDownloadFileListener
											.onDownloadError(downloadUrl);
								}

								return;
							}
						}
					}

					// 文件总长度
					long contentLength = conn.getContentLength();

					raf = new RandomAccessFile(saveAbsPath, "rw");

					if (isCanResume) {
						raf.seek(existSize);
					} else {

						// 清空文件
						raf.setLength(0);
					}

					// 文件开始下载
					if (null != onDownloadFileListener) {
						onDownloadFileListener.onDownloadFileStart(downloadUrl,
								file, contentLength + existSize);
					}

					byte[] buff = new byte[1024 * 10];

					// 当前已下载长度
					long curTotal = 0;
					if (isCanResume) {
						curTotal += existSize;
					}

					int length;
					while ((length = is.read(buff)) != -1) {
						raf.write(buff, 0, length);
						curTotal += length;

						// 回调文件正在下载
						if (null != onDownloadFileListener) {
							onDownloadFileListener.onDownloadingFile(
									downloadUrl, contentLength + existSize,
									curTotal);
						}

						if (isStop) {

							// 文件下载停止
							if (null != onDownloadFileListener) {
								onDownloadFileListener.onDownloadFileStop(
										downloadUrl, file, curTotal);
							}
							break;
						}
					}

					// 下载完成
					if (curTotal == contentLength + existSize) {

						if (null != onDownloadFileListener) {
							onDownloadFileListener.onDownloadFileFinish(
									downloadUrl, file);
						}
						return;
					} else {
						if (!isStop && null != onDownloadFileListener) {
							onDownloadFileListener.onDownloadError(downloadUrl);
						}
						return;
					}
				} catch (IOException e) {
					e.printStackTrace();

					// 下载出错
					if (null != onDownloadFileListener) {
						onDownloadFileListener.onDownloadError(downloadUrl);
					}
				} finally {
					try {
						if (null != is) {
							is.close();
						}
						// if (null != fos)
						// {
						// fos.close();
						// }

						if (null != raf) {
							raf.close();
						}
					} catch (IOException e) {
						e.printStackTrace();
					}

					// 删除线程引用
					downloadThreadMap.remove(downloadUrl);
				}
			}

		}
	}

	/**
	 * 同步下载文件
	 * 
	 * @param downloadUrl
	 *            下载文件URL
	 * @param saveAbsPath
	 *            下载文件保存的绝对路径
	 * @param
	 * @return
	 */
	public static boolean downloadFile(String downloadUrl, String saveAbsPath,
			boolean isCover) {
		return downloadFile(downloadUrl, saveAbsPath, isCover, false);
	}

	/**
	 * 同步下载文件
	 * 
	 * @param downloadUrl
	 *            下载文件URL
	 * @param saveAbsPath
	 *            下载文件保存的绝对路径
	 * @param isCover
	 *            如果文件存在，是否覆盖原文件
	 * @param isResumeIfExist
	 *            如果文件存在，是否续传，当isCover为true时，此项无效
	 * @return
	 */
	public static boolean downloadFile(String downloadUrl, String saveAbsPath,
			boolean isCover, boolean isResumeIfExist) {
		if (TextUtils.isEmpty(downloadUrl) || TextUtils.isEmpty(saveAbsPath)) {
			return false;
		}

		HttpURLConnection conn = null;

		InputStream is = null;

		RandomAccessFile raf = null;

		if (URLUtil.isNetworkUrl(downloadUrl.trim())) {
			try {
				File fileTemp = new File(saveAbsPath);
				long existSize = 0;

				// 是否可以续传
				boolean isCanResume = false;
				if (fileTemp.exists() && !isCover && isResumeIfExist) {
					isCanResume = true;
					existSize = fileTemp.length();
				}

				URL myUrl = new URL(downloadUrl);
				conn = (HttpURLConnection) myUrl.openConnection();

				conn.setRequestMethod("GET");
				// 设置 User-Agent
				conn.setRequestProperty("User-Agent", "NetFox");
				conn.setConnectTimeout(HttpConstant.CLIENT_GET_TIMOUT);
				if (isCanResume) {

					// 设置续传断点
					conn.setRequestProperty("RANGE", "bytes=" + existSize + "-");
				}

				int responseCode = conn.getResponseCode();

				if (responseCode != 200 && responseCode != 206) {
					return false;
				}

				if (responseCode == 206) {
					isCanResume = true;
					existSize = fileTemp.length();
				} else {
					isCanResume = false;
					existSize = 0;
				}

				is = conn.getInputStream();

				File file = new File(saveAbsPath);
				File parentFile = file.getParentFile();

				// 创建目录
				if (!parentFile.exists()) {
					parentFile.mkdirs();
				}

				// 创建文件
				if (!file.exists()) {
					String dirPath = FileUtil.getCatalogPath(saveAbsPath);
					File dir = new File(dirPath);
					dir.mkdirs();
					file.createNewFile();
				} else {
					if (isCover) {

						// 删除原有文件
						boolean isDeleteSuccess = file.delete();

						// 删除失败，文件被占用
						if (!isDeleteSuccess) {
							return false;
						}
					}
				}

				// 剩余文件总长度
				long contentLength = conn.getContentLength();

				raf = new RandomAccessFile(saveAbsPath, "rw");

				if (isCanResume) {
					raf.seek(existSize);
				} else {

					// 清空文件
					raf.setLength(0);
				}

				byte[] buff = new byte[1024 * 10];

				// 当前已下载长度
				long curTotal = 0;
				if (isCanResume) {
					curTotal += existSize;
				}

				int length;
				while ((length = is.read(buff)) != -1) {
					raf.write(buff, 0, length);
					curTotal += length;
				}

				// 下载完成
				if (curTotal == contentLength + existSize) {
					return true;
				} else {
					return false;
				}
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				try {
					if (null != is) {
						is.close();
					}

					if (null != raf) {
						raf.close();
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		return false;
	}

	/**
	 * 获取网络文件的大小
	 * 
	 * @param url
	 * @return
	 */
	public static long getNetFileSize(String url) {
		long result = -1;
		if (TextUtils.isEmpty(url)) {
			return result;
		}

		HttpURLConnection conn = null;

		if (URLUtil.isNetworkUrl(url.trim())) {
			try {
				URL myUrl = new URL(url);
				conn = (HttpURLConnection) myUrl.openConnection();
				conn.setConnectTimeout(HttpConstant.CLIENT_GET_TIMOUT);

				if (conn.getResponseCode() != 200) {
					return result;
				}
				result = conn.getContentLength();
				conn.disconnect();
				return result;

			} catch (IOException e) {
				e.printStackTrace();

			}
		}
		return result;
	}

	/**
	 * 去掉url中的路径，留下请求参数部分
	 * 
	 * @param strURL
	 *            url地址
	 * @return url请求参数部分
	 */
	private static String truncateUrlPath(String strURL) {
		String strAllParam = null;
		String[] arrSplit = null;

		strURL = strURL.trim().toLowerCase();

		arrSplit = strURL.split("[?]");
		if (strURL.length() > 1) {
			if (arrSplit.length > 1) {
				if (arrSplit[1] != null) {
					strAllParam = arrSplit[1];
				}
			}
		}

		return strAllParam;
	}

	/**
	 * 解析出url参数中的键值对 如 "index.jsp?Action=del&id=123"，解析出Action:del,id:123存入map中
	 * 
	 * @param url
	 *            url地址
	 * @return url请求参数部分
	 */
	public static Map<String, String> parseURLParams(String url) {
		Map<String, String> mapRequest = new HashMap<String, String>();

		String[] arrSplit = null;

		String strUrlParam = truncateUrlPath(url);
		if (strUrlParam == null) {
			return mapRequest;
		}
		// 每个键值为一组
		arrSplit = strUrlParam.split("[&]");
		for (String strSplit : arrSplit) {
			String[] arrSplitEqual = null;
			arrSplitEqual = strSplit.split("[=]");

			// 解析出键值
			if (arrSplitEqual.length > 1) {
				// 正确解析
				mapRequest.put(arrSplitEqual[0], arrSplitEqual[1]);

			} else {

				// 只有参数没有值，不加入
				if (arrSplitEqual[0] != "") {
					mapRequest.put(arrSplitEqual[0], "");
				}
			}
		}
		return mapRequest;
	}

	/**
	 * 将普通url编码，主要处理中文、空格的转换
	 * 
	 * @param url
	 * @return
	 */
	public static String encodeURL(String url) {
		if (TextUtils.isEmpty(url)) {
			return "";
		}

		String playPath = "";
		String path = url;

		int index = -1;
		String str = "";
		while (true) {
			int lastIndex = index + 1;
			index = path.indexOf("\\", lastIndex);
			if (index == -1)
				index = path.length();
			str = path.substring(lastIndex, index);
			boolean pattern = Pattern.matches("[\u4e00-\u9fa5]+", str);
			if (str.indexOf("%") != -1 || str.indexOf(":") != -1
					|| str.indexOf(" ") != -1 || pattern) {
				try {
					if (TextUtils.isEmpty(playPath)) {
						playPath += URLEncoder.encode(str, "UTF-8");
					} else {

						playPath += "/" + URLEncoder.encode(str, "UTF-8");
					}
				} catch (UnsupportedEncodingException e) {
					e.printStackTrace();
				}
			} else {
				if (TextUtils.isEmpty(playPath)) {
					playPath += str;
				} else {
					playPath += "/" + str;

				}
			}
			if (index == path.length())
				break;
		}
		return path;
	}

}
