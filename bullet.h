#pragma once
#include "gameNode.h"
#include "camera.h"
#include <vector>

#define SHOTGUNMAX 4

enum class BTYPE
{
	ENEMY,
	PLAYER,
	BOOK
};

struct tagBullet
{
	image* bulletImage;		//�Ѿ��� �̹���
	RECT rc;				//�Ѿ��� ��Ʈ
	float x, y;				//�Ѿ��� ����
	float angle;			//�Ѿ��� ����
	float radius;			//�Ѿ��� ������
	float speed;			//�Ѿ��� ���ǵ�
	float fireX, fireY;		//�Ѿ��� �߻���ġ
	bool isFire;			//�Ѿ��� �߻�����
	bool isFired;			//�̹� �� �Ѿ��̳�
	BTYPE btype;			//�Ѿ� Ÿ��
};

//�������� ����� �Ѿ� Ŭ�� -> �Ƹ� normal
class bullet : public gameNode
{
private:
	//camera* _cam;

	vector<tagBullet>			_vBullet;
	vector<tagBullet>::iterator _viBullet;

	vector<TILE> _vMap;

	const char* _imageName;
	float _range;
	int _bulletMax;

	bool _end;					// å�� �ҷ��� ���� �򲨳� (�� ������ �Ѿ��� ���� �Ŀ�)
	int _timer;					// å�� �ҷ� �������� (�����Ѿ˵鸸 ����ϱ� ���ؼ�)


	TILE _tile;
public:

	bullet() {};
	~bullet() {};

	//�Ѿ� �̹���, �ִ� �Ѿ� ����, �Ÿ�
	HRESULT init(const char* imageName, int bulletMax, float range = WINSIZEX);
	void release();
	void update();
	void render();

	//�Ѿ� �߽� ������x,y,�Ѿ� ����, �Ѿ� �ӵ�, �Ѿ� Ÿ��(����Ʈ == ��)
	void fire(float x, float y, float angle, float speed, BTYPE btype = BTYPE::ENEMY);
	void move();

	//�浹�� ����
	void removeBullet(int arrNum);

	// getter, setter
	//void setCamera(camera* cam) { _cam = cam; }
	//camera* getCamera() { return _cam; }

	//�Ѿ� ��er
	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet() { return _viBullet; }

	//å�� �Ѿ��� ���ư��� �Ǵ��� ����
	bool getEnd() { return _end; }
};