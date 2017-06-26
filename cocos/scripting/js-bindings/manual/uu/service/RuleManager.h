/********************************************************************
	created:	2017/06/15
	created:	15:6:2017   21:48
	file base:	RuleManager
	file ext:	h
	author:		sulei
	
	purpose:	
*********************************************************************/
#ifndef __RULE_MANAGER_CPP_H__
#define __RULE_MANAGER_CPP_H__
#include "../model/GameModel.h"
#include <map>
#include <memory>
using namespace std;

/************************************************************************/
/* 扑克个数信息                                                                     */
/************************************************************************/
class PokerCountInfo : public BaseModel
{
public:
	PokerCountInfo();

	int32_t _singleCount; // 单牌总数(不包含王，不包含替用)
	int32_t _twosomeCount; // 对子总数（不包含王，不包含替用）
	int32_t _threesomeCount; // 三张总数（不包含王，不包含替用）
	int32_t _foursomeCount; // 四张牌总数（不包含王，不包含替用）

	int32_t _blackPeachCount; // 黑桃总数
	int32_t _redPeachCount; // 红桃总数
	int32_t _plumBlossomCount; // 梅花总数
	int32_t _redSquareCount; // 红方总数
	int32_t _markTypeCount; // 花色种类总数


	int32_t _pokerValueCount; // 牌总点数（pokerValue之和）

	int32_t _kingCount; // 王总数
	int32_t _substituteCount; // 替用个数

	/**
	 * 按花色分类的扑克集合
	 */
	map<PokerMarkType, vector<shared_ptr<PokerModel>>> _pokerMarkMap;

	/**
	 * 按扑克值分类的扑克集合
	 */
	map<PokerValueType, vector<shared_ptr<PokerModel>>> _pokerTypeMap;

	/**
	* 每种个数分类的扑克集合，例如：key为1时，就是单张牌的集合，key为2时，就是对子牌的集合
	*/
	map<int32_t, vector<shared_ptr<vector<shared_ptr<PokerModel>>>>> _everyCountPokerMap;

	/**
	* 每种扑克的个数
	*/
	map<PokerValueType, uint32_t> _countEveryPokerTypeMap;
	/**
	 * 每种扑克花色的个数
	 */
	map<PokerMarkType, uint32_t> _countEveryPokerMarkMap;
};


/************************************************************************/
/* 规则管理器抽象类                                                                     */
/************************************************************************/
class RuleManagerAbstract
{

public:

	/**
	* 获取游戏规则类型
	*/
	virtual RuleType getRuleType() = 0;

	/**
	 * 根据手牌获得推荐牌型
	 * @param pokerVector 
	 */
	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getRecommondPokerCombination(vector<shared_ptr<PokerModel>> pokerVector) = 0;

	/**
	 * 根据扑克集合获取牌型
	 * @param pokerVector 扑克集合，可以是3张的头道，也可以是5张的中尾道
	 */
	virtual shared_ptr<PokerCombinationModel> getPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector) = 0;
public:
	/**
	 * 获取牌个数信息
	 */
	shared_ptr<PokerCountInfo> getPokerCountInfo(vector<shared_ptr<PokerModel>> pokerVector);

	/**
	 * 获取牌点数总和
	 */
	int32_t getPokerValueTotal(const vector<shared_ptr<PokerModel>>& pokerVector);

	/**
	 * 比较两个牌型的大小
	 * @return >0:model1大于model2，=0：model1等于model2，<0:model1小于model2
	 */
	virtual int32_t comparePokerCombination(shared_ptr<PokerCombinationModel> model1, shared_ptr<PokerCombinationModel> model2);

	/**
	* 是否为相公牌型
	*/
	virtual bool isXiangGong(vector<shared_ptr<PokerCombinationModel>> pokerCombinationVector);

	/**
	* 是否是特殊牌型
	*/
	virtual bool isSpecialPokerCombination(const PokerCombinationType& pcType);

	/**
	* 是否包含特殊牌型
	*/
	virtual bool hasPeculiarCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector);

	/**
	* 从所有牌型中取出特殊牌型
	*/
	virtual shared_ptr<PokerCombinationModel> getPeculiarPokerCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector);

protected:


	/**
	* 根据牌型获取牌型对应的分数
	* @param pokerCountInfo 扑克统计信息，不为NULL时将使用统计信息中的数据
	*/
	virtual int32_t getPokerValueByCombination(const PokerCombinationType& type, const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo = nullptr) = 0;
};

/************************************************************************/
/* 基础玩法规则管理器                                                                     */
/************************************************************************/
class SimpleRuleManager : public RuleManagerAbstract
{
public:
	static const int32_t POKER_COUNT_ROW_1 = 3;// 头道牌数
	static const int32_t POKER_COUNT_ROW_2 = 5;// 中道牌数
	static const int32_t POKER_COUNT_ROW_3 = 5;// 尾道牌数

	static const int32_t POKER_COUNT_SINGLE_PLAYER = 13; // 玩家手牌总数

	static const int32_t CHAIN_FIND_TYPE_TONGHUA_ALL = 0; // 查找连子，所有类型
	static const int32_t CHAIN_FIND_TYPE_TONGHUA_ONLY = 1; // 查找连子，仅同花顺
	static const int32_t CHAIN_FIND_TYPE_TONGHUA_OTHER = 2; // 查找连子，非同花顺

	static const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_3_OR_5 = 0; // 查找连子个数类型，3张或5张
	static const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_3 = 1; // 查找连子个数类型，仅三张
	static const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_5 = 2; // 查找连子个数类型，仅五张
	static const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_LONG = 3; // 查找最长的顺子


	static const int32_t WULONG_FIND_TYPE_COUNT_TYPE_3_OR_5 = 0; // 查找乌龙个数类型，3张或5张
	static const int32_t WULONG_FIND_TYPE_COUNT_TYPE_3 = 1; // 查找乌龙个数类型，仅三张
	static const int32_t WULONG_FIND_TYPE_COUNT_TYPE_5 = 2; // 查找乌龙个数类型，仅五张
	
public:

	virtual RuleType getRuleType();

	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getRecommondPokerCombination(vector<shared_ptr<PokerModel>> pokerVector);

	virtual shared_ptr<PokerCombinationModel> getPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector);
	
protected:

	/**
	 * 找出特殊牌型
	 */
	virtual shared_ptr<PokerCombinationModel> findPeculiarPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	* 找出所有普通推荐牌型
	*/
	virtual vector<shared_ptr<PokerCombinationModel>> findSimplePokerCombination(vector<shared_ptr<PokerModel>>& pokerVector,const shared_ptr<PokerCountInfo> pokerCountInfo);

	

	/**
	 * 找出指定连子
	 * @param tonghuaType 同花类型
	 * @param countType  个数类型
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findChainPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& tonghuaType, const int32_t& countType);

	/**
	 * 找出所有葫芦牌型
	 *
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findHuLuPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * 查找所有铁支牌型
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findTieZhiPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * 查找所有同花牌型
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findTongHuaPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * 查找所有三条牌型
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findSanTiaoPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * 查找所有两对牌型
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findLiangDuiPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * 查找所有对子牌型
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findDuiZiPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * 查找乌龙牌型
	 * @param wulongCountType 乌龙个数类型
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findWuLongPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& wulongCountType);

	virtual int32_t getPokerValueByCombination(const PokerCombinationType& type, const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo = nullptr);

	
private:

	/**
	 * 递归查找普通牌型
	 */
	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> findSimplePokerCombinationRecursion(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& step);
};

#endif  /* __RULE_MANAGER_CPP_H__ */