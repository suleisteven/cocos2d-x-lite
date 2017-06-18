#include "BaseModel.h"

BaseModel::BaseModel():_userData(nullptr)
{
}

BaseModel::~BaseModel()
{
}

void* BaseModel::getUserData()
{
	return this->_userData;
}

const void* BaseModel::getUserData() const
{
	return this->_userData;
}


void BaseModel::setUserData(void* userData)
{
	this->_userData = userData;
}

void BaseModel::serIntData(const int32_t& intData)
{
	this->_intData = intData;
}

int BaseModel::getIntData()
{
	return this->_intData;
}
