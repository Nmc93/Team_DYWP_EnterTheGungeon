#include "pch.h"
#include "dataStorage.h"

HRESULT dataStorage::init()
{
	_archiveNum = 0;
	//데이터 세팅
	loadTileMap();

	//플레이어 관련 초기화
	_stage		= 0;	//스테이지
	_PlayerType = 0;	//플레이어 타입
	_maxHeart	= 6;	//플레이어 최대 생명력
	_heart		= 6;	//플레이어 생명
	_barrior	= 0;	//방어막
	_blankBullet= 2;	//공포탄
	_key		= 0;	//열쇠
	_money		= 0;	//돈
	_sGun		= true;//샷건 해금 여부
	
	return S_OK;
}

//타일맵 저장
void dataStorage::saveTileMap(int archivenum, vector<TILE> vtile)
{
	switch (archivenum)
	{
		case 0: //틈
		{
			for (int i = 0; i < vtile.size(); ++i)
			{
				tile0[i] = vtile[i];
			}
			break;
		}
		case 1: //플레이어 스테이지 첫번째방 플레이어 소환되는방
		{
			for (int i = 0; i < vtile.size(); ++i)
			{
				tile1[i] = vtile[i];
			}
			break;
		}
		case 2: //2 ~ 6번부터 랜덤으로 생성되는 몬스터구간
		{
			for (int i = 0; i < vtile.size(); ++i)
			{
				tile2[i] = vtile[i];
			}
			break;
		}
		case 3:
		{
			for (int i = 0; i < vtile.size(); ++i)
			{
				tile3[i] = vtile[i];
			}
			break;
		}
		case 4:
		{
			for (int i = 0; i < vtile.size(); ++i)
			{
				tile4[i] = vtile[i];
			}
			break;
		}
		case 5:
		{
			for (int i = 0; i < vtile.size(); ++i)
			{
				tile5[i] = vtile[i];
			}
			break;
		}
		case 6:
		{
			for (int i = 0; i < vtile.size(); ++i)
			{
				tile6[i] = vtile[i];
			}
			break;
		}
		case 7: //상점
		{
			for (int i = 0; i < vtile.size(); ++i)
			{
				tile7[i] = vtile[i];
			}
			break;
		}
		case 8: //보스 50 50
		{
			for (int i = 0; i < vtile.size(); ++i)
			{
				tile8[i] = vtile[i];
			}
			break;
		}
	}
}
//타일맵 저장
void dataStorage::saveTileMap()
{
	//0
	HANDLE file0 = CreateFile("map/BreakRoom.map", GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD write0;
	WriteFile(file0, tile0, sizeof(TILE) * TMAPSIZEX0 * TMAPSIZEY0, &write0, NULL);
	CloseHandle(file0);
	//1
	HANDLE file1 = CreateFile("map/StartRoom.map", GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD write1;
	WriteFile(file1, tile1, sizeof(TILE) * TMAPSIZEX1 * TMAPSIZEY1, &write1, NULL);
	CloseHandle(file1);
	//2
	HANDLE file2 = CreateFile("map/EnemyRoom1.map", GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD write2;
	WriteFile(file2, tile2, sizeof(TILE) * TMAPSIZEX2 * TMAPSIZEY2, &write2, NULL);
	CloseHandle(file2);
	//3
	HANDLE file3 = CreateFile("map/EnemyRoom2.map", GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD write3;
	WriteFile(file3, tile3, sizeof(TILE) * TMAPSIZEX3 * TMAPSIZEY3, &write3, NULL);
	CloseHandle(file3);
	//4
	HANDLE file4 = CreateFile("map/EnemyRoom3.map", GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD write4;
	WriteFile(file4, tile4, sizeof(TILE) * TMAPSIZEX4 * TMAPSIZEY4, &write4, NULL);
	CloseHandle(file4);
	//5
	HANDLE file5 = CreateFile("map/EnemyRoom4.map", GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD write5;
	WriteFile(file5, tile5, sizeof(TILE) * TMAPSIZEX5 * TMAPSIZEY5, &write5, NULL);
	CloseHandle(file5);
	//6
	HANDLE file6 = CreateFile("map/EnemyRoom5.map", GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD write6;
	WriteFile(file6, tile6, sizeof(TILE) * TMAPSIZEX6 * TMAPSIZEY6, &write6, NULL);
	CloseHandle(file6);
	//7
	HANDLE file7 = CreateFile("map/Shop.map", GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD write7;
	WriteFile(file7, tile7, sizeof(TILE) * TMAPSIZEX7 * TMAPSIZEY7, &write7, NULL);
	CloseHandle(file7);
	//8
	HANDLE file8 = CreateFile("map/BossRoom.map", GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD write8;
	WriteFile(file8, tile8, sizeof(TILE) * TMAPSIZEX8 * TMAPSIZEY8, &write8, NULL);
	CloseHandle(file8);
	
	//핸들 종료
	
}

//타일맵 불러오기
void dataStorage::loadTileMap()
{
	//저장소 변경
	//0
	HANDLE file0 = CreateFile("map/BreakRoom.map", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD read0;
	ReadFile(file0, tile0, sizeof(TILE) * TMAPSIZEX0 * TMAPSIZEY0, &read0, NULL);
	CloseHandle(file0);
	//1
	HANDLE file1 = CreateFile("map/StartRoom.map", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD read1;
	ReadFile(file1, tile1, sizeof(TILE) * TMAPSIZEX1 * TMAPSIZEY1, &read1, NULL);
	CloseHandle(file1);
	HANDLE file2 = CreateFile("map/EnemyRoom1.map", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//2
	DWORD read2;
	ReadFile(file2, tile2, sizeof(TILE) * TMAPSIZEX2 * TMAPSIZEY2, &read2, NULL);
	CloseHandle(file2);
	HANDLE file3 = CreateFile("map/EnemyRoom2.map", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//3
	DWORD read3;
	ReadFile(file3, tile3, sizeof(TILE) * TMAPSIZEX3 * TMAPSIZEY3, &read3, NULL);
	CloseHandle(file3);
	HANDLE file4 = CreateFile("map/EnemyRoom3.map", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//4
	DWORD read4;
	ReadFile(file4, tile4, sizeof(TILE) * TMAPSIZEX4 * TMAPSIZEY4, &read4, NULL);
	CloseHandle(file4);
	HANDLE file5 = CreateFile("map/EnemyRoom4.map", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//5
	DWORD read5;
	ReadFile(file5, tile5, sizeof(TILE) * TMAPSIZEX5 * TMAPSIZEY5, &read5, NULL);
	CloseHandle(file5);
	HANDLE file6 = CreateFile("map/EnemyRoom5.map", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//6
	DWORD read6;
	ReadFile(file6, tile6, sizeof(TILE) * TMAPSIZEX6 * TMAPSIZEY6, &read6, NULL);
	CloseHandle(file6);
	HANDLE file7 = CreateFile("map/Shop.map", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//7
	DWORD read7;
	ReadFile(file7, tile7, sizeof(TILE) * TMAPSIZEX7 * TMAPSIZEY7, &read7, NULL);
	CloseHandle(file7);
	//8
	HANDLE file8 = CreateFile("map/BossRoom.map", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD read8;
	ReadFile(file8, tile8, sizeof(TILE) * TMAPSIZEX8 * TMAPSIZEY8, &read8, NULL);
	CloseHandle(file8);
}

//타일맵 받아오기
void dataStorage::addTileMap(int archivenum, vector<TILE>& vtile, bool tf)
{

	//저장소 변경
	switch (archivenum)
	{
		case 0:
		{
			for (int i = 0; i < TMAPSIZEX0 * TMAPSIZEY0; ++i)
			{
				vtile.push_back(tile0[i]);
			}
			break;
		}
		case 1:
		{
			for (int i = 0; i < TMAPSIZEX1 * TMAPSIZEY1; ++i)
			{
				vtile.push_back(tile1[i]);
			}
			break;
		}
		case 2:
		{
			for (int i = 0; i < TMAPSIZEX2 * TMAPSIZEY2; ++i)
			{
				vtile.push_back(tile2[i]);
			}
			break;
		}
		case 3:
		{
			for (int i = 0; i < TMAPSIZEX3 * TMAPSIZEY3; ++i)
			{
				vtile.push_back(tile3[i]);
			}
			break;
		}
		case 4:
		{
			for (int i = 0; i < TMAPSIZEX4 * TMAPSIZEY4; ++i)
			{
				vtile.push_back(tile4[i]);
			}
			break;
		}
		case 5:
		{
			for (int i = 0; i < TMAPSIZEX5 * TMAPSIZEY5; ++i)
			{
				vtile.push_back(tile5[i]);
			}
			break;
		}
		case 6:
		{
			for (int i = 0; i < TMAPSIZEX6 * TMAPSIZEY6; ++i)
			{
				vtile.push_back(tile6[i]);
			}
			break;
		}
		case 7:
		{
			for (int i = 0; i < TMAPSIZEX7 * TMAPSIZEY7; ++i)
			{
				vtile.push_back(tile7[i]);
			}
			break;
		}
		case 8:
		{
			for (int i = 0; i < TMAPSIZEX8 * TMAPSIZEY8; ++i)
			{
				vtile.push_back(tile8[i]);
			}
			break;
		}
	}
}

void dataStorage::addTileMap(int archivenum, vector<TILE>& vtile, int& sizex, int& sizey, bool tf)
{
	//저장소 변경
	switch (archivenum)
	{
	case 0:
	{
		for (int i = 0; i < TMAPSIZEX0 * TMAPSIZEY0; ++i)
		{
			vtile.push_back(tile0[i]);
			sizex = TMAPSIZEX0;
			sizey = TMAPSIZEY0;
		}
		break;
	}
	case 1:
	{
		for (int i = 0; i < TMAPSIZEX1 * TMAPSIZEY1; ++i)
		{
			vtile.push_back(tile1[i]);
			sizex = TMAPSIZEX1;
			sizey = TMAPSIZEY1;
		}
		break;
	}
	case 2:
	{
		for (int i = 0; i < TMAPSIZEX2 * TMAPSIZEY2; ++i)
		{
			vtile.push_back(tile2[i]);
			sizex = TMAPSIZEX2;
			sizey = TMAPSIZEY2;
		}
		break;
	}
	case 3:
	{
		for (int i = 0; i < TMAPSIZEX3 * TMAPSIZEY3; ++i)
		{
			vtile.push_back(tile3[i]);
			sizex = TMAPSIZEX3;
			sizey = TMAPSIZEY3;
		}
		break;
	}
	case 4:
	{
		for (int i = 0; i < TMAPSIZEX4 * TMAPSIZEY4; ++i)
		{
			vtile.push_back(tile4[i]);
			sizex = TMAPSIZEX4;
			sizey = TMAPSIZEY4;
		}
		break;
	}
	case 5:
	{
		for (int i = 0; i < TMAPSIZEX5 * TMAPSIZEY5; ++i)
		{
			vtile.push_back(tile5[i]);
			sizex = TMAPSIZEX5;
			sizey = TMAPSIZEY5;
		}
		break;
	}
	case 6:
	{
		for (int i = 0; i < TMAPSIZEX6 * TMAPSIZEY6; ++i)
		{
			vtile.push_back(tile6[i]);
			sizex = TMAPSIZEX6;
			sizey = TMAPSIZEY6;
		}
		break;
	}
	case 7:
	{
		for (int i = 0; i < TMAPSIZEX7 * TMAPSIZEY7; ++i)
		{
			vtile.push_back(tile7[i]);
			sizex = TMAPSIZEX7;
			sizey = TMAPSIZEY7;
		}
		break;
	}
	case 8:
	{
		for (int i = 0; i < TMAPSIZEX8 * TMAPSIZEY8; ++i)
		{
			vtile.push_back(tile8[i]);
			sizex = TMAPSIZEX8;
			sizey = TMAPSIZEY8;
		}
		break;
	}
	}
}

void dataStorage::setPlayerData(int maxHeart, int heart, int barrior, int blankBullet, int key, int money)
{
	//플레이어 관련
	_maxHeart		= maxHeart;		//플레이어 최대 생명력
	_heart			= heart;		//플레이어 생명
	_barrior		= barrior;		//방어막
	_blankBullet	= blankBullet;	//공포탄
	_key			= key;			//열쇠
	_money			= money;		//돈
}

