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

	bullet* _bull;	//총알

	GunState _gState;	// 총의 상태
	GunType _gType;		// 총 타입
	POINT	_frame;		// 총기 이미지의 프레임
	int		_fCount;	//프레임 카운트

	//좌표 및 이미지
	image*	_gImg;		// 총 이미지
	POINTF	_pXY;		// 플레이어 중점좌표
	int		_plusX;		// X보정값
	int		_plusY;		// Y보정값
	POINTF	_sXY;		// 그림자의 중점좌표
	POINTF	_bXY;		// 총의 중점좌표
	RECT	_rc;		// 총의 렉트
	float	_fly;		// 총의 떠있는 높이
	float	_angle;		// 총의 각도

	float _shotAngle;	//발사 앵글

	//총기 성능
	int		_maxBullet;	// 최대 총알 갯수
	int		_maxDelay;	// 총 맥스 발사 딜레이
	int		_reloadTime;// 재장전 시간
	int		_accuracy;	// 총의 정확도

	//딜레이 및 카운트
	int		_bullet;	// 총알
	int		_sCount;	// 발사 딜레이
	int		_rCount;	// 재장전
	bool	_isReload;  // 재장전 중?
	bool	_isFire;	// 총 발사?

	int _imageKey;		//Zorder용 키값
	
	bool isShotGun;		//총기 해금

public:

	// 센터 값
	virtual HRESULT init(POINTF pPt, GunType guntype);
	virtual void release();
	virtual void update();
	virtual void render();

	//총이 돌아감
	void rotate();

	//총의 상태에 따른 변화
	void gunState();

	//총 변경
	void gunChage();

	//총 발사 함수
	void isFire();

	//총알 장전
	void addReload();

	//Z오더용 키값 생성
	void keyMake();

	//그림자의 중점좌표
	POINTF getSXY() { return _sXY; }
	//몸통의 중점좌표
	POINTF getGXY() { return _sXY; }
	//몸통의 렉트좌표
	RECT getGRc() { return _rc; }
	//플레이어의 중점좌표
	POINTF getPXY() { return _pXY; }
	//플레이어 중점좌표 받기
	void setPXY(POINTF pXY) {_pXY = pXY; }
	//총의 프레임
	POINT getFrame() { return _frame; }
	//총의 이미지
	image* getImage() { return _gImg; }

	//앵글 값
	void setAngle(float angle) { _angle = angle; }
	float getAngle() { return _angle; }

	//총알 받아오기
	void setBullet(bullet* bullet) { _bull = bullet; }

	void setSAngle(float angle) { _shotAngle = angle; }

	int getKey() { return _imageKey; }
};
