#pragma once
#include "singletonBase.h"
#include <vector>


class dataStorage : public singletonBase<dataStorage>
{
private:

	//타일 관련

	//타일 저장소
	TILE tile0[TMAPSIZEX0 * TMAPSIZEY0];
	TILE tile1[TMAPSIZEX1 * TMAPSIZEY1];
	TILE tile2[TMAPSIZEX2 * TMAPSIZEY2];
	TILE tile3[TMAPSIZEX3 * TMAPSIZEY3];
	TILE tile4[TMAPSIZEX4 * TMAPSIZEY4];
	TILE tile5[TMAPSIZEX5 * TMAPSIZEY5];
	TILE tile6[TMAPSIZEX6 * TMAPSIZEY6];
	TILE tile7[TMAPSIZEX7 * TMAPSIZEY7];
	TILE tile8[TMAPSIZEX8 * TMAPSIZEY8];

	string	_archiveName;					//저장소 이름
	int		_archiveNum;					//저장소 번호

	//플레이어 관련
	int		_stage;			//스테이지
	int		_PlayerType;	//플레이어 타입
	int		_maxHeart;		//플레이어 최대 생명력
	int		_heart;			//플레이어 생명
	int		_barrior;		//방어막
	int		_blankBullet;	//공포탄
	int		_key;			//열쇠
	int		_money;			//돈
	bool	_sGun;			//해금 여부

public:

	HRESULT init();		//시작

	//타일맵 관련 저장소
	void saveTileMap(int archivenum, vector<TILE> vtile);			//타일맵을 배열에 저장
	void saveTileMap();												//배열을 텍스트에 저장
	void loadTileMap();												//타일맵 불러오기
	void addTileMap(int archivenum, vector<TILE>& vtile , bool tf);	//타일맵 받아오기
	void addTileMap(int archivenum, vector<TILE>& vtile ,int& sizex, int& sizey, bool tf);	//타일맵과 가로,세로 크기 받아오기

	//플레이어 관련 저장소
	void setPlayerData(
		int	maxHeart	//최대 생명력
		, int	heart		//생명력
		, int	barrior		//방어막
		, int	blankBullet	//공포탄
		, int	key			//열쇠
		, int	money);		//돈		

	void setPlayerType(int playerType) { _PlayerType = playerType; }
	int getPlayerType() { return _PlayerType; }


	//스테이지 저장용 자료들 받아가는 게터
	void setStage(int stageNum) { _stage = stageNum; }
	int getStage()	{ return _stage; }
	int getMaxHp()	{ return _maxHeart; }
	int getHp()		{ return _heart; }
	int getBarrior() { return _barrior; }
	int getBlank()	{ return _blankBullet; }
	int getKey()	{ return _key; }
	int getMoney() { return _money; }
	
	void setsGun(bool sgun) { _sGun = sgun; }
	bool getSGun() { return _sGun; }
};