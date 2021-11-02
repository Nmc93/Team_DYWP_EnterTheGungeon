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

	//�÷��̾� ��ġ ������
	POINTF _cCurrent;

	//���콺 ��ġ ������
	POINTF _cMouse;


	// Ű���忡 ���� ��ǥ
	float _x, _y;

	// ���콺�� ���� ��ǥ
	float _mx,_my;
	float _mSpeed;

	bool _isSelect;		// �� �߿� ������ �� ī�޶� ������(�¿츸 �̵�)

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

	//ī�޶� �޾ƿ���
	void setCmove(POINTF cmove) { _cCurrent = cmove; }
	//ī�޶� �����ֱ�
	POINTF getCurrent() { return _cCurrent; }
	//ī�޶� �޾ƿ���
	void setCMouse(POINTF cmove) { _cMouse = cmove; }
	//ī�޶� �����ֱ�
	POINTF getCMouse() { return _cMouse; }

	//====================

	// ī�޶� getter, setter
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

