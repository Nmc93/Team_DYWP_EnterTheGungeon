#pragma once
#include "gameNode.h"
#include <vector>

#define MBTSIZE 3

class mainMenu : public gameNode
{
private:
	//======================= ȭ�� ==========================

	BUTTON _bt[MBTSIZE];	//��ư
	bool _bActive[MBTSIZE];	//��ư ��������
	int _menuIndex;			//�޴� ������ ī��Ʈ
	int _menuTimer;			//�޴� Ÿ�̸�
	image* _menuImg;		//�޴� �̹���
	int	_sleepCount;		//�������̽� ����� ī��Ʈ
	
	//====��---��=====
	int _bCount;	//3��° ��ư ���� ���� ī����

private:

	void createButton();			//��ư ����
	void clickButton(bool push);	//��ư ��ȣ�ۿ�

public:

	virtual HRESULT init();			//�ʱ�ȭ �Լ�
	virtual void release();			//�޸� ���� �Խ�
	virtual void update();			//�����ϴ� �Լ�
	virtual void render();			//�׸��� �Լ�
};

