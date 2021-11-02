#include "pch.h"
#include "progressBar.h"


progressBar::progressBar()
{
}


progressBar::~progressBar()
{
}

HRESULT progressBar::init(int x, int y, int x1, int y1, int width, int height, int width1, int height1)
{
	_x = x;
	_y = y;
	_x1 = x1;
	_y1 - y1;

	_rcProgress = RectMake(x, y, width, height);
	_rcProgress1 = RectMake(x1, y1, width1, height1);

	_progressBarBottom = IMAGEMANAGER->addImage("backBar", "hpBarBottom.bmp", width, height, true, RGB(255, 0, 255));
	_progressBarTop = IMAGEMANAGER->addImage("frontBar", "hpBarTop.bmp", width1, height1, true, RGB(255, 0, 255));

	_width = _progressBarTop->getWidth();

	return S_OK;
}

void progressBar::release()
{
}

void progressBar::update()
{
	_rcProgress1 = RectMakeCenter(_x1, _y1, _progressBarTop->getWidth(),
		_progressBarTop->getHeight());
}

void progressBar::render()
{
	IMAGEMANAGER->render("backBar", getMemDC(),
		_rcProgress.left + _progressBarBottom->getWidth() / 2,
		_y + _progressBarBottom->getHeight() / 2, 0, 0,
		_progressBarBottom->getWidth(), _progressBarBottom->getHeight());

	IMAGEMANAGER->render("frontBar", getMemDC(),
		_rcProgress1.left + _progressBarTop->getWidth() / 2,
		_y1 + _progressBarTop->getHeight() / 2, 0, 0,
		_width, _progressBarTop->getHeight());
}

//게이지 조절하는 함수
void progressBar::setGauge(float currentGauge, float maxGauge)
{
	//이해 안가신다면 외우셔도 좋습니다 *-_-*
	_width = (currentGauge / maxGauge) * _progressBarTop->getWidth();

}
