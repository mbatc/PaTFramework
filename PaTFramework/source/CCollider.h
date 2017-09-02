#pragma once

#include "CVector2.h"

#include "MathTransform.h"
#include "CSprite.h"
#include <Windows.h>

enum COL_SIDE
{
	COL_LEFT,
	COL_TOP,
	COL_RIGHT,
	COL_BOTTOM,
	COL_UNKNOWN
};

class CSprite;
class CGameobject;
class SCOLLISIONBOX;

struct SCOLINFO
{
	SCOLINFO()
		:
		side(CVector2(0.0f,0.0f),CVector2(1.0f,1.0f)),
		withSide(CVector2(0.0f, 0.0f), CVector2(1.0f, 1.0f))
	{}
	CVector2		pos;
	CVector2		velocity;
	CVector2		withVelocity;
	CVector2		normal;

	unsigned int	side_id;
	unsigned int	withSide_id;
	
	CLine			side;
	CLine			withSide;
	SCOLLISIONBOX*	with;
};

struct SCOLLISIONBOX
{
	SCOLLISIONBOX()
	{
		ZeroMemory(this, sizeof(SCOLLISIONBOX));
	}

	void Init(CSprite* spr, CGameobject* obj);
	void Update();
	void AddColInfo(SCOLINFO c);

	int GetBoundLeft() { return x - width / 2 - abs(velocity.x) * 2; }
	int GetBoundRight() { return x + width / 2 + abs(velocity.x) * 2; }
	int GetBoundTop() { return y - height / 2 - abs(velocity.y) * 2; }
	int GetBoundBottom() { return y + height / 2 + abs(velocity.y) * 2; }

	int GetCollisionCount() { return nCollisions; }

	bool CheckCollisionWith(CSprite* spr);
	bool CheckCollisionWith(CGameobject* obj);
	bool CheckCollisionWith(SCOLLISIONBOX * box);

	bool GetCollisionInfo(SCOLINFO*& info, int & nInfo, SCOLLISIONBOX * spr);
	bool GetCollisionInfo(SCOLINFO*& info, int& nInfo, CSprite* spr);
	bool GetCollisionInfo(SCOLINFO*& info, int& nInfo, CGameobject* o);
	bool GetCollisionInfo(SCOLINFO*& info, int& nInfo, char* group);

	RECT			rect;
	RECT			prev;
	V_RECT			transform_rect;

	CLine GetSide(unsigned int side);
	CVector2 GetSideNormal(unsigned int side);

	CVector2		velocity;

	float				x;
	float				y;

	float				x_prev;
	float				y_prev;

	int				xOff;
	int				yOff;

	int				width;
	int				height;

	float			rot;
	float			angular_velocity;
	
	/////////////////////////////
	//IDENTIFICATION
	CSprite*		pSprite;
	CGameobject*	pObj;
	/////////////////////////////
	SCOLINFO*		pCollisionInfo;
	int				nCollisions;

	//DEBUGGING PURPOSES
	void RenderCollider();
};

class CCollider
{
public:
	struct SCOLLISIONGROUP
	{
		SCOLLISIONBOX*	pCollider;
		int				nColliders;
		char*			group_name;
	};
	CCollider();
	~CCollider();

	//Used for editor
	CGameobject* GetMouseCollision();

	void Remove(CSprite* spr, const char* group = NULL);
	void Remove(CGameobject* obj, const char* group = NULL);
	void ClearColliders();

	int RegisterSprite(CSprite* sprite, const char* group);
	int RegisterSprite(CSprite* sprite, CGameobject* obj = NULL);
	int AddCollider(CGameobject* obj, int width, int height, int xOff, int yOff, const char* group = NULL);

	SCOLLISIONBOX**GetColliderList(CSprite* spr, int& nColliders, const char* group = NULL);
	SCOLLISIONBOX**GetColliderList(CGameobject* obj,int& nColliders, const char* group = NULL);

	SCOLLISIONBOX* GetCollider(CSprite* spr, const char* group = NULL);
	SCOLLISIONBOX* GetCollider(CGameobject* obj, const char* group = NULL);
	SCOLLISIONBOX* GetCollider(int index, const char* group = NULL);

	SCOLLISIONBOX* GetGroup(const char* name, int& nColliders);
	void Update(float deltaTime);

	//Will be used for debugging if needed
	void Render();

	void CheckCollision(SCOLLISIONBOX* a, SCOLLISIONBOX* b);
private:
	void UpdateColliderInfo();


	int					m_nGroup;
	SCOLLISIONGROUP*	m_pGroup;

	CSprite				m_spr_mouse;
};