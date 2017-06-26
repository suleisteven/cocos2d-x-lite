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

		uint32_t oriMarkCount = result->_countEveryPokerMarkMap[curPokerMarkType];
		result->_countEveryPokerMarkMap[curPokerMarkType] = ++oriMarkCount; // ����ͳ��ÿ�໨ɫ�˿˵ĸ���
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
	result->_markTypeCount = result->_pokerMarkMap.size();

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

bool RuleManagerAbstract::hasPeculiarCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector)
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


shared_ptr<PokerCombinationModel> RuleManagerAbstract::getPeculiarPokerCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector)
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

#include "cocos2d.h"

vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> SimpleRuleManager::getRecommondPokerCombination(vector<shared_ptr<PokerModel>> pokerVector)
{
	PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_DESC); // ����������

	vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> result;
	shared_ptr<PokerCountInfo> pokerCountInfo = getPokerCountInfo(pokerVector); // ͳ��������Ϣ

	auto time1 = CommonUtils::getCurrentTimeMillis();
	shared_ptr<PokerCombinationModel> peculiarPCM = findPeculiarPokerCombination(pokerVector, pokerCountInfo); // �Ȳ�����������

	if (peculiarPCM)
	{
		shared_ptr<vector<shared_ptr<PokerCombinationModel>>> peculiarPCMVector(new vector<shared_ptr<PokerCombinationModel>>);
		peculiarPCMVector->push_back(peculiarPCM); // ������������
	}
	auto time2 = CommonUtils::getCurrentTimeMillis();

	cocos2d::log("time:%lld", (time2 - time1));

	time1 = CommonUtils::getCurrentTimeMillis();
	findSimplePokerCombination(pokerVector, pokerCountInfo); // �ҳ�������ͨ����
	time2 = CommonUtils::getCurrentTimeMillis();

	cocos2d::log("time2:%lld", (time2 - time1));

	return result;
}

shared_ptr<PokerCombinationModel> SimpleRuleManager::getPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector)
{
	throw std::logic_error("The method or operation is not implemented.");
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findSimplePokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo)
{
	vector<shared_ptr<PokerCombinationModel>> result;

	vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>>  r = findSimplePokerCombinationRecursion(pokerVector, pokerCountInfo, 2);
	
	return result;
}

vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> SimpleRuleManager::findSimplePokerCombinationRecursion(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& step)
{
	vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> result;
	

	int32_t typeCount = 0; // ��������

	vector<shared_ptr<PokerCombinationModel>> tmpVector;

	// ����ͬ��˳����
	vector<shared_ptr<PokerCombinationModel>> tonghuashunPCMVector = findChainPokerCombination(pokerVector, pokerCountInfo, CHAIN_FIND_TYPE_TONGHUA_ONLY, CHAIN_FIND_TYPE_COUNT_TYPE_5);
	tmpVector.insert(tmpVector.end(), tonghuashunPCMVector.begin(), tonghuashunPCMVector.end());

	// ������֧
	vector<shared_ptr<PokerCombinationModel>> tiezhiPCMVector = findTieZhiPokerCombination(pokerVector, pokerCountInfo);
	tmpVector.insert(tmpVector.end(), tiezhiPCMVector.begin(), tiezhiPCMVector.end());
		
	// ���Һ�«
	vector<shared_ptr<PokerCombinationModel>> huluPCMVector = findHuLuPokerCombination(pokerVector, pokerCountInfo);
	tmpVector.insert(tmpVector.end(), huluPCMVector.begin(), huluPCMVector.end());

	// ����ͬ��
	vector<shared_ptr<PokerCombinationModel>> tonghuaPCMVector = findTongHuaPokerCombination(pokerVector, pokerCountInfo);
	tmpVector.insert(tmpVector.end(), tonghuaPCMVector.begin(), tonghuaPCMVector.end());

	// ����˳��
	vector<shared_ptr<PokerCombinationModel>> shunziPCMVector = findChainPokerCombination(pokerVector, pokerCountInfo, CHAIN_FIND_TYPE_TONGHUA_OTHER, CHAIN_FIND_TYPE_COUNT_TYPE_5);
	tmpVector.insert(tmpVector.end(), shunziPCMVector.begin(), shunziPCMVector.end());

	// ��������
	vector<shared_ptr<PokerCombinationModel>> santiaoPCMVector = findSanTiaoPokerCombination(pokerVector, pokerCountInfo);
	tmpVector.insert(tmpVector.end(), santiaoPCMVector.begin(), santiaoPCMVector.end());

	// ��������
	vector<shared_ptr<PokerCombinationModel>> liangduiPCMVector = findLiangDuiPokerCombination(pokerVector, pokerCountInfo);
	tmpVector.insert(tmpVector.end(), liangduiPCMVector.begin(), liangduiPCMVector.end());

	// ���Ҷ���
	vector<shared_ptr<PokerCombinationModel>> duiziPCMVector = findDuiZiPokerCombination(pokerVector, pokerCountInfo);
	tmpVector.insert(tmpVector.end(), duiziPCMVector.begin(), duiziPCMVector.end());

	// ��������
	vector<shared_ptr<PokerCombinationModel>> wulongPCMVector = findWuLongPokerCombination(pokerVector, pokerCountInfo, step == 0 ? WULONG_FIND_TYPE_COUNT_TYPE_3 : WULONG_FIND_TYPE_COUNT_TYPE_5);
	tmpVector.insert(tmpVector.end(), wulongPCMVector.begin(), wulongPCMVector.end());
	

	for (int32_t i = 0, sizeI = tmpVector.size(); i < sizeI;++i)
	{
		shared_ptr<PokerCombinationModel> curPCM = tmpVector.at(i);

		if (step == 0)
		{
			shared_ptr<vector<shared_ptr<PokerCombinationModel>>> curV(new vector<shared_ptr<PokerCombinationModel>>);
			curV->push_back(curPCM);
			result.push_back(curV);
		}
		else
		{
			vector<shared_ptr<PokerModel>> pokerVectorTmp;
			PokerModel::concatSpecifyPokerVector(pokerVectorTmp, pokerVector);
			PokerModel::removeSpecifyPokerVector(pokerVectorTmp, curPCM->getPokerModelVector());


			vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> nextPCMV = findSimplePokerCombinationRecursion(pokerVectorTmp, getPokerCountInfo(pokerVectorTmp), step - 1);

			/*vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> curResult;
			shared_ptr<vector<shared_ptr<PokerCombinationModel>>> curV(new vector<shared_ptr<PokerCombinationModel>>);*/

			//curV->push_back(curPCM);

			for (int32_t j = 0, sizeJ = nextPCMV.size(); j < sizeJ; ++j)
			{
				shared_ptr<vector<shared_ptr<PokerCombinationModel>>> curTmpV = nextPCMV.at(j);
				curTmpV->push_back(curPCM);
				/*for (int32_t k = 0, sizeK = curTmpV.get()->size(); k < sizeK; ++k)
				{
					curV->push_back(curTmpV->at(k));
				}*/
			}
			
			//curResult.push_back(curV);
			result.insert(result.end(), nextPCMV.begin(), nextPCMV.end());
		}
	}
	typeCount++;
	
	return result;
}

shared_ptr<PokerCombinationModel> SimpleRuleManager::findPeculiarPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfoPtr)
{
	
	shared_ptr<PokerCombinationModel> result = nullptr;
	int32_t pokerCount = pokerVector.size();

	PokerCountInfo pokerCountInfo = *pokerCountInfoPtr;

	// �ж��Ƿ�Ϊ����
	if (pokerCountInfo._singleCount == POKER_COUNT_SINGLE_PLAYER && (pokerCountInfo._blackPeachCount == POKER_COUNT_SINGLE_PLAYER
		|| pokerCountInfo._redPeachCount == POKER_COUNT_SINGLE_PLAYER
		|| pokerCountInfo._plumBlossomCount == POKER_COUNT_SINGLE_PLAYER
		|| pokerCountInfo._redSquareCount == POKER_COUNT_SINGLE_PLAYER))
	{
		PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_ASC);
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_QING_LONG, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_QING_LONG, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// �ж��Ƿ�Ϊһ����
	if (pokerCountInfo._singleCount == POKER_COUNT_SINGLE_PLAYER)
	{
		PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_ASC);
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_YI_TIAO_LONG, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_YI_TIAO_LONG, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// �ж�ʮ������
	if (pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_A] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_K] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_Q] +
		pokerCountInfo._countEveryPokerTypeMap[PokerValueType::VALUE_TYPE_J] >= POKER_COUNT_SINGLE_PLAYER)
	{
		PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_ASC);
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SHI_ER_HUANG_ZU, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SHI_ER_HUANG_ZU, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// �ж���ͬ��˳�����ж���ͬ����
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
		(pokerCountInfo._redSquareCount == 8 && (pokerCountInfo._redPeachCount == 5 || pokerCountInfo._plumBlossomCount == 5 || pokerCountInfo._blackPeachCount == 5)))
	{

		vector<shared_ptr<PokerModel>> foundResultVector;

		vector<shared_ptr<PokerCombinationModel>> chainAll5 = findChainPokerCombination(pokerVector, pokerCountInfoPtr, CHAIN_FIND_TYPE_TONGHUA_ONLY, CHAIN_FIND_TYPE_COUNT_TYPE_5); // ���ҳ�5��ͬ��˳�ļ���
		if (chainAll5.size() >= 2) // ������ͬ��˳�Ŀ���
		{
			vector<shared_ptr<PokerCombinationModel>> chainLong = findChainPokerCombination(pokerVector, pokerCountInfoPtr, CHAIN_FIND_TYPE_TONGHUA_ONLY, CHAIN_FIND_TYPE_COUNT_TYPE_LONG); // �ҳ����ͬ��˳
			if (!chainLong.empty())
			{
				shared_ptr<PokerCombinationModel> longPCM = chainLong.front();
				if (longPCM->getPokerCount() == 10) // 3 10
				{

					vector<shared_ptr<PokerModel>> pokerVectorTmp;
					pokerVectorTmp.insert(pokerVectorTmp.end(), pokerVector.begin(), pokerVector.end());
					PokerModel::removeSpecifyPokerVector(pokerVectorTmp, longPCM->getPokerModelVector()); // �ų���������

					vector<shared_ptr<PokerCombinationModel>> chain3 = findChainPokerCombination(pokerVectorTmp, getPokerCountInfo(pokerVectorTmp), CHAIN_FIND_TYPE_TONGHUA_ONLY, CHAIN_FIND_TYPE_COUNT_TYPE_3); // �ҳ����ŵ�ͬ��˳
					if (!chain3.empty())
					{
						shared_ptr<PokerCombinationModel> pcm3 = chain3.front();

						// ��ӽ��
						PokerModel::concatSpecifyPokerVector(foundResultVector, pcm3->getPokerModelVector());
						PokerModel::concatSpecifyPokerVector(foundResultVector, longPCM->getPokerModelVector());
					}

				}
				else if (longPCM->getPokerCount() == 8) // 3 8
				{
					vector<shared_ptr<PokerModel>> pokerVectorTmp;
					pokerVectorTmp.insert(pokerVectorTmp.end(), pokerVector.begin(), pokerVector.end());

					PokerModel::removeSpecifyPokerVector(pokerVectorTmp, longPCM->getPokerModelVector()); // �ų���������

					vector<shared_ptr<PokerCombinationModel>> chain5 = findChainPokerCombination(pokerVectorTmp, getPokerCountInfo(pokerVectorTmp), CHAIN_FIND_TYPE_TONGHUA_ONLY, CHAIN_FIND_TYPE_COUNT_TYPE_5); // �ҳ�5�ŵ�ͬ��˳
					if (!chain5.empty())
					{
						shared_ptr<PokerCombinationModel> pcm5 = chain5.front();

						// ��ӽ��
						PokerModel::concatSpecifyPokerVector(foundResultVector, longPCM->getPokerModelVector());
						PokerModel::concatSpecifyPokerVector(foundResultVector, pcm5->getPokerModelVector());
					}
				}
				else if (longPCM->getPokerCount() == 5) // 3 5 5
				{
					if (chainAll5.size() == 2)
					{
						vector<shared_ptr<PokerModel>> pokerVectorTmp;
						pokerVectorTmp.insert(pokerVectorTmp.end(), pokerVector.begin(), pokerVector.end());

						vector<shared_ptr<PokerModel>> pokerVectorMergeTmp;
						PokerModel::mergeSpecifyPokerVector(pokerVectorMergeTmp, chainAll5.at(0).get()->getPokerModelVector());
						PokerModel::mergeSpecifyPokerVector(pokerVectorMergeTmp, chainAll5.at(1).get()->getPokerModelVector());

						PokerModel::removeSpecifyPokerVector(pokerVectorTmp, pokerVectorMergeTmp); // �ų���������

						vector<shared_ptr<PokerCombinationModel>> chain3 = findChainPokerCombination(pokerVectorTmp, getPokerCountInfo(pokerVectorTmp), CHAIN_FIND_TYPE_TONGHUA_ONLY, CHAIN_FIND_TYPE_COUNT_TYPE_3); // �ҳ����ŵ�ͬ��˳
						if (!chain3.empty())
						{
							shared_ptr<PokerCombinationModel> pcm3 = chain3.front();

							// ��ӽ��
							PokerModel::concatSpecifyPokerVector(foundResultVector, pcm3->getPokerModelVector());
							PokerModel::concatSpecifyPokerVector(foundResultVector, pokerVectorMergeTmp);
						}
					}
				}
			}
		}

		// �ҵ���ͬ��˳
		if (!foundResultVector.empty())
		{
			result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA, foundResultVector, pokerCountInfoPtr));
			return result;
		}
	}

	// �ж���������
	if (pokerCountInfo._foursomeCount == 3)
	{
		PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_ASC);
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA, pokerVector, pokerCountInfoPtr));
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
		PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_ASC);
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_QUAN_DA, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_QUAN_DA, pokerVector, pokerCountInfoPtr));
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
		PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_ASC);
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_QUAN_DA, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_QUAN_XIAO, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// �жϴ�һɫ
	if (pokerCountInfo._blackPeachCount + pokerCountInfo._plumBlossomCount >= POKER_COUNT_SINGLE_PLAYER ||
		pokerCountInfo._redPeachCount + pokerCountInfo._redSquareCount >= POKER_COUNT_SINGLE_PLAYER)
	{
		PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_ASC, SortPokerType::SORT_POKER_TYPE_MARK_FIRST); // ��ɫ��������
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_COU_YI_SE, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_COU_YI_SE, pokerVector, pokerCountInfoPtr));
		return result;
	}

	// �ж���������
	if (pokerCountInfo._threesomeCount == 4)
	{
		vector<shared_ptr<vector<shared_ptr<PokerModel>>>> tmpV = pokerCountInfoPtr->_everyCountPokerMap[3];

		vector<shared_ptr<PokerModel>> tmpVector;

		for (int32_t i = 0; i < tmpV.size(); ++i)
		{
			shared_ptr<vector<shared_ptr<PokerModel>>> tmp = tmpV.at(i);
			tmpVector.insert(tmpVector.end(), tmp->begin(), tmp->end());
		}
		PokerModel::sortPokerModel(tmpVector, SortType::SORT_TYPE_ASC);

		vector<shared_ptr<vector<shared_ptr<PokerModel>>>> tmpV2 = pokerCountInfoPtr->_everyCountPokerMap[1];
		tmpVector.push_back(tmpV2.front().get()->front());

		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SI_TAO_SAN_TIAO, tmpVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SI_TAO_SAN_TIAO, tmpVector, nullptr));
		return result;
	}

	// �ж��������
	if (pokerCountInfo._twosomeCount == 5 &&
		pokerCountInfo._threesomeCount == 1)
	{
		vector<shared_ptr<vector<shared_ptr<PokerModel>>>> tmpV = pokerCountInfoPtr->_everyCountPokerMap[2];

		vector<shared_ptr<PokerModel>> tmpVector;

		for (int32_t i = 0; i < tmpV.size(); ++i)
		{
			shared_ptr<vector<shared_ptr<PokerModel>>> tmp = tmpV.at(i);
			tmpVector.insert(tmpVector.end(), tmp->begin(), tmp->end());
		}
		PokerModel::sortPokerModel(tmpVector, SortType::SORT_TYPE_ASC);

		vector<shared_ptr<vector<shared_ptr<PokerModel>>>> tmpV2 = pokerCountInfoPtr->_everyCountPokerMap[3];
		for (int32_t i = 0; i < tmpV2.size(); ++i)
		{
			shared_ptr<vector<shared_ptr<PokerModel>>> tmp = tmpV2.at(i);
			tmpVector.insert(tmpVector.end(), tmp->begin(), tmp->end());
		}

		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_WU_DUI_SAN_TIAO, tmpVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_WU_DUI_SAN_TIAO, tmpVector, nullptr));
		return result;
	}

	// �ж����԰�
	if (pokerCountInfo._twosomeCount == 6)
	{
		vector<shared_ptr<vector<shared_ptr<PokerModel>>>> tmpV = pokerCountInfoPtr->_everyCountPokerMap[2];

		vector<shared_ptr<PokerModel>> tmpVector;

		for (int32_t i = 0; i < tmpV.size(); ++i)
		{
			shared_ptr<vector<shared_ptr<PokerModel>>> tmp = tmpV.at(i);
			tmpVector.insert(tmpVector.end(), tmp->begin(), tmp->end());
		}
		PokerModel::sortPokerModel(tmpVector, SortType::SORT_TYPE_ASC);

		vector<shared_ptr<vector<shared_ptr<PokerModel>>>> tmpV2 = pokerCountInfoPtr->_everyCountPokerMap[1];
		tmpVector.push_back(tmpV2.front().get()->front());

		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_LIU_DUI_BAN, tmpVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_LIU_DUI_BAN, tmpVector, nullptr));
		return result;
	}


	// �ж���˳��
	{
		vector<shared_ptr<PokerModel>> foundResultVector;

		vector<shared_ptr<PokerCombinationModel>> chainAll5 = findChainPokerCombination(pokerVector, pokerCountInfoPtr, CHAIN_FIND_TYPE_TONGHUA_OTHER, CHAIN_FIND_TYPE_COUNT_TYPE_5); // ���ҳ�5���Ƶ�˳��
		

		if (chainAll5.size() >= 2) // ��������˳��
		{
			vector<shared_ptr<PokerCombinationModel>> chainLong = findChainPokerCombination(pokerVector, pokerCountInfoPtr, CHAIN_FIND_TYPE_TONGHUA_OTHER, CHAIN_FIND_TYPE_COUNT_TYPE_LONG); // �ҳ������
			if (!chainLong.empty())
			{
				
				if (chainAll5.size() >= 2) // ����˳�ӵĿ�����
				{
					for (int32_t i = chainAll5.size()-1; i >= 0; --i) // �Ӵ���С����
					{
						 
						shared_ptr<PokerCombinationModel> curChain = chainAll5.at(i);
						vector<shared_ptr<PokerModel>> pokerVectorTmp;

						PokerModel::concatSpecifyPokerVector(pokerVectorTmp, pokerVector);
						PokerModel::removeSpecifyPokerVector(pokerVectorTmp, curChain->getPokerModelVector());
						
						vector<shared_ptr<PokerCombinationModel>> chainAll5_2 = findChainPokerCombination(pokerVectorTmp, getPokerCountInfo(pokerVectorTmp), CHAIN_FIND_TYPE_TONGHUA_OTHER, CHAIN_FIND_TYPE_COUNT_TYPE_5); // �ҳ��е�5���Ƶ�˳��
						if (!chainAll5_2.empty())
						{
							for (int32_t j = chainAll5_2.size() - 1; j >=0; --j)
							{
								shared_ptr<PokerCombinationModel> curChain_2 = chainAll5_2.at(j);
								vector<shared_ptr<PokerModel>> pokerVectorTmp_2;

								PokerModel::concatSpecifyPokerVector(pokerVectorTmp_2, pokerVectorTmp);
								PokerModel::removeSpecifyPokerVector(pokerVectorTmp_2, curChain_2->getPokerModelVector());

								vector<shared_ptr<PokerCombinationModel>> chainAll5_1 = findChainPokerCombination(pokerVectorTmp_2, getPokerCountInfo(pokerVectorTmp_2), CHAIN_FIND_TYPE_TONGHUA_OTHER, CHAIN_FIND_TYPE_COUNT_TYPE_3); // �ҳ�ͷ��˳��

								if (!chainAll5_1.empty()) // �ҵ���˳��
								{
									PokerModel::concatSpecifyPokerVector(foundResultVector, chainAll5_1.front().get()->getPokerModelVector()); // ���ͷ��
									PokerModel::concatSpecifyPokerVector(foundResultVector, curChain_2.get()->getPokerModelVector()); // ����е�
									PokerModel::concatSpecifyPokerVector(foundResultVector, curChain.get()->getPokerModelVector()); // ���β��
									break;
								}
							}
						}
					}
					
				}
			}
		}

		if (!foundResultVector.empty())
		{
			result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_SHUN_ZI, foundResultVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_SHUN_ZI, foundResultVector, pokerCountInfoPtr));
			return result;
		}
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
		(pokerCountInfo._redSquareCount == 8 && (pokerCountInfo._redPeachCount == 5 || pokerCountInfo._plumBlossomCount == 5 || pokerCountInfo._blackPeachCount == 5)))
	{

		PokerModel::sortPokerModel(pokerVector, SortType::SORT_TYPE_ASC, SortPokerType::SORT_POKER_TYPE_MARK_FIRST); // ��ɫ��������
		result = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_TONG_HUA, pokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_TONG_HUA, pokerVector, pokerCountInfoPtr));
		return result;
	}
	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findChainPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& tonghuaType, const int32_t& countType)
{
	vector<shared_ptr<PokerCombinationModel>> result;

	vector<shared_ptr<PokerModel>> allMixPokerModelVector; // �������������������ĸ����������Ҷ�ֻȡ�����е�һ��

	for (auto it = pokerCountInfo->_pokerTypeMap.begin(); it != pokerCountInfo->_pokerTypeMap.end(); ++it)
	{
		allMixPokerModelVector.push_back((it->second).at(0));
	}
	
	int canCutMaxCount = 0; // ����Ͽ����������
	
	vector<vector<shared_ptr<PokerModel>>> allSortPokerModelVector;

	if (tonghuaType == CHAIN_FIND_TYPE_TONGHUA_ALL || tonghuaType == CHAIN_FIND_TYPE_TONGHUA_ONLY)
	{
		for (auto it = pokerCountInfo->_pokerMarkMap.begin(); it != pokerCountInfo->_pokerMarkMap.end(); ++it)// ����ɫ����ȡ��
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

	vector<shared_ptr<PokerModel>> curLongVector; // ��ǰ��������Ƽ���
	bool curLongIsTongHua = false;
	vector<int32_t> targetPokerCountVec;

	if (countType == CHAIN_FIND_TYPE_COUNT_TYPE_3_OR_5)
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
	else if (countType == CHAIN_FIND_TYPE_COUNT_TYPE_LONG)
	{
		targetPokerCountVec.push_back(-1);
	}
	
	
	for (int32_t k = 0; k < allSortPokerModelVector.size(); ++k)
	{
		vector<shared_ptr<PokerModel>> targetPokerVector = allSortPokerModelVector.at(k);

		int32_t minCount = POKER_COUNT_ROW_1;
		if (countType == CHAIN_FIND_TYPE_COUNT_TYPE_5)
		{
			minCount = POKER_COUNT_ROW_2;
		}

		if (targetPokerVector.size() >= minCount) // �������ϣ��Ž���ѡ��
		{

			bool isTongHua = false; // �Ƿ�Ϊͬ��
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
			
			bool isCut = false; // �Ƿ�Ͽ�����
			bool isFound = false; // �Ƿ��ҵ�Ŀ������
			for (int32_t j = 0; j < targetPokerCountVec.size(); ++j)
			{
				int32_t targetPokerCount = targetPokerCountVec.at(j);

				vector<shared_ptr<PokerCombinationModel>> curTargetPokerCountResult; // ��ǰָ�������ҵ������Ӽ���

				vector<shared_ptr<PokerModel>> curFoundChainPokerModelVector; // �����ҵ�����

				shared_ptr<PokerModel> startFindPokerModel = targetPokerVector.front(); // ��ĳ��Ԫ�ؿ�ʼ��

				
				for (int32_t i = 0, sizei = targetPokerVector.size(); i < sizei; ++i)
				{
					isCut = false;
					isFound = false;

					shared_ptr<PokerModel> curPokerModel = targetPokerVector.at(i);
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

							isCut = true;
						}

						if (curFoundChainPokerModelVector.size() == targetPokerCount) // �ҵ�һ������
						{

							PokerCombinationType type = isTongHua ? PokerCombinationType::POKER_COMBINATION_TYPE_TONG_HUA_SHUN : PokerCombinationType::POKER_COMBINATION_TYPE_SHUN_ZI;
							shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(type, curFoundChainPokerModelVector, getPokerValueByCombination(type, curFoundChainPokerModelVector, nullptr));

							curTargetPokerCountResult.push_back(combination);
							
							startFindPokerModel = curFoundChainPokerModelVector.at(1); // �����￪ʼ����һ������
							i = 0; // �����±�λ�ã���������Ϊ0������-1������Ϊ�ٴβ���ʱ�����Ǵӵڶ���Ԫ�ؿ�ʼ��

							isFound = true;
						}

						if (i == sizei - 1 || isCut)
						{
							if (targetPokerCount == -1) // ��������ӣ���¼�´˴�����
							{
								if (curFoundChainPokerModelVector.size() >= curLongVector.size())
								{
									curLongVector.clear();
									curLongVector.insert(curLongVector.end(), curFoundChainPokerModelVector.begin(), curFoundChainPokerModelVector.end());
									curLongIsTongHua = isTongHua;
								}
							}


							if (curTargetPokerCountResult.size() >= 2) // ��ǰ���ҵ���һ�������У������õ����ӣ����磺2345678���ҵ�23456��34567��45678������34567���������ӣ�
							{
								result.push_back(curTargetPokerCountResult.front());
								result.push_back(curTargetPokerCountResult.back());
							}
							else if (curTargetPokerCountResult.size() == 1)
							{
								result.push_back(curTargetPokerCountResult.front());
							}
							curTargetPokerCountResult.clear(); // ��յ�ǰ�ҵ���һ�����ӣ�׼������һ��
						}

						if (isCut || isFound)
						{
							curFoundChainPokerModelVector.clear(); // ������ҵ������ӣ�׼����һ�ֲ���
						}
					}
				}
			}
		}
	}
	
	if (curLongVector.size() > 0)
	{
		PokerCombinationType type = curLongIsTongHua ? PokerCombinationType::POKER_COMBINATION_TYPE_TONG_HUA_SHUN : PokerCombinationType::POKER_COMBINATION_TYPE_SHUN_ZI;
		shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(type, curLongVector, getPokerValueByCombination(type, curLongVector, nullptr));
		result.push_back(combination);
	}
	

	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findHuLuPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo)
{
	vector<shared_ptr<PokerCombinationModel>> result;
	
	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_3 =pokerCountInfo->_everyCountPokerMap[3];
	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_2 = pokerCountInfo->_everyCountPokerMap[2];

	for (int32_t i = 0, sizeI = pokerVector_3.size(); i < sizeI;++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker3 = pokerVector_3.at(i);
		for (int32_t j = 0, sizeJ = pokerVector_2.size(); j < sizeJ; ++j)
		{
			shared_ptr<vector<shared_ptr<PokerModel>>> curPoker2 = pokerVector_2.at(j);

			// �ҵ���«����
			vector<shared_ptr<PokerModel>> tmpPokerVector;
			tmpPokerVector.insert(tmpPokerVector.end(), curPoker3->begin(), curPoker3->end());
			tmpPokerVector.insert(tmpPokerVector.end(), curPoker2->begin(), curPoker2->end());

			shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_HU_LU, tmpPokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_HU_LU, tmpPokerVector, nullptr));
			result.push_back(combination);
		}
	}

	return result;
}



vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findTieZhiPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo)
{
	vector<shared_ptr<PokerCombinationModel>> result;

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_4 = pokerCountInfo->_everyCountPokerMap[4];
	

	for (int32_t i = 0, sizeI = pokerVector_4.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker4 = pokerVector_4.at(i);

		// �ҵ���֧����
		vector<shared_ptr<PokerModel>> tmpPokerVector;
		tmpPokerVector.insert(tmpPokerVector.end(), curPoker4->begin(), curPoker4->end());
		shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_TIE_ZHI, tmpPokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_TIE_ZHI, tmpPokerVector, nullptr));
		result.push_back(combination);
	}

	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findTongHuaPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo)
{
	vector<shared_ptr<PokerCombinationModel>> result;

	vector<vector<shared_ptr<PokerModel>>> allSortPokerModelVector;
	for (auto it = pokerCountInfo->_pokerMarkMap.begin(); it != pokerCountInfo->_pokerMarkMap.end(); ++it)// ����ɫ����ȡ��
	{
		vector<shared_ptr<PokerModel>> curMarkVector = it->second;
		if (curMarkVector.size() >= POKER_COUNT_ROW_2)
		{

			// �ҵ�ͬ������
			vector<shared_ptr<PokerModel>> tmpPokerVector;
			tmpPokerVector.insert(tmpPokerVector.end(), curMarkVector.begin(), curMarkVector.begin() + POKER_COUNT_ROW_2);
			shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_TONG_HUA, tmpPokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_TONG_HUA, tmpPokerVector, nullptr));
			result.push_back(combination);
		}
	}
	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findSanTiaoPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo)
{
	vector<shared_ptr<PokerCombinationModel>> result;

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_3 = pokerCountInfo->_everyCountPokerMap[3];

	for (int32_t i = 0, sizeI = pokerVector_3.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker3 = pokerVector_3.at(i);

		// �ҵ���������
		vector<shared_ptr<PokerModel>> tmpPokerVector;
		tmpPokerVector.insert(tmpPokerVector.end(), curPoker3->begin(), curPoker3->end());
		shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_TIAO, tmpPokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_TIAO, tmpPokerVector, nullptr));
		result.push_back(combination);
	}

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_4 = pokerCountInfo->_everyCountPokerMap[4];
	for (int32_t i = 0, sizeI = pokerVector_4.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker4 = pokerVector_4.at(i);

		// �ҵ���������
		vector<shared_ptr<PokerModel>> tmpPokerVector;
		tmpPokerVector.insert(tmpPokerVector.end(), curPoker4->begin(), curPoker4->end()-1);
		shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_TIAO, tmpPokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_SAN_TIAO, tmpPokerVector, nullptr));
		result.push_back(combination);
	}

	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findLiangDuiPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo)
{
	vector<shared_ptr<PokerCombinationModel>> result;


	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> twosomeVector;

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_4 = pokerCountInfo->_everyCountPokerMap[4];
	for (int32_t i = 0, sizeI = pokerVector_4.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker4 = pokerVector_4.at(i);

		shared_ptr<vector<shared_ptr<PokerModel>>> tmp(new vector<shared_ptr<PokerModel>>);

		tmp->insert(tmp->end(), curPoker4->begin(), curPoker4->begin() + 2);
		twosomeVector.push_back(tmp);
	}

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_3 = pokerCountInfo->_everyCountPokerMap[3];
	for (int32_t i = 0, sizeI = pokerVector_3.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker3 = pokerVector_3.at(i);

		shared_ptr<vector<shared_ptr<PokerModel>>> tmp(new vector<shared_ptr<PokerModel>>);

		tmp->insert(tmp->end(), curPoker3->begin(), curPoker3->begin() + 2);
		twosomeVector.push_back(tmp);
	}

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_2 = pokerCountInfo->_everyCountPokerMap[2];
	for (int32_t i = 0, sizeI = pokerVector_2.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker2 = pokerVector_2.at(i);

		shared_ptr<vector<shared_ptr<PokerModel>>> tmp(new vector<shared_ptr<PokerModel>>);

		tmp->insert(tmp->end(), curPoker2->begin(), curPoker2->end());
		twosomeVector.push_back(tmp);
	}

	for (int32_t i = 0, sizeI = twosomeVector.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curTwosomeI = twosomeVector.at(i);
		for (int32_t j = i; j < sizeI; ++j)
		{
			shared_ptr<vector<shared_ptr<PokerModel>>> curTwosomeJ = twosomeVector.at(j);

			if (curTwosomeI->front().get()->getPokerValueType() != curTwosomeJ->front().get()->getPokerValueType())
			{

				// �ҵ���������
				vector<shared_ptr<PokerModel>> tmpPokerVector;
				tmpPokerVector.insert(tmpPokerVector.end(), curTwosomeI->begin(), curTwosomeI->end());
				tmpPokerVector.insert(tmpPokerVector.end(), curTwosomeJ->begin(), curTwosomeJ->end());
				shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_LIANG_DUI, tmpPokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_LIANG_DUI, tmpPokerVector, nullptr));
				result.push_back(combination);
			}
		}
	}

	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findDuiZiPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo)
{
	vector<shared_ptr<PokerCombinationModel>> result;

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_4 = pokerCountInfo->_everyCountPokerMap[4];
	for (int32_t i = 0, sizeI = pokerVector_4.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker4 = pokerVector_4.at(i);

		// �ҵ���������
		vector<shared_ptr<PokerModel>> tmpPokerVector;
		tmpPokerVector.insert(tmpPokerVector.end(), curPoker4->begin(), curPoker4->end()-2);
		shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_DUI_ZI, tmpPokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_DUI_ZI, tmpPokerVector, nullptr));
		result.push_back(combination);
	}

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_3 = pokerCountInfo->_everyCountPokerMap[3];
	for (int32_t i = 0, sizeI = pokerVector_3.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker3 = pokerVector_3.at(i);

		// �ҵ���������
		vector<shared_ptr<PokerModel>> tmpPokerVector;
		tmpPokerVector.insert(tmpPokerVector.end(), curPoker3->begin(), curPoker3->end()-1);
		shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_DUI_ZI, tmpPokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_DUI_ZI, tmpPokerVector, nullptr));
		result.push_back(combination);
	}

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_2 = pokerCountInfo->_everyCountPokerMap[2];
	for (int32_t i = 0, sizeI = pokerVector_2.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker2 = pokerVector_2.at(i);

		// �ҵ���������
		vector<shared_ptr<PokerModel>> tmpPokerVector;
		tmpPokerVector.insert(tmpPokerVector.end(), curPoker2->begin(), curPoker2->end());
		shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_DUI_ZI, tmpPokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_DUI_ZI, tmpPokerVector, nullptr));
		result.push_back(combination);
	}

	return result;
}

vector<shared_ptr<PokerCombinationModel>> SimpleRuleManager::findWuLongPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& wulongCountType)
{
	vector<shared_ptr<PokerCombinationModel>> result;

	vector<int32_t> targetPokerCountVec;

	if (wulongCountType == WULONG_FIND_TYPE_COUNT_TYPE_3_OR_5)
	{
		targetPokerCountVec.push_back(POKER_COUNT_ROW_1);
		targetPokerCountVec.push_back(POKER_COUNT_ROW_2);
	}
	else if (wulongCountType == WULONG_FIND_TYPE_COUNT_TYPE_3)
	{
		targetPokerCountVec.push_back(POKER_COUNT_ROW_1);
	}
	else if (wulongCountType == WULONG_FIND_TYPE_COUNT_TYPE_5)
	{
		targetPokerCountVec.push_back(POKER_COUNT_ROW_2);
	}

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_1 = pokerCountInfo->_everyCountPokerMap[1];


	vector < shared_ptr<PokerModel>> singlePokerVector;
	for (int32_t i = 0, sizeI = pokerVector_1.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker1 = pokerVector_1.at(i);
		singlePokerVector.push_back(curPoker1->front());
	}

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_2 = pokerCountInfo->_everyCountPokerMap[2];
	for (int32_t i = 0, sizeI = pokerVector_2.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker2 = pokerVector_2.at(i);
		singlePokerVector.push_back(curPoker2->front());
	}

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_3 = pokerCountInfo->_everyCountPokerMap[3];
	for (int32_t i = 0, sizeI = pokerVector_3.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker3 = pokerVector_3.at(i);
		singlePokerVector.push_back(curPoker3->front());
	}

	vector<shared_ptr<vector<shared_ptr<PokerModel>>>> pokerVector_4 = pokerCountInfo->_everyCountPokerMap[4];
	for (int32_t i = 0, sizeI = pokerVector_4.size(); i < sizeI; ++i)
	{
		shared_ptr<vector<shared_ptr<PokerModel>>> curPoker4 = pokerVector_4.at(i);

		singlePokerVector.push_back(curPoker4->front());
	}
	
	for (int32_t j = 0, sizeJ = targetPokerCountVec.size(); j < sizeJ; ++j)
	{
		int32_t count = targetPokerCountVec.at(j);
		

		// �ҵ���������
		vector<shared_ptr<PokerModel>> tmpPokerVector;

		if (singlePokerVector.size() >= count)
		{
			tmpPokerVector.insert(tmpPokerVector.end(), singlePokerVector.begin(), singlePokerVector.begin() + count);

			bool isWuLong = true;
			if (count == POKER_COUNT_ROW_2)
			{

				int32_t curIndex = count-1;
				while (true)
				{
					if (curIndex == singlePokerVector.size() - 1)
					{
						isWuLong = false;
						break; // û���ҵ����ʵ�����
					}
					
					// �ж��Ƿ�������
					if (tmpPokerVector.back().get()->getPokerValueType() == tmpPokerVector.front().get()->getPokerValueType() + (count - 1))
					{
						tmpPokerVector.pop_back();
						tmpPokerVector.push_back(singlePokerVector.at(++curIndex));
					}
					else
					{
						break;
					}
				}
			}
			
			if (isWuLong)
			{
				shared_ptr<PokerCombinationModel> combination = PokerCombinationModel::create(PokerCombinationType::POKER_COMBINATION_TYPE_WU_LONG, tmpPokerVector, getPokerValueByCombination(PokerCombinationType::POKER_COMBINATION_TYPE_WU_LONG, tmpPokerVector, nullptr));
				result.push_back(combination);
			}
		}
	}

	return result;
}

int32_t SimpleRuleManager::getPokerValueByCombination(const PokerCombinationType& type, const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo /*= nullptr*/)
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
	case POKER_COMBINATION_TYPE_LIANG_DUI:
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

		// �������Ϳ�ʼ
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



