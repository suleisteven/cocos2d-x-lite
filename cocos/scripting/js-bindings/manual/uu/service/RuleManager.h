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

	int32_t _pokerValueCount; // ���ܵ�����pokerValue֮�ͣ�

	int32_t _kingCount; // ������
	int32_t _substituteCount; // ���ø���

	/**
	 * ÿ�ָ���������˿˼��ϣ����磺keyΪ1ʱ�����ǵ����Ƶļ��ϣ�keyΪ2ʱ�����Ƕ����Ƶļ���
	 */
	map<int32_t, vector<shared_ptr<vector<shared_ptr<PokerModel>>>>> _everyCountPokerMap;

	/**
	 * ����ɫ������˿˼���
	 */
	map<PokerMarkType, vector<shared_ptr<PokerModel>>> _pokerMarkMap;

	/**
	 * ���˿�ֵ������˿˼���
	 */
	map<PokerValueType, vector<shared_ptr<PokerModel>>> _pokerTypeMap;

	/**
	* ÿ���˿˵ĸ���
	*/
	map<PokerValueType, uint32_t> _countEveryPokerTypeMap;
};


/************************************************************************/
/* ���������������                                                                     */
/************************************************************************/
class RuleManagerAbstract
{
public:
	/**
	 * ��ȡ�Ƹ�����Ϣ
	 */
	shared_ptr<PokerCountInfo> RuleManagerAbstract::getPokerCountInfo(vector<shared_ptr<PokerModel>> pokerVector);

	/**
	 * �Ƚ��������͵Ĵ�С
	 * @return >0:model1����model2��=0��model1����model2��<0:model1С��model2
	 */
	virtual int32_t comparePokerCombination(shared_ptr<PokerCombinationModel> model1, shared_ptr<PokerCombinationModel> model2);

	/**
	* �Ƿ�Ϊ�๫����
	*/
	virtual bool isXiangGong(vector<shared_ptr<PokerCombinationModel>> pokerCombinationVector);

	/**
	* �Ƿ�����������
	*/
	virtual bool isSpecialPokerCombination(const PokerCombinationType& pcType);

	/**
	* �Ƿ������������
	*/
	virtual bool hasSpecialPokerCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector);

	/**
	* ������������ȡ����������
	*/
	virtual shared_ptr<PokerCombinationModel> getSpecialPokerCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector);

public:

	/**
	* ��ȡ��Ϸ��������
	*/
	virtual RuleType getRuleType() = 0;

	/**
	* �������ƻ�ȡ�����������
	*/
	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getPokerCombination(vector<shared_ptr<PokerModel>> pokerVector) = 0;

};

/************************************************************************/
/* �����淨���������                                                                     */
/************************************************************************/
class SimpleRuleManager : public RuleManagerAbstract
{
public:
	static const int32_t POKER_COUNT_ROW_1 = 3;// ͷ������
	static const int32_t POKER_COUNT_ROW_2 = 5;// �е�����
	static const int32_t POKER_COUNT_ROW_3 = 5;// β������

	static const int32_t POKER_COUNT_SINGLE_PLAYER = 13; // �����������
public:

	virtual RuleType getRuleType();

	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getPokerCombination(vector<shared_ptr<PokerModel>> pokerVector);

protected:

	/**
	 * �ҳ���������
	 */
	virtual shared_ptr<PokerCombinationModel> findSpecialPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * �ҳ���������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findChainPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * �ҳ�������ͨ�Ƽ�����
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findSimplePokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, shared_ptr<PokerCountInfo> pokerCountInfo);
private:

};

#endif  /* __RULE_MANAGER_CPP_H__ */