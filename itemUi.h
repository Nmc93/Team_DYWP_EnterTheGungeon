#pragma once
#include "gameNode.h"
#include <vector>

enum class HTYPE
{
	ONE,
	HALF,
	ZERO
};

enum class ITYPE
{
	HEART,
	BLANK_BULLET,
	BARRIOR,
	KEY,
	MONEY,
	DROPTAN
};

class item : public gameNode
{
protected:
	float _x, _y;
	float _dropX, _dropY;

	RECT _rc;
	image* _img;

	ITYPE _iType;
	
	bool _isFollow;

	// 하트만을 위한 변수
	int _curretFrameX;	// frameX 값

public:
	virtual HRESULT init(int x, int y) = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	virtual void itemNum(bool plus) {};

	// getter, setter
	virtual ITYPE getItype() { return _iType; }

	virtual int getX() { return _x; }
	virtual void setX(int x) { _x = x; }
	virtual int getY() { return _y; }
	virtual void setY(int y) { _y = y; }

	virtual int getDx() { return _dropX; }
	virtual void setDx(int x) { _dropX = x; }
	virtual int getDy() { return _dropY; }
	virtual void setDy(int y) { _dropY = y; }

	virtual RECT getRect() { return _rc; }

	virtual int getCurrentFrameX() { return _curretFrameX; }

	virtual void setIsFollow(bool isFollow) {}
	virtual bool getIsFollow() { return _isFollow; }

};

class heart : public item
{
private:
	HTYPE _hType;

public:
	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void itemNum(bool plus);
	void changeItem();
};

class barrior : public item
{
public:
	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();
};

class blankBullet : public item
{
public:
	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();
};

class key : public item
{
public:
	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();
};

class money : public item
{
public:
	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();
};

class dropTan : public item
{
public:
	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();
};