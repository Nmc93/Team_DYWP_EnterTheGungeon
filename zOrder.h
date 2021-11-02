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

//z오더 구조체
struct Unit
{
	UnitType unitType;	//비교용 타입
	//============ 키값 ===========
	float	key;	//비교용 그림자 중점좌표
	//========== 좌표 관련 ========
	POINTF	shadow;	//그림자 LEFT TOP 좌표
	POINTF	body;	//몸통 LEFT TOP 좌표
	//========== 이미지 관련 =======
	POINT	frame;	//이미지 프레임
	image* bImage;	//몸통 이미지
	image* sImage;	//그림자 이미지
	//========== 추가 변수 ========
	image* plusImg;	//추가 이미지
	int	plusint;	//추가 숫자
	POINTF plusPointf;//추가 포인트F
	POINT plusPoint;	//추가 포인트
	RECT plusRc;	//추가 렉트
	bool   plusbool; //추가 변수값
};

class zOrder : public gameNode
{
private:

	vector<Unit*> _vObj;
	POINTF _cMove;
	POINTF _cMouse;

private:
	//벡터 초기화 함수
	void vClear() { _vObj.clear(); }

public:

	virtual HRESULT init();						//초기화 함수
	virtual void release();						//메모리 해제 함슈
	virtual void update();						//연산하는 함수
	virtual void render();						//그리기 함수
	
	//이미지 출력용 함수
	void drawImage(vector<Unit*>& vOrder);	
	
	//정렬반복
	void merge(int left, int right);	
	//합병정렬
	void mergeSort(int left, int right);		

	//플레이어푸시
	void pushObject(player* player);
	//적푸시
	void pushObject(enemy* em);
	//보스푸시
	void pushObject(boss* boss);
	//플레이어 총
	void pushObject(playerGun* gun);
	//총알
	void pushObject(tagBullet bullet);
	//아이템
	void pushObject(item* item);

	//=========================
	
	//카메라
	void setCmove(POINTF cmove) { _cMove = cmove; }
	void setCmouse(POINTF cmouse) { _cMouse = cmouse; }


};

