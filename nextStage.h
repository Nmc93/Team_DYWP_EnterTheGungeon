#pragma once
#include "gameNode.h"

class nextStage : public gameNode
{
private:

	int _nextNum;

public:
	virtual HRESULT init();			//�ʱ�ȭ �Լ�
	virtual void release();			//�޸� ���� �Խ�
	virtual void update();			//�����ϴ� �Լ�
	virtual void render();			//�׸��� �Լ�

	void stageUp();
};

