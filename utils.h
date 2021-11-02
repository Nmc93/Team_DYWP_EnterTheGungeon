#pragma once

#define PI 3.14156f
#define PI2 PI * 2

//저장소에 따른 타일 크기
#define TMAPSIZEX0	20
#define TMAPSIZEY0	20
#define TMAPSIZEX1	15
#define TMAPSIZEY1	15
#define TMAPSIZEX2	20
#define TMAPSIZEY2	15
#define TMAPSIZEX3	15
#define TMAPSIZEY3	20
#define TMAPSIZEX4	30
#define TMAPSIZEY4	30
#define TMAPSIZEX5	30
#define TMAPSIZEY5	30
#define TMAPSIZEX6	40
#define TMAPSIZEY6	30
#define TMAPSIZEX7	25
#define TMAPSIZEY7	25
#define TMAPSIZEX8	50
#define TMAPSIZEY8	50

#define MMSIZE 50

//이미지 회전할때 편하게 하려고
#define PI8		float(PI / 8.0f)
#define PI12	float(PI / 12.0f)
#define PI16	float(PI / 16.0f)
#define PI32	float(PI / 32.0f)
#define PI64	float(PI / 64.0f)
#define PI128	float(PI / 128.0f)

#define TILESIZE 48

enum TileState
{
	MAP_MOVE,
	MAP_UNMOVE
};

enum SetObject
{
	SET_MARINE,
	SET_PRISONER,
	SET_PLAYER,
	SET_ENEMY,
	SET_DOOR,
	SET_TABLE,
	SET_BOX,
	SET_POTAL,
	SET_LEFT_WAY,
	SET_RIGHT_WAY,
	SET_TOP_WAY,
	SET_BOTTOM_WAY,
	SET_NULL,
	SET_EVENT,
	SET_BOSS
};

struct POINTF
{
	float x;
	float y;
};

struct BUTTON
{
	image* img;
	POINT	pt;
	RECT	rc;
	string	name;
	bool	active;
};

struct TILE
{
	POINTF		mpt;		//맵 좌표
	int			num;		//맵 번호
	RECT		rc;			//타일 렉트

	POINT		tFrame;		//타일 이미지 프레임
	TileState	tState;		//타일 속성
	POINT		oFrame;		//오브젝트 이미지 프레임
	SetObject	oState;		//설치 오브젝트 종류

	int			sector;		//맵 번호
};

//TWENTY NINE
namespace TN_UTILS
{
	float getDistance(float x1, float y1, float x2, float y2);

	float getAngle(float x1, float y1, float x2, float y2);
}