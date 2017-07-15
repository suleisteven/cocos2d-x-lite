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
/* �˿˸�����Ϣ                                                                     */
/************************************************************************/
class PokerCountInfo : public BaseModel
{
public:
	PokerCountInfo();

	int32_t _singleCount; // ��������(��������������������)
	int32_t _twosomeCount; // �����������������������������ã�
	int32_t _threesomeCount; // �����������������������������ã�
	int32_t _foursomeCount; // �������������������������������ã�

	int32_t _blackPeachCount; // ��������
	int32_t _redPeachCount; // ��������
	int32_t _plumBlossomCount; // ÷������
	int32_t _redSquareCount; // �췽����
	int32_t _markTypeCount; // ��ɫ��������


	int32_t _pokerValueCount; // ���ܵ�����pokerValue֮�ͣ�

	int32_t _kingCount; // ������
	int32_t _substituteCount; // ���ø���

	/**
	 * ����ɫ������˿˼���
	 */
	map<PokerMarkType, vector<shared_ptr<PokerModel>>> _pokerMarkMap;

	/**
	 * ���˿�ֵ������˿˼���
	 */
	map<PokerValueType, vector<shared_ptr<PokerModel>>> _pokerTypeMap;

	/**
	* ÿ�ָ���������˿˼��ϣ����磺keyΪ1ʱ�����ǵ����Ƶļ��ϣ�keyΪ2ʱ�����Ƕ����Ƶļ���
	*/
	map<int32_t, vector<shared_ptr<vector<shared_ptr<PokerModel>>>>> _everyCountPokerMap;

	/**
	* ÿ���˿˵ĸ���
	*/
	map<PokerValueType, uint32_t> _countEveryPokerTypeMap;
	/**
	 * ÿ���˿˻�ɫ�ĸ���
	 */
	map<PokerMarkType, uint32_t> _countEveryPokerMarkMap;
};


/************************************************************************/
/* ���������������                                                                     */
/************************************************************************/
class RuleManagerAbstract
{

public:

	RuleManagerAbstract() :_ptr(nullptr)
	{
	}

	/**
	* ��ȡ��Ϸ��������
	*/
	virtual RuleType getRuleType() = 0;

	/**
	 * �������ƻ���Ƽ�����
	 * @param pokerVector 
	 */
	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getRecommendPokerCombination(vector<shared_ptr<PokerModel>> pokerVector) = 0;

	/**
	 * �����Զ��źõ��˿˼��ϻ�ȡ����
	 * @param pokerVector �˿˼��ϣ�13����
	 * @return empty���ʾû�з��Ϲ�������ͣ���֮���ص�ǰ����
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> getPokerCombinationAllRow(vector<shared_ptr<PokerModel>>& pokerVector) = 0;

	/**
	 * ��ȡ��ǰ�淨�������˿�
	 */
	virtual vector<shared_ptr<PokerModel>> getAllPokerVector() = 0;

	/**
	* �Ƿ�Ϊ�๫����
	*/
	virtual bool isXiangGong(vector<shared_ptr<PokerCombinationModel>> pokerCombinationVector);

	/**
	 * �Ƿ�����������
	 */
	virtual bool isPeculiarPokerCombination(const PokerCombinationType& pokerCombinationType);
	
	/**
	* �Ƿ������������
	*/
	virtual bool hasPeculiarCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector);

	/**
	* �Ƚ��������͵Ĵ�С
	* @return >0:model1����model2��=0��model1����model2��<0:model1С��model2
	*/
	virtual int32_t comparePokerCombination(shared_ptr<PokerCombinationModel> model1, shared_ptr<PokerCombinationModel> model2);

public:
	/**
	 * ��ȡ�Ƹ�����Ϣ
	 */
	shared_ptr<PokerCountInfo> getPokerCountInfo(vector<shared_ptr<PokerModel>> pokerVector);

	/**
	 * ��ȡ�Ƶ����ܺ�
	 */
	int32_t getPokerValueTotal(const vector<shared_ptr<PokerModel>>& pokerVector);

	/**
	* ����ĳ���˿˼��ϻ�ȡ����
	* @param pokerVector �˿˼��ϣ�������3�ŵ�ͷ����Ҳ������5�ŵ���β��
	* @param row 0-2 �����
	*/
	virtual shared_ptr<PokerCombinationModel> getPokerCombinationAtRow(vector<shared_ptr<PokerModel>>& pokerVector, const int32_t& row) = 0;


	/**
	* ������������ȡ����������
	*/
	virtual shared_ptr<PokerCombinationModel> getPeculiarPokerCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector);

public:
	void setRefPtr(void *ptr)
	{
		this->_ptr = ptr;
	}

	void* getRefPtr()
	{
		return this->_ptr;
	}

protected:


	/**
	* �������ͻ�ȡ���Ͷ�Ӧ�ķ���
	* @param pokerCountInfo �˿�ͳ����Ϣ����ΪNULLʱ��ʹ��ͳ����Ϣ�е�����
	*/
	virtual int32_t getPokerValueByCombination(const PokerCombinationType& type, vector<shared_ptr<PokerModel>> pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo = nullptr) = 0;

	/**
	 * �ж�һ�����Ƿ�������
	 */
	virtual bool isChain(vector<shared_ptr<PokerModel>> pokerVector);

private:
	void* _ptr;
};

/************************************************************************/
/* �����淨���������                                                                     */
/************************************************************************/
class SimpleRuleManager : public RuleManagerAbstract
{
public:
	const int32_t POKER_COUNT_ROW_1 = 3;// ͷ������
	const int32_t POKER_COUNT_ROW_2 = 5;// �е�����
	const int32_t POKER_COUNT_ROW_3 = 5;// β������

	const int32_t POKER_COUNT_SINGLE_PLAYER = 13; // �����������

	const int32_t CHAIN_FIND_TYPE_TONGHUA_ALL = 0; // �������ӣ���������
	const int32_t CHAIN_FIND_TYPE_TONGHUA_ONLY = 1; // �������ӣ���ͬ��˳
	const int32_t CHAIN_FIND_TYPE_TONGHUA_OTHER = 2; // �������ӣ���ͬ��˳

	const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_3_OR_5 = 0; // �������Ӹ������ͣ�3�Ż�5��
	const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_3 = 1; // �������Ӹ������ͣ�������
	const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_5 = 2; // �������Ӹ������ͣ�������
	const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_LONG = 3; // �������˳��


	const int32_t WULONG_FIND_TYPE_COUNT_TYPE_3_OR_5 = 0; // ���������������ͣ�3�Ż�5��
	const int32_t WULONG_FIND_TYPE_COUNT_TYPE_3 = 1; // ���������������ͣ�������
	const int32_t WULONG_FIND_TYPE_COUNT_TYPE_5 = 2; // ���������������ͣ�������
	
public:

	SimpleRuleManager()
	{
		this->_allPokerVector = PokerModelFactory::createPokerModelPack(RuleType::RULE_TYPE_SIMPLE); // ������ǰ�淨�������˿�
	}

	virtual RuleType getRuleType();

	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getRecommendPokerCombination(vector<shared_ptr<PokerModel>> pokerVector);

	virtual shared_ptr<PokerCombinationModel> getPokerCombinationAtRow(vector<shared_ptr<PokerModel>>& pokerVector, const int32_t& row);

	virtual vector<shared_ptr<PokerCombinationModel>> getPokerCombinationAllRow(vector<shared_ptr<PokerModel>>& pokerVector);

	virtual vector<shared_ptr<PokerModel>> getAllPokerVector();
protected:

	/**
	 * �ҳ���������
	 */
	virtual shared_ptr<PokerCombinationModel> findPeculiarPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	* �ҳ�������ͨ�Ƽ�����
	*/
	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> findSimplePokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	

	/**
	 * �ҳ�ָ������
	 * @param tonghuaType ͬ������
	 * @param countType  ��������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findChainPokerCombination(const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& tonghuaType, const int32_t& countType);

	/**
	 * �ҳ����к�«����
	 *
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findHuLuPokerCombination(const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * ����������֧����
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findTieZhiPokerCombination(const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * ��������ͬ������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findTongHuaPokerCombination(const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * ����������������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findSanTiaoPokerCombination(const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * ����������������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findLiangDuiPokerCombination(const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * �������ж�������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findDuiZiPokerCombination(const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * ������������
	 * @param wulongCountType ������������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findWuLongPokerCombination(const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& wulongCountType);

	virtual int32_t getPokerValueByCombination(const PokerCombinationType& type, vector<shared_ptr<PokerModel>> pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo = nullptr);

	
private:

	/**
	 * �ݹ������ͨ����
	 */
	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> findSimplePokerCombinationRecursion(const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, shared_ptr<PokerCombinationModel> curPokerCombination, const int32_t& step);

	/**
	 * ��������ֵ
	 */
	virtual int32_t calcPokerCombinationValue(const PokerCombinationType& type, vector<shared_ptr<PokerModel>>& pokerVector);

private:
	vector<shared_ptr<PokerModel>> _allPokerVector; // �����˿�

};

#endif  /* __RULE_MANAGER_CPP_H__ */