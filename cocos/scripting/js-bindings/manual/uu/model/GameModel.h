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


/************************************************************************/
/* 游戏规则类型                                                                     */
/************************************************************************/
enum RuleType : signed int
{
	RULE_TYPE_SIMPLE, // 基础玩法
	RULE_TYPE_MUCH_THREE, // 多三张玩法
	RULE_TYPE_AMAZING, // 百变玩法
};


/**
* 扑克数值类型
*/
enum PokerValueType : signed int
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
enum PokerValueTypeOfValue : signed int
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
enum PokerMarkType : signed int
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
	PokerModel(PokerValueType pokerValueType, PokerMarkType pokerMarkType, int originalValue, int sortValue);
	~PokerModel();

	/**
	* 克隆一个扑克数据模型
	*/
	virtual PokerModel* clone();
	/**
	* 排序扑克
	* @param pokerModelVector 待排序扑克集合
	* @param sortType 排序类型
	* @param sortSubstitute 是否排序替用牌
	*/
	static void sortPokerModel(Vector<PokerModel*>& pokerModelVector, const SortType& sortType, const bool& sortSubstitute = true);

	/**
	* 排序分组扑克（按相同的牌进行分组）,所有组里一定有元素
	*/
	static void sortPokerModelWithGroup(vector<Vector<PokerModel*>>& pokerModelGroupVector, const SortType& sortType, const bool& sortSubstitute = true);

	/**
	* 查找扑克
	* @param pokerModelVector 待查找的扑克集合
	* @param pokerValueType 查找扑克的值类型
	* @param pokerMarkType 查找扑克的花色类型
	* @param isCheckSubstitute 是否检查替用的变化值，false-不检查（此时会忽略替用牌变化的牌型），true-检查（此时会优先比较牌型，如果对比不上，再对比替用变化的值，例如：替用已变成黑桃4，此时查找黑桃4，能够返回该替用牌）
	* @return null-没有找到，否则返回扑克对象指针
	*/
	static PokerModel* findPoker(Vector<PokerModel*>& pokerModelVector, const PokerValueType &pokerValueType, const PokerMarkType &pokerMarkType, const bool& isCheckSubstituteChangeValue = false);

	/**
	* 查找扑克，忽略扑克花色类型
	* @param pokerModelVector 待查找的扑克集合
	* @param pokerValueType 查找扑克的值类型
	* @param isCheckSubstitute 是否检查替用的变化值，false-不检查（此时会忽略替用牌变化的牌型），true-检查（此时会优先比较牌型，如果对比不上，再对比替用变化的值，例如：替用已变成黑桃4，此时查找黑桃4，能够返回该替用牌）
	* @return null-没有找到，否则返回找到的第一个扑克对象指针
	*/
	static PokerModel* findPoker(Vector<PokerModel*>& pokerModelVector, const PokerValueType &pokerValueType, const bool& isCheckSubstituteChangeValue = false);

	/**
	* 查找某种牌的总数
	*/
	static int findCountByPokerValue(Vector<PokerModel*>& pokerModelVector, const PokerValueType& pokerValueType);

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
	int getOriginalValue();
	/**
	* 获取排序值大小
	*/
	int getSortValue();

	/**
	* 变化扑克，替用牌才会生效
	*/
	void setChange(IN_P const PokerValueType& changeToPokerValueType, IN_P const PokerMarkType& changeToPokerMarkType, IN_P const int& changeToValue, IN_P const int& changeToSortValue);
	/**
	* 清空变化
	*/
	void clearChanged();
	/**
	* 获取扑克变化后的值大小
	*/
	int getChangedValue();
	/**
	* 获取扑克变化后的排序值大小
	*/
	int getChangedSortValue();
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

	static string toString(Vector<PokerModel*> pokerVector)
	{
		if (pokerVector.empty())
		{
			return "0";
		}
		else
		{
			string result;
			for (PokerModel* poker : pokerVector)
			{
				result.append(StringUtils::format("%d-%d,", poker->getPokerValueType(), poker->getPokerMarkType()));
			}
			return result;
		}

	}
private:
	PokerValueType _pokerValueType;
	PokerMarkType _pokerMarkType;
	int _originalValue;
	int _sortValue;

	PokerValueType _changedPokerValueType;
	PokerMarkType _changedPokerMarkType;
	int _changedValue;
	int _changedSortValue;
	bool _isChanged;
};

#endif  /* __GAME_MODEL_CPP_H__ */