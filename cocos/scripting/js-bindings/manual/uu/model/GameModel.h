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
/* ��Ϸ��������                                                                     */
/************************************************************************/
enum RuleType : signed int
{
	RULE_TYPE_SIMPLE, // �����淨
	RULE_TYPE_MUCH_THREE, // �������淨
	RULE_TYPE_AMAZING, // �ٱ��淨
};


/**
* �˿���ֵ����
*/
enum PokerValueType : signed int
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
enum PokerValueTypeOfValue : signed int
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
enum PokerMarkType : signed int
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
	PokerModel(PokerValueType pokerValueType, PokerMarkType pokerMarkType, int originalValue, int sortValue);
	~PokerModel();

	/**
	* ��¡һ���˿�����ģ��
	*/
	virtual PokerModel* clone();
	/**
	* �����˿�
	* @param pokerModelVector �������˿˼���
	* @param sortType ��������
	* @param sortSubstitute �Ƿ�����������
	*/
	static void sortPokerModel(Vector<PokerModel*>& pokerModelVector, const SortType& sortType, const bool& sortSubstitute = true);

	/**
	* ��������˿ˣ�����ͬ���ƽ��з��飩,��������һ����Ԫ��
	*/
	static void sortPokerModelWithGroup(vector<Vector<PokerModel*>>& pokerModelGroupVector, const SortType& sortType, const bool& sortSubstitute = true);

	/**
	* �����˿�
	* @param pokerModelVector �����ҵ��˿˼���
	* @param pokerValueType �����˿˵�ֵ����
	* @param pokerMarkType �����˿˵Ļ�ɫ����
	* @param isCheckSubstitute �Ƿ������õı仯ֵ��false-����飨��ʱ����������Ʊ仯�����ͣ���true-��飨��ʱ�����ȱȽ����ͣ�����ԱȲ��ϣ��ٶԱ����ñ仯��ֵ�����磺�����ѱ�ɺ���4����ʱ���Һ���4���ܹ����ظ������ƣ�
	* @return null-û���ҵ������򷵻��˿˶���ָ��
	*/
	static PokerModel* findPoker(Vector<PokerModel*>& pokerModelVector, const PokerValueType &pokerValueType, const PokerMarkType &pokerMarkType, const bool& isCheckSubstituteChangeValue = false);

	/**
	* �����˿ˣ������˿˻�ɫ����
	* @param pokerModelVector �����ҵ��˿˼���
	* @param pokerValueType �����˿˵�ֵ����
	* @param isCheckSubstitute �Ƿ������õı仯ֵ��false-����飨��ʱ����������Ʊ仯�����ͣ���true-��飨��ʱ�����ȱȽ����ͣ�����ԱȲ��ϣ��ٶԱ����ñ仯��ֵ�����磺�����ѱ�ɺ���4����ʱ���Һ���4���ܹ����ظ������ƣ�
	* @return null-û���ҵ������򷵻��ҵ��ĵ�һ���˿˶���ָ��
	*/
	static PokerModel* findPoker(Vector<PokerModel*>& pokerModelVector, const PokerValueType &pokerValueType, const bool& isCheckSubstituteChangeValue = false);

	/**
	* ����ĳ���Ƶ�����
	*/
	static int findCountByPokerValue(Vector<PokerModel*>& pokerModelVector, const PokerValueType& pokerValueType);

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
	int getOriginalValue();
	/**
	* ��ȡ����ֵ��С
	*/
	int getSortValue();

	/**
	* �仯�˿ˣ������ƲŻ���Ч
	*/
	void setChange(IN_P const PokerValueType& changeToPokerValueType, IN_P const PokerMarkType& changeToPokerMarkType, IN_P const int& changeToValue, IN_P const int& changeToSortValue);
	/**
	* ��ձ仯
	*/
	void clearChanged();
	/**
	* ��ȡ�˿˱仯���ֵ��С
	*/
	int getChangedValue();
	/**
	* ��ȡ�˿˱仯�������ֵ��С
	*/
	int getChangedSortValue();
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