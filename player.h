#pragma once
#include "gameNode.h"
#include <vector>

#define MOVESPPED 5.0f
#define ROLLSPEED 4.0f

//플레이어 방향
enum class PlayerDir
{
	CENTER,
	L,
	LT,
	T,
	RT,
	R,
	RB,
	B,
	LB
};

//플레이어 종류
enum class PlayerType
{
	MARINE,
	PRISONER
};

//플레이어 행동
enum class PlayerState
{
	IDLE,
	WALK,
	ROLL,
	HIT,
	DEAD,
	TABLE,
	FALL,
	ROLLFALL,
	TABLESLIDE,
	CHARSELECT,
	INTOSTAGE
};

//총의 타입
enum class PlayerGunType
{
	NORMAL,
	ONEHAND,
	TWOHAND
};

class player : public gameNode
{
private:

	//itemManager* _itemManager;

	//생명
	int _heart;
	//최대 생명
	int _maxHeart;
	//방어막
	int _barrior;
	//공포탄
	int _blankBullet;
	//열쇠
	int _key;
	//돈
	int _money;

	//공포탄 관련
	int _windX, _windY;
	int _windWidth, _windHeight;
	image* _windImg;
	RECT _windRc;

	bool _isWind;

protected:

	//==========================================================================

	//bullet*		_bullet;		// 값을 받아올 것

	//====== 몸통, 그림자 이미지 관련==============================================

	image*		_bImg;		//몸통 이미지 
	image*		_sImg;		//그림자 이미지
	PlayerType	_pType;		//플레이어 타입

	POINT		_frame;		//이미지 프레임
	int			_frameCount;//프레임 카운트

	// =========== 몸통 ===========================================================

	POINTF		_bXY;		//몸통의 중점좌표
	int			_fly;		//몸통과 그림자의 거리
	RECT		_bRc;		//몸통의 RECT

	//========== 그림자 ===========================================================

	POINTF		_sXY;		//그림자의 중점좌표
	RECT		_sRc;		//그림자의 RECT

	//======= 플레이어 방향 ========================================================

	PlayerDir	_pDir;			//플레이어의 방향
	PlayerState _pState;		//플레이어의 상태
	PlayerGunType _armType;		//플레이어의 손 갯수
	bool		_isControl;		//플레이어 조종 선택
	float		_angle;			//플레이어와 마우스의 각도

	bool		_isRoll;		// 구르는 중인지 유무
	int			_rollTime;		// 구르는 시간
	bool		_isHit;			// 맞았는지 유무
	bool		_isTableUp;		// 테이블 주변인지 유무

	//======= 카메라 보정값 =======================================================

	POINTF		_camXY;			//카메라 이동값
	POINTF		_cMove;			//카메라 이동 보정치
	//POINTF		_

	//======= 타일검출때 필요한것 ==================================================

	vector<TILE> _vMap;	//타일 저장용 벡터
	POINT		 _tileMax;	//타일의 최대XY크기

	RECT		_rcCollision[4];
	int			_playerTileX, _playerTileY;	//플레이어 x y
	int			_tileIndex[4];				//플레이어랑 닿는 인덱스 번호

	bool		_isStart;					//게임 시작

	//====== 타일 벡터 ========================
	/*
	vector<TILE>		_map; //TILE형식의 백터를 가지고잇는 _map
	int _mapSizeX;			  //맵가로크기
	int _mapSizeY;
	*/

	// 알파애니렌더
	int _alpha;
	int _alphaCount;


	// 총의 상태들
	//gun* _gun;
	//normalGun* _normalGun;
	//shotGun* _shotGun;

public:

	// 기본 함수  ===========================================================

	//플레이어 소환(중점xy,조종여부,플레이어 종류)
	virtual HRESULT init(POINTF xy, bool active, PlayerType playertype, PlayerGunType armtype);
	virtual HRESULT init(POINTF xy, bool active, PlayerGunType armtype);
	virtual void release();
	virtual void update();
	virtual void render();

	//플레이어 이동
	void playerMove(PlayerDir pd);

	void frameUpdate();
	void imageChange();
	//void keyMove();
	void mouseMove();
	void draw();
	void gunDraw();

	void hit();
	void wind();

	void dataSave();

	//타일 검색용
	void playerTile(PlayerDir pd);

	//이벤트 시작알림
	bool getEvent() { return _isStart;  }

	// Getter,Setter ===========================================================
	
	//그림자 중점 좌표
	void setShadowXY(POINTF position) { _sXY = position; }
	//그림자 중점 좌표
	POINTF getShadowXY() { return _sXY; }

	//그림자 LEFT TOP 좌표
	RECT getShadowRc() { return _sRc; }
	
	//몸통 LEFT TOP 좌표
	RECT getBodyRc() { return _bRc; }	

	//체공 변수변경
	void setFly(int fly) { _fly = fly; }
	int getFly() { return _fly; }
	
	//그림자 중점 좌표
	void setBodyXY(POINTF position) { _sXY = position; }
	//그림자 중점 좌표
	POINTF getBodyXY() { return _sXY; }

	//이미지 프레임
	void setFrame(POINT frame) { _frame = frame; }
	//이미지 프레임
	POINT getFrame() { return  _frame; }
	
	//몸통 이미지
	image* getBodyImg() { return _bImg;}
	//그림자 이미지
	image* getShadowImg() { return _sImg;}

	//캐릭터 조종 여부
	bool getActive() { return _isControl; }
	//캐릭터 조종 입력
	void setActive(bool active) { _isControl = active; }
	
	//카메라 이동 보정치
	POINTF getCMove() { return _cMove; }
	//앵글값
	float getAngle() { return _angle; }

	//타일셋 받아오기
	void setTile0(vector<TILE> tile, int tilex, int tiley);

	// 구르기
	bool getIsRoll() { return _isRoll; }
	// 구르기
	void addIsRoll();

	//상태 반환
	PlayerState getState() { return _pState; }

	PlayerType getPlayerType() { return _pType; }
	
	//총 상태 불러오기
	PlayerGunType getgunType() { return _armType; }
	//총 상태 변환
	void setgunType(PlayerGunType armtype) { _armType = armtype; }
	//=====================================================================


	// 플레이어 공통 status
	int getHeart() { return _heart; }
	void setHeart(int heart) { _heart = heart; }

	int getBarrior() { return _barrior; }
	void setBarrior(int barrior) { _barrior = barrior; }

	int getBlankBullet() { return _blankBullet; }
	void setBlankBullet(int blankBullet) { _blankBullet = blankBullet; }

	int getKey() { return _key; }
	void setKey(int key) { _key = key; }

	int getMoney() { return _money; }
	void setMoney(int money) { _money = money; }

	PlayerDir getDirection() { return _pDir; }

	void deleteVMap() { _vMap.clear(); vector<TILE>().swap(_vMap); }

	// 지울 꺼
	//void setIM(itemManager* im) { _itemManager = im; }
};

