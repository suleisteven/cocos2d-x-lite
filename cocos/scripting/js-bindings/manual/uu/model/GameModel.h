/********************************************************************
	created:	2017/06/15
	created:	15:6:2017   21:48
	file base:	GameModel
	file ext:	h
	author:		sulei
	purpose:	
*********************************************************************/
#ifndef __GAME_MODEL_CPP_H__
#define __GAME_MODEL_CPP_H__

#include "BaseModel.h"
#include "../util/StringUtils.h"
#include "../util/CommonUtils.h"
#include <algorithm>
#include <vector>
#include <stdint.h>
#include <memory>

using namespace std;

class RuleManagerAbstract;
class SimpleRuleManager;

/**
* ��������
*/
enum SortType : uint32_t
{
	/**
	* ����
	*/
	SORT_TYPE_DESC = 1,
	/**
	* ����
	*/
	SORT_TYPE_ASC
};

/************************************************************************/
/* �����˿�����                                                                     */
/************************************************************************/
enum SortPokerType : uint32_t
{
	/**
	 * �˿�ֵ��������
	 */
	SORT_POKER_TYPE_VALUE_FIRST = 1,
	/**
	 * �˿˻�ɫ��������
	 */
	SORT_POKER_TYPE_MARK_FIRST = 2,
};

/************************************************************************/
/* ��Ϸ��������                                                                     */
/************************************************************************/
enum RuleType : int32_t
{
	RULE_TYPE_SIMPLE, // �����淨
	RULE_TYPE_MUCH_THREE, // �������淨
	RULE_TYPE_AMAZING, // �ٱ��淨
};


/**
* �˿���ֵ����
*/
enum PokerValueType : int32_t
{
	// ����2��A��С���������
	VALUE_TYPE_NONE = 0,
	VALUE_TYPE_2=2,
	VALUE_TYPE_3,
	VALUE_TYPE_4,
	VALUE_TYPE_5,
	VALUE_TYPE_6,
	VALUE_TYPE_7,
	VALUE_TYPE_8,
	VALUE_TYPE_9,
	VALUE_TYPE_10,
	VALUE_TYPE_J,
	VALUE_TYPE_Q,
	VALUE_TYPE_K,
	VALUE_TYPE_A,
	VALUE_TYPE_KING_SMALL,
	VALUE_TYPE_KING_BIG,
	VALUE_TYPE_SUBSTITUTE
};


/**
* �˿�ֵ��С
*/
enum PokerValueTypeOfValue : int32_t
{
	// ����2��A��С���������
	VALUE_TYPE_OF_VALUE_NONE = 0,
	VALUE_TYPE_OF_VALUE_2 = 2,
	VALUE_TYPE_OF_VALUE_3 = 3,
	VALUE_TYPE_OF_VALUE_4 = 4,
	VALUE_TYPE_OF_VALUE_5 = 5,
	VALUE_TYPE_OF_VALUE_6 = 6,
	VALUE_TYPE_OF_VALUE_7 = 7,
	VALUE_TYPE_OF_VALUE_8 = 8,
	VALUE_TYPE_OF_VALUE_9 = 9,
	VALUE_TYPE_OF_VALUE_10 = 10,
	VALUE_TYPE_OF_VALUE_J = 11,
	VALUE_TYPE_OF_VALUE_Q = 12,
	VALUE_TYPE_OF_VALUE_K = 13,
	VALUE_TYPE_OF_VALUE_A = 14,
	VALUE_TYPE_OF_VALUE_KING_SMALL = 17,
	VALUE_TYPE_OF_VALUE_KING_BIG = 18,
	VALUE_TYPE_OF_VALUE_SUBSTITUTE = 19
};


/**
* �˿˻�ɫ����
*/
enum PokerMarkType : int32_t
{
	/**
	* �޻�ɫ,���磺���С������
	*/
	MARK_TYPE_NONE = 0,
	/**
	* �췽
	*/
	MARK_TYPE_RED_SQUARE = 1,
	/**
	* ÷��
	*/
	MARK_TYPE_PLUM_BLOSSOM = 2,
	/**
	* ����
	*/
	MARK_TYPE_RED_PEACH = 3,
	/**
	* ����
	*/
	MARK_TYPE_BLACK_PEACH = 4,
};


/**
* �˿�����ģ��
*/
class PokerModel : public BaseModel
{
public:
	
	/**
	* ͨ���˿����͡���ɫ��ԭʼֵ��С����һ���˿�model
	* @param pokerValueType �˿�����
	* @param pokerMarkType �˿˻�ɫ����
	* @param originalValue �˿�ԭʼֵ��С
	* @param sortValue �˿�����ֵ,Խ������Խ��ǰ
	*/
	PokerModel(PokerValueType pokerValueType, PokerMarkType pokerMarkType, int32_t originalValue, int32_t sortValue);
	~PokerModel();

	/**
	* ��¡һ���˿�����ģ��
	*/
	virtual shared_ptr<PokerModel> clone();
	/**
	* �����˿�
	* @param pokerModelVector �������˿˼���
	* @param sortType ��������
	* @param sortPokerType �˿��������ȼ�����
	* @param sortSubstitute �Ƿ�����������
	*/
	static void sortPokerModel(vector<shared_ptr<PokerModel>>& pokerModelVector, const SortType& sortType, const SortPokerType& sortPokerType = SortPokerType::SORT_POKER_TYPE_VALUE_FIRST, const bool& sortSubstitute = true);

	/**
	* ��������˿ˣ�����ͬ���ƽ��з��飩,��������һ����Ԫ��
	*/
	static void sortPokerModelWithGroup(vector<shared_ptr<vector<shared_ptr<PokerModel>>>>& pokerModelGroupVector, const SortType& sortType, const bool& sortSubstitute = true);

	/**
	* �����˿�
	* @param pokerModelVector �����ҵ��˿˼���
	* @param pokerValueType �����˿˵�ֵ����
	* @param pokerMarkType �����˿˵Ļ�ɫ����
	* @param isCheckSubstitute �Ƿ������õı仯ֵ��false-����飨��ʱ����������Ʊ仯�����ͣ���true-��飨��ʱ�����ȱȽ����ͣ�����ԱȲ��ϣ��ٶԱ����ñ仯��ֵ�����磺�����ѱ�ɺ���4����ʱ���Һ���4���ܹ����ظ������ƣ�
	* @return null-û���ҵ������򷵻��˿˶���ָ��
	*/
	static shared_ptr<PokerModel> findPoker(const vector<shared_ptr<PokerModel>>& pokerModelVector, const PokerValueType &pokerValueType, const PokerMarkType &pokerMarkType, const bool& isCheckSubstituteChangeValue = false);

	/**
	* �����˿ˣ������˿˻�ɫ����
	* @param pokerModelVector �����ҵ��˿˼���
	* @param pokerValueType �����˿˵�ֵ����
	* @param isCheckSubstitute �Ƿ������õı仯ֵ��false-����飨��ʱ����������Ʊ仯�����ͣ���true-��飨��ʱ�����ȱȽ����ͣ�����ԱȲ��ϣ��ٶԱ����ñ仯��ֵ�����磺�����ѱ�ɺ���4����ʱ���Һ���4���ܹ����ظ������ƣ�
	* @return null-û���ҵ������򷵻��ҵ��ĵ�һ���˿˶���ָ��
	*/
	static shared_ptr<PokerModel> findPoker(const vector<shared_ptr<PokerModel>>& pokerModelVector, const PokerValueType &pokerValueType, const bool& isCheckSubstituteChangeValue = false);

	/**
	* ����ĳ���Ƶ�����
	*/
	static int32_t findCountByPokerValue(const vector<shared_ptr<PokerModel>>& pokerModelVector, const PokerValueType& pokerValueType);

	/**
	* ���Ƽ������ų�ָ���Ƽ���
	* @param pokerVector
	* @param beRemovePokerVector ��Ҫ���Ƴ������ͼ���
	*/
	static void removeSpecifyPokerVector(vector<shared_ptr<PokerModel>>& pokerVector, const vector<shared_ptr<PokerModel>>& beRemovePokerVector);

	/**
	* �ϲ�ָ���Ƽ��ϵ��Ƽ����У������ظ�
	* @param pokerVector
	* @param beMergePokerVector ��Ҫ���ϲ������ͼ���
	*/
	static void mergeSpecifyPokerVector(vector<shared_ptr<PokerModel>>& pokerVector, const vector<shared_ptr<PokerModel>>& beMergePokerVector);

	/**
	 * ������������,�������ظ�
	 * @param pokerVector
	 * @param beConcatPokerVector ��Ҫ���ϲ������ͼ���
	 */
	static void concatSpecifyPokerVector(vector<shared_ptr<PokerModel>>& pokerVector, const vector<shared_ptr<PokerModel>>& beConcatPokerVector);

	/**
	* ��ȡ�˿�����
	*/
	PokerValueType getPokerValueType();
	/**
	* ��ȡ�˿˻�ɫ����
	*/
	PokerMarkType getPokerMarkType();

	/**
	* ��ȡ�˿�ԭʼֵ��С
	*/
	int32_t getOriginalValue();
	/**
	* ��ȡ����ֵ��С
	*/
	int32_t getSortValue();

	/**
	* �仯�˿ˣ������ƲŻ���Ч
	*/
	void setChange(const PokerValueType& changeToPokerValueType, const PokerMarkType& changeToPokerMarkType, const int32_t& changeToValue, const int32_t& changeToSortValue);
	/**
	* ��ձ仯
	*/
	void clearChanged();
	/**
	* ��ȡ�˿˱仯���ֵ��С
	*/
	int32_t getChangedValue();
	/**
	* ��ȡ�˿˱仯�������ֵ��С
	*/
	int32_t getChangedSortValue();
	/**
	* ��ȡ�˿˱仯�������
	*/
	PokerValueType getChangedPokerValueType();
	/**
	* ��ȡ�˿˱仯��Ļ�ɫ����
	*/
	PokerMarkType getChangedPokerMarkType();

	/**
	* �˿��Ƿ��ѷ������仯��ֻ�������Ʋ������˱仯
	*/
	bool isChanged();

	static string toString(vector<shared_ptr<PokerModel>> pokerVector)
	{
		if (pokerVector.empty())
		{
			return "0";
		}
		else
		{
			string result;
			for (shared_ptr<PokerModel> poker : pokerVector)
			{
				result.append(StringUtils::format("%d-%d,", poker->getPokerValueType(), poker->getPokerMarkType()));
			}
			return result;
		}

	}
private:
	PokerValueType _pokerValueType;
	PokerMarkType _pokerMarkType;
	int32_t _originalValue;
	int32_t _sortValue;

	PokerValueType _changedPokerValueType;
	PokerMarkType _changedPokerMarkType;
	int32_t _changedValue;
	int32_t _changedSortValue;
	bool _isChanged;
};



/************************************************************************/
/* �����˿˻�ɫ���ͺ�ֵ���Ͳ����˿����ݽṹ����ʹ��std::find_ifʱʹ��                                                                     */
/************************************************************************/
class PokerFindByValueAndType {
public:

	PokerFindByValueAndType(PokerValueType pokerValueType, PokerMarkType pokerMarkType, bool isCheckSubstituteChangeValue) :_pokerValueType(pokerValueType), _pokerMarkType(pokerMarkType), _isCheckSubstituteChangeValue(isCheckSubstituteChangeValue)
	{
	}

	bool operator () (PokerModel& poker) {

		if (!_isCheckSubstituteChangeValue)
		{
			return poker.getPokerValueType() == _pokerValueType && poker.getPokerMarkType() == _pokerMarkType;
		}
		else
		{
			return (poker.getPokerValueType() == _pokerValueType && poker.getPokerMarkType() == _pokerMarkType) || (poker.getChangedPokerValueType() == _pokerValueType && poker.getChangedPokerMarkType() == _pokerMarkType);
		}

	}

	bool operator () (shared_ptr<PokerModel> poker) {
		if (!_isCheckSubstituteChangeValue)
		{
			return poker->getPokerValueType() == _pokerValueType && poker->getPokerMarkType() == _pokerMarkType;
		}
		else
		{
			return (poker->getPokerValueType() == _pokerValueType && poker->getPokerMarkType() == _pokerMarkType) || (poker->getChangedPokerValueType() == _pokerValueType && poker->getChangedPokerMarkType() == _pokerMarkType);
		}
	}

private:
	PokerValueType _pokerValueType;
	PokerMarkType _pokerMarkType;
	bool _isCheckSubstituteChangeValue;
};

/************************************************************************/
/* �����˿�ֵ���Ͳ����˿����ݽṹ����ʹ��std::find_ifʱʹ��                                                                     */
/************************************************************************/
class PokerFindByValue {
public:

	PokerFindByValue(PokerValueType pokerValueType, bool isCheckSubstituteChangeValue) :_pokerValueType(pokerValueType), _isCheckSubstituteChangeValue(isCheckSubstituteChangeValue)
	{
	}

	bool operator () (PokerModel& poker) {

		if (!_isCheckSubstituteChangeValue)
		{
			return poker.getPokerValueType() == _pokerValueType;
		}
		else
		{
			return poker.getPokerValueType() == _pokerValueType || poker.getChangedPokerValueType() == _pokerValueType;
		}
	}

	bool operator () (shared_ptr<PokerModel> poker) {
		if (!_isCheckSubstituteChangeValue)
		{
			return poker->getPokerValueType() == _pokerValueType;
		}
		else
		{
			return poker->getPokerValueType() == _pokerValueType || poker->getChangedPokerValueType() == _pokerValueType;
		}
	}

private:
	PokerValueType _pokerValueType;
	bool _isCheckSubstituteChangeValue;
};

/**
* �˿˹�����ģ��
*/
class PokerBuildModel : public BaseModel
{
public:

	/**
	* ͨ���˿����͡���ɫ��ԭʼֵ��С����һ���˿�model
	* @param pokerValueType �˿�����
	* @param pokerMarkType �˿˻�ɫ����
	* @param originalValue �˿�ԭʼֵ��С
	* @param sortValue �˿�����ֵ
	*/
	PokerBuildModel(PokerValueType pokerValueType, PokerMarkType pokerMarkType, int32_t originalValue, int32_t sortValue);

	PokerValueType getPokerValueType() const;
	PokerMarkType getPokerMarkType() const;

	int32_t getOriginalValue() const;
	int32_t getSortValue() const;
private:

	PokerValueType _pokerValueType;
	PokerMarkType _pokerMarkType;
	int32_t _originalValue;
	int32_t _sortValue;
};

/*
* �����˿˻�ɫ���ͺ�ֵ���Ͳ����˿˹��������ݽṹ����ʹ��std::find_ifʱʹ��
*/
class PokerBuildModelFindByValueAndType
{
public:

	PokerBuildModelFindByValueAndType(PokerValueType pokerValueType, PokerMarkType pokerMarkType) :_pokerValueType(pokerValueType), _pokerMarkType(pokerMarkType)
	{
	}

	bool operator () (const PokerBuildModel& pokerBuildModel)
	{
		return pokerBuildModel.getPokerValueType() == _pokerValueType && pokerBuildModel.getPokerMarkType() == _pokerMarkType;
	}

	bool operator () (const PokerBuildModel* pokerBuildModel)
	{
		return pokerBuildModel->getPokerValueType() == _pokerValueType && pokerBuildModel->getPokerMarkType() == _pokerMarkType;
	}

private:
	const PokerValueType _pokerValueType;
	const PokerMarkType _pokerMarkType;
};


/************************************************************************/
/* ����                                                                     */
/************************************************************************/
enum PokerCombinationType : uint32_t
{
	/**
	* δ֪����
	*/
	POKER_COMBINATION_TYPE_NONE = 0,

	// ��������
	/**
	* ������3/5�ŵ��ţ���3+7+9+J+K��
	*/
	POKER_COMBINATION_TYPE_WU_LONG,
	/**
	* ���ӣ�һ������+3�ŵ��ţ���77+3+5+9��
	*/
	POKER_COMBINATION_TYPE_DUI_ZI,
	/**
	* ���ԣ���������+���ţ���77+99+3��
	*/
	POKER_COMBINATION_TYPE_LIANG_DUI,
	/**
	* ������3����ͬ������+2�ŵ����Ҳ��ڵ�һ������333+79��
	*/
	POKER_COMBINATION_TYPE_SAN_TIAO,
	/**
	 * ������3����ͬ�������Ʋ����ڵ�һ�����磺555��
	 */
	POKER_COMBINATION_TYPE_CHONG_SAN,
	/**
	* ˳�ӣ����ŵ���ɫ��ͬ��3���ƣ������4+����5+÷��6����
	*/
	POKER_COMBINATION_TYPE_SHUN_ZI,
	/**
	* ͬ������ͬ��ɫ��3/5���ƣ������358��
	*/
	POKER_COMBINATION_TYPE_TONG_HUA,
	/**
	* ��«��3����ͬ������+�������ڵ���������333+88��
	*/
	POKER_COMBINATION_TYPE_HU_LU,
	/**
	 * �жպ�«��3����ͬ������+���Ӳ����ڵڶ�������333+88��
	 */
	POKER_COMBINATION_TYPE_ZHONG_DUN_HU_LU,
	
	/**
	 * ��֧��4����ͬ������ + 1�ŵ������ڵ���������AAAA+3��
	 */
	POKER_COMBINATION_TYPE_TIE_ZHI,
	/**
	* �ж���֧��4����ͬ������ + 1�ŵ������ڵڶ�������AAAA+3��
	*/
	POKER_COMBINATION_TYPE_ZHONG_DUN_TIE_ZHI,
	/**
	* ͬ��˳����ͬ��ɫ�����ŵ�3/5�������ڵ��������� ����56789��
	*/
	POKER_COMBINATION_TYPE_TONG_HUA_SHUN,
	/**
	 * �ж�ͬ��˳����ͬ��ɫ�����ŵ�3/5�������ڵڶ������� ����56789��
	 */
	POKER_COMBINATION_TYPE_ZHONG_DUN_TONG_HUA_SHUN,
	// ��������
	/**
	* ��ͬ����ͷ��Ϊ3����ͬ��ɫ���ƣ��С�β��Ϊͬ����
	*/
	POKER_COMBINATION_TYPE_SAN_TONG_HUA,
	/**
	* ��˳�ӣ�ͷ��Ϊ3���������ƣ��С�β��Ϊ˳�ӣ�
	*/
	POKER_COMBINATION_TYPE_SAN_SHUN_ZI,
	/**
	* ���԰루13������6�����+1�ŵ�����ɣ�
	*/
	POKER_COMBINATION_TYPE_LIU_DUI_BAN,
	/**
	* ��������� 13������5������+1��������3����ͬ�������ƣ���ɣ�
	*/
	POKER_COMBINATION_TYPE_WU_DUI_SAN_TIAO,
	/**
	* ����������13������4��������3����ͬ�������ƣ�+1�ŵ�����ɣ�
	*/
	POKER_COMBINATION_TYPE_SI_TAO_SAN_TIAO,
	/**
	* ��һɫ��13���ƻ�ɫ��Ϊ��ɫ�����ҡ����飩���ɫ��÷�������ң���
	*/
	POKER_COMBINATION_TYPE_COU_YI_SE,
	/**
	* ȫС��13�ž���2-8���ƣ����ۻ�ɫ��
	*/
	POKER_COMBINATION_TYPE_QUAN_XIAO,
	/**
	* ȫ��13�ž���8-A���ƣ����ۻ�ɫ��
	*/
	POKER_COMBINATION_TYPE_QUAN_DA,
	/**
	* �������£�3����֧������ͬ�����������ƣ�+1�ŵ��ƣ�
	*/
	POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA,
	/**
	* ��ͬ��˳����һ�������Ż�ɫ��ͬ���������ƣ��ڶ�������Ϊͬ��˳��
	*/
	POKER_COMBINATION_TYPE_SAN_TONG_HUA_SHUN,
	/**
	* ʮ�����壨ʮ�����ƾ���AKQJ��
	*/
	POKER_COMBINATION_TYPE_SHI_ER_HUANG_ZU,
	/**
	* һ��������ͬ��ɫ���Ӵ�С��һ�ţ������Ϊ����˳�ӣ���
	*/
	POKER_COMBINATION_TYPE_YI_TIAO_LONG,
	/**
	* ������ͬһ�ֻ�ɫ���Ӵ�С��һ�ţ��������AKQJ 10 98765432�������Ϊ����ͬ��˳����
	*/
	POKER_COMBINATION_TYPE_QING_LONG,
};

/**
* ��������ģ��
*/
class PokerCombinationModel : public BaseModel
{
public:

	static shared_ptr<PokerCombinationModel> clone(shared_ptr<PokerCombinationModel> pokerCombinationModel);

	/**
	* ��������,���ص�һ���ҵ��Ķ���ָ��
	* @param pokerCombinationModelVector �����ҵ��˿˼���
	* @param pokerCombinationType �������͵�����
	* @return null-û���ҵ������򷵻��ҵ������Ͷ���ָ��
	*/
	static shared_ptr<PokerCombinationModel> findPokerCombination(vector<shared_ptr<PokerCombinationModel>>& pokerCombinationModelVector, const PokerCombinationType &pokerCombinationType);

	/**
	* �������ͣ����������ҵ��Ķ���
	* @param pokerCombinationModelVector �����ҵ��˿˼���
	* @param pokerCombinationType �������͵�����
	* @param value ���͵ķ���,-1��ʾֻҪ�Ǹ����͵����ͼ�����������>=0��ʾ����Ҫ��������ͬʱҪ���ڴ�value
	* @return �յļ���-û���ҵ������򷵻��ҵ������Ͷ���ָ�뼯��
	*/
	static vector<shared_ptr<PokerCombinationModel>> findPokerCombinationAll(vector<shared_ptr<PokerCombinationModel>>& pokerCombinationModelVector, const PokerCombinationType &pokerCombinationType);

	/**
	* ��������ת��Ϊ���ӻ�string
	*/
	static string toString(const PokerCombinationType& type);

	~PokerCombinationModel();

	/**
	* ����Ϸ����������ܹ�����������ģ��
	*/
	friend class RuleManagerAbstract;
	friend class SimpleRuleManager;

	/**
	* ��ȡ����
	*/
	PokerCombinationType& getPokerCombinationType();
	/**
	* ��ȡ������
	*/
	uint32_t getPokerCount();
	/**
	* ��ȡ�Ƶ�ֵ
	*/
	int32_t getValue();

	
	/**
	* ��ȡ�˿˼���
	*/
	vector<shared_ptr<PokerModel>>& getPokerModelVector();

	string toString()
	{
		return PokerModel::toString(_pokerModelVector);
	}
protected:
	/**
	* ��������
	* @param pokerCombinationType ����
	* @param pokerModelVector �˿˼���
	* @param value �Ƶ�ֵ�ܺ�
	*/
	PokerCombinationModel(const PokerCombinationType& pokerCombinationType, const vector<shared_ptr<PokerModel>>& pokerModelVector, const int32_t& value);

	static shared_ptr<PokerCombinationModel> create(const PokerCombinationType& pokerCombinationType, const vector<shared_ptr<PokerModel>>& pokerModelVector, const int32_t& value);

	void setValue(const int32_t& value);

	/**
	 * �������ͣ����ڽ����������ͣ�ֻ�й�����������Ե���
	 */
	void setPokerCombinationType(const PokerCombinationType& pokerCombinationType);
private:
	PokerCombinationType _pokerCombinationType;
	signed int _value;
	vector<shared_ptr<PokerModel>> _pokerModelVector;
};

/************************************************************************/
/* �����������Ͳ���ָ���������ݽṹ����ʹ��std::find_ifʱʹ��                                                                     */
/************************************************************************/
class PokerCombinationFindByType {
public:

	PokerCombinationFindByType(PokerCombinationType pokerCombinationType) :_pokerCombinationType(pokerCombinationType)
	{
	}

	bool operator () (PokerCombinationModel& pokerCombination) {
		return pokerCombination.getPokerCombinationType() == _pokerCombinationType;

	}

	bool operator () (shared_ptr<PokerCombinationModel> pokerCombination) {
		return pokerCombination->getPokerCombinationType() == _pokerCombinationType;
	}

private:
	PokerCombinationType _pokerCombinationType;
	
};


/**
* �˿�ģ����������
*/
class PokerModelFactory
{

public:

	/**
	* �����˿˹���������һ���˿�ģ��
	*/
	static shared_ptr<PokerModel> createPokerModel(const PokerBuildModel& pokerBuildModel);
	/**
	* ����ָ�����򴴽�һ���˿�ģ��
	*/
	static shared_ptr<PokerModel> createPokerModel(const RuleType& gameRuleType, const PokerValueType& pokerValueType, const PokerMarkType& pokerMarkType);

	/**
	* ����ָ����Ϸ�淨����һ���˿�
	* @param gameRuleType ��Ϸ��������
	*/
	static vector<shared_ptr<PokerModel>> createPokerModelPack(const RuleType& gameRuleType);

	/**
	* ������Ϸ�淨��ȡ��Ӧ�˿˵�ֵ��С������ֵ��С
	*/
	static bool getPokerValueAndSortValue(const RuleType& gameRuleType, const PokerValueType& pokerValueType, const PokerMarkType& pokerMarkType, int32_t& outPokerValue, int32_t& outSortValue);

	/**
	* ����ָ����Ϸ�淨��ȡһ���˿˹�����ģ��
	*/
	static PokerBuildModel* getPokerBuildModel(const RuleType& gameRuleType, const PokerValueType& pokerValueType, const PokerMarkType& pokerMarkType);
};


/**
* �����淨��һ���˿ˣ���52�ţ��޴�С�������
*/
static const PokerBuildModel POKER_SIMPLE[] =
{

	// 2
	PokerBuildModel(VALUE_TYPE_2, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_2, 20),
	PokerBuildModel(VALUE_TYPE_2, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_2, 20),
	PokerBuildModel(VALUE_TYPE_2, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_2, 20),
	PokerBuildModel(VALUE_TYPE_2, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_2, 20),

	//// 3
	PokerBuildModel(VALUE_TYPE_3, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_3, 30),
	PokerBuildModel(VALUE_TYPE_3, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_3, 30),
	PokerBuildModel(VALUE_TYPE_3, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_3, 30),
	PokerBuildModel(VALUE_TYPE_3, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_3, 30),
	// 4
	PokerBuildModel(VALUE_TYPE_4, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_4, 40),
	PokerBuildModel(VALUE_TYPE_4, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_4, 40),
	PokerBuildModel(VALUE_TYPE_4, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_4, 40),
	PokerBuildModel(VALUE_TYPE_4, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_4, 40),
	// 5
	PokerBuildModel(VALUE_TYPE_5, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_5, 50),
	PokerBuildModel(VALUE_TYPE_5, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_5, 50),
	PokerBuildModel(VALUE_TYPE_5, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_5, 50),
	PokerBuildModel(VALUE_TYPE_5, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_5, 50),
	// 6
	PokerBuildModel(VALUE_TYPE_6, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_6, 60),
	PokerBuildModel(VALUE_TYPE_6, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_6, 60),
	PokerBuildModel(VALUE_TYPE_6, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_6, 60),
	PokerBuildModel(VALUE_TYPE_6, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_6, 60),
	// 7
	PokerBuildModel(VALUE_TYPE_7, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_7, 70),
	PokerBuildModel(VALUE_TYPE_7, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_7, 70),
	PokerBuildModel(VALUE_TYPE_7, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_7, 70),
	PokerBuildModel(VALUE_TYPE_7, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_7, 70),
	// 8
	PokerBuildModel(VALUE_TYPE_8, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_8, 80),
	PokerBuildModel(VALUE_TYPE_8, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_8, 80),
	PokerBuildModel(VALUE_TYPE_8, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_8, 80),
	PokerBuildModel(VALUE_TYPE_8, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_8, 80),
	// 9
	PokerBuildModel(VALUE_TYPE_9, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_9, 90),
	PokerBuildModel(VALUE_TYPE_9, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_9, 90),
	PokerBuildModel(VALUE_TYPE_9, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_9, 90),
	PokerBuildModel(VALUE_TYPE_9, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_9, 90),
	// 10
	PokerBuildModel(VALUE_TYPE_10, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_10, 100),
	PokerBuildModel(VALUE_TYPE_10, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_10, 100),
	PokerBuildModel(VALUE_TYPE_10, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_10, 100),
	PokerBuildModel(VALUE_TYPE_10, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_10, 100),
	// J
	PokerBuildModel(VALUE_TYPE_J, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_J, 110),
	PokerBuildModel(VALUE_TYPE_J, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_J, 110),
	PokerBuildModel(VALUE_TYPE_J, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_J, 110),
	PokerBuildModel(VALUE_TYPE_J, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_J, 110),
	// Q
	PokerBuildModel(VALUE_TYPE_Q, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_Q, 120),
	PokerBuildModel(VALUE_TYPE_Q, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_Q, 120),
	PokerBuildModel(VALUE_TYPE_Q, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_Q, 120),
	PokerBuildModel(VALUE_TYPE_Q, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_Q, 120),
	// K
	PokerBuildModel(VALUE_TYPE_K, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_K, 130),
	PokerBuildModel(VALUE_TYPE_K, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_K, 130),
	PokerBuildModel(VALUE_TYPE_K, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_K, 130),
	PokerBuildModel(VALUE_TYPE_K, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_K, 130),
	// A
	PokerBuildModel(VALUE_TYPE_A, MARK_TYPE_BLACK_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_A, 140),
	PokerBuildModel(VALUE_TYPE_A, MARK_TYPE_RED_PEACH, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_A, 140),
	PokerBuildModel(VALUE_TYPE_A, MARK_TYPE_PLUM_BLOSSOM, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_A, 140),
	PokerBuildModel(VALUE_TYPE_A, MARK_TYPE_RED_SQUARE, PokerValueTypeOfValue::VALUE_TYPE_OF_VALUE_A, 140),

};

#endif  /* __GAME_MODEL_CPP_H__ */