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

	int32_t _pokerValueCount; // 牌总点数（pokerValue之和）

	int32_t _kingCount; // 王总数
	int32_t _substituteCount; // 替用个数

	/**
	 * 每种个数分类的扑克集合，例如：key为1时，就是单张牌的集合，key为2时，就是对子牌的集合
	 */
	map<int32_t, vector<shared_ptr<vector<shared_ptr<PokerModel>>>>> _everyCountPokerMap;

	/**
	 * 按花色分类的扑克集合
	 */
	map<PokerMarkType, vector<shared_ptr<PokerModel>>> _pokerMarkMap;

	/**
	 * 按扑克值分类的扑克集合
	 */
	map<PokerValueType, vector<shared_ptr<PokerModel>>> _pokerTypeMap;

	/**
	* 每种扑克的个数
	*/
	map<PokerValueType, uint32_t> _countEveryPokerTypeMap;
};


/************************************************************************/
/* 规则管理器抽象类                                                                     */
/************************************************************************/
class RuleManagerAbstract
{
public:
	/**
	 * 获取牌个数信息
	 */
	shared_ptr<PokerCountInfo> RuleManagerAbstract::getPokerCountInfo(vector<shared_ptr<PokerModel>> pokerVector);

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
	virtual bool hasSpecialPokerCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector);

	/**
	* 从所有牌型中取出特殊牌型
	*/
	virtual shared_ptr<PokerCombinationModel> getSpecialPokerCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector);

public:

	/**
	* 获取游戏规则类型
	*/
	virtual RuleType getRuleType() = 0;

	/**
	* 根据手牌获取所有牌型组合
	*/
	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getPokerCombination(vector<shared_ptr<PokerModel>> pokerVector) = 0;

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
public:

	virtual RuleType getRuleType();

	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getPokerCombination(vector<shared_ptr<PokerModel>> pokerVector);

protected:

	/**
	 * 找出特殊牌型
	 */
	virtual shared_ptr<PokerCombinationModel> findSpecialPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * 找出所有链子
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findChainPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * 找出所有普通推荐牌型
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findSimplePokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo);
private:

};

#endif  /* __RULE_MANAGER_CPP_H__ */