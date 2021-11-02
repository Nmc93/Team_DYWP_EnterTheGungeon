#pragma once
#include "gameNode.h"
#include "mainMenu.h"
#include "mapTool.h"
#include "breakStage.h"
#include "nextStage.h"
#include "stage1.h"
#include "enemy.h"
#include "camera.h"

class playGround : public gameNode
{
private:
	enemy* _em;

public:
	playGround();
	~playGround();

	virtual HRESULT init();			//�ʱ�ȭ �Լ�
	virtual void release();			//�޸� ���� �Խ�
	virtual void update();			//�����ϴ� �Լ�
	virtual void render();			//�׸��� �Լ�

	void addImage();				//�ʱ� �̹��� ���� �Լ�
};

