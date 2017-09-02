#include "CCollider.h"

//#include "CGameobject.h"
#include "CSprite.h"

//#include "CSystem.h"
#include "helpers/Helper.h"

#include "MathTransform.h"

#include <Windows.h>

CCollider::CCollider()
	:
	m_pGroup(0),
	m_nGroup(0)
{
	ClearColliders();
}

CCollider::~CCollider()
{
	ClearColliders();
	MEMORY::SafeDeleteArr(m_pGroup);
}

CGameobject * CCollider::GetMouseCollision()
{
	for (int i = 0; i < m_nGroup; i++)
	{
		for (int ii = 0; ii < m_pGroup[i].nColliders; ii++)
		{
			SCOLINFO* info = 0; int nInfo = 0;
			if (GetCollider(&m_spr_mouse)->GetCollisionInfo(info, nInfo, &m_pGroup[i].pCollider[ii]))
			{
				if(info[0].with->pObj)
					return info[0].with->pObj;
			}
		}
	}

	return nullptr;
}

void CCollider::Remove(CSprite * spr, const char * group)
{
	int g_i = 0;
	if (group)
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			if (!strcmp(m_pGroup[i].group_name, group))
			{
				g_i = i;
				break;
			}
		}

		if (g_i == 0)
		{
			cout << "CCollider: Cannot remove collider for CSprite[" << spr << "] - Specified group does not exist...\n";
			return;
		}
	}

	for (int i = 0; i < m_pGroup[g_i].nColliders; i++)
	{
		if(m_pGroup[g_i].pCollider[i].pSprite == spr)
			MEMORY::LIST::RemoveItem(m_pGroup[g_i].pCollider, m_pGroup[g_i].nColliders, i);

	}
}

void CCollider::Remove(CGameobject * obj, const char * group)
{
	int g_i = 0;
	if (group)
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			if (!strcmp(m_pGroup[i].group_name, group))
			{
				g_i = i;
				break;
			}
		}

		if (g_i == 0)
		{
			cout << "CCollider: Cannot remove collider for CSprite[" << obj << "] - Specified group does not exist...\n";
			return;
		}
	}

	for (int i = 0; i < m_pGroup[g_i].nColliders; i++)
	{
		if (m_pGroup[g_i].pCollider[i].pObj == obj)
			MEMORY::LIST::RemoveItem(m_pGroup[g_i].pCollider, m_pGroup[g_i].nColliders, i);
	}
}

void CCollider::ClearColliders()
{
	for (int i = 0; i < m_nGroup; i++)
	{
		MEMORY::SafeDeleteArr(m_pGroup[i].group_name);
		MEMORY::SafeDeleteArr(m_pGroup[i].pCollider);
	}
	MEMORY::SafeDeleteArr(m_pGroup);
	m_nGroup = 0;

	SCOLLISIONGROUP g;
	g.nColliders = 0;
	g.pCollider = 0;
	g.group_name = 0;
	STRING::Copy(g.group_name,"default");

	MEMORY::LIST::AddToList(m_pGroup, &g, m_nGroup);

	m_spr_mouse.Resize(2, 2);
	this->RegisterSprite(&m_spr_mouse);
}

int CCollider::RegisterSprite(CSprite * sprite, const char * group)
{
	if (!sprite)
	{
		//Output Error
	}

	int g_i = 0;

	if (group)
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			if (!strcmp(m_pGroup[i].group_name, group))
			{
				g_i = i;
			}
		}

		if (g_i == 0)
		{
			SCOLLISIONGROUP g;
			g.nColliders = 0;
			g.pCollider = 0;
			g.group_name = 0;
			STRING::Copy(g.group_name, group);
			MEMORY::LIST::AddToList(m_pGroup, &g, m_nGroup);

			g_i = m_nGroup - 1;
		}
	}


	for (int i = 0; i < m_pGroup[g_i].nColliders; i++)
	{
		if (m_pGroup[g_i].pCollider[i].pSprite == sprite)
		{
			if (group)
				cout << "\nCCollider: CSprite[" << sprite << "] already has a collider registered to group '" << group << "'...\n\n";
			else
				cout << "\nCCollider: CSprite[" << sprite << "] already has a collider registered...\n\n";
			return i;
		}
	}
	SCOLLISIONBOX b;
	b.Init(sprite, NULL);
	MEMORY::LIST::AddToList(m_pGroup[g_i].pCollider, &b, m_pGroup[g_i].nColliders);

	return m_pGroup[g_i].nColliders - 1;
}

int CCollider::RegisterSprite(CSprite * sprite, CGameobject* obj)
{
	if (!sprite)
	{
		//OUTPUT ERROR MSG
		return -1;
	}

	for (int i = 0; i < m_pGroup[0].nColliders; i++)
	{
		if (m_pGroup[0].pCollider[0].pSprite == sprite)
		{
			cout << "CCollider: CSprite[" << sprite << "] is already registered...\n";
			return i;
		}
	}

	SCOLLISIONBOX b;
	b.Init(sprite, obj);

	MEMORY::LIST::AddToList(m_pGroup[0].pCollider, &b, m_pGroup[0].nColliders);
	return m_pGroup[0].nColliders - 1;
}

int CCollider::AddCollider(CGameobject * obj, int width, int height, int xOff, int yOff, const char* group)
{
	if (!obj)
	{
		//Output Error
	}

	int g_i = 0;

	if (group)
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			if (!strcmp(m_pGroup[i].group_name, group))
			{
				g_i = i;
			}
		}

		if (g_i == 0)
		{
			SCOLLISIONGROUP g;
			g.nColliders = 0;
			g.pCollider = 0;
			STRING::Copy(g.group_name, group);
			MEMORY::LIST::AddToList(m_pGroup, &g, m_nGroup);

			g_i = m_nGroup - 1;
		}
	}


	for (int i = 0; i < m_pGroup[g_i].nColliders; i++)
	{
		if (m_pGroup[g_i].pCollider[i].pObj == obj)
		{
			if (group)
				cout << "\nCCollider: CGameobject[" << obj << "] already has a collider registered to group '" << group << "'...\n\n";
			else
				cout << "\nCCollider: CGameobject[" << obj << "] already has a collider registered...\n\n";
			return i;
		}
	}
	SCOLLISIONBOX b;
	b.width = width;
	b.height = height;
	b.xOff = xOff;
	b.yOff = yOff;
	b.Init(NULL, obj);
	MEMORY::LIST::AddToList(m_pGroup[g_i].pCollider, &b, m_pGroup[g_i].nColliders);

	return m_pGroup[g_i].nColliders -1;
}

SCOLLISIONBOX ** CCollider::GetColliderList(CSprite * spr, int& nColliders, const char* group)
{
	SCOLLISIONBOX** col_list = 0;
	nColliders = 0;
	for (int i = 0; i < m_nGroup; i++)
	{
		if (group)
			if (!strcmp(m_pGroup[i].group_name, group))
				continue;

		for (int ii = 0; ii < m_pGroup[i].nColliders; i++)
		{
			if (m_pGroup[i].pCollider[ii].pSprite == spr)
				MEMORY::LIST::AddToListArr(col_list, &m_pGroup[i].pCollider[ii],nColliders);
		}
	}
	return col_list;
}

SCOLLISIONBOX ** CCollider::GetColliderList(CGameobject * obj, int& nColliders, const char* group)
{
	SCOLLISIONBOX** col_list = 0;
	nColliders = 0;
	for (int i = 0; i < m_nGroup; i++)
	{
		if (group)
			if (!strcmp(m_pGroup[i].group_name, group))
				continue;

		for (int ii = 0; ii < m_pGroup[i].nColliders; ii++)
		{
			if (m_pGroup[i].pCollider[ii].pObj == obj)
				MEMORY::LIST::AddToListArr(col_list, &m_pGroup[i].pCollider[ii], nColliders);
		}
	}
	return col_list;
}

SCOLLISIONBOX * CCollider::GetCollider(CSprite * spr, const char* group)
{
	int g_i = 0;
	if (group)
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			if (!strcmp(m_pGroup[i].group_name, group))
			{
				g_i = i;
				break;;
			}
		}

		if (g_i == 0)
			return NULL;

		for (int i = 0; i < m_pGroup[g_i].nColliders; i++)
		{
			if (m_pGroup[g_i].pCollider[i].pSprite == spr)
				return &m_pGroup[g_i].pCollider[i];
		}
	}
	else
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			for (int ii = 0; ii < m_pGroup[i].nColliders; ii++)
			{
				if (m_pGroup[i].pCollider[ii].pSprite == spr)
				{
					return &m_pGroup[i].pCollider[ii];
				}
			}
		}
	}

	return nullptr;
}

SCOLLISIONBOX * CCollider::GetCollider(CGameobject * obj, const char* group)
{
	int g_i = 0;
	if (group)
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			if (!strcmp(m_pGroup[i].group_name, group))
			{
				g_i = i;
				break;
			}
		}
		if (g_i == 0)
			return nullptr;

		for (int i = 0; i < m_pGroup[g_i].nColliders; i++)
		{
			if (m_pGroup[g_i].pCollider[i].pObj == obj)
			{
				return &m_pGroup[g_i].pCollider[i];
			}
		}
	}
	else
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			for (int ii = 0; ii < m_pGroup[i].nColliders; ii++)
			{
				if (m_pGroup[i].pCollider[ii].pObj == obj)
				{
					return &m_pGroup[i].pCollider[ii];
				}
			}
		}
	}

	return nullptr;
}

SCOLLISIONBOX * CCollider::GetCollider(int index,const char* group)
{
	int g_i = 0;
	if (group)
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			if (!strcmp(m_pGroup[i].group_name, group))
			{
				g_i = i;
				break;
			}
		}
		if (g_i == 0)
			return nullptr;
	}

	if(index < 0 ||index >= m_pGroup[g_i].nColliders)
		return nullptr;

	return &m_pGroup[g_i].pCollider[index];
}

void CCollider::Update(float deltaTime)
{
	UpdateColliderInfo();
}

void CCollider::Render()
{
}

SCOLLISIONBOX * CCollider::GetGroup(const char * name, int& nColliders)
{
	nColliders = 0;
	for (int i = 0; i < m_nGroup; i++)
	{
		if (!strcmp(name, m_pGroup[i].group_name))
		{
			nColliders = m_pGroup[i].nColliders;
			return m_pGroup[i].pCollider;
		}
	}
	return nullptr;
}

void CCollider::UpdateColliderInfo()
{
	//m_spr_mouse.SetPosition(MOUSE::GetMouseX() + WORLD.GetWorldOffsetX(), MOUSE::GetMouseY() + WORLD.GetWorldOffsetY());
	for (int i = 0; i < m_nGroup; i++)
	{
		for (int ii = 0; ii < m_pGroup[i].nColliders; ii++)
		{
			//Check if the object the collider points to still exists.
			if (m_pGroup[i].pCollider[ii].pObj)
			{
				//if (!SYSTEM::GetGMData()->GetScene()->DoesObjectExist(
				//	m_pGroup[i].pCollider[ii].pObj))
				//{
				//	MEMORY::LIST::RemoveItem(m_pGroup[i].pCollider, m_pGroup[i].nColliders, ii);
				//	ii--;
				//	continue;
				//}
			}

			m_pGroup[i].pCollider[ii].Update();
		}
	}

}

void CCollider::CheckCollision(SCOLLISIONBOX* a, SCOLLISIONBOX* b)
{
	CVector2 point;

	for (int i = 0; i < 4; i++)
	{
		for (int ii = 0; ii < 4; ii++)
		{
			if (!GetIntersect(point, a->GetSide(i), b->GetSide(ii)))	
				continue;

			if (a->GetSide(i).WithinGivenPoints(point) && b->GetSide(ii).WithinGivenPoints(point))
			{
				SCOLINFO c;
				c.with = b;
				c.pos = point;
				c.withVelocity = c.with->velocity;
				c.velocity = a->velocity;
				c.normal = b->GetSideNormal(ii);
				c.side = a->GetSide(i);
				c.withSide = b->GetSide(ii);
				c.side_id = i;
				c.withSide_id = ii;

				a->AddColInfo(c);
			}
		}
	}
	if (
		a->rect.bottom > b->rect.top &&
		a->rect.top < b->rect.bottom &&
		a->rect.left < b->rect.right &&
		a->rect.right > b->rect.left)
	{
		SCOLINFO c;
		c.with = b;
		c.pos = CVector2((a->rect.right + a->rect.left) / 2,
			(a->rect.top + a->rect.bottom) / 2);
		c.velocity = a->velocity;
		c.withVelocity = c.with->velocity;
		c.normal = CVector2(0.0f, 0.0f);
		c.side_id = -1;
		c.withSide_id = -1;
		a->AddColInfo(c);
	}
}

void SCOLLISIONBOX::Init(CSprite* spr, CGameobject* obj)
{
	pSprite = spr;
	pObj = obj;
	Update();

	prev = rect;
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	velocity.x = 0.0f; velocity.y = 0.0f;
}

void SCOLLISIONBOX::Update()
{
	MEMORY::SafeDeleteArr(pCollisionInfo);
	nCollisions = 0;

	prev = rect;
	x_prev = x;
	y_prev = y;

	if (!pSprite)
	{
		x = 0;//pObj->GetTransform().GetTranslation().x;
		y = 0;//pObj->GetTransform().GetTranslation().y;
		rect.left = x - xOff;
		rect.top = y - yOff;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;

		angular_velocity = 0;// pObj->GetTransform().GetRotation().z - rot;
		rot = 0;// pObj->GetTransform().GetRotation().z;
	}
	else
	{
		x = pSprite->GetX();
		y = pSprite->GetY();

		width = pSprite->GetWidth() * pSprite->GetScaleX();
		height = pSprite->GetWidth() * pSprite->GetScaleY();

 		float a_x = pSprite->GetAnchorX();
		float a_y = pSprite->GetAnchorY();

		rect.top = y - a_y - yOff;
		rect.left = x - a_x - xOff;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;

		angular_velocity = pSprite->GetRotation() - rot;
		rot = pSprite->GetRotation();
	}

	velocity.x = x - x_prev;
	velocity.y = y - y_prev;

	transform_rect = RotateRectangle(rect, CVector2(x, y), rot);



	//USED FOR DEUGGING --------------------------- COMMENT SO ITS EASY TO FIND LMAO
	//RenderCollider();
}

void SCOLLISIONBOX::AddColInfo(SCOLINFO c)
{
	MEMORY::LIST::AddToList(pCollisionInfo, &c, nCollisions);
}

bool SCOLLISIONBOX::CheckCollisionWith(CSprite * spr)
{
	//SCOLLISIONBOX* with = COLLIDER::GetColliderBox(spr);
	//if (!with)
	//	return false;
	//
	//CSystem::g_collider.CheckCollision(this, with);
	//for (int i = 0; i < nCollisions; i++)
	//{
	//	if (pCollisionInfo[i].with == with)
	//		return true;
	//}
	return false;
}

bool SCOLLISIONBOX::CheckCollisionWith(CGameobject * obj)
{
	//SCOLLISIONBOX* with = COLLIDER::GetColliderBox(obj);
	//if (!with)
	//	return false;
	//
	//CSystem::g_collider.CheckCollision(this, with);
	//for (int i = 0; i < nCollisions; i++)
	//{
	//	if (pCollisionInfo[i].with == COLLIDER::GetColliderBox(obj))
	//		return true;
	//}
	return false;
}

bool SCOLLISIONBOX::CheckCollisionWith(SCOLLISIONBOX * box)
{
	//if (!box)
	//	return false;
	//
	//CSystem::g_collider.CheckCollision(this, box);
	//for (int i = 0; i < nCollisions; i++)
	//{
	//	if (pCollisionInfo[i].with == box)
	//		return true;
	//}
	return false;
}

bool SCOLLISIONBOX::GetCollisionInfo(SCOLINFO* & info, int& nInfo, SCOLLISIONBOX * box)
{
	if (box == this)
		return false;

	if (!CheckCollisionWith(box))
		return false;
	MEMORY::SafeDeleteArr(info);
	for (int i = 0; i < nCollisions; i++)
	{
		if (pCollisionInfo[i].with == box)
		{
			MEMORY::LIST::AddToList(info, &pCollisionInfo[i], nInfo);
		}
	}
	if (info)
		return true;
	return false;
}

bool SCOLLISIONBOX::GetCollisionInfo(SCOLINFO* & info, int& nInfo, CSprite * spr)
{
	//if (!CheckCollisionWith(spr))
	//	return false;
	//MEMORY::SafeDeleteArr(info);
	//for (int i = 0; i < nCollisions; i++)
	//{
	//	if (pCollisionInfo[i].with == COLLIDER::GetColliderBox(spr))
	//	{
	//		MEMORY::LIST::AddToList(info, &pCollisionInfo[i], nInfo);
	//	}
	//}
	//if (info)
	//	return true;
	return false;
}

bool SCOLLISIONBOX::GetCollisionInfo(SCOLINFO* & info, int& nInfo, CGameobject * obj)
{
	//if (!CheckCollisionWith(obj))
	//	return false;
	//MEMORY::SafeDeleteArr(info);
	//for (int i = 0; i < nCollisions; i++)
	//{
	//	if (pCollisionInfo[i].with == COLLIDER::GetColliderBox(obj))
	//	{
	//		MEMORY::LIST::AddToList(info, &pCollisionInfo[i], nInfo);
	//	}
	//}
	//if (info)
	//	return true;
	return false;
}

bool SCOLLISIONBOX::GetCollisionInfo(SCOLINFO *& info, int & nInfo, char * group)
{
	//int nCol = 0;
	//SCOLLISIONBOX* pCol = CSystem::g_collider.GetGroup(group,nCol);
	//MEMORY::SafeDeleteArr(info);
	//
	//for (int i = 0; i < nCol; i++)
	//{
	//	if (!CheckCollisionWith(&pCol[i]))
	//		continue;
	//	for (int ii = 0; ii < nCollisions; ii++)
	//	{
	//		if (pCollisionInfo[ii].with == &pCol[i])
	//		{
	//			MEMORY::LIST::AddToList(info, &pCollisionInfo[ii], nInfo);
	//		}
	//	}
	//}
	//
	return true;
}

CLine SCOLLISIONBOX::GetSide(unsigned int side)
{
	CLine line(CVector2(0.0f, 0.0f), CVector2(1.0f, 1.0f));
	switch (side)
	{
	case COL_SIDE::COL_LEFT:
		line.SetA(transform_rect.TL);
		line.SetB(transform_rect.BL);
		break;
	case COL_SIDE::COL_RIGHT:
		line.SetA(transform_rect.TR);
		line.SetB(transform_rect.BR);
		break;
	case COL_SIDE::COL_TOP:
		line.SetA(transform_rect.TL);
		line.SetB(transform_rect.TR);
		break;
	case COL_SIDE::COL_BOTTOM:
		line.SetA(transform_rect.BL);
		line.SetB(transform_rect.BR);
		break;
	}
	return line;
}

CVector2 SCOLLISIONBOX::GetSideNormal(unsigned int side)
{
	CVector2 n;
	n.x = 0.0f;
	n.y = 0.0f;
	switch (side)
	{
	case COL_SIDE::COL_LEFT:
		n.x = -1.0f;
		break;
	case COL_SIDE::COL_RIGHT:
		n.x = 1.0f;
		break;
	case COL_SIDE::COL_TOP:
		n.y = -1.0f;
		break;
	case COL_SIDE::COL_BOTTOM:
		n.y = 1.0f;
		break;
	}

	n = RotatePoint(n, CVector2(0.0f, 0.0f), rot);

	return n;
}

void SCOLLISIONBOX::RenderCollider()
{
	//CSprite s = CSystem::m_gfx.DEFAULTSPRITE;
	//s.Resize(width, height);
	//s.SetPosition(x,y);
	//s.SetRotation(rot);
	//s.SetAnchor(xOff, yOff);
	//
	//WORLD.RenderSprite(s, TILE_DEFAULT);
}
