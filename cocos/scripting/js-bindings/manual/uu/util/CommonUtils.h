#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#define UU_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define UU_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define UU_SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define UU_BREAK_IF(cond)           if(cond) break

#include <chrono>

using namespace std::chrono;

class CommonUtils
{
public:
	template <class T> static int getArrayLen(T& array)
	{
		return (sizeof(array) / sizeof(array[0]));
	}


	/**
	* 获取时间,单位毫秒
	*/
	static unsigned long long getCurrentTimeMillis()
	{
		return  duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}
};

#endif /* defined(__COMMON_UTILS_H__) */
