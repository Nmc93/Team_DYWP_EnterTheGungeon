#pragma once
#include "gameNode.h"
#include "camera.h"
#include "zOrder.h"
#include "player.h"
#include "playerHandState.h"
#include "boss.h"
#include "enemy.h"
#include "itemManager.h"
#include <vector>

#define STAGE1X 400
#define STAGE1Y 400
#define ROOMNUM 5

struct MapStage
{
	int min;
	int max;
};

class stage1 : public gameNode
{
private:
	/*
	camera* _cam;
	
	//맵 세팅
	mapSetting* _mapSet;

	//맵세팅용 벡터와 타일(벡터는 초기 저장용, 실사용은 배열)
	vector<TILE>	_vMap;					// 맵타일 
	TILE			_map[STAGE1X* STAGE1Y];	// 맵
	vector<int>		_sectorNum;				// 섹터 순서

	bool _isEvent[ROOMNUM];					//방 이벤트 발동
	
	//죽은 변수
	POINT setInt;

	//플레이어 관련
	

	//건데드용 변수들
	//보스
	boss* _boss;

	//잡몹
	vector<enemy*> _vEnemy;

	//관리용 테스트 변수
	bool _tileData[2];
	bool _clearStage;

	//맵찾기용 테스트
	int _x, _y;
	int testnum;
	int eventTest;
	*/

	image*	_spaceImg;
	POINTF	_spaceXY;

	//배경.==================================

	camera* _camera;
	zOrder* _zOrder;

	bool _isSelect;

	//==== 플레이어 ================================================

	player* _player;		//해병
	int		_pNum;			//플레이어 설정용 변수

	//==== 적 관련 =================================================

	vector<enemy*> _vEnemy;	//적 벡터
	boss* _boss;			//보스벡터
	bool _isboss;			//보스 활성화 여부

	//==== 총기 관련 ===============================================

	playerGun*	_gun;		//총

	//==== 총알 관련 ===============================================

	bullet* _bullet;		//총알
	itemManager* _itemManager;//아이템매니저

	//==== 타일맵 ================================================

	//맵 생성
	vector<TILE>	_vMap;						//맵 타일
	vector<TILE>	_upMap;	//저장용 윗맵
	vector<TILE>	_downMap;//아랫맵
	int			_mapSizeX,_mapSizeY;			//맵의 사이즈
	int			_sponCheck;						//스폰위치 체크
	

	//이벤트용 변수
	MapStage _mapStage[3];
	int		_sN;
	bool	isNext;
	bool	_stageEnd;
	int		nextNum;
	int		_nCount;	//z카운트
	//이벤트 진행용 변수

	bool _event;	//이벤트 가능 여부
	int _eCont;		//이벤트 중복 횟수
	int _node[3];		//맵 변환용 노드
	bool _nodeEnd;	//맵 변환이 끝났는가?

	//정보 관리용
	int _infoNum;	//정보 출력용 번호

	//키 입력용 변수
	bool _isKey[4];

private:

public:

	virtual HRESULT init();			//초기화 함수
	virtual void release();			//메모리 해제 함슈
	virtual void update();			//연산하는 함수
	virtual void render();			//그리기 함수

	//충돌체크
	void collision();

	void nextStage();
	//캐릭터 플레이
	void playGame();
	//맵생성
	void createMap();
	//플레이어 설정
	void selectPlayer();
	//키보드 입력 받는 함수
	void keySelect();

	//적 생성
	void createEnemy(POINTF exy);

	//이벤트
	void stageEvent(int stage);

	//종료시 타일변경
	void changeTile();
};

