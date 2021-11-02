#pragma once
#include "singletonBase.h"
#include <vector>


class dataStorage : public singletonBase<dataStorage>
{
private:

	//Ÿ�� ����

	//Ÿ�� �����
	TILE tile0[TMAPSIZEX0 * TMAPSIZEY0];
	TILE tile1[TMAPSIZEX1 * TMAPSIZEY1];
	TILE tile2[TMAPSIZEX2 * TMAPSIZEY2];
	TILE tile3[TMAPSIZEX3 * TMAPSIZEY3];
	TILE tile4[TMAPSIZEX4 * TMAPSIZEY4];
	TILE tile5[TMAPSIZEX5 * TMAPSIZEY5];
	TILE tile6[TMAPSIZEX6 * TMAPSIZEY6];
	TILE tile7[TMAPSIZEX7 * TMAPSIZEY7];
	TILE tile8[TMAPSIZEX8 * TMAPSIZEY8];

	string	_archiveName;					//����� �̸�
	int		_archiveNum;					//����� ��ȣ

	//�÷��̾� ����
	int		_stage;			//��������
	int		_PlayerType;	//�÷��̾� Ÿ��
	int		_maxHeart;		//�÷��̾� �ִ� �����
	int		_heart;			//�÷��̾� ����
	int		_barrior;		//��
	int		_blankBullet;	//����ź
	int		_key;			//����
	int		_money;			//��
	bool	_sGun;			//�ر� ����

public:

	HRESULT init();		//����

	//Ÿ�ϸ� ���� �����
	void saveTileMap(int archivenum, vector<TILE> vtile);			//Ÿ�ϸ��� �迭�� ����
	void saveTileMap();												//�迭�� �ؽ�Ʈ�� ����
	void loadTileMap();												//Ÿ�ϸ� �ҷ�����
	void addTileMap(int archivenum, vector<TILE>& vtile , bool tf);	//Ÿ�ϸ� �޾ƿ���
	void addTileMap(int archivenum, vector<TILE>& vtile ,int& sizex, int& sizey, bool tf);	//Ÿ�ϸʰ� ����,���� ũ�� �޾ƿ���

	//�÷��̾� ���� �����
	void setPlayerData(
		int	maxHeart	//�ִ� �����
		, int	heart		//�����
		, int	barrior		//��
		, int	blankBullet	//����ź
		, int	key			//����
		, int	money);		//��		

	void setPlayerType(int playerType) { _PlayerType = playerType; }
	int getPlayerType() { return _PlayerType; }


	//�������� ����� �ڷ�� �޾ư��� ����
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