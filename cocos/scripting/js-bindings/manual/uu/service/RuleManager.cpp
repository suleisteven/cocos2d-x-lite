/********************************************************************
	created:	2017/06/15
	created:	15:6:2017   21:48
	file base:	RuleManager
	file ext:	cpp
	author:		sulei
	
	purpose:	
*********************************************************************/
#include "RuleManager.h"


////////////////////////////////扑克个数信息//////////////////////////////////////////
PokerCountInfo::PokerCountInfo() :_singleCount(0), _twosomeCount(0), _threesomeCount(0), _foursomeCount(0), _kingCount(0), _substituteCount(0)
{
}


////////////////////////////规则管理器抽象类//////////////////////////////////////////////
shared_ptr<PokerCountInfo> RuleManagerAbstract::getPokerCountInfo(vector<shared_ptr<PokerModel>> pokerVector)
{

	shared_ptr<PokerCountInfo> result(new(std::nothrow) PokerCountInfo);
	PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_ASC); // 排序扑克

	map<PokerValueType, uint32_t> groupCountMap;
	int32_t groupSize = 0; // 分组总数

	int32_t kingCount = 0; // “王”的个数
	int32_t oneCount = 0; // 单牌个数
	int32_t twoCount = 0; // 对子个数
	int32_t threeCount = 0; // 三张个数
	int32_t fourCount = 0; // 四张个数
	int32_t substituteCount = 0; // 替用个数

	int32_t blackPeachCount = 0; // 黑桃总数
	int32_t redPeachCount = 0; // 红桃总数
	int32_t plumBlossomCount = 0; // 梅花总数
	int32_t redSquareCount = 0; // 红方总数

	int32_t pokerValueCount = 0;// 牌总点数

	bool isAllTypeSameCount = true;// 是否所有类型的牌个数一样，比如3344、45678，如4443就不是
	int32_t prePokerValueCount = 0; // 上次统计的扑克类型总数

	vector<shared_ptr<PokerModel>> curCountPokerModelVector; // 当前统计的扑克数据模型集合
	for (uint32_t i = 0, sizeI = pokerVector.size(); i < sizeI; ++i)
	{
		shared_ptr<PokerModel> curPokerModel = pokerVector.at(i);
		PokerValueType curPokerValueType = curPokerModel->getPokerValueType();
		PokerMarkType curPokerMarkType = curPokerModel->getPokerMarkType();

		pokerValueCount += (int32_t)curPokerValueType;

		uint32_t oriValue = groupCountMap[curPokerValueType];
		groupCountMap[curPokerValueType] = ++oriValue; // 分类统计每类扑克的个数

		vector<shared_ptr<PokerModel>>* pokerValueVec = &(result->_pokerTypeMap[curPokerValueType]);
		pokerValueVec->push_back(curPokerModel);

		if (curPokerValueType == PokerValueType::VALUE_TYPE_KING_BIG || curPokerValueType == PokerValueType::VALUE_TYPE_KING_SMALL) // 统计“王”的个数
		{
			++kingCount;
			continue;
		}
		if (curPokerValueType == PokerValueType::VALUE_TYPE_SUBSTITUTE)
		{
			++substituteCount;
			continue;
		}
		
		curCountPokerModelVector.push_back(curPokerModel);

		vector<shared_ptr<PokerModel>>* vec = &(result->_pokerMarkMap[curPokerMarkType]);
		vec->push_back(curPokerModel); // 添加当前花色扑克

		

		switch (curPokerMarkType)
		{
		case MARK_TYPE_NONE:
			break;
		case MARK_TYPE_RED_SQUARE:
			++redSquareCount;
			break;
		case MARK_TYPE_PLUM_BLOSSOM:
			++plumBlossomCount;
			break;
		case MARK_TYPE_RED_PEACH:
			++redPeachCount;
			break;
		case MARK_TYPE_BLACK_PEACH:
			++blackPeachCount;
			break;
		default:
			break;
		}

		bool isCanCountThis = false; // 是否可以进行同一类型扑克的总数统计
		if (i != sizeI - 1)
		{
			if (pokerVector.at(i + 1)->getPokerValueType() != curPokerValueType)
			{
				isCanCountThis = true;
			}
			else
			{
				isCanCountThis = false;
			}
		}
		else
		{
			isCanCountThis = true;
		}
		if (isCanCountThis)
		{

			shared_ptr<vector<shared_ptr<PokerModel>>> curPokerModelVector(new vector<shared_ptr<PokerModel>>);
			curPokerModelVector->insert(curPokerModelVector->end(), curCountPokerModelVector.begin(), curCountPokerModelVector.end());
			
			curCountPokerModelVector.clear();
			switch (oriValue) {
			case 1:
			{
				++oneCount;
			}
				break;
			case 2:
			{
				++twoCount;
			}
				break;
			case 3:
			{
				++threeCount;
			}
				break;
			case 4:
				++fourCount;
				break;
			default:
				break;
			}
			vector<shared_ptr<vector<shared_ptr<PokerModel>>>>* vecTmp = &(result->_everyCountPokerMap[oriValue]);
			vecTmp->push_back(curPokerModelVector);
		}
	}

	if (kingCount == 2)
	{
		fourCount++;
	}
	else if (kingCount == 1)
	{
		oneCount++;
	}

	result->_pokerValueCount = pokerValueCount;

	result->_singleCount = oneCount;
	result->_twosomeCount = twoCount;
	result->_threesomeCount = threeCount;
	result->_foursomeCount = fourCount;

	result->_blackPeachCount = blackPeachCount;
	result->_redPeachCount = redPeachCount;
	result->_plumBlossomCount = plumBlossomCount;
	result->_redSquareCount = redSquareCount;

	result->_kingCount = kingCount;
	result->_substituteCount = substituteCount;
	result->_countEveryPokerTypeMap = groupCountMap;

	return result;
}

int32_t RuleManagerAbstract::getPokerValueTotal(const vector<shared_ptr<PokerModel>>& pokerVector)
{
	int32_t result = 0;
	for (int32_t i = 0, sizeI = pokerVector.size(); i <sizeI; ++i)
	{
		result += pokerVector.at(i).get()->getPokerValueType();
	}
	return result;
}

int32_t RuleManagerAbstract::comparePokerCombination(shared_ptr<PokerCombinationModel> model1, shared_ptr<PokerCombinationModel> model2)
{
	return model1->getValue() - model2->getValue();
}

bool RuleManagerAbstract::isXiangGong(vector<shared_ptr<PokerCombinationModel>> pokerCombinationVector)
{
	bool result = false;
	for (int32_t i = 0, sizeI = pokerCombinationVector.size(); i < sizeI; ++i)
	{
		if (i == 0)
		{
			continue;
		}
		int32_t compResult = comparePokerCombination(pokerCombinationVector[i - 1], pokerCombinationVector[i]);
		if (compResult >= 0)
		{
			result = true;
			break;
		}
	}
	return result;
}

bool RuleManagerAbstract::isSpecialPokerCombination(const PokerCombinationType& pcType)
{
	return pcType >= PokerCombinationType::POKER_COMBINATION_TYPE_SAN_TONG_HUA;
}

bool RuleManagerAbstract::hasSpecialPokerCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector)
{
	bool result = false;
	for (int32_t i = 0, sizeI = pokerCombinationVector.size(); i < sizeI; ++i)
	{
		result = pokerCombinationVector[i]->size() == 1;
		if (result)
		{
			break;
		}
	}
	return result;
}


shared_ptr<PokerCombinationModel> RuleManagerAbstract::getSpecialPokerCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector)
{
	shared_ptr<PokerCombinationModel> result = nullptr;
	for (int32_t i = 0, sizeI = pokerCombinationVector.size(); i < sizeI; ++i)
	{
		result = pokerCombinationVector.at(i)->size() == 1 ? pokerCombinationVector.at(i)->at(0) : nullptr;
		if (result)
		{
			break;
		}
	}
	return nullptr;
}


//////////////////////////基础玩法规则管理器  ////////////////////////////////////////////////
RuleType SimpleRuleManager::getRuleType()
{
	return RuleType::RULE_TYPE_SIMPLE;
}

vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> SimpleRuleManager::getPokerCombination(vector<shared_ptr<PokerModel>> pokerVector)
{
	PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_DESC); // 按升序排列

	vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> result;
	shared_ptr<PokerCountInfo> pokerCountInfo = getPokerCountInfo(pokerVector); // 统计牌数信息

	findSpecialPokerCombination(pokerVector, pokerCountInfo);
	return result;
}


shared_ptr<PokerCombinationModel> SimpleRuleManager::findSpecialPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfoPtr)
{
	
	shared_ptr<PokerCombinationModel> result = nullptr;
	int32_t pokerCount = pokerVector.size();

	PokerCountInfo pokerCountInfo = *pokerCountInfoPtr;


	vector<shared_ptr<PokerCombinationModel>> chain = findChainPokerCombination(pokerVector, pokerCountInfoPtr, CHAIN_FIND_TYPE_TONGHUA_ONLY, CHAIN_FIND_TYPE_COUNT_TYPE_5);

	// 判断是否为清龙
	if (pokerCountInfo._singleCount == POKER_COUNT_SINGLE_PLAYER && (pokerCountInfo._blackPeachCount == POKER_COUNT_SINGLE_PLAYER
		|| pokerCountInfo._redPeachCount == POKER_COUNT_SINGLE_PLAYER
		|| pokerCountInfo._plumBlossomCount == POKER_COUNT_SINGLE_PLAYER
		|| pokerCountInfo._redSquareCount == POKER_COUNT_SINGLE_PLAYER))
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_QING_LONG, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_QING_LONG, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// 判断是否为一条龙
	if (pokerCountInfo._singleCount == POKER_COUNT_SINGLE_PLAYER)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_YI_TIAO_LONG, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_YI_TIAO_LONG, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// 判断十二皇族
	if (pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_A] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_K] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_Q] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_J] >= POKER_COUNT_SINGLE_PLAYER)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SHI_ER_HUANG_ZU, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SHI_ER_HUANG_ZU, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// 判断三同花顺
	if (false)
	{
		return result;
	}

	// 判断三分天下
	if (pokerCountInfo._foursomeCount == 3)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// 判断全大
	if (pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_A] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_K] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_Q] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_J] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_10] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_9] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_8] >= POKER_COUNT_SINGLE_PLAYER)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_QUAN_DA, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_QUAN_DA, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// 判断全小
	if (pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_2] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_3] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_4] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_5] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_6] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_7] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_8] >= POKER_COUNT_SINGLE_PLAYER)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_QUAN_DA, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_QUAN_XIAO, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// 判断凑一色
	if (pokerCountInfo._blackPeachCount + pokerCountInfo._plumBlossomCount >= POKER_COUNT_SINGLE_PLAYER ||
		pokerCountInfo._redPeachCount + pokerCountInfo._redSquareCount >= POKER_COUNT_SINGLE_PLAYER)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_COU_YI_SE, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_COU_YI_SE, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// 判断四套三条
	if (pokerCountInfo._threesomeCount == 4)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SI_TAO_SAN_TIAO, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SI_TAO_SAN_TIAO, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// 判断五对三条
	if (pokerCountInfo._twosomeCount == 5 &&
		pokerCountInfo._threesomeCount == 1)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_WU_DUI_SAN_TIAO, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_WU_DUI_SAN_TIAO, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// 判断六对半
	if (pokerCountInfo._twosomeCount == 6)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_LIU_DUI_BAN, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_LIU_DUI_BAN, pokerVector, pokerCountInfoPtr));
		return result;
	}


	// 判断三顺子
	if (pokerCountInfo._twosomeCount == 6)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_SHUN_ZI, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_SHUN_ZI, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// 判断三同花
	if ((pokerCountInfo._blackPeachCount == 3 && (pokerCountInfo._redPeachCount == pokerCountInfo._plumBlossomCount || pokerCountInfo._redPeachCount == pokerCountInfo._redSquareCount || pokerCountInfo._plumBlossomCount == pokerCountInfo._redSquareCount)) || // 判断3 5 5 情况
		(pokerCountInfo._redPeachCount == 3 && (pokerCountInfo._blackPeachCount == pokerCountInfo._plumBlossomCount || pokerCountInfo._blackPeachCount == pokerCountInfo._redSquareCount || pokerCountInfo._plumBlossomCount == pokerCountInfo._redSquareCount)) ||
		(pokerCountInfo._plumBlossomCount == 3 && (pokerCountInfo._redPeachCount == pokerCountInfo._blackPeachCount || pokerCountInfo._redPeachCount == pokerCountInfo._redSquareCount || pokerCountInfo._blackPeachCount == pokerCountInfo._redSquareCount)) ||
		(pokerCountInfo._redSquareCount == 3 && (pokerCountInfo._redPeachCount == pokerCountInfo._plumBlossomCount || pokerCountInfo._redPeachCount == pokerCountInfo._blackPeachCount || pokerCountInfo._plumBlossomCount == pokerCountInfo._blackPeachCount)) ||

		(pokerCountInfo._blackPeachCount == 3 && (pokerCountInfo._redPeachCount == 10 || pokerCountInfo._plumBlossomCount == 10 || pokerCountInfo._redSquareCount == 10)) || // 判断 3 10 情况
		(pokerCountInfo._redPeachCount == 3 && (pokerCountInfo._blackPeachCount == 10 || pokerCountInfo._plumBlossomCount == 10 || pokerCountInfo._redSquareCount == 10)) ||
		(pokerCountInfo._plumBlossomCount == 3 && (pokerCountInfo._redPeachCount == 10 || pokerCountInfo._blackPeachCount == 10 || pokerCountInfo._redSquareCount == 10)) ||
		(pokerCountInfo._redSquareCount == 3 && (pokerCountInfo._redPeachCount == 10 || pokerCountInfo._plumBlossomCount == 10 || pokerCountInfo._blackPeachCount == 10)) ||

		(pokerCountInfo._blackPeachCount == 8 && (pokerCountInfo._redPeachCount == 5 || pokerCountInfo._plumBlossomCount == 5 || pokerCountInfo._redSquareCount == 5)) || // 判断 8 5 情况
		(pokerCountInfo._redPeachCount == 8 && (pokerCountInfo._blackPeachCount == 5 || pokerCountInfo._plumBlossomCount == 5 || pokerCountInfo._redSquareCount == 5)) ||
		(pokerCountInfo._plumBlossomCount == 8 && (pokerCountInfo._redPeachCount == 5 || pokerCountInfo._blackPeachCount == 5 || pokerCountInfo._redSquareCount == 5)) ||
		(pokerCountInfo._redSquareCount == 8 && (pokerCountInfo._redPeachCount == 5 || pokerCountInfo._plumBlossomCount == 5 || pokerCountInfo._blackPeachCount == 5))
		)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_TONG_HUA, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_TONG_HUA, pokerVector, pokerCountInfoPtr));
		return result;
	}
	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findChainPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& tonghuaType, const int32_t& countType)
{
	vector<shared_ptr<PokerCombinationModel>> result;

	vector<shared_ptr<PokerModel>> allMixPokerModelVector; // 单个、两个、三个、四个混合牌组合且都只取了其中第一张

	for (auto it = pokerCountInfo->_pokerTypeMap.begin(); it != pokerCountInfo->_pokerTypeMap.end(); ++it)
	{
		allMixPokerModelVector.push_back((it->second).at(0));
	}
	
	int canCutMaxCount = 0; // 允许断开的最大张数
	
	vector<vector<shared_ptr<PokerModel>>> allSortPokerModelVector;

	if (tonghuaType == CHAIN_FIND_TYPE_TONGHUA_ALL || tonghuaType == CHAIN_FIND_TYPE_TONGHUA_ONLY)
	{
		for (auto it = pokerCountInfo->_pokerMarkMap.begin(); it != pokerCountInfo->_pokerMarkMap.end(); ++it)// 按花色分类取牌
		{
			allSortPokerModelVector.push_back(it->second);
		}
	}
	
	if (tonghuaType == CHAIN_FIND_TYPE_TONGHUA_ALL || tonghuaType == CHAIN_FIND_TYPE_TONGHUA_OTHER)
	{
		allSortPokerModelVector.push_back(allMixPokerModelVector);
	}
	

	for (int32_t k = 0; k < allSortPokerModelVector.size(); ++k)
	{
		vector<shared_ptr<PokerModel>>* targetPokerVector = &(allSortPokerModelVector.at(k));
		int sizeAllMix = targetPokerVector->size();
		int tmpIndex = 0;

		for (vector<shared_ptr<PokerModel>>::iterator it = targetPokerVector->begin(); it != targetPokerVector->end();)
		{
			shared_ptr<PokerModel> curPokerModel = *it;

			bool isNextCut = false;
			bool isPreCut = false;

			if (tmpIndex != sizeAllMix - 1) // 检查下一张牌
			{
				shared_ptr<PokerModel> nextPokerModel = *(it + 1);
				if (nextPokerModel->getPokerValueType() - curPokerModel->getPokerValueType() > canCutMaxCount + 1) // 断开超过一张，此牌不需要
				{
					isNextCut = true;
				}
			}
			else
			{
				isNextCut = true;
			}

			if (tmpIndex != 0) // 检查上一张牌
			{
				shared_ptr<PokerModel> prePokerModel = *(it - 1);
				if (curPokerModel->getPokerValueType() - prePokerModel->getPokerValueType() > canCutMaxCount + 1) // 断开超过一张，此牌不需要
				{
					isPreCut = true;
				}
			}
			else
			{
				isPreCut = true;
			}

			if (isPreCut && isNextCut)
			{
				it = targetPokerVector->erase(it);
				sizeAllMix = targetPokerVector->size();
			}
			else
			{
				++tmpIndex;
				++it;
			}
		}
	}


	vector<int32_t> targetPokerCountVec;

	if (countType == CHAIN_FIND_TYPE_COUNT_TYPE_ALL)
	{
		targetPokerCountVec.push_back(POKER_COUNT_ROW_1);
		targetPokerCountVec.push_back(POKER_COUNT_ROW_2);
	}
	else if (countType == CHAIN_FIND_TYPE_COUNT_TYPE_3)
	{
		targetPokerCountVec.push_back(POKER_COUNT_ROW_1);
	}
	else if (countType == CHAIN_FIND_TYPE_COUNT_TYPE_5)
	{
		targetPokerCountVec.push_back(POKER_COUNT_ROW_2);
	}
	
	
	for (int32_t k = 0; k < allSortPokerModelVector.size(); ++k)
	{
		vector<shared_ptr<PokerModel>> targetPokerVector = allSortPokerModelVector.at(k);

		int32_t minCount = POKER_COUNT_ROW_1;
		if (countType == CHAIN_FIND_TYPE_COUNT_TYPE_5)
		{
			minCount = POKER_COUNT_ROW_2;
		}

		if (targetPokerVector.size() >= minCount) // 牌数符合，才进行选择
		{

			bool isTongHua = false; // 是否为同花
			if (tonghuaType == CHAIN_FIND_TYPE_TONGHUA_ALL)
			{
				isTongHua = k != allSortPokerModelVector.size() - 1;
			}
			else if (tonghuaType == CHAIN_FIND_TYPE_TONGHUA_ONLY)
			{
				isTongHua = true;
			}
			else if (tonghuaType == CHAIN_FIND_TYPE_TONGHUA_OTHER)
			{
				isTongHua = false;
			}
			
			for (int32_t j = 0; j < targetPokerCountVec.size(); ++j)
			{
				int32_t targetPokerCount = targetPokerCountVec.at(j);
				vector<shared_ptr<PokerModel>> curFoundChainPokerModelVector; // 本轮找的链子

				shared_ptr<PokerModel> startFindPokerModel = targetPokerVector.front(); // 从某个元素开始找

				for (int32_t i = 0, sizei = targetPokerVector.size(); i < sizei; ++i)
				{
					shared_ptr<PokerModel> curPokerModel = targetPokerVector.at(i);

					int startOriginalValue = startFindPokerModel->getOriginalValue();
					if (curPokerModel->getOriginalValue() < startOriginalValue) // 没有到达预期的开始位置，继续循环
					{
						continue;
					}

					bool isAddedNormalPoker = false; // 本轮循环是否添加了普通扑克到链子中，在本来循环结束时需要判断刚添加的这张牌是单牌、对子、三张、四张的类型
					if (curFoundChainPokerModelVector.empty())
					{
						curFoundChainPokerModelVector.push_back(curPokerModel);
						isAddedNormalPoker = true;
					}
					else
					{
						shared_ptr<PokerModel> lastPokerModel = curFoundChainPokerModelVector.back();

						int lastPokerModelValue = lastPokerModel->getOriginalValue();
						if (curPokerModel->getOriginalValue() - lastPokerModelValue == 1)
						{
							curFoundChainPokerModelVector.push_back(curPokerModel);
							isAddedNormalPoker = true;
						}
						else // 这里断开了连续性
						{
							startFindPokerModel = curPokerModel; // 从当前元素开始查找
							--i; // 将下标往前移动，以便从需要的位置开始查找

							curFoundChainPokerModelVector.clear(); // 清除刚找到的链子，准备下一轮查找

						}

						if (curFoundChainPokerModelVector.size() == targetPokerCount) // 找到一个牌型
						{

							PokerCombinationType type = isTongHua ? PokerCombinationType::POKER_COMBINATION_TYPE_TONG_HUA_SHUN : PokerCombinationType::POKER_COMBINATION_TYPE_SHUN_ZI;
							shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(type, curFoundChainPokerModelVector, getPokerValueByCombination(type, curFoundChainPokerModelVector, nullptr));

							result.push_back(combination);

							startFindPokerModel = curFoundChainPokerModelVector.at(1); // 从这里开始找下一组链子
							i = 0; // 重置下标位置，这里重置为0而不是-1，是因为再次查找时至少是从第二个元素开始查

							curFoundChainPokerModelVector.clear(); // 清除刚找到的链子，准备下一轮查找
						}
					}
				}
			}

		}
	}
	

	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findHuLuPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo)
{

}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findSimplePokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo)
{
	vector<shared_ptr<PokerCombinationModel>> result;
	return result;
}

int32_t SimpleRuleManager::getPokerValueByCombination(const PokerCombinationType& type, const vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo)
{
	int32_t value = pokerCountInfo != nullptr ? pokerCountInfo->_pokerValueCount : getPokerValueTotal(pokerVector);

	
	switch (type)
	{
	case POKER_COMBINATION_TYPE_NONE:
		break;
	case POKER_COMBINATION_TYPE_WU_LONG:
		
		break;
	case POKER_COMBINATION_TYPE_DUI_ZI:
		value += 43000;
		break;
	case POKER_COMBINATION_TYPE_LINAG_DUI:
		value += 44000;
		break;
	case POKER_COMBINATION_TYPE_SAN_TIAO:
		value += 45000;
		break;
	case POKER_COMBINATION_TYPE_SHUN_ZI:
		value += 46000;
		break;
	case POKER_COMBINATION_TYPE_TONG_HUA:
		value += 47000;
		break;
	case POKER_COMBINATION_TYPE_HU_LU:
		value += 48000;
		break;
	case POKER_COMBINATION_TYPE_TIE_ZHI:
		value += 49000;
		break;
	case POKER_COMBINATION_TYPE_TONG_HUA_SHUN:
		value += 50000;
		break;

		// 特殊牌型开始
	case POKER_COMBINATION_TYPE_SAN_TONG_HUA:
		value += 88000;
		break;
	case POKER_COMBINATION_TYPE_SAN_SHUN_ZI:
		value += 89000;
		break;
	case POKER_COMBINATION_TYPE_LIU_DUI_BAN:
		value += 90000;
		break;
	case POKER_COMBINATION_TYPE_WU_DUI_SAN_TIAO:
		value += 91000;
		break;
	case POKER_COMBINATION_TYPE_SI_TAO_SAN_TIAO:
		value += 92000;
		break;
	case POKER_COMBINATION_TYPE_COU_YI_SE:
		value += 93000;
		break;
	case POKER_COMBINATION_TYPE_QUAN_XIAO:
		value += 94000;
		break;
	case POKER_COMBINATION_TYPE_QUAN_DA:
		value += 95000;
		break;
	case POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA:
		value += 96000;
		break;
	case POKER_COMBINATION_TYPE_SAN_TONG_HUA_SHUN:
		value += 97000;
		break;
	case POKER_COMBINATION_TYPE_SHI_ER_HUANG_ZU:
		value += 98000;
		break;
	case POKER_COMBINATION_TYPE_YI_TIAO_LONG:
		value += 99000;
		break;
	case POKER_COMBINATION_TYPE_QING_LONG:
		value += 100000;
		break;
	default:
		break;
	}
	
	return value;
}

