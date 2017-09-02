#pragma once

#include "CD3DGraphics.h"
#include "CSprite.h"

#define NSPRITE_RENDER_LAYERS 10
#define NLAYER_MAX_SPRITES 50
class CSystemSpriteRenderPipeline; class CSystemSpriteRenderLayer;

class CSystemSpriteRenderInfo
{
	friend CSystemSpriteRenderPipeline;
	friend CSystemSpriteRenderLayer;
public:
	CSystemSpriteRenderInfo(CSprite sprite);
	~CSystemSpriteRenderInfo();
private:
	CSprite sprite;
};

class CSystemSpriteRenderLayer
{
public:
	CSystemSpriteRenderLayer(int index)
		:
		m_layer_index(index),
		m_bLayer_full(false),
		m_pContinued_layer(nullptr),
		m_nSprites(0)
	{}

	void		AddToQueue(CSystemSpriteRenderInfo* add);
	CSprite*	GetSprite(int index);
	int			GetLayerIndex() { return m_layer_index; }

	void Clear();
	void Cleanup();
private:
	CSystemSpriteRenderInfo* m_ppSprite_list[NLAYER_MAX_SPRITES];
	int m_nSprites;

	CSystemSpriteRenderLayer* m_pContinued_layer;

	int m_layer_index;

	bool m_bLayer_full;
};


class CSystemSpriteRenderPipeline
{
public:
	CSystemSpriteRenderPipeline();
	~CSystemSpriteRenderPipeline();

	void AddToQueue(CSprite sprite, int layer);
	CSprite* GetNext();
	
private:
	CSystemSpriteRenderLayer* GetLayer(int layer);
	CSystemSpriteRenderLayer* AddLayer(int layer);

	void ClearPipeline();

	int m_nCurrent_layer;
	int m_nSprite_index;

	CSystemSpriteRenderLayer**	m_pLayers;
	int							m_nLayers;
};