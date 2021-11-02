#pragma once
#include "gameNode.h"
#include "enemyState.h"
#include "camera.h"
#include "bullet.h"
#include "itemManager.h"

class enemy : public gameNode
{
protected:
	// ������
	int _idX, _idY;
	int _iCount;
	bool _isLoop;			// ���� ����������
	bool _isTrigger;		// �ݹ� �Լ�ó��

	// ������ ����
	int _continueCount;	// ���� �̹���

	itemManager* _im;
	//camera* _camera;
	bullet* _bullet;

	// ������ 
	POINTF	_sXY;					// �׸��� ������ǥ
	POINTF	_bXY;					// ������ ������ǥ
	float _currentX, _currentY;		// �̵�ġ

	RECT		_sRc;				// �׸��� ��Ʈ
	RECT		_bRc;				// ���� ��Ʈ
	float		_fly;				// �׸��ڿ� ������ �Ÿ�

	//setter�� init������ �ʱ�ȭ �ʿ���
	POINTF		_pXY;				// �÷��̾��� �׸��� ������ǥ
	RECT		_playerRc;			// �÷��̾��� ���� ��Ʈ

	image* _image;					// ��ȯ�� �̹���
	image* _sImg;					// �׸��� �̹���
	image* _enemyImg;				// �� ���� �̹���

	bool _dropTan;

	int _alpha;
	int _alphaCount;
	int _dieType;
	int _attackType;

	float _currentAngle;
	float _angle;

	bool _isMove;
	int _count;
	int _count2;

	bool _isEnemyDie;

	bool _appear;

	int _currentHP;
	int _maxHP;
	bool _isHit;

	enemyState* _emState;

	ENEMYSTATE _enemyState; //enum class STATE �޾ƿ��� ����
	ENEMYDIRECTION _enemyDir;
	ENEMYTYPE _enemyType;

	//enemyState���� �������� Ŭ������!
	enemyIdle* _idle;
	enemyMove* _move;
	enemyAttack* _attack;
	enemyHit* _hit;
	enemyDie* _die;

	animation* _enemyMotion;
	animation* _enemyAppearMotion;

public:
	enemy();
	~enemy();

	virtual HRESULT init(ENEMYTYPE enemyType, POINTF spt);
	virtual void release();
	virtual void update();
	virtual void render();

	void frameCount();
	void dir();
	void move();
	void setImage();
	void setAni();

	virtual void attack() = 0;

	virtual void changeState(ENEMYSTATE state) = 0;
	virtual void changeAni() = 0;
	void changeAlpha();

	//void enemyBulletFire();

	void hitDamage(int damage);

	static void returnIdle(void* obj);
	static void appearEnemy(void* obj);

	bool getDropTan() { return _dropTan; }
	void setDropTan(bool dropTan) { _dropTan = dropTan; }

	// ��, ��
	ENEMYDIRECTION getEDir() { return _enemyDir; }

	//camera* getCamera() { return _camera; }
	void setIM(itemManager* im) { _im = im; }

	//void setCamera(camera* camera) { _camera = camera; }
	void setBullet(bullet* bullet) { _bullet = bullet; }

	//�׸����� ������ǥ
	POINTF getShadow() { return _sXY; }
	//�׸����� ��Ʈ
	RECT getSRect() { return _sRc; }
	//������ ������ǥ
	POINTF getBody() { return _bXY; }
	//������ ��Ʈ
	RECT getBRect() { return _bRc; }

	image* getBImg() { return _enemyImg; }
	image* getSImg() { return _sImg; }
	image* getCImg() { return _image; }

	bool getAppear() { return _appear; }

	//�̹��� ������
	int getIdX() { return _idX; }
	int getIdY() { return _idY; }

	//�÷��̾
	POINTF getPlayerXY() { return _pXY; }
	void setPlayerXY(POINTF pxy) { _pXY = pxy; }
	RECT getPlayerRc() { return _playerRc; }
	void setPlayerRc(RECT prc) { _playerRc = prc; }

	int getEnemyHP() { return _currentHP; }

	ENEMYTYPE getEnemyType() { return _enemyType; }
	ENEMYSTATE getEnemyState() { return _enemyState; }

	void setAppear(bool appear) { _appear = appear; }

	bool getEnemyDie() { return _isEnemyDie; }
};

class normalEnemy : public enemy
{
public:
	virtual HRESULT init(ENEMYTYPE enemyType, POINTF spt);
	virtual void release();
	virtual void update();
	virtual void render();

	void setImage();
	void setAni();

	void attack();

	virtual void changeState(ENEMYSTATE state);
	virtual void changeAni();
};

class shotgunEnemy : public enemy
{
public:
	virtual HRESULT init(ENEMYTYPE enemyType, POINTF spt);
	virtual void release();
	virtual void update();
	virtual void render();

	void setImage();
	void setAni();

	void attack();

	virtual void changeState(ENEMYSTATE state);
	virtual void changeAni();
};

class bookEnemy : public enemy
{
public:
	virtual HRESULT init(ENEMYTYPE enemyType, POINTF spt);
	virtual void release();
	virtual void update();
	virtual void render();

	void setImage();
	void setAni();

	void attack();

	virtual void changeState(ENEMYSTATE state);
	virtual void changeAni();
};
