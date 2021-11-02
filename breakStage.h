#pragma once
#include "gameNode.h"
#include "camera.h"
#include "zOrder.h"
#include "player.h"
#include "itemManager.h"
#include "enemy.h"
#include <vector>


class breakStage : public gameNode
{
private:

	camera* _camera;
	zOrder* _zOrder;
	
	bool _isSelect;
	
	//==== 플레이어 ================================================

	player* _marine;		//해병
	player* _prisoner;		//죄수
	int		_pNum;			//플레이어 설정용 변수

	//맵 생성
	vector<TILE>	_vMap;							//맵 타일
	vector<Unit*> _vOrder;							//Z오더용 벡터

	//이벤트용 변수
	bool	_isNext;
	int		_nextNum;

	//키 입력용 변수
	bool _isKey[4];
	
	//키 변경값
	int _nextInfo;

private:

	void createTileMap();			//맵 생성 함수

public:

	virtual HRESULT init();			//초기화 함수
	virtual void release();			//메모리 해제 함슈
	virtual void update();			//연산하는 함수
	virtual void render();			//그리기 함수

	void selectPlayer(bool select);	//플레이어 선택

	void nextStage();				//다음 맵으로 넘어가는 함수
	void keySelect();				//키이름
	
};

