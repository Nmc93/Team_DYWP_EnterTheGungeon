#pragma once
#include "gameNode.h"
#include <vector>

#define MOVESPPED 5.0f
#define ROLLSPEED 4.0f

//�÷��̾� ����
enum class PlayerDir
{
	CENTER,
	L,
	LT,
	T,
	RT,
	R,
	RB,
	B,
	LB
};

//�÷��̾� ����
enum class PlayerType
{
	MARINE,
	PRISONER
};

//�÷��̾� �ൿ
enum class PlayerState
{
	IDLE,
	WALK,
	ROLL,
	HIT,
	DEAD,
	TABLE,
	FALL,
	ROLLFALL,
	TABLESLIDE,
	CHARSELECT,
	INTOSTAGE
};

//���� Ÿ��
enum class PlayerGunType
{
	NORMAL,
	ONEHAND,
	TWOHAND
};

class player : public gameNode
{
private:

	//itemManager* _itemManager;

	//����
	int _heart;
	//�ִ� ����
	int _maxHeart;
	//��
	int _barrior;
	//����ź
	int _blankBullet;
	//����
	int _key;
	//��
	int _money;

	//����ź ����
	int _windX, _windY;
	int _windWidth, _windHeight;
	image* _windImg;
	RECT _windRc;

	bool _isWind;

protected:

	//==========================================================================

	//bullet*		_bullet;		// ���� �޾ƿ� ��

	//====== ����, �׸��� �̹��� ����==============================================

	image*		_bImg;		//���� �̹��� 
	image*		_sImg;		//�׸��� �̹���
	PlayerType	_pType;		//�÷��̾� Ÿ��

	POINT		_frame;		//�̹��� ������
	int			_frameCount;//������ ī��Ʈ

	// =========== ���� ===========================================================

	POINTF		_bXY;		//������ ������ǥ
	int			_fly;		//����� �׸����� �Ÿ�
	RECT		_bRc;		//������ RECT

	//========== �׸��� ===========================================================

	POINTF		_sXY;		//�׸����� ������ǥ
	RECT		_sRc;		//�׸����� RECT

	//======= �÷��̾� ���� ========================================================

	PlayerDir	_pDir;			//�÷��̾��� ����
	PlayerState _pState;		//�÷��̾��� ����
	PlayerGunType _armType;		//�÷��̾��� �� ����
	bool		_isControl;		//�÷��̾� ���� ����
	float		_angle;			//�÷��̾�� ���콺�� ����

	bool		_isRoll;		// ������ ������ ����
	int			_rollTime;		// ������ �ð�
	bool		_isHit;			// �¾Ҵ��� ����
	bool		_isTableUp;		// ���̺� �ֺ����� ����

	//======= ī�޶� ������ =======================================================

	POINTF		_camXY;			//ī�޶� �̵���
	POINTF		_cMove;			//ī�޶� �̵� ����ġ
	//POINTF		_

	//======= Ÿ�ϰ��⶧ �ʿ��Ѱ� ==================================================

	vector<TILE> _vMap;	//Ÿ�� ����� ����
	POINT		 _tileMax;	//Ÿ���� �ִ�XYũ��

	RECT		_rcCollision[4];
	int			_playerTileX, _playerTileY;	//�÷��̾� x y
	int			_tileIndex[4];				//�÷��̾�� ��� �ε��� ��ȣ

	bool		_isStart;					//���� ����

	//====== Ÿ�� ���� ========================
	/*
	vector<TILE>		_map; //TILE������ ���͸� �������մ� _map
	int _mapSizeX;			  //�ʰ���ũ��
	int _mapSizeY;
	*/

	// ���ľִϷ���
	int _alpha;
	int _alphaCount;


	// ���� ���µ�
	//gun* _gun;
	//normalGun* _normalGun;
	//shotGun* _shotGun;

public:

	// �⺻ �Լ�  ===========================================================

	//�÷��̾� ��ȯ(����xy,��������,�÷��̾� ����)
	virtual HRESULT init(POINTF xy, bool active, PlayerType playertype, PlayerGunType armtype);
	virtual HRESULT init(POINTF xy, bool active, PlayerGunType armtype);
	virtual void release();
	virtual void update();
	virtual void render();

	//�÷��̾� �̵�
	void playerMove(PlayerDir pd);

	void frameUpdate();
	void imageChange();
	//void keyMove();
	void mouseMove();
	void draw();
	void gunDraw();

	void hit();
	void wind();

	void dataSave();

	//Ÿ�� �˻���
	void playerTile(PlayerDir pd);

	//�̺�Ʈ ���۾˸�
	bool getEvent() { return _isStart;  }

	// Getter,Setter ===========================================================
	
	//�׸��� ���� ��ǥ
	void setShadowXY(POINTF position) { _sXY = position; }
	//�׸��� ���� ��ǥ
	POINTF getShadowXY() { return _sXY; }

	//�׸��� LEFT TOP ��ǥ
	RECT getShadowRc() { return _sRc; }
	
	//���� LEFT TOP ��ǥ
	RECT getBodyRc() { return _bRc; }	

	//ü�� ��������
	void setFly(int fly) { _fly = fly; }
	int getFly() { return _fly; }
	
	//�׸��� ���� ��ǥ
	void setBodyXY(POINTF position) { _sXY = position; }
	//�׸��� ���� ��ǥ
	POINTF getBodyXY() { return _sXY; }

	//�̹��� ������
	void setFrame(POINT frame) { _frame = frame; }
	//�̹��� ������
	POINT getFrame() { return  _frame; }
	
	//���� �̹���
	image* getBodyImg() { return _bImg;}
	//�׸��� �̹���
	image* getShadowImg() { return _sImg;}

	//ĳ���� ���� ����
	bool getActive() { return _isControl; }
	//ĳ���� ���� �Է�
	void setActive(bool active) { _isControl = active; }
	
	//ī�޶� �̵� ����ġ
	POINTF getCMove() { return _cMove; }
	//�ޱ۰�
	float getAngle() { return _angle; }

	//Ÿ�ϼ� �޾ƿ���
	void setTile0(vector<TILE> tile, int tilex, int tiley);

	// ������
	bool getIsRoll() { return _isRoll; }
	// ������
	void addIsRoll();

	//���� ��ȯ
	PlayerState getState() { return _pState; }

	PlayerType getPlayerType() { return _pType; }
	
	//�� ���� �ҷ�����
	PlayerGunType getgunType() { return _armType; }
	//�� ���� ��ȯ
	void setgunType(PlayerGunType armtype) { _armType = armtype; }
	//=====================================================================


	// �÷��̾� ���� status
	int getHeart() { return _heart; }
	void setHeart(int heart) { _heart = heart; }

	int getBarrior() { return _barrior; }
	void setBarrior(int barrior) { _barrior = barrior; }

	int getBlankBullet() { return _blankBullet; }
	void setBlankBullet(int blankBullet) { _blankBullet = blankBullet; }

	int getKey() { return _key; }
	void setKey(int key) { _key = key; }

	int getMoney() { return _money; }
	void setMoney(int money) { _money = money; }

	PlayerDir getDirection() { return _pDir; }

	void deleteVMap() { _vMap.clear(); vector<TILE>().swap(_vMap); }

	// ���� ��
	//void setIM(itemManager* im) { _itemManager = im; }
};

