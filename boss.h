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
	ChairActState _cAS;			//보스 의자 액트 스테이트
	image* _cImg;				//보스 의자 이미지
	RECT _chairRC;				//보스 의자 렉트
	float _cx, _cy;				//보스 의자 좌표
	int _cCount;				//으자 프레임 카운트
	int _cIndex;				//으자 프레임 넘버 인덱스

	bossActState _bAS;			//보스 캐릭터 액트 스테이트
	image* _bImg;				//보스 캐릭터 이미지
	RECT _bossRC;				//보스 캐릭터 렉트
	float _bx, _by;				//보스 캐릭터 좌표
	int _bIndex;				//보스 프레임 넘버 인덱스

	float _sx, _sy;				//그림자 좌표
	image* _sImg;				//그림자 이미지
	RECT _sRc;					//그림자 렉트
	bool _isActive;				//등장씬

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
	progressBar* _hpBar;		//hp바
	float _currentHP, _maxHP;	//hp바

	image* _bulletKingBack;		//등장 씬에 필요한것들 
	image* _prisonerBack;		//죄수 등장씬 이미지
	image* _marineBack;			//해병 등장씬 이미지
	bool _isAppear;				//등장 했니 안했니?
	bool _isWho;				//죄수니 해병이니?
	int _appearTimer;			//등장 씬 타이머용
	int _bCount;				//등장 씬 프레임 카운트용
	RECT _bRc;					//등장 씬 렉트

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

	//그림자 x중점좌표
	float getShadowX() { return _sx; }
	//그림자 y중점좌표
	float getShadowY() { return _sy; }
	//몸통 x중점좌표
	float getBodyX() { return _bx; }
	//몸통 y중점좌표
	float getBodyY() { return _by; }
	
	//그림자 렉트
	RECT getShadowRc() { return _sRc; }
	//몸통 렉트
	RECT getBodyRc() { return _bossRC; }
	//의자 렉트
	RECT getChairRc() { return _chairRC; }

	//그림자 이미지
	image* getShadowImg() { return _sImg; }
	//몸통 이미지
	image* getBodyImg() { return _bImg; }
	//의자 이미지
	image* getChairImg() { return _cImg; }
	
	//몸통 프레임
	int getBFrame() { return _bIndex; }
	//의자 프레임
	int getCFrame() { return _cIndex; }

	void setAppear(bool appear) { _isAppear = appear; }
	void setWho(bool iswho) { _isWho = iswho; }

};
