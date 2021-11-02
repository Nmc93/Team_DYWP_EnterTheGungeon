#pragma once
#include "gameNode.h"
#include "bullet.h"
#include "progressBar.h"

enum ChairActState
{
	CHAIRIDLE,
	CHAIRATTACK1,
	CHAIRATTACK2,
	CHAIRATTACK3,
	CHAIRNONE
};

enum bossActState
{
	BOSSIDLE,
	BOSSATTACK1,
	BOSSATTACK2,
	BOSSATTACK3,
	BOSSSTUN,
	BOSSHIT,
	BOSSDIE,
	BOSSNONE
};

class boss : public gameNode
{
private:
	ChairActState _cAS;			//���� ���� ��Ʈ ������Ʈ
	image* _cImg;				//���� ���� �̹���
	RECT _chairRC;				//���� ���� ��Ʈ
	float _cx, _cy;				//���� ���� ��ǥ
	int _cCount;				//���� ������ ī��Ʈ
	int _cIndex;				//���� ������ �ѹ� �ε���

	bossActState _bAS;			//���� ĳ���� ��Ʈ ������Ʈ
	image* _bImg;				//���� ĳ���� �̹���
	RECT _bossRC;				//���� ĳ���� ��Ʈ
	float _bx, _by;				//���� ĳ���� ��ǥ
	int _bIndex;				//���� ������ �ѹ� �ε���

	float _sx, _sy;				//�׸��� ��ǥ
	image* _sImg;				//�׸��� �̹���
	RECT _sRc;					//�׸��� ��Ʈ
	bool _isActive;				//�����

	bool _isCheck;

	float _speed;
	int _distance;
	float _angle;
	int _active;
	int _actTimer;
	int _actMaxTimer;
	float _plus;
	RECT _mRC;
	float _px, _py;
	bullet* _bullet;
	progressBar* _hpBar;		//hp��
	float _currentHP, _maxHP;	//hp��

	image* _bulletKingBack;		//���� ���� �ʿ��Ѱ͵� 
	image* _prisonerBack;		//�˼� ����� �̹���
	image* _marineBack;			//�غ� ����� �̹���
	bool _isAppear;				//���� �ߴ� ���ߴ�?
	bool _isWho;				//�˼��� �غ��̴�?
	int _appearTimer;			//���� �� Ÿ�̸ӿ�
	int _bCount;				//���� �� ������ ī��Ʈ��
	RECT _bRc;					//���� �� ��Ʈ

public:

	virtual HRESULT init(int x , int y);
	virtual void update();
	virtual void release();
	virtual void render();

	virtual void imageAll();
	virtual void move();
	virtual void draw();
	virtual void frameChange();
	virtual void bossAi();
	virtual void bulletController();
	virtual void hitDamage(float damage);
	virtual void imageAppear();
	virtual void bulletCollision();

	void setPlayerXY(float x, float y) { _px = x; _py = y; }

	void setBullet(bullet* bullet) { _bullet = bullet; }

	//�׸��� x������ǥ
	float getShadowX() { return _sx; }
	//�׸��� y������ǥ
	float getShadowY() { return _sy; }
	//���� x������ǥ
	float getBodyX() { return _bx; }
	//���� y������ǥ
	float getBodyY() { return _by; }
	
	//�׸��� ��Ʈ
	RECT getShadowRc() { return _sRc; }
	//���� ��Ʈ
	RECT getBodyRc() { return _bossRC; }
	//���� ��Ʈ
	RECT getChairRc() { return _chairRC; }

	//�׸��� �̹���
	image* getShadowImg() { return _sImg; }
	//���� �̹���
	image* getBodyImg() { return _bImg; }
	//���� �̹���
	image* getChairImg() { return _cImg; }
	
	//���� ������
	int getBFrame() { return _bIndex; }
	//���� ������
	int getCFrame() { return _cIndex; }

	void setAppear(bool appear) { _isAppear = appear; }
	void setWho(bool iswho) { _isWho = iswho; }

};
