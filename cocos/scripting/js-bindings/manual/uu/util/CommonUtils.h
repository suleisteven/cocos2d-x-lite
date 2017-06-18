#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#define UU_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define UU_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define UU_SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define UU_BREAK_IF(cond)           if(cond) break


class CommonUtils
{
public:
	template <class T> static int getArrayLen(T& array)
	{
		return (sizeof(array) / sizeof(array[0]));
	}
};

#endif /* defined(__COMMON_UTILS_H__) */
