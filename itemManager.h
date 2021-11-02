#pragma once
#include "gameNode.h"
#include "camera.h"
#include "itemUI.h"

#define DEFAULTHEART 3

class player;

class itemManager : public gameNode
{
private:
	camera* _cam;

	vector<item*> _vItem;
	vector<item*>::iterator _viItem;

	player* _p;

	bool _isFollow;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void draw();
	void setUp();

	void dropMoney(POINTF dropXY);

	void dropMoney(float x, float y);
	void followMoney();

	// �������� ��� ��Ʈ �߰�
	void addHeart();

	// ������ ���� ��ȭ
	void changeHeart(bool plus);
	void changeBarrior(bool plus);
	void changeBlankBullet(bool plus);

	// getter, setter
	void setPlayer(player* p) { _p = p; }
	//void setCamera(camera* cam) { _cam = cam; }
	
	void setIsFollow(bool isFollow) { _isFollow = isFollow; }

	//������ ������
	vector<item*> getItem() { return _vItem; }
};

