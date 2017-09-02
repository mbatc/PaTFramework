#include "CSystemSpriteRenderPipeline.h"

#include "helpers/StringHelper.h"
#include "helpers/MemoryHelper.h"
#include "CSystemSpriteRenderPipeline.h"

#include <iostream>

void CSystemSpriteRenderLayer::AddToQueue(CSystemSpriteRenderInfo * add)
{
	if (!add)
		return;

	if (m_nSprites >= NLAYER_MAX_SPRITES)
	{
		m_bLayer_full = true;
		if (!m_pContinued_layer)
			m_pContinued_layer = new CSystemSpriteRenderLayer(m_layer_index);

		m_pContinued_layer->AddToQueue(add);
		m_nSprites++;

		return;
	}

	m_ppSprite_list[m_nSprites] = add;
	m_nSprites++;
}

CSprite * CSystemSpriteRenderLayer::GetSprite(int index)
{
	if (index < 0)
		return nullptr;
	if (index >= m_nSprites)
		return nullptr;

	if (index >= NLAYER_MAX_SPRITES)
	{
		if (!m_pContinued_layer)
			return nullptr;

		return m_pContinued_layer->GetSprite(index - NLAYER_MAX_SPRITES);
	}

	return &m_ppSprite_list[index]->sprite;
}

void CSystemSpriteRenderLayer::Clear()
{
	//Set the sprites in the continued layer to NULL (they are not deleted so that 
	//they do not have to be reallocated for the next frame)
	if(m_pContinued_layer)
		m_pContinued_layer->Clear();

	//If the continued layer was not used in the previous frame delete it to
	//reduce memory usage
	if (!m_bLayer_full && m_pContinued_layer)
		MEMORY::SafeDelete(m_pContinued_layer);
	
	//Reset the sprite pointers
	if (m_nSprites > NLAYER_MAX_SPRITES)
		m_nSprites = NLAYER_MAX_SPRITES;

	for (int i = 0; i < m_nSprites; i++)
	{
		MEMORY::SafeDelete(m_ppSprite_list[i]);
	}

	m_bLayer_full = false;
	m_nSprites = 0;
}

void CSystemSpriteRenderLayer::Cleanup()
{
	Clear();
	if (m_pContinued_layer)
	{
		m_pContinued_layer->Cleanup();
		MEMORY::SafeDelete(m_pContinued_layer);
	}
}


CSystemSpriteRenderPipeline::CSystemSpriteRenderPipeline()
	:
	m_pLayers(NULL),
	m_nLayers(0)
{}

CSystemSpriteRenderPipeline::~CSystemSpriteRenderPipeline()
{
	for (int i = 0; i < m_nLayers; i++)
	{
		m_pLayers[i]->Cleanup();
	}
}

void CSystemSpriteRenderPipeline::AddToQueue(CSprite sprite, int layer)
{
	/*
	if (!sprite.WithinLock())
		return;

	if (layer < 0)
		return;
	if (layer >= NSPRITE_RENDER_LAYERS)
		return;

	if (!m_pFirst[layer])
		m_pFirst[layer] = new CSystemSpriteRenderInfo(sprite);
	else
		m_pFirst[layer]->AddToQueue(new CSystemSpriteRenderInfo(sprite));
		*/
	
	if(!sprite.WithinLock())
		return;

	CSystemSpriteRenderLayer* pLayer = GetLayer(layer);
	if(!pLayer)
	{
		std::cout << "CSystemSpriteRenderPipeline: ERROR - Unable to add render layer (" << layer << ")\n";
		return;
	}

	pLayer->AddToQueue(new CSystemSpriteRenderInfo(sprite));
}

CSprite * CSystemSpriteRenderPipeline::GetNext()
{
	do
	{
		CSprite* spr = m_pLayers[m_nCurrent_layer]->GetSprite(m_nSprite_index);

		if (spr)
		{
			m_nSprite_index++;
			return spr;
		}

		if (!spr)
		{
			m_nCurrent_layer++;
			m_nSprite_index = 0;

			if (m_nCurrent_layer >= m_nLayers)
			{
				ClearPipeline();
				return nullptr;
			}
		}
	} while (true);
}

CSystemSpriteRenderLayer * CSystemSpriteRenderPipeline::GetLayer(int layer)
{
	for (int i = 0; i < m_nLayers; i++)
	{
		if (m_pLayers[i]->GetLayerIndex() == layer)
			return m_pLayers[i];
	}

	return AddLayer(layer);
}

CSystemSpriteRenderLayer * CSystemSpriteRenderPipeline::AddLayer(int layer)
{
	CSystemSpriteRenderLayer* pLayer = new CSystemSpriteRenderLayer(layer);

	int insert = 0;
	for (int i = 0; i < m_nLayers; i++)
	{
		if (layer > m_pLayers[i]->GetLayerIndex())
		{
			insert = i + 1;
		}
	}
	MEMORY::LIST::InsertInList(m_pLayers, pLayer, insert, m_nLayers);
	return m_pLayers[insert];
}

void CSystemSpriteRenderPipeline::ClearPipeline()
{
	m_nCurrent_layer = 0;
	m_nSprite_index = 0;

	for (int i = 0; i < m_nLayers; i++)
	{
		m_pLayers[i]->Clear();
	}
}

CSystemSpriteRenderInfo::CSystemSpriteRenderInfo(CSprite sprite)
	:
	sprite(sprite)
{}

CSystemSpriteRenderInfo::~CSystemSpriteRenderInfo()
{}

