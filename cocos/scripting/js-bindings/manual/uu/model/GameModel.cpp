/********************************************************************
	created:	2017/06/15
	created:	15:6:2017   21:48
	file base:	GameModel
	file ext:	cpp
	author:		sulei
	
	purpose:	
*********************************************************************/
#include "GameModel.h"


/////////////////////////扑克模型数据//////////////////////////////////////
PokerModel::PokerModel(PokerValueType pokerValueType, PokerMarkType pokerMarkType, int32_t originalValue, int32_t sortValue)
{
	this->_pokerValueType = pokerValueType;
	this->_pokerMarkType = pokerMarkType;
	this->_originalValue = originalValue;
	this->_sortValue = sortValue;

	clearChanged();
}

PokerModel::~PokerModel()
{

}

shared_ptr<PokerModel> PokerModel::clone()
{
	shared_ptr<PokerModel> ret(new(std::nothrow) PokerModel(this->_pokerValueType, this->_pokerMarkType, this->_originalValue, this->_sortValue));
	if (ret)
	{
		ret->_changedPokerValueType = this->_changedPokerValueType;
		ret->_changedPokerMarkType = this->_changedPokerMarkType;
		ret->_changedValue = this->_changedValue;
		ret->_changedSortValue = this->_changedSortValue;
		ret->_isChanged = this->_isChanged;
	}
	return ret;
}

void PokerModel::sortPokerModel(vector<shared_ptr<PokerModel>>& pokerModelVector, const SortType& sortType, const SortPokerType& sortPokerType/* = SortPokerType::SORT_POKER_TYPE_VALUE_FIRST*/, const bool& sortSubstitute/* = true*/)
{
	std::sort(pokerModelVector.begin(), pokerModelVector.end(), [=](shared_ptr<PokerModel> pokerModel1, shared_ptr<PokerModel> pokerModel2)
	{
		bool result = false;

		int32_t sortValue1 = 0;
		int32_t sortValue2 = 0;


		if (sortPokerType == SortPokerType::SORT_POKER_TYPE_VALUE_FIRST)
		{
			if (sortSubstitute && pokerModel1->isChanged())
			{
				sortValue1 = pokerModel1->getChangedSortValue();
			}
			else
			{
				sortValue1 = pokerModel1->getSortValue();
			}

			if (sortSubstitute && pokerModel2->isChanged())
			{
				sortValue2 = pokerModel2->getChangedSortValue();
			}
			else
			{
				sortValue2 = pokerModel2->getSortValue();
			}

			if (sortValue1 != sortValue2)
			{
				if (sortType == SORT_TYPE_ASC)
				{

					result = sortValue1 < sortValue2;
				}
				else if (sortType == SORT_TYPE_DESC)
				{
					result = sortValue2 < sortValue1;
				}
			}
			else
			{
				if (sortSubstitute && pokerModel1->isChanged())
				{
					sortValue1 = pokerModel1->getChangedPokerMarkType();
				}
				else
				{
					sortValue1 = pokerModel1->getPokerMarkType();
				}

				if (sortSubstitute && pokerModel2->isChanged())
				{
					sortValue2 = pokerModel2->getChangedPokerMarkType();
				}
				else
				{
					sortValue2 = pokerModel2->getPokerMarkType();
				}

				if (sortType == SORT_TYPE_ASC)
				{
					result = sortValue1 < sortValue2;
				}
				else if (sortType == SORT_TYPE_DESC)
				{
					result = sortValue2 < sortValue1;
				}
			}
		}
		else if (sortPokerType == SortPokerType::SORT_POKER_TYPE_MARK_FIRST)
		{
			if (sortSubstitute && pokerModel1->isChanged())
			{
				sortValue1 = pokerModel1->getChangedPokerMarkType();
			}
			else
			{
				sortValue1 = pokerModel1->getPokerMarkType();
			}

			if (sortSubstitute && pokerModel2->isChanged())
			{
				sortValue2 = pokerModel2->getChangedPokerMarkType();
			}
			else
			{
				sortValue2 = pokerModel2->getPokerMarkType();
			}

			if (sortValue1 != sortValue2)
			{
				if (sortType == SORT_TYPE_ASC)
				{
					result = sortValue1 < sortValue2;
				}
				else if (sortType == SORT_TYPE_DESC)
				{
					result = sortValue2 < sortValue1;
				}
			}
			else
			{
				if (sortSubstitute && pokerModel1->isChanged())
				{
					sortValue1 = pokerModel1->getChangedSortValue();
				}
				else
				{
					sortValue1 = pokerModel1->getSortValue();
				}

				if (sortSubstitute && pokerModel2->isChanged())
				{
					sortValue2 = pokerModel2->getChangedSortValue();
				}
				else
				{
					sortValue2 = pokerModel2->getSortValue();
				}

				if (sortType == SORT_TYPE_ASC)
				{
					result = sortValue1 < sortValue2;
				}
				else if (sortType == SORT_TYPE_DESC)
				{
					result = sortValue2 < sortValue1;
				}
			}
		}

		
		return result;
	});
}

void PokerModel::sortPokerModelWithGroup(vector<shared_ptr<vector<shared_ptr<PokerModel>>>>& pokerModelGroupVector, const SortType& sortType, const bool& sortSubstitute/* = true*/)
{
	std::sort(pokerModelGroupVector.begin(), pokerModelGroupVector.end(), [=](shared_ptr<vector<shared_ptr<PokerModel>>> pokerModelVector1, shared_ptr<vector<shared_ptr<PokerModel>>> pokerModelVector2)
	{
		bool result = false;

		int32_t sortValue1 = 0;
		int32_t sortValue2 = 0;

		if (sortSubstitute && pokerModelVector1->front()->isChanged())
		{
			sortValue1 = pokerModelVector1->front()->getChangedSortValue();
		}
		else
		{
			sortValue1 = pokerModelVector1->front()->getSortValue();
		}

		if (sortSubstitute && pokerModelVector2->front()->isChanged())
		{
			sortValue2 = pokerModelVector2->front()->getChangedSortValue();
		}
		else
		{
			sortValue2 = pokerModelVector2->front()->getSortValue();
		}

		if (sortType == SORT_TYPE_ASC)
		{

			result = sortValue1 < sortValue2;
		}
		else if (sortType == SORT_TYPE_DESC)
		{
			result = sortValue2 < sortValue1;
		}
		return result;
	});
}

shared_ptr<PokerModel> PokerModel::findPoker(const vector<shared_ptr<PokerModel>>& pokerModelVector, const PokerValueType &pokerValueType, const PokerMarkType &pokerMarkType, const bool& isCheckSubstituteChangeValue/*=false*/)
{
	shared_ptr<PokerModel> targetPoker = nullptr;
	std::vector<shared_ptr<PokerModel>>::const_iterator it = std::find_if(pokerModelVector.cbegin(), pokerModelVector.cend(), PokerFindByValueAndType(pokerValueType, pokerMarkType, isCheckSubstituteChangeValue));
	if (it != pokerModelVector.end()) // 找到对象
	{
		targetPoker = (*it);
	}
	return targetPoker;
}

shared_ptr<PokerModel> PokerModel::findPoker(const vector<shared_ptr<PokerModel>>& pokerModelVector, const PokerValueType &pokerValueType, const bool& isCheckSubstituteChangeValue/*=false*/)
{
	shared_ptr<PokerModel> targetPoker = nullptr;
	std::vector<shared_ptr<PokerModel>>::const_iterator it = std::find_if(pokerModelVector.cbegin(), pokerModelVector.cend(), PokerFindByValue(pokerValueType, isCheckSubstituteChangeValue));
	if (it != pokerModelVector.end()) // 找到对象
	{
		targetPoker = (*it);
	}
	return targetPoker;

}

int32_t PokerModel::findCountByPokerValue(const vector<shared_ptr<PokerModel>>& pokerModelVector, const PokerValueType& pokerValueType)
{
	int32_t reuslt = 0;
	for (shared_ptr<PokerModel> poker : pokerModelVector)
	{
		if (poker->getPokerValueType() == pokerValueType)
		{
			++reuslt;
		}
	}
	return reuslt;
}


void PokerModel::removeSpecifyPokerVector(vector<shared_ptr<PokerModel>>& pokerVector, const vector<shared_ptr<PokerModel>>& beRemovePokerVector)
{
	for (auto itor = pokerVector.begin(); itor != pokerVector.end();)
	{
		if (PokerModel::findPoker(beRemovePokerVector, (*itor).get()->getPokerValueType(), (*itor).get()->getPokerMarkType()))
		{
			itor = pokerVector.erase(itor);
		}
		else
		{
			++itor;
		}
	}
}

void PokerModel::mergeSpecifyPokerVector(vector<shared_ptr<PokerModel>>& pokerVector, const vector<shared_ptr<PokerModel>>& beMergePokerVector)
{
	for (auto itor = beMergePokerVector.begin(); itor != beMergePokerVector.end();)
	{
		if (!PokerModel::findPoker(pokerVector, (*itor).get()->getPokerValueType(), (*itor).get()->getPokerMarkType()))
		{
			pokerVector.push_back(*itor);
		}
		++itor;
	}
}

void PokerModel::concatSpecifyPokerVector(vector<shared_ptr<PokerModel>>& pokerVector, const vector<shared_ptr<PokerModel>>& beConcatPokerVector)
{
	pokerVector.insert(pokerVector.end(), beConcatPokerVector.begin(), beConcatPokerVector.end());
}

PokerValueType PokerModel::getPokerValueType()
{
	return this->_pokerValueType;
}

PokerMarkType PokerModel::getPokerMarkType()
{
	return this->_pokerMarkType;
}

int32_t PokerModel::getOriginalValue()
{
	return this->_originalValue;
}

int32_t PokerModel::getSortValue()
{
	return this->_sortValue;
}

void PokerModel::setChange(const PokerValueType& changeToPokerValueType, const PokerMarkType& changeToPokerMarkType, const int32_t& changeToValue, const int32_t& changeToSortValue)
{
	if (this->_pokerValueType == PokerValueType::VALUE_TYPE_SUBSTITUTE)
	{
		if (changeToPokerValueType == PokerValueType::VALUE_TYPE_NONE || changeToPokerValueType == PokerValueType::VALUE_TYPE_SUBSTITUTE) // 想变的牌是替用或者空，则不算变牌
		{
			clearChanged();
		}
		else
		{
			this->_isChanged = true;
			this->_changedPokerValueType = changeToPokerValueType;
			this->_changedPokerMarkType = changeToPokerMarkType;
			this->_changedValue = changeToValue;
			this->_changedSortValue = changeToSortValue;
		}
	}
}

void PokerModel::clearChanged()
{
	this->_isChanged = false;
	this->_changedPokerValueType = PokerValueType::VALUE_TYPE_NONE;
	this->_changedPokerMarkType = PokerMarkType::MARK_TYPE_NONE;
	this->_changedValue = 0;
	this->_changedSortValue = 0;
}

PokerValueType PokerModel::getChangedPokerValueType()
{
	return this->_changedPokerValueType;
}

PokerMarkType PokerModel::getChangedPokerMarkType()
{
	return this->_changedPokerMarkType;
}

int32_t PokerModel::getChangedValue()
{
	return this->_changedValue;
}

int32_t PokerModel::getChangedSortValue()
{
	return this->_changedSortValue;
}

bool PokerModel::isChanged()
{
	return this->_isChanged;
}




////////////////////////扑克构造器模型数据//////////////////////////////////
PokerBuildModel::PokerBuildModel(PokerValueType pokerValueType, PokerMarkType pokerMarkType, int32_t originalValue, int32_t sortValue)
{
	this->_pokerValueType = pokerValueType;
	this->_pokerMarkType = pokerMarkType;
	this->_originalValue = originalValue;
	this->_sortValue = sortValue;
}

PokerValueType PokerBuildModel::getPokerValueType() const
{
	return this->_pokerValueType;
}

PokerMarkType PokerBuildModel::getPokerMarkType() const
{
	return this->_pokerMarkType;
}

int32_t PokerBuildModel::getOriginalValue() const
{
	return this->_originalValue;
}
int32_t PokerBuildModel::getSortValue() const
{
	return this->_sortValue;
}


////////////////////////牌型数据模型//////////////////////////////////////////////////
PokerCombinationModel::PokerCombinationModel(const PokerCombinationType& pokerCombinationType, const vector<shared_ptr<PokerModel>>& pokerModelVector, const int32_t& value) :_pokerCombinationType(pokerCombinationType), _pokerModelVector(pokerModelVector), _value(value)
{
}


PokerCombinationModel::~PokerCombinationModel()
{
}

shared_ptr<PokerCombinationModel> PokerCombinationModel::create(const PokerCombinationType& pokerCombinationType, const vector<shared_ptr<PokerModel>>& pokerModelVector, const int32_t& value)
{
	shared_ptr<PokerCombinationModel> pRet(new(std::nothrow) PokerCombinationModel(pokerCombinationType, pokerModelVector, value));
	return pRet;
}


void PokerCombinationModel::setValue(const int32_t& value)
{
	this->_value = value;
}

void PokerCombinationModel::setPokerCombinationType(const PokerCombinationType& pokerCombinationType)
{
	this->_pokerCombinationType = pokerCombinationType;
}

PokerCombinationType& PokerCombinationModel::getPokerCombinationType()
{
	return this->_pokerCombinationType;
}

uint32_t PokerCombinationModel::getPokerCount()
{
	return this->_pokerModelVector.size();
}

signed int PokerCombinationModel::getValue()
{
	return this->_value;
}

vector<shared_ptr<PokerModel>>& PokerCombinationModel::getPokerModelVector()
{
	return this->_pokerModelVector;
}

shared_ptr<PokerCombinationModel> PokerCombinationModel::clone(shared_ptr<PokerCombinationModel> pokerCombinationModel)
{
	return PokerCombinationModel::create(pokerCombinationModel->getPokerCombinationType(), pokerCombinationModel->getPokerModelVector(), pokerCombinationModel->getValue());
}

shared_ptr<PokerCombinationModel> PokerCombinationModel::findPokerCombination(vector<shared_ptr<PokerCombinationModel>>& pokerCombinationModelVector, const PokerCombinationType &pokerCombinationType)
{
	shared_ptr<PokerCombinationModel> target = nullptr;
	std::vector<shared_ptr<PokerCombinationModel>>::iterator it = std::find_if(pokerCombinationModelVector.begin(), pokerCombinationModelVector.end(), PokerCombinationFindByType(pokerCombinationType));

	if (it != pokerCombinationModelVector.end())// 找到对象
	{
		target = *it;
	}
	return target;
}

vector<shared_ptr<PokerCombinationModel>> PokerCombinationModel::findPokerCombinationAll(vector<shared_ptr<PokerCombinationModel>>& pokerCombinationModelVector, const PokerCombinationType &pokerCombinationType)
{
	vector<shared_ptr<PokerCombinationModel>> result;

	std::vector<shared_ptr<PokerCombinationModel>>::iterator it = pokerCombinationModelVector.begin();
	std::vector<shared_ptr<PokerCombinationModel>>::iterator it_end = pokerCombinationModelVector.end();
	while (true) {

		it = std::find_if(it, pokerCombinationModelVector.end(), PokerCombinationFindByType(pokerCombinationType));

		if (it != it_end)// 找到对象
		{
			result.push_back(*it);
		}
		else
		{
			break;
		}
		++it;
	}

	return result;
}


std::string PokerCombinationModel::toString(const PokerCombinationType& type)
{

	switch (type)
	{
	case POKER_COMBINATION_TYPE_NONE:
		return "none";
	case POKER_COMBINATION_TYPE_WU_LONG:
		return "wu_long";
	case POKER_COMBINATION_TYPE_DUI_ZI:
		return "dui_zi";
	case POKER_COMBINATION_TYPE_LIANG_DUI:
		return "liang_dui";
	case POKER_COMBINATION_TYPE_SAN_TIAO:
		return "san_tiao";
	case POKER_COMBINATION_TYPE_CHONG_SAN:
		return "chong_san";
	case POKER_COMBINATION_TYPE_SHUN_ZI:
		return "shun_zi";
	case POKER_COMBINATION_TYPE_TONG_HUA:
		return "tong_hua";
	case POKER_COMBINATION_TYPE_HU_LU:
		return "hu_lu";
	case POKER_COMBINATION_TYPE_ZHONG_DUN_HU_LU:
		return "zhong_dun_hu_lu";
	case POKER_COMBINATION_TYPE_TIE_ZHI:
		return "tie_zhi";
	case POKER_COMBINATION_TYPE_ZHONG_DUN_TIE_ZHI:
		return "zhong_dun_tie_zhi";
	case POKER_COMBINATION_TYPE_TONG_HUA_SHUN:
		return "tong_hua_shun";
	case POKER_COMBINATION_TYPE_ZHONG_DUN_TONG_HUA_SHUN:
		return "zhong_dun_tong_hua_shun";

		// 特殊牌型开始
	case POKER_COMBINATION_TYPE_SAN_TONG_HUA:
		return "super_san_tong_hua";
	case POKER_COMBINATION_TYPE_SAN_SHUN_ZI:
		return "super_san_shun_zi";
	case POKER_COMBINATION_TYPE_LIU_DUI_BAN:
		return "super_liu_dui_ban";
	case POKER_COMBINATION_TYPE_WU_DUI_SAN_TIAO:
		return "super_wu_dui_san_tiao";
	case POKER_COMBINATION_TYPE_SI_TAO_SAN_TIAO:
		return "super_si_tao_san_tiao";
	case POKER_COMBINATION_TYPE_COU_YI_SE:
		return "super_cou_yi_se";
	case POKER_COMBINATION_TYPE_QUAN_XIAO:
		return "super_quan_xiao";
	case POKER_COMBINATION_TYPE_QUAN_DA:
		return "super_quan_da";
	case POKER_COMBINATION_TYPE_SAN_FEN_TIAN_XIA:
		return "super_san_fen_tian_xia";
	case POKER_COMBINATION_TYPE_SAN_TONG_HUA_SHUN:
		return "super_san_tong_hua_shun";
	case POKER_COMBINATION_TYPE_SHI_ER_HUANG_ZU:
		return "super_shi_er_huang_zu";
	case POKER_COMBINATION_TYPE_YI_TIAO_LONG:
		return "super_yi_tiao_long";
	case POKER_COMBINATION_TYPE_QING_LONG:
		return "super_qing_long";
	default:
		return "unknow";
	}
}

///////////////////////////扑克模型生产工厂/////////////////////////////////////////////
shared_ptr<PokerModel> PokerModelFactory::createPokerModel(const PokerBuildModel& pokerBuildModel)
{
	shared_ptr<PokerModel> pRet(new(std::nothrow) PokerModel(pokerBuildModel.getPokerValueType(), pokerBuildModel.getPokerMarkType(), pokerBuildModel.getOriginalValue(), pokerBuildModel.getSortValue()));
	return pRet;
}

shared_ptr<PokerModel> PokerModelFactory::createPokerModel(const RuleType& gameRuleType, const PokerValueType& pokerValueType, const PokerMarkType& pokerMarkType)
{
	PokerBuildModel* pokerBuildModel = getPokerBuildModel(gameRuleType, pokerValueType, pokerMarkType);
	return createPokerModel(*pokerBuildModel);
}


vector<shared_ptr<PokerModel>> PokerModelFactory::createPokerModelPack(const RuleType& gameRuleType)
{
	vector<shared_ptr<PokerModel>> resutlVector;
	switch (gameRuleType) {
	case RULE_TYPE_SIMPLE:
	{
		for (PokerBuildModel pokerBuildModel : POKER_SIMPLE)
		{
			shared_ptr<PokerModel> pokerModel = createPokerModel(pokerBuildModel);
			resutlVector.push_back(pokerModel);
		}
	}
		break;
	case RULE_TYPE_MUCH_THREE:
	{
		throw std::logic_error("not support createPokerModelPack RULE_TYPE_MUCH_THREE");
	}
		break;
	case RULE_TYPE_AMAZING:
	{
		throw std::logic_error("not support createPokerModelPack RULE_TYPE_AMAZING");
	}
		break;
	default:
		break;
	}

	return resutlVector;
}

bool PokerModelFactory::getPokerValueAndSortValue(const RuleType& gameRuleType, const PokerValueType& pokerValueType, const PokerMarkType& pokerMarkType, int32_t& outPokerValue, int32_t& outSortValue)
{

	bool result = false;
	switch (gameRuleType) {
	case RULE_TYPE_SIMPLE:
	{
		int32_t len = CommonUtils::getArrayLen(POKER_SIMPLE);
		const PokerBuildModel* targetPokerBuild = std::find_if(POKER_SIMPLE, POKER_SIMPLE + len, PokerBuildModelFindByValueAndType(pokerValueType, pokerMarkType));


		if (targetPokerBuild != POKER_SIMPLE + len)// 找到对象
		{
			outPokerValue = (*targetPokerBuild).getOriginalValue(); // 返回相应的值
			outSortValue = (*targetPokerBuild).getSortValue();
			result = true;
		}
		else
		{
			outPokerValue = 0;
			outSortValue = 0;
			result = false;
		}
	}
		break;
	case RULE_TYPE_MUCH_THREE:
	{
		throw std::logic_error("not support getPokerValueAndSortValue RULE_TYPE_MUCH_THREE");
	}
		break;
	case RULE_TYPE_AMAZING:
	{
		throw std::logic_error("not support getPokerValueAndSortValue RULE_TYPE_AMAZING");
	}
		break;
	default:
		break;
	}
	return result;
}

PokerBuildModel* PokerModelFactory::getPokerBuildModel(const RuleType& gameRuleType, const PokerValueType& pokerValueType, const PokerMarkType& pokerMarkType)
{
	PokerBuildModel* result = nullptr;
	switch (gameRuleType) {
	case RULE_TYPE_SIMPLE:
	{
		int32_t len = CommonUtils::getArrayLen(POKER_SIMPLE);

		PokerMarkType pokerMarkTypeFind = pokerMarkType;

		// 大鬼、小鬼、替用的花色只能是NONE
		if (pokerValueType == PokerValueType::VALUE_TYPE_KING_BIG || pokerValueType == PokerValueType::VALUE_TYPE_KING_SMALL || pokerValueType == PokerValueType::VALUE_TYPE_SUBSTITUTE)
		{
			pokerMarkTypeFind = PokerMarkType::MARK_TYPE_NONE;
		}
		const PokerBuildModel* targetPokerBuild = std::find_if(POKER_SIMPLE, POKER_SIMPLE + len, PokerBuildModelFindByValueAndType(pokerValueType, pokerMarkTypeFind));


		if (targetPokerBuild != POKER_SIMPLE + len)// 找到对象
		{
			result = const_cast<PokerBuildModel*>(targetPokerBuild);
		}
	}
		break;
	case RULE_TYPE_MUCH_THREE:
	{
		throw std::logic_error("not support getPokerValueAndSortValue RULE_TYPE_MUCH_THREE");
	}
		break;
	case RULE_TYPE_AMAZING:
	{
		throw std::logic_error("not support getPokerValueAndSortValue RULE_TYPE_AMAZING");
	}
		break;
	default:
		break;
	}
	return result;
}
