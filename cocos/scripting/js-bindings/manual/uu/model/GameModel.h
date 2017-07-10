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
* 排序类型
*/
enum SortType : uint32_t
{
	/**
	* 降序
	*/
	SORT_TYPE_DESC = 1,
	/**
	* 升序
	*/
	SORT_TYPE_ASC
};

/************************************************************************/
/* 排序扑克类型                                                                     */
/************************************************************************/
enum SortPokerType : uint32_t
{
	/**
	 * 扑克值类型优先
	 */
	SORT_POKER_TYPE_VALUE_FIRST = 1,
	/**
	 * 扑克花色类型优先
	 */
	SORT_POKER_TYPE_MARK_FIRST = 2,
};

/************************************************************************/
/* 游戏规则类型                                                                     */
/************************************************************************/
enum RuleType : int32_t
{
	RULE_TYPE_SIMPLE, // 基础玩法
	RULE_TYPE_MUCH_THREE, // 多三张玩法
	RULE_TYPE_AMAZING, // 百变玩法
};


/**
* 扑克数值类型
*/
enum PokerValueType : int32_t
{
	// 牌型2到A，小鬼、大鬼、替用
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
* 扑克值大小
*/
enum PokerValueTypeOfValue : int32_t
{
	// 牌型2到A，小鬼、大鬼、替用
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
* 扑克花色类型
*/
enum PokerMarkType : int32_t
{
	/**
	* 无花色,例如：大鬼、小鬼、替用
	*/
	MARK_TYPE_NONE = 0,
	/**
	* 红方
	*/
	MARK_TYPE_RED_SQUARE = 1,
	/**
	* 梅花
	*/
	MARK_TYPE_PLUM_BLOSSOM = 2,
	/**
	* 红桃
	*/
	MARK_TYPE_RED_PEACH = 3,
	/**
	* 黑桃
	*/
	MARK_TYPE_BLACK_PEACH = 4,
};


/**
* 扑克数据模型
*/
class PokerModel : public BaseModel
{
public:
	
	/**
	* 通过扑克类型、花色、原始值大小构建一张扑克model
	* @param pokerValueType 扑克类型
	* @param pokerMarkType 扑克花色类型
	* @param originalValue 扑克原始值大小
	* @param sortValue 扑克排序值,越大排序越靠前
	*/
	PokerModel(PokerValueType pokerValueType, PokerMarkType pokerMarkType, int32_t originalValue, int32_t sortValue);
	~PokerModel();

	/**
	* 克隆一个扑克数据模型
	*/
	virtual shared_ptr<PokerModel> clone();
	/**
	* 排序扑克
	* @param pokerModelVector 待排序扑克集合
	* @param sortType 排序类型
	* @param sortPokerType 扑克排序优先级类型
	* @param sortSubstitute 是否排序替用牌
	*/
	static void sortPokerModel(vector<shared_ptr<PokerModel>>& pokerModelVector, const SortType& sortType, const SortPokerType& sortPokerType = SortPokerType::SORT_POKER_TYPE_VALUE_FIRST, const bool& sortSubstitute = true);

	/**
	* 排序分组扑克（按相同的牌进行分组）,所有组里一定有元素
	*/
	static void sortPokerModelWithGroup(vector<shared_ptr<vector<shared_ptr<PokerModel>>>>& pokerModelGroupVector, const SortType& sortType, const bool& sortSubstitute = true);

	/**
	* 查找扑克
	* @param pokerModelVector 待查找的扑克集合
	* @param pokerValueType 查找扑克的值类型
	* @param pokerMarkType 查找扑克的花色类型
	* @param isCheckSubstitute 是否检查替用的变化值，false-不检查（此时会忽略替用牌变化的牌型），true-检查（此时会优先比较牌型，如果对比不上，再对比替用变化的值，例如：替用已变成黑桃4，此时查找黑桃4，能够返回该替用牌）
	* @return null-没有找到，否则返回扑克对象指针
	*/
	static shared_ptr<PokerModel> findPoker(const vector<shared_ptr<PokerModel>>& pokerModelVector, const PokerValueType &pokerValueType, const PokerMarkType &pokerMarkType, const bool& isCheckSubstituteChangeValue = false);

	/**
	* 查找扑克，忽略扑克花色类型
	* @param pokerModelVector 待查找的扑克集合
	* @param pokerValueType 查找扑克的值类型
	* @param isCheckSubstitute 是否检查替用的变化值，false-不检查（此时会忽略替用牌变化的牌型），true-检查（此时会优先比较牌型，如果对比不上，再对比替用变化的值，例如：替用已变成黑桃4，此时查找黑桃4，能够返回该替用牌）
	* @return null-没有找到，否则返回找到的第一个扑克对象指针
	*/
	static shared_ptr<PokerModel> findPoker(const vector<shared_ptr<PokerModel>>& pokerModelVector, const PokerValueType &pokerValueType, const bool& isCheckSubstituteChangeValue = false);

	/**
	* 查找某种牌的总数
	*/
	static int32_t findCountByPokerValue(const vector<shared_ptr<PokerModel>>& pokerModelVector, const PokerValueType& pokerValueType);

	/**
	* 从牌集合中排除指定牌集合
	* @param pokerVector
	* @param beRemovePokerVector 将要被移除的牌型集合
	*/
	static void removeSpecifyPokerVector(vector<shared_ptr<PokerModel>>& pokerVector, const vector<shared_ptr<PokerModel>>& beRemovePokerVector);

	/**
	* 合并指定牌集合到牌集合中，会检测重复
	* @param pokerVector
	* @param beMergePokerVector 将要被合并的牌型集合
	*/
	static void mergeSpecifyPokerVector(vector<shared_ptr<PokerModel>>& pokerVector, const vector<shared_ptr<PokerModel>>& beMergePokerVector);

	/**
	 * 连接两个牌型,不会检测重复
	 * @param pokerVector
	 * @param beConcatPokerVector 将要被合并的牌型集合
	 */
	static void concatSpecifyPokerVector(vector<shared_ptr<PokerModel>>& pokerVector, const vector<shared_ptr<PokerModel>>& beConcatPokerVector);

	/**
	* 获取扑克类型
	*/
	PokerValueType getPokerValueType();
	/**
	* 获取扑克花色类型
	*/
	PokerMarkType getPokerMarkType();

	/**
	* 获取扑克原始值大小
	*/
	int32_t getOriginalValue();
	/**
	* 获取排序值大小
	*/
	int32_t getSortValue();

	/**
	* 变化扑克，替用牌才会生效
	*/
	void setChange(const PokerValueType& changeToPokerValueType, const PokerMarkType& changeToPokerMarkType, const int32_t& changeToValue, const int32_t& changeToSortValue);
	/**
	* 清空变化
	*/
	void clearChanged();
	/**
	* 获取扑克变化后的值大小
	*/
	int32_t getChangedValue();
	/**
	* 获取扑克变化后的排序值大小
	*/
	int32_t getChangedSortValue();
	/**
	* 获取扑克变化后的类型
	*/
	PokerValueType getChangedPokerValueType();
	/**
	* 获取扑克变化后的花色类型
	*/
	PokerMarkType getChangedPokerMarkType();

	/**
	* 扑克是否已发生过变化，只有替用牌才能做此变化
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
/* 根据扑克花色类型和值类型查找扑克数据结构，在使用std::find_if时使用                                                                     */
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
/* 根据扑克值类型查找扑克数据结构，在使用std::find_if时使用                                                                     */
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
* 扑克构造器模型
*/
class PokerBuildModel : public BaseModel
{
public:

	/**
	* 通过扑克类型、花色、原始值大小构建一张扑克model
	* @param pokerValueType 扑克类型
	* @param pokerMarkType 扑克花色类型
	* @param originalValue 扑克原始值大小
	* @param sortValue 扑克排序值
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
* 根据扑克花色类型和值类型查找扑克构造器数据结构，在使用std::find_if时使用
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
/* 牌型                                                                     */
/************************************************************************/
enum PokerCombinationType : uint32_t
{
	/**
	* 未知牌型
	*/
	POKER_COMBINATION_TYPE_NONE = 0,

	// 基础牌型
	/**
	* 乌龙（3/5张单张，如3+7+9+J+K）
	*/
	POKER_COMBINATION_TYPE_WU_LONG,
	/**
	* 对子（一个对子+3张单张，如77+3+5+9）
	*/
	POKER_COMBINATION_TYPE_DUI_ZI,
	/**
	* 两对（两个对子+单张，如77+99+3）
	*/
	POKER_COMBINATION_TYPE_LIANG_DUI,
	/**
	* 三条（3张相同点数牌+2张单张且不在第一道，如333+79）
	*/
	POKER_COMBINATION_TYPE_SAN_TIAO,
	/**
	 * 冲三（3张相同点数的牌并且在第一道，如：555）
	 */
	POKER_COMBINATION_TYPE_CHONG_SAN,
	/**
	* 顺子（连着但花色不同的3张牌，如黑桃4+红桃5+梅花6；）
	*/
	POKER_COMBINATION_TYPE_SHUN_ZI,
	/**
	* 同花（相同花色的3/5张牌，如黑桃358）
	*/
	POKER_COMBINATION_TYPE_TONG_HUA,
	/**
	* 葫芦（3张相同点数牌+对子且在第三道，如333+88）
	*/
	POKER_COMBINATION_TYPE_HU_LU,
	/**
	 * 中墩葫芦（3张相同点数牌+对子并且在第二道，如333+88）
	 */
	POKER_COMBINATION_TYPE_ZHONG_DUN_HU_LU,
	
	/**
	 * 铁支（4张相同点数牌 + 1张单张且在第三道，如AAAA+3）
	 */
	POKER_COMBINATION_TYPE_TIE_ZHI,
	/**
	* 中墩铁支（4张相同点数牌 + 1张单张且在第二道，如AAAA+3）
	*/
	POKER_COMBINATION_TYPE_ZHONG_DUN_TIE_ZHI,
	/**
	* 同花顺（相同花色且连着的3/5张牌且在第三道，如 黑桃56789）
	*/
	POKER_COMBINATION_TYPE_TONG_HUA_SHUN,
	/**
	 * 中墩同花顺（相同花色且连着的3/5张牌且在第二道，如 黑桃56789）
	 */
	POKER_COMBINATION_TYPE_ZHONG_DUN_TONG_HUA_SHUN,
	// 特殊牌型
	/**
	* 三同花（头道为3张相同花色的牌，中、尾道为同花）
	*/
	POKER_COMBINATION_TYPE_SAN_TONG_HUA,
	/**
	* 三顺子（头道为3张连续的牌，中、尾道为顺子）
	*/
	POKER_COMBINATION_TYPE_SAN_SHUN_ZI,
	/**
	* 六对半（13张牌由6组对子+1张单牌组成）
	*/
	POKER_COMBINATION_TYPE_LIU_DUI_BAN,
	/**
	* 五对三条（ 13张牌由5个对子+1组三条（3张相同点数的牌）组成）
	*/
	POKER_COMBINATION_TYPE_WU_DUI_SAN_TIAO,
	/**
	* 四套三条（13张牌由4组三条（3张相同点数的牌）+1张单牌组成）
	*/
	POKER_COMBINATION_TYPE_SI_TAO_SAN_TIAO,
	/**
	* 凑一色（13张牌花色均为红色（红桃、方块）或黑色（梅花、黑桃））
	*/
	POKER_COMBINATION_TYPE_COU_YI_SE,
	/**
	* 全小（13张均是2-8的牌，无论花色）
	*/
	POKER_COMBINATION_TYPE_QUAN_XIAO,
	/**
	* 全大（13张均是8-A的牌，无论花色）
	*/
	POKER_COMBINATION_TYPE_QUAN_DA,
	/**
	* 三分天下（3组铁支（即相同点数的四张牌）+1张单牌）
	*/
	POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA,
	/**
	* 三同花顺（第一墩是三张花色相同且连续的牌，第二、三墩为同花顺）
	*/
	POKER_COMBINATION_TYPE_SAN_TONG_HUA_SHUN,
	/**
	* 十二皇族（十三张牌均是AKQJ）
	*/
	POKER_COMBINATION_TYPE_SHI_ER_HUANG_ZU,
	/**
	* 一条龙（不同花色，从大到小各一张（可理解为最大的顺子））
	*/
	POKER_COMBINATION_TYPE_YI_TIAO_LONG,
	/**
	* 清龙（同一种花色，从大到小各一张，比如黑桃AKQJ 10 98765432（可理解为最大的同花顺））
	*/
	POKER_COMBINATION_TYPE_QING_LONG,
};

/**
* 牌型数据模型
*/
class PokerCombinationModel : public BaseModel
{
public:

	static shared_ptr<PokerCombinationModel> clone(shared_ptr<PokerCombinationModel> pokerCombinationModel);

	/**
	* 查找牌型,返回第一个找到的对象指针
	* @param pokerCombinationModelVector 待查找的扑克集合
	* @param pokerCombinationType 查找牌型的类型
	* @return null-没有找到，否则返回找到的牌型对象指针
	*/
	static shared_ptr<PokerCombinationModel> findPokerCombination(vector<shared_ptr<PokerCombinationModel>>& pokerCombinationModelVector, const PokerCombinationType &pokerCombinationType);

	/**
	* 查找牌型，返回所有找到的对象
	* @param pokerCombinationModelVector 待查找的扑克集合
	* @param pokerCombinationType 查找牌型的类型
	* @param value 牌型的分数,-1表示只要是该类型的牌型即满足条件，>=0表示必须要满足类型同时要等于此value
	* @return 空的集合-没有找到，否则返回找到的牌型对象指针集合
	*/
	static vector<shared_ptr<PokerCombinationModel>> findPokerCombinationAll(vector<shared_ptr<PokerCombinationModel>>& pokerCombinationModelVector, const PokerCombinationType &pokerCombinationType);

	/**
	* 将牌类型转换为可视化string
	*/
	static string toString(const PokerCombinationType& type);

	~PokerCombinationModel();

	/**
	* 仅游戏规则管理器能构造牌型数据模型
	*/
	friend class RuleManagerAbstract;
	friend class SimpleRuleManager;

	/**
	* 获取牌型
	*/
	PokerCombinationType& getPokerCombinationType();
	/**
	* 获取牌总数
	*/
	uint32_t getPokerCount();
	/**
	* 获取牌的值
	*/
	int32_t getValue();

	
	/**
	* 获取扑克集合
	*/
	vector<shared_ptr<PokerModel>>& getPokerModelVector();

	string toString()
	{
		return PokerModel::toString(_pokerModelVector);
	}
protected:
	/**
	* 构造牌型
	* @param pokerCombinationType 牌型
	* @param pokerModelVector 扑克集合
	* @param value 牌的值总和
	*/
	PokerCombinationModel(const PokerCombinationType& pokerCombinationType, const vector<shared_ptr<PokerModel>>& pokerModelVector, const int32_t& value);

	static shared_ptr<PokerCombinationModel> create(const PokerCombinationType& pokerCombinationType, const vector<shared_ptr<PokerModel>>& pokerModelVector, const int32_t& value);

	void setValue(const int32_t& value);

	/**
	 * 设置牌型，用于矫正特殊牌型，只有规则管理器可以调用
	 */
	void setPokerCombinationType(const PokerCombinationType& pokerCombinationType);
private:
	PokerCombinationType _pokerCombinationType;
	signed int _value;
	vector<shared_ptr<PokerModel>> _pokerModelVector;
};

/************************************************************************/
/* 根据牌型类型查找指定牌型数据结构，在使用std::find_if时使用                                                                     */
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
* 扑克模型生产工厂
*/
class PokerModelFactory
{

public:

	/**
	* 根据扑克构建器创建一张扑克模型
	*/
	static shared_ptr<PokerModel> createPokerModel(const PokerBuildModel& pokerBuildModel);
	/**
	* 根据指定规则创建一张扑克模型
	*/
	static shared_ptr<PokerModel> createPokerModel(const RuleType& gameRuleType, const PokerValueType& pokerValueType, const PokerMarkType& pokerMarkType);

	/**
	* 根据指定游戏玩法创建一套扑克
	* @param gameRuleType 游戏规则类型
	*/
	static vector<shared_ptr<PokerModel>> createPokerModelPack(const RuleType& gameRuleType);

	/**
	* 根据游戏玩法获取对应扑克的值大小及排序值大小
	*/
	static bool getPokerValueAndSortValue(const RuleType& gameRuleType, const PokerValueType& pokerValueType, const PokerMarkType& pokerMarkType, int32_t& outPokerValue, int32_t& outSortValue);

	/**
	* 根据指定游戏玩法获取一个扑克构建者模型
	*/
	static PokerBuildModel* getPokerBuildModel(const RuleType& gameRuleType, const PokerValueType& pokerValueType, const PokerMarkType& pokerMarkType);
};


/**
* 基础玩法的一副扑克，共52张，无大小鬼和替用
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