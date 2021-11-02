#pragma once
#include "gameNode.h"
#include <vector>

enum class MType
{
	EMPTY,
	START,
	END,
};

struct ATile
{
	POINTF		mpt;		//�� ��ǥ
	int			num;		//�� ��ȣ
	RECT		rc;			//Ÿ�� ��Ʈ
	
	TileState	tState;		//Ÿ�� �Ӽ�
	int			sector;		//���� ��ȣ

	float		totalCost;		//F : �� �Ÿ� ���(G+H)
	float		costFromStart;	//G : ����Ÿ�Ϻ��� ����Ÿ�ϱ��� ���
	float		costToGoal;		//H : ���� Ÿ�Ϻ��� ������ ������ ��κ��

	bool		isOpen;		//���� ���ɿ���
	ATile*		pTile;		//�θ� Ÿ��
	MType		type;		//�Ӽ���

	bool		testB;		//�׽�Ʈ�� ��
};

class aStar : public gameNode
{
private:

	//================== ĵ���� ���� ===========================
	
	vector<ATile*>				_vTotalMap;		//ĵ����
	vector<ATile*>::iterator	_viTotalMap;	//ĵ���� �ݺ���
	ATile* _vtemp;	//ĵ���� ����� Ÿ��

	int _canvasX;	// ĵ������ X����
	int _canvasY;	// ĵ������ Y����


	vector<ATile*>				_vOpenList;		//�˻翹�����
	vector<ATile*>::iterator	_viOpenList;	//�˻翹����� �ݺ���

	vector<ATile*>				_vCloseList;	//�˻�������
	vector<ATile*>::iterator	_viCloseList;	//�˻������� �ݺ���

	ATile* _startTile;			//���� Ÿ��
	ATile* _endTile;			//���� Ÿ��
	ATile* _currentTile;		//���� Ÿ��

	int _startNum;				//���� Ÿ�� ��ȣ
	int _endNum;				//���� Ÿ�� ��ȣ
	int _currentNum;			//���� Ÿ�� ��ȣ

	vector<int>	_arrayNum;		//�迭 ��ȣ �����

private:

	vector<ATile*> addOpenList(ATile* currenttile , bool tilecheck);

public:

	aStar();
	~aStar();
	//�ʱ�ȭ �Լ�(ĵ����,ĵ����X,ĵ����Y,���۹迭,����迭)
	virtual HRESULT init(vector<TILE> atile, int sizex , int sizey, int start, int end);	
	virtual void release();												//�޸� ���� �Խ�
	virtual void update();												//�����ϴ� �Լ�
	virtual void render();												//�׸��� �Լ�
	
	//�̵� �˻� �Լ�[T : 4����][F : 8����]
	void waycheck(ATile* tile, bool tilecheck);

	//�˻� ����� �迭 �ʱ�ȭ
	void closeCheck(vector<int>& arraynum);

	vector<int> getWay() { return _arrayNum; }

	//����
	//1. init�� �ִ´�.
	//2. waycheck�� �ִ´�.
	//3. ��� �� closeCheck()�� �ʱ�ȭ �����ش�.

};