#ifndef __BASE_MODEL_H__
#define __BASE_MODEL_H__

#include <stdint.h>

/**
 * 数据模型基类,所有模型都继承自此类
 */
class BaseModel
{
public:
	BaseModel();
    virtual ~BaseModel() = 0;

	/**
	 * 设置用户自定义数据
	 */
	virtual void setUserData(void* userData);

	/**
	 * 获取用户自定义数据
	 */
    virtual void* getUserData();

	virtual const void* getUserData() const;

	virtual void serIntData(const int32_t& intData);

	virtual int getIntData();
private:
	void* _userData;
	int32_t _intData;
};
#endif /* defined(__BASE_MODEL_H__) */
