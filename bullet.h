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
	image* bulletImage;		//총알의 이미지
	RECT rc;				//총알의 렉트
	float x, y;				//총알의 중점
	float angle;			//총알의 각도
	float radius;			//총알의 반지름
	float speed;			//총알의 스피드
	float fireX, fireY;		//총알의 발사위치
	bool isFire;			//총알의 발사유무
	bool isFired;			//이미 쏜 총알이냐
	BTYPE btype;			//총알 타입
};

//공용으로 사용할 총알 클라쓰 -> 아마 normal
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

	bool _end;					// 책의 불렛을 언제 쏠꺼냐 (맨 마지막 총알이 끝난 후에)
	int _timer;					// 책의 불렛 갯수세기 (쐈던총알들만 계산하기 위해서)


	TILE _tile;
public:

	bullet() {};
	~bullet() {};

	//총알 이미지, 최대 총알 숫자, 거리
	HRESULT init(const char* imageName, int bulletMax, float range = WINSIZEX);
	void release();
	void update();
	void render();

	//총알 발싸 시작점x,y,총알 각도, 총알 속도, 총알 타입(디폴트 == 적)
	void fire(float x, float y, float angle, float speed, BTYPE btype = BTYPE::ENEMY);
	void move();

	//충돌시 삭제
	void removeBullet(int arrNum);

	// getter, setter
	//void setCamera(camera* cam) { _cam = cam; }
	//camera* getCamera() { return _cam; }

	//총알 겟er
	vector<tagBullet> getVBullet() { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet() { return _viBullet; }

	//책의 총알이 날아가도 되는지 여부
	bool getEnd() { return _end; }
};