package org.cocos2dx.cpp.update;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

import android.os.Environment;
import android.os.StatFs;
import android.text.TextUtils;

/**
 * <b>文件辅助类</b></br>
 * 
 * @author Steven
 * @2012-7-20
 * @下午01:36:19
 * @since xCloud 1.0
 */
public class FileUtil

{

	/**
	 * SD卡是否存在
	 * 
	 * @return
	 */
	public static boolean isExistSDCard() {
		return Environment.getExternalStorageState().equals(
				android.os.Environment.MEDIA_MOUNTED);
	}

	/**
	 * 转化字节流
	 */
	public static byte[] streamToBytes(InputStream is) {
		ByteArrayOutputStream os = new ByteArrayOutputStream(1024);
		byte[] buffer = new byte[1024];
		int len;
		try {
			while ((len = is.read(buffer)) >= 0) {
				os.write(buffer, 0, len);
			}
		} catch (java.io.IOException e) {
		}
		return os.toByteArray();
	}

	/**
	 * 格式化文件路径
	 * 
	 * @param inputFilePath
	 * @return
	 */
	public static String formatFilePath(String inputFilePath) {
		if (null == inputFilePath) {
			return null;
		}

		// 将一个或多个“\”转化成“/”
		inputFilePath = inputFilePath.replaceAll("\\\\{1,}", "\\\\");

		// 将多个“/”转化成一个“/”
		inputFilePath = inputFilePath.replaceAll("\\/{1,}", "\\\\");
		return inputFilePath;
	}

	/**
	 * 格式化路径
	 * 
	 * @param inputFilePath
	 * @param separator
	 *            路径分隔符
	 * @return
	 */
	public static String formatFilePath(String inputFilePath, String separator) {

		if (null == inputFilePath) {
			return null;
		}

		if (null == separator) {
			separator = "\\\\";
		}
		inputFilePath = inputFilePath.replaceAll("\\\\{1,}", separator);
		inputFilePath = inputFilePath.replaceAll("\\/{1,}", separator);

		return inputFilePath;
	}

	/**
	 * 替换路径中所有非法字符
	 * 
	 * @param filePath
	 * @param separator
	 * @return
	 */
	public static String replaceInvalidSignFilePath(String filePath,
			String separator) {
		if (null == filePath) {
			return null;
		}

		if (null == separator) {
			separator = "_";
		}
		filePath = filePath.replaceAll("[:*><?\"]", separator);
		return filePath;
	}

	/**
	 * 从绝对路径获取文件名
	 * 
	 * @param absPath
	 * @return
	 */
	public static String getFileName(String absPath) {
		if (!TextUtils.isEmpty(absPath)) {
			if (absPath.endsWith(File.separator)) {
				String temp = absPath.substring(0,
						absPath.lastIndexOf(File.separator));
				absPath = temp.substring(temp.lastIndexOf(File.separator) + 1,
						temp.length());
			} else {
				absPath = absPath.substring(
						absPath.lastIndexOf(File.separator) + 1,
						absPath.length());
			}
		}

		return absPath;
	}

	/**
	 * 从绝对路径中获取目录路径
	 * 
	 * @param absPath
	 * @return
	 */
	public static String getCatalogPath(String absPath) {
		if (!TextUtils.isEmpty(absPath)) {
			absPath = formatFilePath(absPath, File.separator);
			absPath = absPath.substring(0,
					absPath.lastIndexOf(File.separator) + 1);
		}
		return absPath;
	}

	/**
	 * 获取SD卡的路径
	 * 
	 * @return
	 */
	public static String getSDCardPath() {
		if (isAvaiableSDCard()) {
			return Environment.getExternalStorageDirectory().getAbsolutePath();
		}
		return null;
	}

	/**
	 * 判断SD卡是否可用
	 * 
	 * @return
	 */
	public static boolean isAvaiableSDCard() {
		return android.os.Environment.getExternalStorageState().equals(
				android.os.Environment.MEDIA_MOUNTED);
	}

	/**
	 * 判断SD卡空间是否大于指定大小
	 * 
	 * @param fileSize
	 *            指定的大小，单位字节
	 * @return
	 */
	public static boolean isAvaiableSpace(long fileSize) {
		if (isAvaiableSDCard()) {
			String sdcard = Environment.getExternalStorageDirectory()
					.getAbsolutePath();
			StatFs statFs = new StatFs(sdcard);
			long blockSize = statFs.getBlockSize();
			long blocks = statFs.getAvailableBlocks();
			long availableSpare = blocks * blockSize;
			if (fileSize > availableSpare) {
				return false;
			} else {
				return true;
			}
		}
		return false;
	}

	/**
	 * 获取sd卡的可用空间
	 * 
	 * @return
	 */
	public static long getSDCardAvaiableSize() {
		long availableSpareSize = 0;
		if (isAvaiableSDCard()) {
			try {

				String sdcard = Environment.getExternalStorageDirectory()
						.getAbsolutePath();
				StatFs statFs = new StatFs(sdcard);
				long blockSize = statFs.getBlockSize();
				long blocks = statFs.getAvailableBlocks();
				availableSpareSize = blocks * blockSize;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return availableSpareSize;
	}

	/**
	 * 删除指定目录下的所有文件，不会删除指定目录
	 * 
	 * @param absPath
	 * @return
	 */
	public static boolean delAllFile(String absPath) {
		boolean flag = false;

		try {

			File file = new File(absPath);
			if (!file.exists()) {
				return flag;
			}
			if (!file.isDirectory()) {
				return flag;
			}
			String[] tempList = file.list();
			File temp = null;
			for (int i = 0; i < tempList.length; i++) {
				if (absPath.endsWith(File.separator)) {
					temp = new File(absPath + tempList[i]);
				} else {
					temp = new File(absPath + File.separator + tempList[i]);
				}
				if (temp.isDirectory()) {
					boolean result = delAllFile(absPath + "/" + tempList[i]);// 先删除文件夹里面的文件
					delFolder(absPath + "/" + tempList[i]);// 再删除空文件夹
					if (!result) {
						flag = false;
						return flag;
					}
				} else if (temp.isFile()) {
					boolean result = temp.delete();
					if (!result) {
						flag = false;
						return flag;
					}
				}
			}
			flag = true;
		} catch (Exception e) {
			e.printStackTrace();
			flag = false;
		}
		return flag;
	}

	/**
	 * 删除指定目录下的所有文件并且删除指定目录
	 * 
	 * @param folderPath
	 */
	public static void delFolder(String folderPath) {
		try {
			delAllFile(folderPath); // 删除完里面所有内容
			String filePath = folderPath;
			filePath = filePath.toString();
			java.io.File myFilePath = new java.io.File(filePath);
			myFilePath.delete(); // 删除空文件夹
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 复制单个文件
	 * 
	 * @param oldPath
	 *            String 原文件路径
	 * @param newPath
	 *            String 复制后路径
	 * @param isCover
	 *            如果复制后的文件已经存在，是否进行覆盖
	 * @return boolean
	 */
	public static boolean copyFile(String oldPath, String newPath,
			boolean isCover) {
		if (TextUtils.isEmpty(oldPath) || TextUtils.isEmpty(newPath)) {
			return false;
		}

		// 格式化路径
		oldPath = formatFilePath(oldPath, File.separator);
		newPath = formatFilePath(newPath, File.separator);

		File oldFile = new File(oldPath);

		// 源文件不存在
		if (!oldFile.exists()) {
			return false;
		}

		// 源文件和新文件路径相同
		if (oldPath.equals(newPath)) {
			return true;
		}

		File newFile = new File(newPath);

		// 复制后的文件已经存在，并且不要覆盖
		if (newFile.exists() && !isCover) {
			return true;
		}

		// 获取复制后的目录路径
		String newFileDirPath = getCatalogPath(newPath);
		File newFileDir = new File(newFileDirPath);

		// 复制后的目录路径不存在，则先创建
		if (!newFileDir.exists()) {
			newFileDir.mkdirs();
		}

		FileInputStream fis = null;
		FileOutputStream fos = null;

		try {
			fis = new FileInputStream(oldFile);
			fos = new FileOutputStream(newPath);

			// 复制文件
			byte[] buffer = new byte[1024 * 1024];
			int length = 0;
			while ((length = fis.read(buffer)) != -1) {
				fos.write(buffer, 0, length);
			}
			return true;
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (null != fis) {
				try {
					fis.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}

			if (null != fos) {
				try {
					fos.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return false;

	}

	/**
	 * 获取该目录(包括子目录)下的所有文件
	 * 
	 * @param path
	 * @return
	 */
	public static List<File> getFiles(String path) {
		if (TextUtils.isEmpty(path)) {
			return null;
		}

		File file = new File(path);

		List<File> fileList = new ArrayList<File>();

		if (!file.exists() || !file.isDirectory() || file.isHidden()) {
			return null;
		}

		File[] files = file.listFiles();

		fileList = scanThisFiles(files, fileList);

		return fileList;
	}

	private static List<File> scanThisFiles(File[] files, List<File> fileList) {
		if (files == null || files.length == 0 || fileList == null) {
			return fileList;
		}

		for (File tempFile : files) {
			if (tempFile.isDirectory() && !tempFile.isHidden()) {
				File[] tempFiles = tempFile.listFiles();
				scanThisFiles(tempFiles, fileList);
			} else {
				fileList.add(tempFile);
			}
		}

		return fileList;
	}

	/**
	 * 创建文件，如果此文件不存在
	 * 
	 * @param absFilePath
	 * @return
	 */
	public static boolean createFileIfNotExist(String absFilePath) {
		if (TextUtils.isEmpty(absFilePath)) {
			return false;
		}
		return createFileIfNotExist(new File(absFilePath));
	}

	public static boolean createFileIfNotExist(File absFile) {
		if (null != absFile) {
			try {
				if (absFile.exists()) {
					return true;
				}

				String newFileDirPath = getCatalogPath(absFile
						.getAbsolutePath());
				File newFileDir = new File(newFileDirPath);

				// 复制后的目录路径不存在，则先创建
				if (!newFileDir.exists()) {
					newFileDir.mkdirs();
				}

				return absFile.createNewFile();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return false;
	}

	/**
	 * 保存单行数据到本地文件
	 * 
	 * @param absFilePath
	 * @param singleLine
	 */
	public static boolean writeSingleLineToFile(String absFilePath,
			String singleLine) {
		if (TextUtils.isEmpty(absFilePath) || TextUtils.isEmpty(singleLine)) {
			return false;
		}
		File file = new File(absFilePath);
		DataOutputStream dos = null;
		RandomAccessFile raf = null;
		try {
			createFileIfNotExist(file);
			raf = new RandomAccessFile(file, "rw");

			// 清空文件
			raf.setLength(0);
			raf.write(singleLine.getBytes());
			return true;
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (null != dos) {
				try {
					dos.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return false;
	}

	/**
	 * 读取本地文件的单行数据
	 * 
	 * @param absFilePath
	 * @return
	 */
	public static String readSingleLineFromFile(String absFilePath) {
		File file = new File(absFilePath);
		String savedSingleLine = null;
		if (file.exists()) {
			DataInputStream dis = null;
			try {
				dis = new DataInputStream(new FileInputStream(file));
				savedSingleLine = dis.readLine();
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (null != dis) {
					try {
						dis.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}
		return savedSingleLine;
	}

	/**
	 * 保存二进制数据到文件,只能保存较少数据
	 * 
	 * @param absFilePath
	 * @param singleLine
	 */
	public static boolean writeSimpleDataToFile(String absFilePath, byte[] data) {
		if (TextUtils.isEmpty(absFilePath) || null == data) {
			return false;
		}
		File file = new File(absFilePath);
		DataOutputStream dos = null;
		RandomAccessFile raf = null;
		try {
			createFileIfNotExist(file);
			raf = new RandomAccessFile(file, "rw");

			// 清空文件
			raf.setLength(0);
			raf.write(data);
			return true;
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (null != dos) {
				try {
					dos.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return false;
	}

	/**
	 * 读取二进制数据到内存,只能处理较少数据
	 * 
	 * @param absFilePath
	 * @return
	 */
	public static byte[] readSimpleDataFromFile(String absFilePath) {
		File file = new File(absFilePath);
		byte[] result = null;
		if (file.exists()) {
			DataInputStream dis = null;
			try {
				dis = new DataInputStream(new FileInputStream(file));
				result = new byte[(int) file.length()];
				dis.read(result);
			} catch (Exception e) {
				e.printStackTrace();
			} finally {
				if (null != dis) {
					try {
						dis.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}
		return result;
	}

	/**
	 * 是否是合法的文件名
	 * 
	 * @param fileName
	 * @return
	 */
	public static boolean isValidFileName(String fileName) {
		if (TextUtils.isEmpty(fileName)) {
			return false;
		} else {
			return Pattern.matches("^[^<>|/\\*?:\"]+$", fileName);
		}
	}

	/**
	 * 根据文件名获得对应MIME类型
	 * 
	 * @param fileName
	 * @return
	 */
	public static String getMIMEType(String fileName) {
		if (null == fileName) {
			fileName = "";
		}
		return getMIMEType(new File(fileName));
	}

	/**
	 * 根据文件后缀名获得对应的MIME类型
	 * 
	 * @param file
	 */
	public static String getMIMEType(File file) {
		String type = "*/*";

		if (null == file) {
			return type;
		}
		String fName = file.getName();
		// 获取后缀名前的分隔符"."在fName中的位置。
		int dotIndex = fName.lastIndexOf(".");
		if (dotIndex < 0) {
			return type;
		}
		/* 获取文件的后缀名 */
		String end = fName.substring(dotIndex, fName.length()).toLowerCase();
		if (end == "")
			return type;
		// 在MIME和文件类型的匹配表中找到对应的MIME类型。
		for (int i = 0; i < MIME_MapTable.length; i++) {
			if (end.equals(MIME_MapTable[i][0]))
				type = MIME_MapTable[i][1];
		}
		return type;
	}

	/**
	 * 文件MIME类型表
	 **/
	private static final String[][] MIME_MapTable = {
			// {后缀名， MIME类型}
			{ ".3gp", "video/3gpp" },
			{ ".apk", "application/vnd.android.package-archive" },
			{ ".asf", "video/x-ms-asf" }, { ".avi", "video/x-msvideo" },
			{ ".bin", "application/octet-stream" }, { ".bmp", "image/bmp" },
			{ ".c", "text/plain" }, { ".class", "application/octet-stream" },
			{ ".conf", "text/plain" }, { ".cpp", "text/plain" },
			{ ".doc", "application/msword" },
			{ ".exe", "application/octet-stream" }, { ".gif", "image/gif" },
			{ ".gtar", "application/x-gtar" }, { ".gz", "application/x-gzip" },
			{ ".h", "text/plain" }, { ".htm", "text/html" },
			{ ".html", "text/html" }, { ".jar", "application/java-archive" },
			{ ".java", "text/plain" }, { ".jpeg", "image/jpeg" },
			{ ".jpg", "image/jpeg" }, { ".js", "application/x-javascript" },
			{ ".log", "text/plain" }, { ".m3u", "audio/x-mpegurl" },
			{ ".m4a", "audio/mp4a-latm" }, { ".m4b", "audio/mp4a-latm" },
			{ ".m4p", "audio/mp4a-latm" }, { ".m4u", "video/vnd.mpegurl" },
			{ ".m4v", "video/x-m4v" }, { ".mov", "video/quicktime" },
			{ ".mp2", "audio/x-mpeg" }, { ".mp3", "audio/x-mpeg" },
			{ ".mp4", "video/mp4" },
			{ ".mpc", "application/vnd.mpohun.certificate" },
			{ ".mpe", "video/mpeg" }, { ".mpeg", "video/mpeg" },
			{ ".mpg", "video/mpeg" }, { ".mpg4", "video/mp4" },
			{ ".mpga", "audio/mpeg" },
			{ ".msg", "application/vnd.ms-outlook" }, { ".ogg", "audio/ogg" },
			{ ".pdf", "application/pdf" }, { ".png", "image/png" },
			{ ".pps", "application/vnd.ms-powerpoint" },
			{ ".ppt", "application/vnd.ms-powerpoint" },
			{ ".prop", "text/plain" }, { ".rar", "application/rar" },
			{ ".rc", "text/plain" }, { ".rmvb", "audio/x-pn-realaudio" },
			{ ".rtf", "application/rtf" }, { ".sh", "text/plain" },
			{ ".tar", "application/x-tar" },
			{ ".tgz", "application/x-compressed" }, { ".txt", "text/plain" },
			{ ".wav", "audio/x-wav" }, { ".wma", "audio/x-ms-wma" },
			{ ".wmv", "audio/x-ms-wmv" },
			{ ".wps", "application/vnd.ms-works" }, { ".xml", "text/plain" },
			{ ".z", "application/x-compress" }, { ".zip", "application/zip" },
			{ "", "*/*" } };

}
