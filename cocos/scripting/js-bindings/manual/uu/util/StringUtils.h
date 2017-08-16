#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <sstream>

using namespace std;
class StringUtils
{
public:
	static std::string format(const char* format, ...);



	static bool startWith(const string &str, const  string &startStr, const bool& ignoreCase = false)
	{
		if (str.length() < startStr.length())
		{
			return false;
		}
		if (ignoreCase)
		{
			string tmpStr(str);
			string tmpStartStr(startStr);
			toLower(tmpStr);
			toLower(tmpStartStr);
			return tmpStartStr.compare(tmpStr.substr(0, tmpStartStr.length())) == 0;
		}
		else
		{
			return startStr.compare(str.substr(0, startStr.length())) == 0;
		}

	}

	static bool endWith(const string &str, const  string &endStr, const bool& ignoreCase = false)
	{
		if (str.length() < endStr.length())
		{
			return false;
		}
		if (ignoreCase)
		{
			string tmpStr(str);
			string tmpEndStr(endStr);
			toLower(tmpStr);
			toLower(tmpEndStr);
			return tmpEndStr.compare(tmpStr.substr(tmpStr.length() - tmpEndStr.length(), tmpStr.length() - 1)) == 0;
		}
		else
		{
			return endStr.compare(str.substr(endStr.length(), str.length())) == 0;
		}
	}

	/**
	* 将字符串转换为大写
	*/
	static void toUpper(string &str)
	{
		transform(str.begin(), str.end(), str.begin(), ::toupper);
	}

	/**
	* 将字符串转换为小写
	*/
	static void toLower(string &str)
	{
		transform(str.begin(), str.end(), str.begin(), ::tolower);
	}



	static std::string trim(const string& str)
	{
		string::size_type pos = str.find_first_not_of(' ');
		if (pos == string::npos)
		{
			return str;
		}
		string::size_type pos2 = str.find_last_not_of(' ');
		if (pos2 != string::npos)
		{
			return str.substr(pos, pos2 - pos + 1);
		}
		return str.substr(pos);
	}

	/**
	* 拆分字符串
	* @params s 待拆分的字符串
	* @params delim 以该字符进行拆分
	* @params elems 已有的字符串集合，拆分完毕后将添加到此集合中，最后返回
	*/
	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}


	/**
	* 拆分字符串
	* @params s 待拆分的字符串
	* @params delim 以该字符进行拆分
	*/
	static std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}
	//isFloat taken from http://stackoverflow.com/questions/447206/c-isfloat-function
	static bool isFloat(std::string myString) {
		std::istringstream iss(myString);
		float f;
		iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
		// Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	}

	static bool isInt(std::string myString) {
		std::istringstream iss(myString);
		int i;
		iss >> std::noskipws >> i;
		return iss.eof() && !iss.fail();
	}

	/**
	* 替换所有目标字符串
	*/
	static string replaceAll(std::string & s, std::string const & t, std::string const & w)
	{
		string::size_type pos = s.find(t), t_size = t.size(), r_size = w.size();
		while (pos != std::string::npos){ // found
			s.replace(pos, t_size, w);
			pos = s.find(t, pos + r_size);
		}
		return s;
	}

	/**
	* 替换第一个目标字符串
	*/
	static string replaceFirst(std::string & s, std::string const & t, std::string const & w)
	{
		string::size_type pos = s.find(t), t_size = t.size();
		while (pos != std::string::npos){ // found
			s.replace(pos, t_size, w);
			break;
		}
		return s;
	}

	/**
	* 查找指定字符串
	* @return 查找成功返回字符串所在下标，未找到返回-1
	*/
	static int findStr(std::string & s, std::string const & t)
	{
		string::size_type pos = s.find(t);
		while (pos != std::string::npos){ // found

			return pos;
		}
		return -1;
	}

	/**
	* 从字符串中提取数字
	*/
	static std::string getNumberFromString(const std::string& s)
	{
		string result = "";
		for (int i = 0; i < s.length(); ++i)
		{
			char c = s.at(i);

			if (c >= '0' && c <= '9')
			{
				result.append(StringUtils::format("%c", c));
			}
		}
		return result;
	}

	/**
	* 解析url中的参数
	*/
	static map<string, string> urlParamsParse(const string url)
	{
		int i = 0;

		int curStep = 0;
		string curKey;
		string curValue;

		map<string, string> allParams;
		int urlLength = url.length();
		if (urlLength > 0)
		{
			while (true)
			{
				string curChar = url.substr(i, 1);

				if (curStep == 0) // 解析？
				{
					if (curChar == "?")
					{
						curStep = 1;
					}
				}
				else if (curStep == 1) // 解析key
				{
					if (curChar == "=")
					{
						curStep = 2;
					}
					else
					{
						curKey.append(curChar);
					}
				}
				else if (curStep == 2) // 解析value
				{
					if (curChar == "&")
					{
						curStep = 1;

						allParams.insert(std::make_pair(curKey, curValue));
						curKey.clear();
						curValue.clear();
					}
					else
					{
						curValue.append(curChar);
					}
				}

				if (urlLength == ++i)
				{
					if (!curKey.empty())
					{
						allParams.insert(std::make_pair(curKey, curValue));
						curKey.clear();
						curValue.clear();
					}
					break;
				}
			}
		}

		return allParams;
	}
};

#endif /* defined(__STRING_UTILS_H__) */
