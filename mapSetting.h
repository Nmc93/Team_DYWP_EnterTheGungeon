#pragma once
#include "gameNode.h"
#include <vector>

struct Phase
{
	int mapMin;
	int mapMax;
};


class mapSetting : public gameNode
{
private:

	vector<TILE> _vMap;		//맵
	vector<TILE> _vtemp;	//교체용
	TILE _tempTile;			// 교환용 타일

	vector<int> _closeMap;	//사용된 맵
	vector<int> _closeSector;//사용된 섹터

	Phase _phase[6];//페이즈
	int _phaseNum;	//페이즈 번호

	int startArray;	//시작배열
	int endArray;	//종료배열

	RECT _cRange;	//검사 범위

	int _maxX;	//캔버스의 X 사이즈
	int _maxY;	//캔버스의 Y 사이즈
	int _sectorNum;	//캔버스 지역 번호
	int _roomnum;	//복사할 방의 번호
	int _tempSizeX;	//복사할 방의 X 사이즈
	int _tempSizeY;	//복사할 방의 Y 사이즈

	POINT ranXY;	//방을 설치할 곳을 정하는 랜덤 xy

	int _direction;	//다음 방 설치 방향
	int _sectorSize;//지역 사이즈
	int aTest[4];	//긴 배열 계산의 값을 대신 담아줄 변수

	vector<int> _vWay;//시작 문부터 종료까지 기록
	int currentWay;	//현재 문 배열 위치
	int startWay;	//시작 문 배열 위치
	int nextWay;	//다음 문 배열 위치

	int test;
private:


public:

	virtual HRESULT init(int num);	//초기화 함수
	virtual void release();			//메모리 해제 함슈

	void createTileMap(vector<TILE>& vtile, int xsize, int ysize);			//맵 생성 함수
	void mapSet(int phasenum);//맵 세팅 함수

	void createTileMap2(vector<TILE>& vtile, int xsize, int ysize, int sectorsize, vector<int>& sectornum);
	void mapset2(int phasenum);		//맵 세팅 함수
	bool sectorCheck2(int secnum);	//중복 지역 체크 함수
	
	void findWay(int sector, int nextsector);	//방향을 찾는 함수
	void createWay2();	//길을 찾는 함수
	void bildWay2();	//길을 생성하는 함수

	
};