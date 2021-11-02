#pragma once
#include "gameNode.h"

class progressBar : public gameNode
{
private:
	RECT _rcProgress, _rcProgress1;
	int _x, _y;
	int _x1, _y1;
	float _width;

	image* _progressBarTop;
	image* _progressBarBottom;

public:
	progressBar();
	~progressBar();

	HRESULT init(int x, int y, int x1, int y1, int width, int height, int width1, int height1);
	void release();
	void update();
	void render();

	void setGauge(float currentGauge, float maxGauge);

	void setX(int x1) { _x1 = x1; }
	void setY(int y1) { _y1 = y1; }
};

