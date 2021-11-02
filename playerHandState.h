#pragma once
#include "gameNode.h"
#include "bullet.h"

#define LENGTH 15

enum class GunState
{
	READY,
	SHOOT,
	RELOAD
};

enum class GunType
{
	HANDGUN,
	SHOTGUN
};

class playerGun : public gameNode
{
protected:

	bullet* _bull;	//�Ѿ�

	GunState _gState;	// ���� ����
	GunType _gType;		// �� Ÿ��
	POINT	_frame;		// �ѱ� �̹����� ������
	int		_fCount;	//������ ī��Ʈ

	//��ǥ �� �̹���
	image*	_gImg;		// �� �̹���
	POINTF	_pXY;		// �÷��̾� ������ǥ
	int		_plusX;		// X������
	int		_plusY;		// Y������
	POINTF	_sXY;		// �׸����� ������ǥ
	POINTF	_bXY;		// ���� ������ǥ
	RECT	_rc;		// ���� ��Ʈ
	float	_fly;		// ���� ���ִ� ����
	float	_angle;		// ���� ����

	float _shotAngle;	//�߻� �ޱ�

	//�ѱ� ����
	int		_maxBullet;	// �ִ� �Ѿ� ����
	int		_maxDelay;	// �� �ƽ� �߻� ������
	int		_reloadTime;// ������ �ð�
	int		_accuracy;	// ���� ��Ȯ��

	//������ �� ī��Ʈ
	int		_bullet;	// �Ѿ�
	int		_sCount;	// �߻� ������
	int		_rCount;	// ������
	bool	_isReload;  // ������ ��?
	bool	_isFire;	// �� �߻�?

	int _imageKey;		//Zorder�� Ű��
	
	bool isShotGun;		//�ѱ� �ر�

public:

	// ���� ��
	virtual HRESULT init(POINTF pPt, GunType guntype);
	virtual void release();
	virtual void update();
	virtual void render();

	//���� ���ư�
	void rotate();

	//���� ���¿� ���� ��ȭ
	void gunState();

	//�� ����
	void gunChage();

	//�� �߻� �Լ�
	void isFire();

	//�Ѿ� ����
	void addReload();

	//Z������ Ű�� ����
	void keyMake();

	//�׸����� ������ǥ
	POINTF getSXY() { return _sXY; }
	//������ ������ǥ
	POINTF getGXY() { return _sXY; }
	//������ ��Ʈ��ǥ
	RECT getGRc() { return _rc; }
	//�÷��̾��� ������ǥ
	POINTF getPXY() { return _pXY; }
	//�÷��̾� ������ǥ �ޱ�
	void setPXY(POINTF pXY) {_pXY = pXY; }
	//���� ������
	POINT getFrame() { return _frame; }
	//���� �̹���
	image* getImage() { return _gImg; }

	//�ޱ� ��
	void setAngle(float angle) { _angle = angle; }
	float getAngle() { return _angle; }

	//�Ѿ� �޾ƿ���
	void setBullet(bullet* bullet) { _bull = bullet; }

	void setSAngle(float angle) { _shotAngle = angle; }

	int getKey() { return _imageKey; }
};
