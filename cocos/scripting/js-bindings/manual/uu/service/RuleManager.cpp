/********************************************************************
	created:	2017/06/15
	created:	15:6:2017   21:48
	file base:	RuleManager
	file ext:	cpp
	author:		sulei
	
	purpose:	
*********************************************************************/
#include "RuleManager.h"


////////////////////////////////�˿˸�����Ϣ//////////////////////////////////////////
PokerCountInfo::PokerCountInfo() :_singleCount(0), _twosomeCount(0), _threesomeCount(0), _foursomeCount(0), _kingCount(0), _substituteCount(0)
{
}


////////////////////////////���������������//////////////////////////////////////////////
shared_ptr<PokerCountInfo> RuleManagerAbstract::getPokerCountInfo(vector<shared_ptr<PokerModel>> pokerVector)
{

	shared_ptr<PokerCountInfo> result(new(std::nothrow) PokerCountInfo);
	PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_ASC); // �����˿�

	map<PokerValueType, uint32_t> groupCountMap;
	int32_t groupSize = 0; // ��������

	int32_t kingCount = 0; // �������ĸ���
	int32_t oneCount = 0; // ���Ƹ���
	int32_t twoCount = 0; // ���Ӹ���
	int32_t threeCount = 0; // ���Ÿ���
	int32_t fourCount = 0; // ���Ÿ���
	int32_t substituteCount = 0; // ���ø���

	int32_t blackPeachCount = 0; // ��������
	int32_t redPeachCount = 0; // ��������
	int32_t plumBlossomCount = 0; // ÷������
	int32_t redSquareCount = 0; // �췽����

	int32_t pokerValueCount = 0;// ���ܵ���

	bool isAllTypeSameCount = true;// �Ƿ��������͵��Ƹ���һ��������3344��45678����4443�Ͳ���
	int32_t prePokerValueCount = 0; // �ϴ�ͳ�Ƶ��˿���������

	vector<shared_ptr<PokerModel>> curCountPokerModelVector; // ��ǰͳ�Ƶ��˿�����ģ�ͼ���
	for (uint32_t i = 0, sizeI = pokerVector.size(); i < sizeI; ++i)
	{
		shared_ptr<PokerModel> curPokerModel = pokerVector.at(i);
		PokerValueType curPokerValueType = curPokerModel->getPokerValueType();
		PokerMarkType curPokerMarkType = curPokerModel->getPokerMarkType();

		pokerValueCount += (int32_t)curPokerValueType;

		uint32_t oriValue = groupCountMap[curPokerValueType];
		groupCountMap[curPokerValueType] = ++oriValue; // ����ͳ��ÿ���˿˵ĸ���

		vector<shared_ptr<PokerModel>>* pokerValueVec = &(result->_pokerTypeMap[curPokerValueType]);
		pokerValueVec->push_back(curPokerModel);

		if (curPokerValueType == PokerValueType::VALUE_TYPE_KING_BIG || curPokerValueType == PokerValueType::VALUE_TYPE_KING_SMALL) // ͳ�ơ������ĸ���
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
		vec->push_back(curPokerModel); // ��ӵ�ǰ��ɫ�˿�

		

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

		bool isCanCountThis = false; // �Ƿ���Խ���ͬһ�����˿˵�����ͳ��
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


//////////////////////////�����淨���������  ////////////////////////////////////////////////
RuleType SimpleRuleManager::getRuleType()
{
	return RuleType::RULE_TYPE_SIMPLE;
}

vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> SimpleRuleManager::getPokerCombination(vector<shared_ptr<PokerModel>> pokerVector)
{
	PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_DESC); // ����������

	vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> result;
	shared_ptr<PokerCountInfo> pokerCountInfo = getPokerCountInfo(pokerVector); // ͳ��������Ϣ

	findSpecialPokerCombination(pokerVector, pokerCountInfo);
	return result;
}


shared_ptr<PokerCombinationModel> SimpleRuleManager::findSpecialPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfoPtr)
{
	
	shared_ptr<PokerCombinationModel> result = nullptr;
	int32_t pokerCount = pokerVector.size();

	PokerCountInfo pokerCountInfo = *pokerCountInfoPtr;


	findChainPokerCombination(pokerVector, pokerCountInfoPtr);

	// �ж��Ƿ�Ϊ����
	if (pokerCountInfo._singleCount == POKER_COUNT_SINGLE_PLAYER && (pokerCountInfo._blackPeachCount == POKER_COUNT_SINGLE_PLAYER
		|| pokerCountInfo._redPeachCount == POKER_COUNT_SINGLE_PLAYER
		|| pokerCountInfo._plumBlossomCount == POKER_COUNT_SINGLE_PLAYER
		|| pokerCountInfo._redSquareCount == POKER_COUNT_SINGLE_PLAYER))
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_QING_LONG, pokerVector, pokerCountInfo._pokerValueCount + 100000);
		return result;
	}

	// �ж��Ƿ�Ϊһ����
	if (pokerCountInfo._singleCount == POKER_COUNT_SINGLE_PLAYER)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_YI_TIAO_LONG, pokerVector, pokerCountInfo._pokerValueCount + 99000);
		return result;
	}

	// �ж�ʮ������
	if (pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_A] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_K] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_Q] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_J] >= POKER_COUNT_SINGLE_PLAYER)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_YI_TIAO_LONG, pokerVector, pokerCountInfo._pokerValueCount + 98000);
		return result;
	}

	// �ж���ͬ��˳
	if (false)
	{
		return result;
	}

	// �ж���������
	if (pokerCountInfo._foursomeCount == 3)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA, pokerVector, pokerCountInfo._pokerValueCount + 97000);
		return result;
	}

	// �ж�ȫ��
	if (pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_A] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_K] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_Q] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_J] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_10] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_9] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_8] >= POKER_COUNT_SINGLE_PLAYER)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_QUAN_DA, pokerVector, pokerCountInfo._pokerValueCount + 96000);
		return result;
	}

	// �ж�ȫС
	if (pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_2] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_3] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_4] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_5] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_6] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_7] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_8] >= POKER_COUNT_SINGLE_PLAYER)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_QUAN_DA, pokerVector, pokerCountInfo._pokerValueCount + 95000);
		return result;
	}

	// �жϴ�һɫ
	if (pokerCountInfo._blackPeachCount + pokerCountInfo._plumBlossomCount >= POKER_COUNT_SINGLE_PLAYER ||
		pokerCountInfo._redPeachCount + pokerCountInfo._redSquareCount >= POKER_COUNT_SINGLE_PLAYER)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_COU_YI_SE, pokerVector, pokerCountInfo._pokerValueCount + 94000);
		return result;
	}

	// �ж���������
	if (pokerCountInfo._threesomeCount == 4)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SI_TAO_SAN_TIAO, pokerVector, pokerCountInfo._pokerValueCount + 93000);
		return result;
	}

	// �ж��������
	if (pokerCountInfo._twosomeCount == 5 &&
		pokerCountInfo._threesomeCount == 1)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_WU_DUI_SAN_TIAO, pokerVector, pokerCountInfo._pokerValueCount + 92000);
		return result;
	}

	// �ж����԰�
	if (pokerCountInfo._twosomeCount == 6)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_LIU_DUI_BAN, pokerVector, pokerCountInfo._pokerValueCount + 91000);
		return result;
	}


	// �ж���˳��
	if (pokerCountInfo._twosomeCount == 6)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_LIU_DUI_BAN, pokerVector, pokerCountInfo._pokerValueCount + 90000);
		return result;
	}

	// �ж���ͬ��
	if ((pokerCountInfo._blackPeachCount == 3 && (pokerCountInfo._redPeachCount == pokerCountInfo._plumBlossomCount || pokerCountInfo._redPeachCount == pokerCountInfo._redSquareCount || pokerCountInfo._plumBlossomCount == pokerCountInfo._redSquareCount)) || // �ж�3 5 5 ���
		(pokerCountInfo._redPeachCount == 3 && (pokerCountInfo._blackPeachCount == pokerCountInfo._plumBlossomCount || pokerCountInfo._blackPeachCount == pokerCountInfo._redSquareCount || pokerCountInfo._plumBlossomCount == pokerCountInfo._redSquareCount)) ||
		(pokerCountInfo._plumBlossomCount == 3 && (pokerCountInfo._redPeachCount == pokerCountInfo._blackPeachCount || pokerCountInfo._redPeachCount == pokerCountInfo._redSquareCount || pokerCountInfo._blackPeachCount == pokerCountInfo._redSquareCount)) ||
		(pokerCountInfo._redSquareCount == 3 && (pokerCountInfo._redPeachCount == pokerCountInfo._plumBlossomCount || pokerCountInfo._redPeachCount == pokerCountInfo._blackPeachCount || pokerCountInfo._plumBlossomCount == pokerCountInfo._blackPeachCount)) ||

		(pokerCountInfo._blackPeachCount == 3 && (pokerCountInfo._redPeachCount == 10 || pokerCountInfo._plumBlossomCount == 10 || pokerCountInfo._redSquareCount == 10)) || // �ж� 3 10 ���
		(pokerCountInfo._redPeachCount == 3 && (pokerCountInfo._blackPeachCount == 10 || pokerCountInfo._plumBlossomCount == 10 || pokerCountInfo._redSquareCount == 10)) ||
		(pokerCountInfo._plumBlossomCount == 3 && (pokerCountInfo._redPeachCount == 10 || pokerCountInfo._blackPeachCount == 10 || pokerCountInfo._redSquareCount == 10)) ||
		(pokerCountInfo._redSquareCount == 3 && (pokerCountInfo._redPeachCount == 10 || pokerCountInfo._plumBlossomCount == 10 || pokerCountInfo._blackPeachCount == 10)) ||

		(pokerCountInfo._blackPeachCount == 8 && (pokerCountInfo._redPeachCount == 5 || pokerCountInfo._plumBlossomCount == 5 || pokerCountInfo._redSquareCount == 5)) || // �ж� 8 5 ���
		(pokerCountInfo._redPeachCount == 8 && (pokerCountInfo._blackPeachCount == 5 || pokerCountInfo._plumBlossomCount == 5 || pokerCountInfo._redSquareCount == 5)) ||
		(pokerCountInfo._plumBlossomCount == 8 && (pokerCountInfo._redPeachCount == 5 || pokerCountInfo._blackPeachCount == 5 || pokerCountInfo._redSquareCount == 5)) ||
		(pokerCountInfo._redSquareCount == 8 && (pokerCountInfo._redPeachCount == 5 || pokerCountInfo._plumBlossomCount == 5 || pokerCountInfo._blackPeachCount == 5))
		)
	{
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_TONG_HUA, pokerVector, pokerCountInfo._pokerValueCount + 89000);
		return result;
	}
	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findChainPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo)
{
	vector<shared_ptr<PokerCombinationModel>> result;

	vector<shared_ptr<PokerModel>> allMixPokerModelVector; // �������������������ĸ����������Ҷ�ֻȡ�����е�һ��

	for (auto it = pokerCountInfo->_pokerTypeMap.begin(); it != pokerCountInfo->_pokerTypeMap.end(); ++it)
	{
		allMixPokerModelVector.push_back((it->second).at(0));
	}
	

	int tmpIndex = 0;
	int sizeAllMix = allMixPokerModelVector.size();
	int canCutMaxCount = 0; // ����Ͽ����������
	

	for (vector<shared_ptr<PokerModel>>::iterator it = allMixPokerModelVector.begin(); it != allMixPokerModelVector.end();)
	{
		shared_ptr<PokerModel> curPokerModel = *it;

		bool isNextCut = false;
		bool isPreCut = false;

		if (tmpIndex != sizeAllMix - 1) // �����һ����
		{
			shared_ptr<PokerModel> nextPokerModel = *(it + 1);
			if (nextPokerModel->getPokerValueType() - curPokerModel->getPokerValueType() > canCutMaxCount + 1) // �Ͽ�����һ�ţ����Ʋ���Ҫ
			{
				isNextCut = true;
			}
		}
		else
		{
			isNextCut = true;
		}

		if (tmpIndex != 0) // �����һ����
		{
			shared_ptr<PokerModel> prePokerModel = *(it - 1);
			if (curPokerModel->getPokerValueType() - prePokerModel->getPokerValueType() > canCutMaxCount + 1) // �Ͽ�����һ�ţ����Ʋ���Ҫ
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
			it = allMixPokerModelVector.erase(it);
			sizeAllMix = allMixPokerModelVector.size();
		}
		else
		{
			++tmpIndex;
			++it;
		}
	}


	if (allMixPokerModelVector.size() >= POKER_COUNT_ROW_1) // �������ϣ��Ž���ѡ��
	{
		vector<shared_ptr<PokerModel> > curFoundChainPokerModelVector; // �����ҵ�����
		bool isUsedSubstitute = false;// �ұ������ӣ��Ƿ��Ѿ���������
		bool curHasTwosome = false; // ��ǰ�Ƿ���˶����γɵ�����
		bool curHasThreesome = false;// ��ǰ�Ƿ���������γɵ�����
		bool curHasFoursome = false; // ��ǰ�Ƿ���������γɵ�����

		shared_ptr<PokerModel> startFindPokerModel = allMixPokerModelVector.front(); // ��ĳ��Ԫ�ؿ�ʼ��

		for (int32_t i = 0, sizei = allMixPokerModelVector.size(); i < sizei; ++i)
		{
			shared_ptr<PokerModel> curPokerModel = allMixPokerModelVector.at(i);

			int startOriginalValue = startFindPokerModel->getOriginalValue();
			if (curPokerModel->getOriginalValue() < startOriginalValue) // û�е���Ԥ�ڵĿ�ʼλ�ã�����ѭ��
			{
				continue;
			}

			bool isAddedNormalPoker = false; // ����ѭ���Ƿ��������ͨ�˿˵������У��ڱ���ѭ������ʱ��Ҫ�жϸ���ӵ��������ǵ��ơ����ӡ����š����ŵ�����
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
				else // ����Ͽ���������
				{
					startFindPokerModel = curPokerModel; // �ӵ�ǰԪ�ؿ�ʼ����
					--i; // ���±���ǰ�ƶ����Ա����Ҫ��λ�ÿ�ʼ����

					isUsedSubstitute = false; // �������ʹ��״̬
					curHasTwosome = false;
					curHasThreesome = false;
					curHasFoursome = false;
					curFoundChainPokerModelVector.clear(); // ������ҵ������ӣ�׼����һ�ֲ���
					
				}

				if (isAddedNormalPoker)
				{
					int flagValue = curPokerModel->getIntData();
					switch (flagValue)
					{
					case 1:
					{
					}
						break;
					case 2:
					{
						curHasTwosome = true;
					}
						break;
					case 3:
					{
						curHasThreesome = true;
					}
						break;
					case 4:
					{
						curHasFoursome = true;
					}
						break;
					default:
						break;
					}
				}

				if (curFoundChainPokerModelVector.size() >= POKER_COUNT_ROW_1) // �ҵ�һ������
				{
					shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SHUN_ZI, curFoundChainPokerModelVector, 0);

					result.push_back(combination);

					startFindPokerModel = curFoundChainPokerModelVector.at(1); // �����￪ʼ����һ������
					i = 0; // �����±�λ�ã���������Ϊ0������-1������Ϊ�ٴβ���ʱ�����Ǵӵڶ���Ԫ�ؿ�ʼ��

					isUsedSubstitute = false; // �������ʹ��״̬
					curHasTwosome = false;
					curHasThreesome = false;
					curHasFoursome = false;
					curFoundChainPokerModelVector.clear(); // ������ҵ������ӣ�׼����һ�ֲ���
				}
			}
		}
	}

	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findSimplePokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo)
{
	vector<shared_ptr<PokerCombinationModel>> result;
	return result;
}
