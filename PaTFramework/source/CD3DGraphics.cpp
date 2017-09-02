#include "CD3DGraphics.h"
#include "CWindow.h"
#include "helpers\MemoryHelper.h"
#include "error.h"

#include "CLog.h"

#include "CSprite.h"

#include <iostream>

CSprite CD3DGraphics::DEFAULTSPRITE;

CD3DGraphics::CD3DGraphics()
	:
	m_pDXContext(NULL),
	m_pDXDevice(NULL),
	m_cTarget(-1)
{
	Log(this, DEBUGLOG_LEVEL_INFO, "CD3DGraphics class initialised (ptr=%p)", this);
	m_refreshRate[0] = 0;
	m_refreshRate[1] = 1;
	SetClearColor();
}

CD3DGraphics::~CD3DGraphics()
{
	Log(this, DEBUGLOG_LEVEL_INFO, "CD3DGraphics class destroyed (ptr=%p)", this);
	Log(this, DEBUGLOG_LEVEL_INFO, "Destroying %d render target(s)", m_pRenderTarget.size());
	for (int i = 0; i < m_pRenderTarget.size(); i++)
	{
		SafeRelease(m_pRenderTarget[i]->m_pTarget);
		SafeRelease(m_pRenderTarget[i]->m_pDepthStencil);
		SafeRelease(m_pRenderTarget[i]->m_pDepthBuffer);

		MEMORY::SafeDelete(m_pRenderTarget[i]);
	}
	m_pRenderTarget.clear();

	Log(this, DEBUGLOG_LEVEL_INFO, "Destroying %d blend state(s)", m_ppBlendState.size());
	for (int i = 0; i < m_ppBlendState.size(); i++)
		SafeRelease(m_ppBlendState[i]);
	m_ppBlendState.clear();

	Log(this, DEBUGLOG_LEVEL_INFO, "Destroying %d depth state(s)", m_ppDepthState.size());
	for (int i = 0; i < m_ppDepthState.size(); i++)
		SafeRelease(m_ppDepthState[i]);
	m_ppDepthState.clear();

	Log(this, DEBUGLOG_LEVEL_INFO, "Destroying %d raster state(s)", m_ppRasterState.size());
	for (int i = 0; i < m_ppRasterState.size(); i++)
		SafeRelease(m_ppRasterState[i]);
	m_ppRasterState.clear();

	SafeRelease(m_pDXContext);
	SafeRelease(m_pDXDevice);
}

unsigned int CD3DGraphics::InitDX(CWindow* pInitialWindow)
{
	Log(this, DEBUGLOG_LEVEL_INFO, "Initialising CD3DGraphics(ptr=%p)", this);
	int result = 0;

	if (result = InitDisplay())
		return result;
	if (result = InitDevice())
		return result;
	if (result = InitDefaultBlendState())
		return result;
	if (result = InitDefaultDepthStecil())
		return result;
	if (result = InitDefaultRasterState())
		return result;

	if (!SetBlendState(true))
		return 7;
	if (!SetRasterState(0))
		return 8;
	if (!SetDepthState(true))
		return 9;

	InitView();

	if (AddRenderTarget(pInitialWindow))
		return ERROR_DX_NOT_INITIALIZED;

	LoadDefaultShaders();
	
	return result;
}

unsigned int CD3DGraphics::AddRenderTarget(CWindow * pTargetWnd)
{
	if (!m_pDXDevice)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to add Render Target[%d]: DirectX not Initialised", m_pRenderTarget.size());
		return ERROR_DX_NOT_INITIALIZED;
	}

	Log(this, DEBUGLOG_LEVEL_INFO, "Adding Render Target[%d]", m_pRenderTarget.size());
	int result = 0;
	m_pRenderTarget.resize(m_pRenderTarget.size() + 1);
	m_pRenderTarget[m_pRenderTarget.size() - 1] = new CD3DTarget(pTargetWnd);

	return InitRenderTarget(m_pRenderTarget.size() - 1);
}

bool CD3DGraphics::SetCurrentTarget(CWindow * pTarget)
{
	if (!pTarget)
	{
		m_cTarget = -1;
		return true;
	}
	for (int i = 0; i < m_pRenderTarget.size(); i++)
	{
		if (m_pRenderTarget[i]->Window() == pTarget)
		{
			m_cTarget = i;
			m_pDXContext->RSSetViewports(1, &m_pRenderTarget[i]->m_viewport);
			m_pDXContext->OMSetRenderTargets(1, &m_pRenderTarget[i]->m_pTarget, m_pRenderTarget[i]->m_pDepthStencil);
			return true;
		}
	}

	return false;
}

unsigned int CD3DGraphics::BeginFrame()
{
	if (!_ValidTargetActive())
		return ERROR_DX_NO_RENDER_TARGET_SET;

	if (!m_pDXDevice)
		return ERROR_DX_NOT_INITIALIZED;

	m_pDXContext->ClearRenderTargetView(m_pRenderTarget[m_cTarget]->m_pTarget, m_clearColor);
	m_pDXContext->ClearDepthStencilView(m_pRenderTarget[m_cTarget]->m_pDepthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);
	return 0;
}

unsigned int CD3DGraphics::EndFrame()
{
	if (!_ValidTargetActive())
		return ERROR_DX_NO_RENDER_TARGET_SET;

	if (!m_pDXDevice)
		return ERROR_DX_NOT_INITIALIZED;


	if (FAILED(m_pRenderTarget[m_cTarget]->m_pSwapChain->Present(0, 0)))
		return ERROR_DX_NOT_INITIALIZED;

	return 0;
}

void CD3DGraphics::SetClearColor(D3DXVECTOR4 color)
{
	m_clearColor.a = color.w;
	m_clearColor.r = color.x;
	m_clearColor.g = color.y;
	m_clearColor.b = color.z;
}

void CD3DGraphics::SetClearColor(float r, float g, float b, float a)
{
	SetClearColor(D3DXVECTOR4(r, g, b, a));
}

void CD3DGraphics::SetViewMatrix(D3DXMATRIX v)
{
	m_matView = v;
}

bool CD3DGraphics::SetRasterState(unsigned int i)
{
	if (i < 0 || i >= m_ppRasterState.size())
		return false;

	if (!m_pDXContext)
		return false;
	m_pDXContext->RSSetState(m_ppRasterState[i]);
	return true;
}

bool CD3DGraphics::SetBlendState(bool b)
{
	if (!m_pDXContext)
		return false;

	int i = (int)b;
	float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };
	m_pDXContext->OMSetBlendState(m_ppBlendState[i], blendFactor, 0xFFFFFF);

	return true;
}

bool CD3DGraphics::SetBlendState(unsigned int i)
{
	if (i < 0 || i >= m_ppBlendState.size())
		return false;

	if (!m_pDXContext)
		return false;

	float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };
	m_pDXContext->OMSetBlendState(m_ppBlendState[i], blendFactor, 0xFFFFFF);
	return true;
}

bool CD3DGraphics::SetDepthState(bool b)
{
	if (!m_pDXContext)
		return false;

	int i = (int)b;
	m_pDXContext->OMSetDepthStencilState(m_ppDepthState[i], 1);

	return true;
}

bool CD3DGraphics::SetDepthState(unsigned int i)
{
	if (i < 0 || i >= m_ppDepthState.size())
		return false;

	if (!m_pDXContext)
		return false;

	m_pDXContext->OMSetDepthStencilState(m_ppDepthState[i], 1);

	return true;
}

unsigned int CD3DGraphics::AddBlendState(D3D11_BLEND_DESC dsc)
{
	if (!m_pDXDevice)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to add Blend State[%d]: DirectX not Initialised", m_ppBlendState.size());
		return ERROR_DX_NOT_INITIALIZED;
	}
	m_ppBlendState.resize(m_ppBlendState.size() + 1, NULL);
	if (FAILED(m_pDXDevice->CreateBlendState(&dsc, &m_ppBlendState[m_ppBlendState.size() - 1])))
	{
		Log(this, DEBUGLOG_LEVEL_FATAL, "Unable to add Blend State[%d]: Invalid Args", m_ppBlendState.size());
		return ERROR_DX_INIT_RASTER_FAILED;
	}
	Log(this, DEBUGLOG_LEVEL_INFO, "Adding Blend State[%d]", m_ppBlendState.size() - 1);
	return 0;
}

unsigned int CD3DGraphics::AddRasterState(D3D11_RASTERIZER_DESC dsc)
{
	if (!m_pDXDevice)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to add Raster State[%d]: DirectX not Initialised", m_ppRasterState.size());
		return ERROR_DX_NOT_INITIALIZED;
	}
	m_ppRasterState.resize(m_ppRasterState.size() + 1, NULL);
	if (FAILED(m_pDXDevice->CreateRasterizerState(&dsc, &m_ppRasterState[m_ppRasterState.size() - 1])))
	{
		Log(this, DEBUGLOG_LEVEL_FATAL, "Unable to add Raster State[%d]: Invalid Args", m_ppRasterState.size());
		return ERROR_DX_INIT_RASTER_FAILED;
	}
	Log(this, DEBUGLOG_LEVEL_INFO, "Adding Raster State[%d]", m_ppRasterState.size() - 1);
	return 0;
}

unsigned int CD3DGraphics::AddDepthStencilState(D3D11_DEPTH_STENCIL_DESC dsc)
{
	if (!m_pDXDevice)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to add Depth State[%d]: DirectX not Initialised", m_ppDepthState.size());
		return ERROR_DX_NOT_INITIALIZED;
	}
	m_ppDepthState.resize(m_ppDepthState.size() + 1,NULL);
	if (FAILED(m_pDXDevice->CreateDepthStencilState(&dsc, &m_ppDepthState[m_ppDepthState.size() - 1])))
	{
		Log(this, DEBUGLOG_LEVEL_FATAL, "Unable to add Depth State[%d]: Invalid Args", m_ppDepthState.size());
		return ERROR_DX_INIT_RASTER_FAILED;
	}
	Log(this, DEBUGLOG_LEVEL_INFO, "Adding Depth State[%d]", m_ppDepthState.size() - 1);

	return 0;
}

unsigned int CD3DGraphics::RenderShader(unsigned int nIndices, D3DXMATRIX world, std::vector<ID3D11ShaderResourceView*> resource_list)
{
	if (!_ValidTargetActive())
		return ERROR_DX_NO_RENDER_TARGET_SET;

	if (!m_pActiveShader)
		return ERROR_DX_SHADER_INVALID;

	_CShader::MatrixBuffer mat;
	
	mat.o = m_pRenderTarget[m_cTarget]->m_matOrtho;
	mat.w = world;
	mat.v = m_matView;
	mat.p = m_pRenderTarget[m_cTarget]->m_matProj;

	return m_pActiveShader->Render(nIndices, mat, resource_list);
}

unsigned int CD3DGraphics::RenderShader(unsigned int nIndices, D3DXMATRIX world, int numResource, ...)
{
	std::vector<ID3D11ShaderResourceView*> resource_list;

	va_list args;
	va_start(args, numResource);
	for (int i = 0; i < numResource; i++)
	{
		resource_list.push_back(va_arg(args, ID3D11ShaderResourceView*));
	}
	va_end(args);

	
	return RenderShader(nIndices, world, resource_list);
}

int CD3DGraphics::CreateVertexBuffer(CUSTOMVERTEX * pVertex, unsigned int nVertices, ID3D11Buffer** vb)
{
	SafeRelease(*vb);
	D3D11_BUFFER_DESC dsc;
	D3D11_SUBRESOURCE_DATA data;

	dsc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	dsc.ByteWidth = sizeof(CUSTOMVERTEX) * nVertices;
	dsc.CPUAccessFlags = 0;
	dsc.Usage = D3D11_USAGE_DEFAULT;
	dsc.StructureByteStride = 0;
	dsc.MiscFlags = 0;

	data.pSysMem = pVertex;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(Device()->CreateBuffer(&dsc, &data, vb))) { return 0; }
	return 1;
}

int CD3DGraphics::CreateIndexBuffer(unsigned long * pIndices, unsigned int nIndices, ID3D11Buffer** ib)
{
	SafeRelease(*ib);
	D3D11_BUFFER_DESC dsc;
	D3D11_SUBRESOURCE_DATA data;

	dsc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	dsc.ByteWidth = sizeof(unsigned long) * nIndices;
	dsc.CPUAccessFlags = 0;
	dsc.Usage = D3D11_USAGE_DEFAULT;
	dsc.StructureByteStride = 0;
	dsc.MiscFlags = 0;

	data.pSysMem = pIndices;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(Device()->CreateBuffer(&dsc, &data, ib))) { return 1; }
	return 0;
}

void CD3DGraphics::ResizeClient(HWND hWnd, unsigned int width, unsigned int height)
{
	int t = 0;
	CWindow* target = 0;
	if(m_cTarget >= 0 && m_cTarget < m_pRenderTarget.size())
		target = m_pRenderTarget[m_cTarget]->m_pWnd;

	for (int i = 0; i < m_pRenderTarget.size(); i++)
	{
		if (m_pRenderTarget[i]->m_pWnd->GetHandle() == hWnd)
		{
			m_pRenderTarget[i]->Resize(this, width, height);
			break;
		}
	}

	if(target)
		SetCurrentTarget(target);
}

unsigned int CD3DGraphics::AddShader(_CShader * pShader, std::string name)
{
	if (!pShader)
		return 1;

	pShader->SetShaderName(name);
	return AddShader(pShader);
}

unsigned int CD3DGraphics::AddShader(_CShader * pShader)
{
	if (!pShader)
		return 1;

	m_ppGFXShader.push_back(pShader);
	return 0;
}

void CD3DGraphics::SetActiveShader(_CShader * pShader)
{
	if (pShader == m_pActiveShader)
		return;

	if (!pShader)
	{
		m_pActiveShader = NULL;
		m_nActiveShaderIndex = NULL;
		return;
	}

	for (int i = 0; i < m_ppGFXShader.size(); i++)
	{
		if (m_ppGFXShader[i] == pShader)
		{
			SetActiveShader(i);
			return;
		}
	}
}

void CD3DGraphics::SetActiveShader(std::string name)
{
	for (int i = 0; i < m_ppGFXShader.size(); i++)
	{
		if (m_ppGFXShader[i]->GetName() == name)
		{
			SetActiveShader(i);
			return;
		}
	}
}

void CD3DGraphics::SetActiveShader(int i)
{
	if (i < 0 || i >= m_ppGFXShader.size())
		return;

	if (m_nActiveShaderIndex == i)
		return;

	m_pActiveShader = m_ppGFXShader[i];
	m_nActiveShaderIndex = i;

	m_pActiveShader->SetGFXShader();
}

unsigned int CD3DGraphics::InitView()
{
	D3DXMatrixIdentity(&m_matView);
	return 0;
}

unsigned int CD3DGraphics::InitDefaultRasterState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.AntialiasedLineEnable = true;
	desc.CullMode = D3D11_CULL_BACK;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = true;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;

	if (AddRasterState(desc))
		return ERROR_DX_INIT_RASTER_FAILED;

	return 0;
}

unsigned int CD3DGraphics::InitDefaultBlendState()
{
	D3D11_BLEND_DESC d;

	ZeroMemory(&d, sizeof(D3D11_BLEND_DESC));
	d.RenderTarget[0].BlendEnable = false;
	d.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	d.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	d.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	d.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	d.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	if (AddBlendState(d))
		return ERROR_DX_INIT_BLEND_FAILED;

	d.RenderTarget[0].BlendEnable = true;
	if (AddBlendState(d))
		return ERROR_DX_INIT_BLEND_FAILED;

	return 0;
}

unsigned int CD3DGraphics::InitDefaultDepthStecil()
{
	D3D11_DEPTH_STENCIL_DESC d;
	ZeroMemory(&d, sizeof(D3D11_DEPTH_STENCIL_DESC));
	d.DepthEnable = false;
	d.DepthFunc = D3D11_COMPARISON_LESS;
	d.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	d.StencilEnable = true;
	d.StencilReadMask = 0xFF;
	d.StencilWriteMask = 0xFF;

	d.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	d.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;

	d.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	d.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;

	if (AddDepthStencilState(d))
		return ERROR_DX_INIT_DEPTH_FAILED;
	d.DepthEnable = true;
	if (AddDepthStencilState(d))
		return ERROR_DX_INIT_DEPTH_FAILED;

	return 0;
}

unsigned int CD3DGraphics::InitDisplay()
{
	IDXGIFactory* pFactory;
	IDXGIAdapter* pAdapter;
	IDXGIOutput*  pOutput;

	DXGI_MODE_DESC* pAdapterMode;
	UINT			nAdapterModes;

	unsigned int result = 0;

	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)))return ERROR_DX_NOT_INITIALIZED;
	if (FAILED(pFactory->EnumAdapters(0, &pAdapter))) return ERROR_DX_NOT_INITIALIZED;
	if (FAILED(pAdapter->EnumOutputs(0, &pOutput))) return ERROR_DX_NOT_INITIALIZED;
	if (FAILED(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nAdapterModes, NULL))) return ERROR_DX_NOT_INITIALIZED;

	pAdapterMode = new DXGI_MODE_DESC[nAdapterModes];
	if (!pAdapterMode) return ERROR_DX_NOT_INITIALIZED;

	if (FAILED(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &nAdapterModes, pAdapterMode))) return ERROR_DX_NOT_INITIALIZED;

	if (m_vsync)
	{
		for (unsigned int i = 0; i < nAdapterModes; i++)
		{
			if (pAdapterMode[i].Width == GetSystemMetrics(SM_CXSCREEN) &&
				pAdapterMode[i].Height == GetSystemMetrics(SM_CYSCREEN))
			{
				m_refreshRate[0] = pAdapterMode[i].RefreshRate.Numerator;
				m_refreshRate[1] = pAdapterMode[i].RefreshRate.Denominator;
				break;
			}
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(pAdapter->GetDesc(&adapterDesc)))return ERROR_DX_NOT_INITIALIZED;
	m_videoMemSize = (adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	if (wcstombs_s(NULL, m_adapterDesc, 128, adapterDesc.Description, 128) != 0) { return ERROR_DX_NOT_INITIALIZED; }

	SafeRelease(pOutput);
	SafeRelease(pAdapter);
	SafeRelease(pFactory);

	return 0;
}

unsigned int CD3DGraphics::InitDevice()
{
	D3D_FEATURE_LEVEL featureLevel[] = {
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_9_1 };

	UINT flags = 0;
#ifdef _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	SafeRelease(m_pDXContext);
	SafeRelease(m_pDXDevice);

	if (FAILED(D3D11CreateDevice(nullptr,D3D_DRIVER_TYPE_HARDWARE,NULL,flags,featureLevel,sizeof(featureLevel)/sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,&m_pDXDevice, NULL, &m_pDXContext))) return ERROR_DX_NOT_INITIALIZED;

	return 0;
}

unsigned int CD3DGraphics::InitRenderTarget(int i)
{
	if (i < 0 || i >= m_pRenderTarget.size())
		return ERROR_DX_NOT_INITIALIZED;

	CWindow* w = m_pRenderTarget[i]->m_pWnd;
	DXGI_SWAP_CHAIN_DESC desc;
	D3D11_TEXTURE2D_DESC depthDesc;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
	ID3D11Texture2D* pBackbuffer = NULL;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	ZeroMemory(&depthDesc, sizeof(D3D11_TEXTURE2D_DESC));

	desc.BufferCount = 1;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags = 0;
	desc.Windowed = true;
	desc.OutputWindow = m_pRenderTarget[i]->m_pWnd->GetHandle();
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.Height = w->GetHeight();
	desc.BufferDesc.Width = w->GetWidth();
	desc.BufferDesc.RefreshRate.Numerator = m_refreshRate[0];
	desc.BufferDesc.RefreshRate.Denominator = m_refreshRate[1];
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//Get interface device
	IDXGIDevice* dxgi_dev = nullptr;
	m_pDXDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgi_dev));

	//Get Interface Adapter
	IDXGIAdapter* dxgi_adp = nullptr;
	dxgi_dev->GetAdapter(&dxgi_adp);

	//Get Interface Factory object
	IDXGIFactory* dxgi_factory = nullptr;
	dxgi_adp->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgi_factory));

	if (FAILED(dxgi_factory->CreateSwapChain(m_pDXDevice,&desc,&m_pRenderTarget[i]->m_pSwapChain))) return ERROR_DX_INIT_TARGET_FAILED;
	if (FAILED(m_pRenderTarget[i]->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackbuffer)))return ERROR_DX_INIT_TARGET_FAILED;
	if (FAILED(m_pDXDevice->CreateRenderTargetView(pBackbuffer, NULL, &m_pRenderTarget[i]->m_pTarget))) return ERROR_DX_INIT_TARGET_FAILED;

	SafeRelease(dxgi_dev);
	SafeRelease(dxgi_adp);
	SafeRelease(dxgi_factory);
	SafeRelease(pBackbuffer);

	depthDesc.ArraySize = 1;
	depthDesc.MipLevels = 1;
	depthDesc.MiscFlags = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.Height = w->GetHeight();
	depthDesc.Width = w->GetWidth();
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(m_pDXDevice->CreateTexture2D(&depthDesc, NULL, &m_pRenderTarget[i]->m_pDepthBuffer))) return ERROR_DX_INIT_TARGET_FAILED;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;

	ZeroMemory(&depthViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthViewDesc.Texture2D.MipSlice = 0;
	depthViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	
	if (FAILED(m_pDXDevice->CreateDepthStencilView(m_pRenderTarget[i]->m_pDepthBuffer, &depthViewDesc,
		&m_pRenderTarget[i]->m_pDepthStencil))) return ERROR_DX_INIT_TARGET_FAILED;

	//Create Projection and Orthographic Matrices from window variables
	D3DXMatrixPerspectiveFovLH(&m_pRenderTarget[i]->m_matProj, 60*(D3DX_PI/180.0f) /*90 degrees*/, 
		w->GetWidth() / w->GetHeight(), 0.1f, 1000.0f);
	D3DXMatrixOrthoLH(&m_pRenderTarget[i]->m_matOrtho, 
		w->GetWidth(), w->GetHeight(), 0.1f, 1000.0f);

	D3D11_VIEWPORT v;
	v.Height = (float)w->GetHeight();
	v.Width = (float)w->GetWidth();
	v.MinDepth = 0.0f;
	v.MaxDepth = 1.0f;
	v.TopLeftX = 0;
	v.TopLeftY = 0;

	m_pRenderTarget[i]->m_viewport = v;

	return 0;
}

bool CD3DGraphics::InitDefaultTexture()
{
	SafeRelease(m_pDefaultTexture);

	ID3D11Texture2D * p2DTex;
	D3D11_TEXTURE2D_DESC desc;
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	D3D11_SUBRESOURCE_DATA initialData;

	initialData.pSysMem = new D3DXVECTOR4(0.7f, 0.7f, 0.7f, 1.0f);
	initialData.SysMemPitch = sizeof(D3DXVECTOR4);
	initialData.SysMemSlicePitch = 0;

	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.Height = 1;
	desc.Width = 1;
	desc.MipLevels = 1;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

	m_pDXDevice->CreateTexture2D(&desc, &initialData, &p2DTex);
	m_pDXDevice->CreateShaderResourceView(p2DTex, NULL, &m_pDefaultTexture);

	MEMORY::SafeDelete(initialData.pSysMem);

	if (m_pDefaultTexture)
		return true;
	else
		return false;
}

void CD3DGraphics::LoadDefaultShaders()
{}

void CD3DGraphics::CD3DTarget::Resize(CD3DGraphics* gfx, unsigned int width, unsigned int height)
{
	//Check a valid GFX pointer has been passed
	if (!gfx)
		return;
	//Check the device and device context exist
	if (!gfx->Context())
		return;
	if (!gfx->Device())
		return;

	ID3D11RenderTargetView* nullView[] = { nullptr };

	gfx->Context()->OMSetRenderTargets(1, nullView, 0);
	gfx->SafeRelease(m_pTarget);
	gfx->Context()->Flush();

	HRESULT hr;
	hr = m_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* pBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	hr = gfx->Device()->CreateRenderTargetView(pBuffer, 0, &m_pTarget);
	pBuffer->Release();

	D3D11_TEXTURE2D_DESC depthDesc;
	m_pDepthBuffer->GetDesc(&depthDesc);

	//Release depth buffer resouces
	gfx->SafeRelease(m_pDepthBuffer);
	gfx->SafeRelease(m_pDepthStencil);
	
	//Resize depth buffer
	depthDesc.Width = width;
	depthDesc.Height = height;

	if (FAILED(gfx->Device()->CreateTexture2D(&depthDesc, NULL, &m_pDepthBuffer))) { return; }

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	ZeroMemory(&depthViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthViewDesc.Texture2D.MipSlice = 0;
	depthViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	if (FAILED(gfx->Device()->CreateDepthStencilView(m_pDepthBuffer, &depthViewDesc,
		&m_pDepthStencil))) return;
	
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_viewport = vp;
	
	float fov, screenAspect;
	fov = (float)D3DX_PI / 4.0f;
	screenAspect = (float)width / (float)height;
	
	D3DXMatrixPerspectiveFovLH(&m_matProj, fov, screenAspect, 0.1f, 1000.0f);
	D3DXMatrixOrthoLH(&m_matOrtho, (float)width, (float)height, 0.1f, 1000.0f);
	
	std::cout	<< "CD3DGraphics: Client resized [" << m_pWnd->GetName() << "] - " << width 
			<< "x" << height << "\n";
}
