#pragma once
#include "gameNode.h"
#include "enemyState.h"
#include "camera.h"
#include "bullet.h"
#include "itemManager.h"

class enemy : public gameNode
{
protected:
	// 프레임
	int _idX, _idY;
	int _iCount;
	bool _isLoop;			// 루프 돌릴것인지
	bool _isTrigger;		// 콜백 함수처럼

	// 프레임 조건
	int _continueCount;	// 시작 이미지

	itemManager* _im;
	//camera* _camera;
	bullet* _bullet;

	// 적관련 
	POINTF	_sXY;					// 그림자 중점좌표
	POINTF	_bXY;					// 몸통의 중점좌표
	float _currentX, _currentY;		// 이동치

	RECT		_sRc;				// 그림자 렉트
	RECT		_bRc;				// 몸통 렉트
	float		_fly;				// 그림자와 몸통의 거리

	//setter로 init이전에 초기화 필요함
	POINTF		_pXY;				// 플레이어의 그림자 중점좌표
	RECT		_playerRc;			// 플레이어의 몸통 렉트

	image* _image;					// 소환시 이미지
	image* _sImg;					// 그림자 이미지
	image* _enemyImg;				// 적 몸통 이미지

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

	ENEMYSTATE _enemyState; //enum class STATE 받아오는 변수
	ENEMYDIRECTION _enemyDir;
	ENEMYTYPE _enemyType;

	//enemyState에서 가져오는 클래스임!
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

	// 설, 접
	ENEMYDIRECTION getEDir() { return _enemyDir; }

	//camera* getCamera() { return _camera; }
	void setIM(itemManager* im) { _im = im; }

	//void setCamera(camera* camera) { _camera = camera; }
	void setBullet(bullet* bullet) { _bullet = bullet; }

	//그림자의 중점좌표
	POINTF getShadow() { return _sXY; }
	//그림자의 렉트
	RECT getSRect() { return _sRc; }
	//몸통의 중점좌표
	POINTF getBody() { return _bXY; }
	//몸통의 렉트
	RECT getBRect() { return _bRc; }

	image* getBImg() { return _enemyImg; }
	image* getSImg() { return _sImg; }
	image* getCImg() { return _image; }

	bool getAppear() { return _appear; }

	//이미지 프레임
	int getIdX() { return _idX; }
	int getIdY() { return _idY; }

	//플레이어값
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
