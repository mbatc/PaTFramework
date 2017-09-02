#include "CComponent.h"
#include "components\CComponentFactory.h"

int CComponent::_base_id;

CComponent::CComponent(CGameObject* parent) : m_pParent(parent), 
	CScriptCOMObject()
{
	_base_id = -1;
}
