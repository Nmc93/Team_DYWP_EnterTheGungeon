#pragma once
#include "gameNode.h"
#include <vector>

struct Phase
{
	int mapMin;
	int mapMax;
};


class mapSetting : public gameNode
{
private:

	vector<TILE> _vMap;		//��
	vector<TILE> _vtemp;	//��ü��
	TILE _tempTile;			// ��ȯ�� Ÿ��

	vector<int> _closeMap;	//���� ��
	vector<int> _closeSector;//���� ����

	Phase _phase[6];//������
	int _phaseNum;	//������ ��ȣ

	int startArray;	//���۹迭
	int endArray;	//����迭

	RECT _cRange;	//�˻� ����

	int _maxX;	//ĵ������ X ������
	int _maxY;	//ĵ������ Y ������
	int _sectorNum;	//ĵ���� ���� ��ȣ
	int _roomnum;	//������ ���� ��ȣ
	int _tempSizeX;	//������ ���� X ������
	int _tempSizeY;	//������ ���� Y ������

	POINT ranXY;	//���� ��ġ�� ���� ���ϴ� ���� xy

	int _direction;	//���� �� ��ġ ����
	int _sectorSize;//���� ������
	int aTest[4];	//�� �迭 ����� ���� ��� ����� ����

	vector<int> _vWay;//���� ������ ������� ���
	int currentWay;	//���� �� �迭 ��ġ
	int startWay;	//���� �� �迭 ��ġ
	int nextWay;	//���� �� �迭 ��ġ

	int test;
private:


public:

	virtual HRESULT init(int num);	//�ʱ�ȭ �Լ�
	virtual void release();			//�޸� ���� �Խ�

	void createTileMap(vector<TILE>& vtile, int xsize, int ysize);			//�� ���� �Լ�
	void mapSet(int phasenum);//�� ���� �Լ�

	void createTileMap2(vector<TILE>& vtile, int xsize, int ysize, int sectorsize, vector<int>& sectornum);
	void mapset2(int phasenum);		//�� ���� �Լ�
	bool sectorCheck2(int secnum);	//�ߺ� ���� üũ �Լ�
	
	void findWay(int sector, int nextsector);	//������ ã�� �Լ�
	void createWay2();	//���� ã�� �Լ�
	void bildWay2();	//���� �����ϴ� �Լ�

	
};