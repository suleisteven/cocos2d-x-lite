package org.cocos2dx.cpp.update;

public class FileConstant {
	/**
	 * 文件头标示
	 */
	public static final String URI_FILE_HEADER = "file://";

	/**
	 * 文件名中的.
	 */
	public static final String DOT = ".";

	/**
	 * 斜杠符
	 */
	public static final String SLASH = "/";

	/**
	 * 强制更新文件名
	 */
	public static String FILE_NAME_FORCE_UPDATE = "uulandlord_force_update.apk";

	/**
	 * 更新文件名
	 */
	public static String FILE_NAME_UPDATE = "uulandlord.apk";

	/**
	 * 渠道文件名
	 */
	public static final String FILE_NAME_CHANNEL = ".channel";

	/**
	 * 下载文件时，临时后缀
	 */
	public static final String DOWNLOAD_TEMP_SUFFIX = ".download";
	/**
	 * 下载文件时,md5文件的临时后缀
	 */
	public static final String DOWNLOAD_TEMP_MD5_SUFFIX = ".md5";

	// /////////////////////////后缀START/////////////////////////////////////
	/**
	 * 图片后缀
	 */
	public static final String[] SUFFIXS_IMAGE = { "jpeg", "jpg", "png", "gif",
			"bmp", "tiff", "pcx", "tga", "exif", "fpx", "svg", "psd", "cdr",
			"pcd", "dxf", "ufo", "eps", "cal", "cur", "ico", "tif", "jpe",
			"svgz", "apng", "ai" };

	/**
	 * 音频后缀
	 */
	public static final String[] SUFFIXS_AUDIO = { "mp3", "m4a", "mid", "xmf",
			"ogg", "wma", "cd", "wav", "aiff", "au", "vqf", "amr", "flac",
			"ape", "aac", "ra", "aac+", "eaac+", "midi", "mp2", "aif", "mpega",
			"ram", "cue", "dts", "mp", "mka" };

	/**
	 * 视频后缀
	 */
	public static final String[] SUFFIXS_VIDEO = { "3gp", "mp4", "rmvb", "avi",
			"mpg", "mov", "swf", "asf", "wmv", "navi", "mkv", "flv", "f4v",
			"webm", "mpeg4", "rm", "m4v", "wmx", "wm", "mpeg", "mpeg2",
			"mpeg4", "mpga", "qt", "wmz", "wmd", "wvx", "ts", "xv", "vod",
			"mts", "m2ts", "3gpp", "3g2", "divx" };

	/**
	 * 文档后缀
	 */
	public static final String[] SUFFIXS_DOCUMENT = { "chm", "ppt", "doc",
			"txt", "pdf", "xlsx", "docx", "xls", "cpp", "c", "java", "log",
			"json", "xml", "php", "html", "wps", "pptx", "umd", "vsdx", "mpp",
			"mppx", "mpt", "accd", "xps" };

	/**
	 * excel后缀
	 */
	public static final String[] SUFFIXS_EXCEL = { "xlsx", "xls" };

	/**
	 * word后缀
	 */
	public static final String[] SUFFIXS_WORD = { "docx", "doc", "wps" };

	/**
	 * pdf 后缀
	 */
	public static final String[] SUFFIXS_PDF = { "pdf" };

	/**
	 * ppt 后缀
	 */
	public static final String[] SUFFIXS_PPT = { "ppt,pptx" };

	/**
	 * txt 后缀
	 */
	public static final String[] SUFFIXS_TXT = { "txt" };

	/**
	 * 应用打开方式文档后缀
	 */
	public static final String[] SUFFIXS_DOCUMENT_APPLICATION = { "chm", "ppt",
			"doc", "pdf", "xlsx", "docx", "xls", "wps" };

	/**
	 * 文本打开方式文档后缀
	 */
	public static final String[] SUFFIXS_DOCUMENT_TEXT = { "txt", "cpp", "c",
			"java", "log", "c", "json", "xml", "php", "html" };

	/**
	 * 压缩包后缀
	 */
	public static final String[] SUFFIXS_ZIP = { "7z", "zip", "zipx", "rar",
			"cab", "kz", "tar" };

	/**
	 * torrent文件后缀
	 */
	public static final String[] SUFFIXS_TORRENT = { "torrent" };

	/**
	 * 安装包后缀
	 */
	public static final String[] INSTALLPACKAGE_SUFFIXS = { "apk" };

	// /////////////////////////后缀END/////////////////////////////////////

	// /////////////////////////文件保存路径START/////////////////////////////////////

	/**
	 * 设置文件保存的根目录
	 * 
	 * @param rootSavePath
	 */
	public static final void setRootSavePath(String rootSavePath) {
		FILE_ROOT_SAVE_PATH = rootSavePath;
		FILE_PHOTO_SAVE_PATH = FILE_ROOT_SAVE_PATH + "/photo";
		FILE_VIDEO_SAVE_PATH = FILE_ROOT_SAVE_PATH + "/video";
		FILE_AUDIO_SAVE_PATH = FILE_ROOT_SAVE_PATH + "/audio";
		FILE_DOCUMENT_SAVE_PATH = FILE_ROOT_SAVE_PATH + "/document";
		FILE_OTHER_SAVE_PATH = FILE_ROOT_SAVE_PATH + "/other";
		FILE_CACHE_PATH = FILE_ROOT_SAVE_PATH + "/.cache";
		FILE_CACHE_ALBUM_PATH = FILE_CACHE_PATH + "/.albumCache";
		FILE_TEMP_PATH = FILE_ROOT_SAVE_PATH + "/.temp";
		FILE_RESOURCE_PATH = FILE_ROOT_SAVE_PATH + "/.res";
		ONLINE_PREVIEW_CACHE_PATH = FILE_CACHE_PATH + "/online";
		ONLINE_IMG_PREVIEW_CACHE_PATH = ONLINE_PREVIEW_CACHE_PATH + "/photo";
		ONLINE_DOC_PREVIEW_CACHE_PATH = ONLINE_PREVIEW_CACHE_PATH + "/doc";
	}

	/**
	 * 文件保存根路径
	 */
	public static String FILE_ROOT_SAVE_PATH = "/uulandlord";

	/**
	 * 照片文件保存路径
	 */
	public static String FILE_PHOTO_SAVE_PATH = FILE_ROOT_SAVE_PATH + "/photo";

	/**
	 * 视频文件保存路径
	 */
	public static String FILE_VIDEO_SAVE_PATH = FILE_ROOT_SAVE_PATH + "/video";

	/**
	 * 音频文件保存路径
	 */
	public static String FILE_AUDIO_SAVE_PATH = FILE_ROOT_SAVE_PATH + "/audio";

	/**
	 * 文档文件保存路径
	 */
	public static String FILE_DOCUMENT_SAVE_PATH = FILE_ROOT_SAVE_PATH
			+ "/document";

	/**
	 * 其他类型文件保存路径
	 */
	public static String FILE_OTHER_SAVE_PATH = FILE_ROOT_SAVE_PATH + "/other";

	/**
	 * 缓存文件目录
	 */
	public static String FILE_CACHE_PATH = FILE_ROOT_SAVE_PATH + "/.cache";

	/**
	 * 缓存在线播放专辑图片目录
	 */
	public static String FILE_CACHE_ALBUM_PATH = FILE_CACHE_PATH
			+ "/.albumCache";

	/**
	 * 文件临时存放目录,下载完毕之后移动到正式目录下
	 */
	public static String FILE_TEMP_PATH = FILE_ROOT_SAVE_PATH + "/.temp";

	/**
	 * 文件资源目录，用于存放项目的其他网络资源或本地资源
	 */
	public static String FILE_RESOURCE_PATH = FILE_ROOT_SAVE_PATH + "/.res";

	/**
	 * 远端在线预览文件存放目录
	 */
	public static String ONLINE_PREVIEW_CACHE_PATH = FILE_CACHE_PATH
			+ "/online";

	/**
	 * 远端图片在线预览文件存放目录
	 */
	public static String ONLINE_IMG_PREVIEW_CACHE_PATH = ONLINE_PREVIEW_CACHE_PATH
			+ "/photo";

	/**
	 * 远端文档在线预览文件存放目录
	 */
	public static String ONLINE_DOC_PREVIEW_CACHE_PATH = ONLINE_PREVIEW_CACHE_PATH
			+ "/doc";

	// /////////////////////////文件保存路径END/////////////////////////////////////

	// ///////////////////////// 文件命名后缀 START//
	// /////////////////////////////////////
	/**
	 * 图片后缀名 .jpeg
	 */
	public static final String JPEG = ".jpeg";

	/**
	 * 图片后缀名 .jpg
	 */
	public static final String JPG = ".jpg";

	/**
	 * 图片后缀名 .png
	 */
	public static final String PNG = ".png";

	/**
	 * 视频后缀 .mp4
	 */
	public static final String MP4 = ".mp4";

	/**
	 * 可安装应用 .apk
	 */
	public static final String APK = ".apk";

	// ///////////////////////// 文件命名后缀 END
	// /////////////////////////////////////

	// ///////////////////////// 本机文件系统数据相关参数 START
	// /////////////////////////////////////

	/**
	 * 内部照相返回intent中获取图片对象的参数
	 */
	public static final String IMAGE_EXTRAS_DATA_PARAM = "data";

	/**
	 * 视频文件返回URI 类型为 file 的开始字符串
	 */
	public static final String VIDEO_URI_START_PARAM = "file:";

	// ///////////////////////// 本机文件系统数据相关参数 END
	// /////////////////////////////////////

	// /////////////////////////文件大小单位START/////////////////////////////////////
	public static final String FILE_SIZE_UNIT_TB = "TB";

	public static final String FILE_SIZE_UNIT_GB = "GB";

	public static final String FILE_SIZE_UNIT_MB = "MB";

	public static final String FILE_SIZE_UNIT_KB = "KB";

	public static final String FILE_SIZE_UNIT_B = "B";

	// /////////////////////////文件大小单位END/////////////////////////////////////

	// /////////////////////////文件大小转换START/////////////////////////////////////
	public static final long FILE_SIZE_CONVERT_KB_TO_B = 1024;

	public static final long FILE_SIZE_CONVERT_MB_TO_B = FILE_SIZE_CONVERT_KB_TO_B * 1024;

	public static final long FILE_SIZE_CONVERT_GB_TO_B = FILE_SIZE_CONVERT_MB_TO_B * 1024;

	public static final long FILE_SIZE_CONVERT_TB_TO_B = FILE_SIZE_CONVERT_GB_TO_B * 1024;
	// /////////////////////////文件大小转换END/////////////////////////////////////

	// /////////////////////////文件分割符START/////////////////////////////////////
	public static final String FILE_SEPARATOR = "\\";
	// /////////////////////////文件分割符END/////////////////////////////////////
}