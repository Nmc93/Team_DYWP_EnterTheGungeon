#pragma once
#include "gameNode.h"
#include "camera.h"
#include "zOrder.h"
#include "player.h"
#include "itemManager.h"
#include "enemy.h"
#include <vector>


class breakStage : public gameNode
{
private:

	camera* _camera;
	zOrder* _zOrder;
	
	bool _isSelect;
	
	//==== �÷��̾� ================================================

	player* _marine;		//�غ�
	player* _prisoner;		//�˼�
	int		_pNum;			//�÷��̾� ������ ����

	//�� ����
	vector<TILE>	_vMap;							//�� Ÿ��
	vector<Unit*> _vOrder;							//Z������ ����

	//�̺�Ʈ�� ����
	bool	_isNext;
	int		_nextNum;

	//Ű �Է¿� ����
	bool _isKey[4];
	
	//Ű ���氪
	int _nextInfo;

private:

	void createTileMap();			//�� ���� �Լ�

public:

	virtual HRESULT init();			//�ʱ�ȭ �Լ�
	virtual void release();			//�޸� ���� �Խ�
	virtual void update();			//�����ϴ� �Լ�
	virtual void render();			//�׸��� �Լ�

	void selectPlayer(bool select);	//�÷��̾� ����

	void nextStage();				//���� ������ �Ѿ�� �Լ�
	void keySelect();				//Ű�̸�
	
};

