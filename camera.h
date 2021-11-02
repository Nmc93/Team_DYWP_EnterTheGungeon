#pragma once
#include "gameNode.h"

#define CAMERASPEED 4.0f

enum class CAMERADIR
{
	CENTER,
	UP,
	DOWN,
	LEFT,
	RIGHT
};


class camera : public gameNode
{
private:

	//플레이어 위치 보정값
	POINTF _cCurrent;

	//마우스 위치 보정값
	POINTF _cMouse;


	// 키보드에 따른 좌표
	float _x, _y;

	// 마우스에 따른 좌표
	float _mx,_my;
	float _mSpeed;

	bool _isSelect;		// 둘 중에 선택할 때 카메라 움직임(좌우만 이동)

	CAMERADIR _camWidth;
	CAMERADIR _camHeight;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void continueMove();
	void cameraMove();
	void breakMove(int center);

	//====================

	//카메라값 받아오기
	void setCmove(POINTF cmove) { _cCurrent = cmove; }
	//카메라값 보내주기
	POINTF getCurrent() { return _cCurrent; }
	//카메라값 받아오기
	void setCMouse(POINTF cmove) { _cMouse = cmove; }
	//카메라값 보내주기
	POINTF getCMouse() { return _cMouse; }

	//====================

	// 카메라 getter, setter
	float getX() { return _x; }
	void setX(float x) { _x = x; }

	float getY() { return _y; }
	void setY(float y) { _y = y; }

	//float getMx() { return _mx; }
	//float getMy() { return _my; }

	//float getCX() { return _currentX; }
	//void setCX(float cx) { _currentX = cx; }

	//float getCY() { return _currentY; }
	//void setCY(float cy) { _currentY = cy; }

	void setIsSelect(bool isSel) { _isSelect = isSel; }
	bool getIsSelect() { return _isSelect; }
};

