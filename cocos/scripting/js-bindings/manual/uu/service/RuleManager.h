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

	/**
	* ��ȡ��Ϸ��������
	*/
	virtual RuleType getRuleType() = 0;

	/**
	 * �������ƻ���Ƽ�����
	 * @param pokerVector 
	 */
	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getRecommondPokerCombination(vector<shared_ptr<PokerModel>> pokerVector) = 0;

	/**
	 * �����˿˼��ϻ�ȡ����
	 * @param pokerVector �˿˼��ϣ�������3�ŵ�ͷ����Ҳ������5�ŵ���β��
	 */
	virtual shared_ptr<PokerCombinationModel> getPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector) = 0;
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
	virtual bool hasPeculiarCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector);

	/**
	* ������������ȡ����������
	*/
	virtual shared_ptr<PokerCombinationModel> getPeculiarPokerCombination(vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> pokerCombinationVector);

protected:


	/**
	* �������ͻ�ȡ���Ͷ�Ӧ�ķ���
	* @param pokerCountInfo �˿�ͳ����Ϣ����ΪNULLʱ��ʹ��ͳ����Ϣ�е�����
	*/
	virtual int32_t getPokerValueByCombination(const PokerCombinationType& type, const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo = nullptr) = 0;
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

	static const int32_t CHAIN_FIND_TYPE_TONGHUA_ALL = 0; // �������ӣ���������
	static const int32_t CHAIN_FIND_TYPE_TONGHUA_ONLY = 1; // �������ӣ���ͬ��˳
	static const int32_t CHAIN_FIND_TYPE_TONGHUA_OTHER = 2; // �������ӣ���ͬ��˳

	static const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_3_OR_5 = 0; // �������Ӹ������ͣ�3�Ż�5��
	static const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_3 = 1; // �������Ӹ������ͣ�������
	static const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_5 = 2; // �������Ӹ������ͣ�������
	static const int32_t CHAIN_FIND_TYPE_COUNT_TYPE_LONG = 3; // �������˳��


	static const int32_t WULONG_FIND_TYPE_COUNT_TYPE_3_OR_5 = 0; // ���������������ͣ�3�Ż�5��
	static const int32_t WULONG_FIND_TYPE_COUNT_TYPE_3 = 1; // ���������������ͣ�������
	static const int32_t WULONG_FIND_TYPE_COUNT_TYPE_5 = 2; // ���������������ͣ�������
	
public:

	virtual RuleType getRuleType();

	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> getRecommondPokerCombination(vector<shared_ptr<PokerModel>> pokerVector);

	virtual shared_ptr<PokerCombinationModel> getPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector);
	
protected:

	/**
	 * �ҳ���������
	 */
	virtual shared_ptr<PokerCombinationModel> findPeculiarPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	* �ҳ�������ͨ�Ƽ�����
	*/
	virtual vector<shared_ptr<PokerCombinationModel>> findSimplePokerCombination(vector<shared_ptr<PokerModel>>& pokerVector,const shared_ptr<PokerCountInfo> pokerCountInfo);

	

	/**
	 * �ҳ�ָ������
	 * @param tonghuaType ͬ������
	 * @param countType  ��������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findChainPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& tonghuaType, const int32_t& countType);

	/**
	 * �ҳ����к�«����
	 *
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findHuLuPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * ����������֧����
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findTieZhiPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * ��������ͬ������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findTongHuaPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * ����������������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findSanTiaoPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * ����������������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findLiangDuiPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * �������ж�������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findDuiZiPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo);

	/**
	 * ������������
	 * @param wulongCountType ������������
	 */
	virtual vector<shared_ptr<PokerCombinationModel>> findWuLongPokerCombination(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& wulongCountType);

	virtual int32_t getPokerValueByCombination(const PokerCombinationType& type, const vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo = nullptr);

	
private:

	/**
	 * �ݹ������ͨ����
	 */
	virtual vector<shared_ptr<vector<shared_ptr<PokerCombinationModel>>>> findSimplePokerCombinationRecursion(vector<shared_ptr<PokerModel>>& pokerVector, const shared_ptr<PokerCountInfo> pokerCountInfo, const int32_t& step);
};

#endif  /* __RULE_MANAGER_CPP_H__ */