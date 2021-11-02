#pragma once
#include "gameNode.h"
#include <vector>

#define TBTSIZE	5

#define PSIZEX	10
#define PSIZEY	10

enum SpuitType
{
	S_MAP,
	S_OBJECT
};

class mapTool : public gameNode
{
private:

	//========================= 화면 =========================

	RECT					_activeRect;	//작업 구역
	BUTTON					_bt[TBTSIZE];	//버튼

	//===================== 타일맵 캔버스 =====================

	TILE					_tile;			//타일
	vector<TILE>			_vTile;			//타일 벡터
	vector<TILE>::iterator	_vITile;		//타일 벡터의 반복자

	POINT					_tMapSize;		//타일맵 캔버스 사이즈
	int						_tX, _tY;		//타일맵 이동값
	bool					_activeMap;		//타일맵 캔버스 생성 여부

	//====================== 정보창 ===========================

	bool		_sampleInfo;				//정보창 활성화 여부
	int			_plusX;						//정보창 이동값

	//캔버스 정보 출력용
	TILE		_cTile;						//정보 출력용 타일

	//======================= 팔레트 =========================

	image*		_paletteImg;				//팔레트 이미지
	TILE		_palette[PSIZEX * PSIZEY];	//팔레트
	
	//======================= 스포이드 =======================

	SpuitType	_spuitType;					//스포이드 타입
	TILE		_spuit;						//스포이드

	//======================== 저장소 ========================

	int			_archiveNum;				//저장소 번호
	int			_palletNum;					//팔레트 번호

private:

	void createTileMap();					//타일맵 생성
	void deleteTileMap();					//타일맵 삭제
	void moveTileMap();						//타일맵 변경
	void setTileData();						//타일맵 변경
	void viewTileData();					//특정 타일 정보 확인

	void paletteManage(int palletnum);		//팔레트 생성 및 관리 함수
	
	void createButton();					//버튼 생성
	void clickButton();						//버튼 상호작용
	
	void moveSampleInfo();					//정보창 상호작용

	void changeSpuit();						//스포이드 값 저장

	void selectArChive();					//저장소 선택

public:

	virtual HRESULT init();					//초기화 함수
	virtual void release();					//메모리 해제 함슈
	virtual void update();					//연산하는 함수
	virtual void render();					//그리기 함수

};