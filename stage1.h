#pragma once
#include "gameNode.h"
#include "camera.h"
#include "zOrder.h"
#include "player.h"
#include "playerHandState.h"
#include "boss.h"
#include "enemy.h"
#include "itemManager.h"
#include <vector>

#define STAGE1X 400
#define STAGE1Y 400
#define ROOMNUM 5

struct MapStage
{
	int min;
	int max;
};

class stage1 : public gameNode
{
private:
	/*
	camera* _cam;
	
	//�� ����
	mapSetting* _mapSet;

	//�ʼ��ÿ� ���Ϳ� Ÿ��(���ʹ� �ʱ� �����, �ǻ���� �迭)
	vector<TILE>	_vMap;					// ��Ÿ�� 
	TILE			_map[STAGE1X* STAGE1Y];	// ��
	vector<int>		_sectorNum;				// ���� ����

	bool _isEvent[ROOMNUM];					//�� �̺�Ʈ �ߵ�
	
	//���� ����
	POINT setInt;

	//�÷��̾� ����
	

	//�ǵ���� ������
	//����
	boss* _boss;

	//���
	vector<enemy*> _vEnemy;

	//������ �׽�Ʈ ����
	bool _tileData[2];
	bool _clearStage;

	//��ã��� �׽�Ʈ
	int _x, _y;
	int testnum;
	int eventTest;
	*/

	image*	_spaceImg;
	POINTF	_spaceXY;

	//���.==================================

	camera* _camera;
	zOrder* _zOrder;

	bool _isSelect;

	//==== �÷��̾� ================================================

	player* _player;		//�غ�
	int		_pNum;			//�÷��̾� ������ ����

	//==== �� ���� =================================================

	vector<enemy*> _vEnemy;	//�� ����
	boss* _boss;			//��������
	bool _isboss;			//���� Ȱ��ȭ ����

	//==== �ѱ� ���� ===============================================

	playerGun*	_gun;		//��

	//==== �Ѿ� ���� ===============================================

	bullet* _bullet;		//�Ѿ�
	itemManager* _itemManager;//�����۸Ŵ���

	//==== Ÿ�ϸ� ================================================

	//�� ����
	vector<TILE>	_vMap;						//�� Ÿ��
	vector<TILE>	_upMap;	//����� ����
	vector<TILE>	_downMap;//�Ʒ���
	int			_mapSizeX,_mapSizeY;			//���� ������
	int			_sponCheck;						//������ġ üũ
	

	//�̺�Ʈ�� ����
	MapStage _mapStage[3];
	int		_sN;
	bool	isNext;
	bool	_stageEnd;
	int		nextNum;
	int		_nCount;	//zī��Ʈ
	//�̺�Ʈ ����� ����

	bool _event;	//�̺�Ʈ ���� ����
	int _eCont;		//�̺�Ʈ �ߺ� Ƚ��
	int _node[3];		//�� ��ȯ�� ���
	bool _nodeEnd;	//�� ��ȯ�� �����°�?

	//���� ������
	int _infoNum;	//���� ��¿� ��ȣ

	//Ű �Է¿� ����
	bool _isKey[4];

private:

public:

	virtual HRESULT init();			//�ʱ�ȭ �Լ�
	virtual void release();			//�޸� ���� �Խ�
	virtual void update();			//�����ϴ� �Լ�
	virtual void render();			//�׸��� �Լ�

	//�浹üũ
	void collision();

	void nextStage();
	//ĳ���� �÷���
	void playGame();
	//�ʻ���
	void createMap();
	//�÷��̾� ����
	void selectPlayer();
	//Ű���� �Է� �޴� �Լ�
	void keySelect();

	//�� ����
	void createEnemy(POINTF exy);

	//�̺�Ʈ
	void stageEvent(int stage);

	//����� Ÿ�Ϻ���
	void changeTile();
};

