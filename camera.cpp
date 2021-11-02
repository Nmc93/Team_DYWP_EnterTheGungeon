#include "pch.h"
#include "camera.h"
#include "player.h"

HRESULT camera::init()
{
	//ī�޶� �̵�ġ
	_cCurrent = { 0,0 };

	//ī�޶� �̵� ����ġ
	_cMouse = { 0,0 };

	//�÷��̾� Ȯ��
	_isSelect = false;

	return S_OK;
}

void camera::release()
{
}

void camera::update()
{
	continueMove();
}

void camera::render()
{
	SetBkMode(getMemDC(), OPAQUE);//TRANSPARENT
	SetTextColor(getMemDC(), RGB(0, 0, 0));

	RECT tRc = RectMake(5, 306, 110, 78);
	Rectangle(getMemDC(), tRc);

	char str[128];
	sprintf_s(str, "  ī�޶� ����");
	TextOut(getMemDC(), 5, 300, str, strlen(str));

	sprintf_s(str, "  X   : %0.2f", _cCurrent.x);
	TextOut(getMemDC(), 5, 316, str, strlen(str));
	sprintf_s(str, "  Y   : %0.2f", _cCurrent.y);
	TextOut(getMemDC(), 5, 332, str, strlen(str));
	sprintf_s(str, " MX : %0.2f", _cMouse.x);
	TextOut(getMemDC(), 5, 348, str, strlen(str));
	sprintf_s(str, " MY : %0.2f", _cMouse.y);
	TextOut(getMemDC(), 5, 364, str, strlen(str));
}

void camera::continueMove()
{
	/*
	// �¿�
	if (_camWidth == CAMERADIR::LEFT)  _currentX -= _speed;
	if (_camWidth == CAMERADIR::RIGHT) _currentX += _speed;

	// ����
	if (_camHeight == CAMERADIR::UP)   _currentY -= _speed;
	if (_camHeight == CAMERADIR::DOWN) _currentY += _speed;

	_x = _mx + _currentX;
	_y = _my + _currentY;
	*/
	//�÷��̾ ���õǾ�����
	if (_isSelect)
	{
		//if (getDistance(_ptMouse.x, _ptMouse.y, MIDDLEX, MIDDLEY) > 100)
		//{
			_cMouse.x = (_ptMouse.x - MIDDLEX);
			_cMouse.y = (_ptMouse.y - MIDDLEY);
		//}
	}
}

void camera::cameraMove()
{
	//�� ī�޶�
	/*
	// Ű���� ��ǥ
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		_camWidth = CAMERADIR::LEFT;
		_currentX -= _speed;
	}
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		_camWidth = CAMERADIR::RIGHT;
		_currentX += _speed;
	}
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		_camHeight = CAMERADIR::UP;
		_currentY -= _speed;

	}
	if (KEYMANAGER->isStayKeyDown('S')) {
		_camHeight = CAMERADIR::DOWN;
		_currentY += _speed;
	}

	if (!KEYMANAGER->isStayKeyDown('A') && !KEYMANAGER->isStayKeyDown('D')) _camWidth = CAMERADIR::CENTER;
	if (!KEYMANAGER->isStayKeyDown('W') && !KEYMANAGER->isStayKeyDown('S')) _camHeight = CAMERADIR::CENTER;
	
	

	// ���콺 ��ǥ
	int stdX = _ptMouse.x - WINSIZEX / 2;
	int stdY = _ptMouse.y - WINSIZEY / 2;

	// �ٶ󺸱�
	if (stdX > 100)  _mx = stdX - 100;
	else if (stdX < -100) _mx = stdX + 100;
	else				  _mx = 0;

	if (stdY > 100)  _my = stdY - 100;
	else if (stdY < -100) _my = stdY + 100;
	else				  _my = 0;

	_x = _mx + _currentX;
	_y = _my + _currentY;
	*/


}

void camera::breakMove(int center)
{
	if (center - _x < WINSIZEX / 2) _x -= 5;
	if (center - _x > WINSIZEX / 2) _x += 5;

	//_currentX = _x;
}


