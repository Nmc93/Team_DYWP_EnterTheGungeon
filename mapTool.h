#pragma once
#include "gameNode.h"
#include <vector>

#define TBTSIZE	5

#define PSIZEX	10
#define PSIZEY	10

enum SpuitType
{
	S_MAP,
	S_OBJECT
};

class mapTool : public gameNode
{
private:

	//========================= ȭ�� =========================

	RECT					_activeRect;	//�۾� ����
	BUTTON					_bt[TBTSIZE];	//��ư

	//===================== Ÿ�ϸ� ĵ���� =====================

	TILE					_tile;			//Ÿ��
	vector<TILE>			_vTile;			//Ÿ�� ����
	vector<TILE>::iterator	_vITile;		//Ÿ�� ������ �ݺ���

	POINT					_tMapSize;		//Ÿ�ϸ� ĵ���� ������
	int						_tX, _tY;		//Ÿ�ϸ� �̵���
	bool					_activeMap;		//Ÿ�ϸ� ĵ���� ���� ����

	//====================== ����â ===========================

	bool		_sampleInfo;				//����â Ȱ��ȭ ����
	int			_plusX;						//����â �̵���

	//ĵ���� ���� ��¿�
	TILE		_cTile;						//���� ��¿� Ÿ��

	//======================= �ȷ�Ʈ =========================

	image*		_paletteImg;				//�ȷ�Ʈ �̹���
	TILE		_palette[PSIZEX * PSIZEY];	//�ȷ�Ʈ
	
	//======================= �����̵� =======================

	SpuitType	_spuitType;					//�����̵� Ÿ��
	TILE		_spuit;						//�����̵�

	//======================== ����� ========================

	int			_archiveNum;				//����� ��ȣ
	int			_palletNum;					//�ȷ�Ʈ ��ȣ

private:

	void createTileMap();					//Ÿ�ϸ� ����
	void deleteTileMap();					//Ÿ�ϸ� ����
	void moveTileMap();						//Ÿ�ϸ� ����
	void setTileData();						//Ÿ�ϸ� ����
	void viewTileData();					//Ư�� Ÿ�� ���� Ȯ��

	void paletteManage(int palletnum);		//�ȷ�Ʈ ���� �� ���� �Լ�
	
	void createButton();					//��ư ����
	void clickButton();						//��ư ��ȣ�ۿ�
	
	void moveSampleInfo();					//����â ��ȣ�ۿ�

	void changeSpuit();						//�����̵� �� ����

	void selectArChive();					//����� ����

public:

	virtual HRESULT init();					//�ʱ�ȭ �Լ�
	virtual void release();					//�޸� ���� �Խ�
	virtual void update();					//�����ϴ� �Լ�
	virtual void render();					//�׸��� �Լ�

};