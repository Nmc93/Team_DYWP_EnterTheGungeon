#pragma once
#include "gameNode.h"

class player;

class woodTable : public gameNode
{
private:
	image* _tableImg;
	animation* _tableAni;

	int _tableX, _tableY;
	RECT _tableRc;
	RECT _playerRc;

	bool _isTableReverse; //테이블 뒤집힘? true == ㅇㅇ false == ㄴㄴ

	player* _player;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	RECT getTableRc() { return _tableRc; }
	void setTableRc(RECT tableRc) { _tableRc = tableRc; }

	void setPlayerMemory(player* player) { _player = player; }

	void tableReverseDir();
	void tableDir();
	void tempMove();

	void tableAni();
	void tableImg();
};

