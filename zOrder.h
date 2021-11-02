#pragma once
#include "gameNode.h"
#include "player.h"
#include "playerHandState.h"
#include "enemy.h"
#include "boss.h"
#include "bullet.h"
#include <vector>

enum class UnitType
{
	PLAYER,
	ENEMY,
	BOSS,
	GUN,
	BULLET,
	ITEM
};

//z���� ����ü
struct Unit
{
	UnitType unitType;	//�񱳿� Ÿ��
	//============ Ű�� ===========
	float	key;	//�񱳿� �׸��� ������ǥ
	//========== ��ǥ ���� ========
	POINTF	shadow;	//�׸��� LEFT TOP ��ǥ
	POINTF	body;	//���� LEFT TOP ��ǥ
	//========== �̹��� ���� =======
	POINT	frame;	//�̹��� ������
	image* bImage;	//���� �̹���
	image* sImage;	//�׸��� �̹���
	//========== �߰� ���� ========
	image* plusImg;	//�߰� �̹���
	int	plusint;	//�߰� ����
	POINTF plusPointf;//�߰� ����ƮF
	POINT plusPoint;	//�߰� ����Ʈ
	RECT plusRc;	//�߰� ��Ʈ
	bool   plusbool; //�߰� ������
};

class zOrder : public gameNode
{
private:

	vector<Unit*> _vObj;
	POINTF _cMove;
	POINTF _cMouse;

private:
	//���� �ʱ�ȭ �Լ�
	void vClear() { _vObj.clear(); }

public:

	virtual HRESULT init();						//�ʱ�ȭ �Լ�
	virtual void release();						//�޸� ���� �Խ�
	virtual void update();						//�����ϴ� �Լ�
	virtual void render();						//�׸��� �Լ�
	
	//�̹��� ��¿� �Լ�
	void drawImage(vector<Unit*>& vOrder);	
	
	//���Ĺݺ�
	void merge(int left, int right);	
	//�պ�����
	void mergeSort(int left, int right);		

	//�÷��̾�Ǫ��
	void pushObject(player* player);
	//��Ǫ��
	void pushObject(enemy* em);
	//����Ǫ��
	void pushObject(boss* boss);
	//�÷��̾� ��
	void pushObject(playerGun* gun);
	//�Ѿ�
	void pushObject(tagBullet bullet);
	//������
	void pushObject(item* item);

	//=========================
	
	//ī�޶�
	void setCmove(POINTF cmove) { _cMove = cmove; }
	void setCmouse(POINTF cmouse) { _cMouse = cmouse; }


};

