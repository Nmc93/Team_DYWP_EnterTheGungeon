#include "pch.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}

//�ʱ�ȭ�� ���⿡�� �ض�!!!
HRESULT playGround::init()
{
	gameNode::init(true);

	addImage();

	//�̹��� ����
	//IMAGEMANAGER->addImage("AIM", "image/ui/aim.bmp", 33, 33, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addImage("MAPBAR", "image/mapbar.bmp", 512, 768, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("MAPSPRITE", "image/mapsp.bmp", 480, 480, 10, 10, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("OBJSPRITE", "image/mapobj.bmp", 480, 480, 10, 10, true, RGB(255, 0, 255));

	//�� ����
	SCENEMANAGER->addScene("MAINMENU", new mainMenu);
	SCENEMANAGER->addScene("MAPTOOL", new mapTool);
	SCENEMANAGER->addScene("BREAKSTAGE", new breakStage);
	SCENEMANAGER->addScene("STAGE1", new stage1);
	SCENEMANAGER->addScene("NEXTSTAGE", new nextStage);

	//�׽�Ʈ
	
	// ���� �� ���� ();
	SCENEMANAGER->changeScene("MAINMENU");

	//���ʹ̿�
	/*
	_bullet = new bullet;
	_bullet->init("enemyBullet", 1000);

	_camera = new camera;
	_camera->init();

	setEnemy(ENEMYTYPE::NORMAL, 300, 300);
	setEnemy(ENEMYTYPE::SHOTGUN, 400, 400);
	setEnemy(ENEMYTYPE::BOOK, 500, 500);
	*/

	return S_OK;
}

//�޸� ������ ����� �ض�!!!!
void playGround::release()
{
	gameNode::release();
}

//����ó���� ����ٰ�!
void playGround::update()
{
	gameNode::update();

	//���ʹ̿�
	/*
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->update();
	}
	_camera->update();
	*/

	SCENEMANAGER->update();
	
}

//����� �׷����!!!
void playGround::render()
{
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);//WHITENESS 
	//==============������ ���� �ǵ帮�� ���� ============
	
	SCENEMANAGER->render();

	IMAGEMANAGER->findImage("AIM")->render(getMemDC(), _ptMouse.x - IMAGEMANAGER->findImage("AIM")->getWidth() / 2,
		_ptMouse.y - IMAGEMANAGER->findImage("AIM")->getHeight() / 2);

	//_em->render();
	/*
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->render();
	}
	_camera->render();
	*/

	//TIMEMANAGER->render(getMemDC());
	//=============== �ؿ��� �ǵ������� ================
	_backBuffer->render(getHDC(), 0, 0);

}

void playGround::addImage()
{
	//���콺 Ŀ��
	IMAGEMANAGER->addImage("AIM", "image/ui/aim.bmp", 33, 33, true, RGB(255, 0, 255));
	//���� ����â
	IMAGEMANAGER->addImage("MAPBAR", "image/mapbar.bmp", 512, 768, true, RGB(255, 0, 255));

	//��������Ʈ
	IMAGEMANAGER->addFrameImage("MAPSPRITE", "image/mapsp.bmp", 480, 480, 10, 10, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("OBJSPRITE", "image/mapobj.bmp", 480, 480, 10, 10, true, RGB(255, 0, 255));

	//������~~~��
	IMAGEMANAGER->addImage("SPACE", "image/menu/space.bmp", 1800, 1800, true, RGB(255, 0, 255));
	//��
	IMAGEMANAGER->addFrameImage("����", "image/gun/normalGun.bmp", 1200, 200, 24, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("��ź��", "image/gun/shotGun.bmp", 1728, 360, 24, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("GUN", "image/gun/hand_normalGun.bmp", 1200, 200, 24, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("PUMP", "image/gun/hand_shotGun.bmp", 1728, 360, 24, 5, true, RGB(255, 0, 255));

	//����ź,�׸���,�Ѿ�
	IMAGEMANAGER->addImage("WIND", "image/ui/wind.bmp", 50, 50, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("SHADOW", "image/player_shadow.bmp", 48, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("NULLSHADOW", "image/player_shadow_null.bmp", 48, 15, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("BULLET", "image/enemyBullet.bmp", 22, 23, true, RGB(255, 0, 255));

	//��丶��
	IMAGEMANAGER->addFrameImage("HELMET_IDLE", "image/h_normal_idle.bmp", 204, 360, 4, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_1H_IDLE", "image/h_onehand_idle.bmp", 204, 360, 4, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_2H_IDLE", "image/h_twohand_idle.bmp", 180, 360, 4, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_WALK", "image/h_normal_walk.bmp", 324, 396, 6, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_1H_WALK", "image/h_onehand_walk.bmp", 306, 396, 6, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_2H_WALK", "image/h_twohand_walk.bmp", 270, 396, 6, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_ROLL", "image/h_dodgeRoll.bmp", 567, 468, 9, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_DEAD", "image/h_dead.bmp", 945, 114, 15, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_FALL", "image/fall_h.bmp", 255, 108, 5, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_TABLEKICK", "image/tableKick_h.bmp", 171, 240, 3, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_ONE_TABLEKICK", "image/tableKick_oneHand_h.bmp", 162, 240, 3, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_TWO_TABLEKICK", "image/tableKick_twoHand_h.bmp", 144, 240, 3, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HELMET_TABLESLIDE", "image/h_tableSlide.bmp", 54, 228, 1, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HERMET_SELECT", "image/select_marine.bmp", 459, 75, 9, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("HERMET_INTOSTAGE", "image/decending_staircase.bmp", 405, 60, 9, 1, true, RGB(255, 0, 255));

	//����
	IMAGEMANAGER->addFrameImage("NO_IDLE", "image/normal_idle.bmp", 204, 342, 4, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_1H_IDLE", "image/onehand_idle.bmp", 204, 342, 4, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_2H_IDLE", "image/twohand_idle.bmp", 180, 342, 4, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_WALK", "image/normal_walk.bmp", 324, 378, 6, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_1H_WALK", "image/onehand_walk.bmp", 306, 378, 6, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_2H_WALK", "image/twohand_walk.bmp", 270, 378, 6, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_ROLL", "image/dodgeRoll.bmp", 594, 450, 9, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_DEAD", "image/dead.bmp", 945, 108, 15, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("NO_FALL", "image/fall_marine.bmp", 255, 108, 5, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_TABLEKICK", "image/tableKick_marine.bmp", 171, 240, 3, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_ONE_TABLEKICK", "image/tableKick_oneHand_marine.bmp", 162, 240, 3, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_TWO_TABLEKICK", "image/tableKick_twoHand_marine.bmp", 144, 240, 3, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("NO_TABLESLIDE", "image/tableSlide_marine.bmp", 54, 228, 1, 4, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("HELMET_VFX", "image/helmet_VFX.bmp", 504, 84, 12, 2, true, RGB(255, 0, 255));

	//�˼�
	IMAGEMANAGER->addFrameImage("PRISONER_IDLE", "image/prisoner_idle.bmp", 192, 360, 4, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("PRISONER_1H_IDLE", "image/prisoner_idle_oneWeapon.bmp", 192, 360, 4, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("PRISONER_2H_IDLE", "image/prisoner_idle_twoWeapon.bmp", 192, 360, 4, 6, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("PRISONER_WALK", "image/prisoner_walk.bmp", 324, 414, 6, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("PRISONER_1H_WALK", "image/prisoner_walk_oneWeapon.bmp", 306, 414, 6, 6, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("PRISONER_2H_WALK", "image/prisoner_walk_twoWeapon.bmp", 288, 414, 6, 6, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("PRISONER_ROLL", "image/prisoner_dodgeRoll.bmp", 567, 432, 9, 6, true, RGB(255, 0, 255));

	//�ð� ���� ������ �״� �̹���
	IMAGEMANAGER->addFrameImage("PRISONER_DEAD", "image/prisoner_dead.bmp", 440, 120, 8, 2, true, RGB(255, 0, 255));

	// �ð� ������ ���ڿ� �״� �̹���
	//IMAGEMANAGER->addFrameImage("PRISONER_TIMEAFTER_DEAD", "image/prisoner_time_dead.bmp", 726, 176, 11, 2, true, Magenta);

	//fall
	IMAGEMANAGER->addFrameImage("PRISONER_FALL", "image/fall_prisoner.bmp", 288, 108, 6, 2, true, RGB(255, 0, 255));

	//tableSlide
	IMAGEMANAGER->addFrameImage("PRISONER_TABLESLIDE", "image/tableSlide_prisoner.bmp", 57, 228, 1, 4, true, RGB(255, 0, 255));

	//ĳ���������� �� �̹�����
	IMAGEMANAGER->addFrameImage("BREAK_PRISONER_IDLE", "image/breakstage_prisoner_idle.bmp", 249, 61, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("BREAK_PRISONER_HAIRFLICK", "image/breakstage_prisoner_hairFlick.bmp", 440, 61, 8, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("BREAK_PRISONER_CROSSARMS", "image/breakstage_prisoner_crossingArms.bmp", 368, 61, 8, 1, true, RGB(255, 0, 255));

	//�ð� �̹�����
	IMAGEMANAGER->addFrameImage("BREAK_PRISONER_LIGHTCIGA", "image/breakstage_prisoner_lightCiga.bmp", 432, 60, 9, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("BREAK_PRISONER_SMOKECIGA", "image/breakstage_prisoner_smokeCiga.bmp", 250, 75, 5, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("BREAK_PRISONER_IDLECIGA", "image/breakstage_prisoner_idleCiga.bmp", 196, 62, 4, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("BREAK_PRISONER_THROWCIGA", "image/breakstage_prisoner_throwCiga.bmp", 324, 60, 6, 1, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("PRISONER_TABLE", "image/tableKick_prisoner.bmp", 204, 252, 4, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("PRISONER_TABLE_ONEHAND", "image/tableKick_oneHand_prisoner.bmp", 204, 252, 4, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("PRISONER_TABLE_TWOHAND", "image/tableKick_twoHand_prisoner.bmp", 204, 252, 4, 4, true, RGB(255, 0, 255));

	//�˼� �������� ���� �̹���
	IMAGEMANAGER->addFrameImage("PRISONER_INTOSTAGE", "image/intoStage_prisoner.bmp", 470, 62, 10, 1, true, RGB(255, 0, 255));

	//�˼� ���õ�
	IMAGEMANAGER->addFrameImage("PRISONER_SELECT", "image/selectchar_prisoner.bmp", 504, 74, 9, 1, true, RGB(255, 0, 255));
}

