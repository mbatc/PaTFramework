#pragma once

//////////////////////////////////////////////////
//DirectX Headers
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "D3DX11.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <D3DX11async.h>

///////////////////////////////////////////////////
//Standard C Headers
#include <vector>


///////////////////////////////////////////////////
//Banger2D Headers
#include "CWindow.h"
#include "CShaderNEW.h"

#define MAX_STATES 32

class CSprite;

struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;
	D3DXVECTOR2 UV;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 bitangent;
};

class CD3DGraphics
{
public:
	CD3DGraphics();
	~CD3DGraphics();
	ID3D11Device*			Device() { return m_pDXDevice; }
	ID3D11DeviceContext*	Context() { return m_pDXContext; }
	unsigned int GetClientWidth() {
		if (m_cTarget < 0 || m_cTarget >= m_pRenderTarget.size()) return 0; 
		else if (!m_pRenderTarget[m_cTarget]->m_pWnd) return 0; 
		else return m_pRenderTarget[m_cTarget]->m_pWnd->GetWidth();
	}
	unsigned int GetClientHeight() { 
		if (m_cTarget < 0 || m_cTarget >= m_pRenderTarget.size()) return 0; 
		else if (!m_pRenderTarget[m_cTarget]->m_pWnd) return 0; 
		else return m_pRenderTarget[m_cTarget]->m_pWnd->GetHeight();
	}

	//---------------------------------------------------
	//Initialise directX for use in the programs. Must be
	//called before before any rendering can occur
	unsigned int InitDX(CWindow* pInitialWindow);

	//---------------------------------------------------
	//Add a window to the list of Render Targets allowing
	//Direct X to render to it.
	unsigned int AddRenderTarget(CWindow* pTargetWnd);
	bool SetCurrentTarget(CWindow* pTarget);
	//---------------------------------------------------
	//This function must be called at the beginning of
	//each frame to lock the required buffers
	unsigned int BeginFrame();
	//---------------------------------------------------

	//---------------------------------------------------
	//This must be called at the end of each frame to 
	//present the current frame and clear the buffers.
	//This function also unlocks the buffers for OS use.
	unsigned int EndFrame();
	//---------------------------------------------------

	//---------------------------------------------------
	//Sets the colour that the frame is cleared to at the
	//beginning of each frame
	void SetClearColor(D3DXVECTOR4 color);
	void SetClearColor(float r = 0.0f, float g = 0.0f, float b = 0.5f, float a = 1.0f);
	//---------------------------------------------------

	//---------------------------------------------------
	//Sets the position of the camera
	void SetViewMatrix(D3DXMATRIX v);
	//---------------------------------------------------

	//---------------------------------------------------
	//
	bool SetRasterState(unsigned int i);

	bool SetBlendState(bool b);
	bool SetBlendState(unsigned int i);

	bool SetDepthState(bool b);
	bool SetDepthState(unsigned int i);
	//---------------------------------------------------

	//---------------------------------------------------
	//Add customized settings for rendering process
	//Functions return an ID associated with the state
	//which can be passed to the respective 'set'
	//function to set the current state to the desired
	//custom state
	unsigned int AddBlendState(D3D11_BLEND_DESC dsc);
	unsigned int AddRasterState(D3D11_RASTERIZER_DESC dsc);
	unsigned int AddDepthStencilState(D3D11_DEPTH_STENCIL_DESC dsc);
	//---------------------------------------------------

	//---------------------------------------------------
	//Render the shader
	unsigned int RenderShader(unsigned int nIndices, D3DXMATRIX world, std::vector<ID3D11ShaderResourceView*> resource_list);
	unsigned int RenderShader(unsigned int nIndices, D3DXMATRIX world, int numResource, ...);
	//---------------------------------------------------

	int	CreateVertexBuffer(CUSTOMVERTEX* pVertex, unsigned int nVertices, ID3D11Buffer** vb);
	int	CreateIndexBuffer(unsigned long* pIndices, unsigned int nIndices, ID3D11Buffer** ib);

	//---------------------------------------------------
	//Resize the resources associated with the specified HWND
	void ResizeClient(HWND hWnd, unsigned int width, unsigned int height);
	//---------------------------------------------------

	//---------------------------------------------------
	//Add a shader which can be used for rendering and set 
	//its name the the string passed (this name can be used to
	//set the shader using SetActiveShader(std::string name))
	unsigned int AddShader(_CShader* pShader, std::string name);
	
	//Add a shader so that it is available for use when 
	//rendering
	unsigned int AddShader(_CShader* pShader);

	//Set the shader used when calling RenderShader() function
	void SetActiveShader(_CShader* pShader);

	//Set the shader used when calling RenderShader() function
	void SetActiveShader(std::string name);

	//Set the shader used when calling RenderShader() function
	void SetActiveShader(int i);
	//---------------------------------------------------

	static CSprite DEFAULTSPRITE;

	template <class T> void SafeRelease(T*& t);
	bool _ValidTargetActive() { return (m_cTarget >= 0 && m_cTarget < m_pRenderTarget.size()); }
private:
	//---------------------------------------------------
	//Initialisation functions which create the default
	//states that can be used for rendering
	unsigned int InitView();
	unsigned int InitDefaultRasterState();
	unsigned int InitDefaultBlendState();
	unsigned int InitDefaultDepthStecil();
	unsigned int InitDisplay();
	unsigned int InitDevice();
	unsigned int InitRenderTarget(int index);
	bool InitDefaultTexture();

	void LoadDefaultShaders();
	//---------------------------------------------------

	class CD3DTarget
	{
		friend CD3DGraphics;
		//This class defines the resources needed for an individual render target in DirectX
	public:
		//Constructor will initialse the resources using the CWindow passed or HWND, Width and height
		CD3DTarget(CWindow* wnd) { m_pWnd = wnd; }
		~CD3DTarget() {}

		//Return the render targets resources
		ID3D11RenderTargetView* Target() { return m_pTarget; }
		ID3D11Texture2D*		DepthBuffer() { return m_pDepthBuffer; }
		ID3D11DepthStencilView*	DepthStencil() { return m_pDepthStencil; }
		D3D11_VIEWPORT			Viewport() { return m_viewport; }
		CWindow*				Window() { return m_pWnd; }

		void					Resize(CD3DGraphics* gfx, unsigned int width, unsigned int height);

	private:
		IDXGISwapChain*			m_pSwapChain;

		CWindow*				m_pWnd;

		//---------------------------------------------
		//Pointer to the buffer that gets displayed
		//in the windows client area
		ID3D11RenderTargetView* m_pTarget;
		//---------------------------------------------

		//---------------------------------------------
		//Depth stencil information to allow for proper
		//z-ordering during the rendering process
		ID3D11Texture2D*		m_pDepthBuffer;
		ID3D11DepthStencilView* m_pDepthStencil;
		//---------------------------------------------

		D3D11_VIEWPORT			m_viewport;

		//---------------------------------------------
		//Matrices describing the fov, aspect and size 
		//of the window. These insure that the rendered
		//images are not distorted
		D3DXMATRIX				m_matOrtho;
		D3DXMATRIX				m_matProj;
		//---------------------------------------------
	};
	
	///////////////////////////////////////////////////////
	//Direct X Specific Member Variables

	//DirectX 11 Device
	ID3D11Device*			m_pDXDevice;
	ID3D11DeviceContext*	m_pDXContext;

	// Lists of states which can be used when rendering 
	//
	//	Default states are initiaalised on startup however the user can define and add there own
	//	if the provided states do not suit there usage
	//
	//	m_pRenderTarget - The window to draw to
	//	m_ppBlendState	- Describes a new polygon should be blended with the target (polygons already rendered) 
	//					- in terms of colour values, e.g. Transparency
	//	m_ppDepthState	- Determines which objects to render in front or behind 
	//					- e.g. order they are rendered or z-value

	std::vector<CD3DTarget*>				m_pRenderTarget;
	std::vector<ID3D11BlendState*>			m_ppBlendState;
	std::vector<ID3D11DepthStencilState*>	m_ppDepthState;
	std::vector<ID3D11RasterizerState*>		m_ppRasterState;

	int	m_cTarget; //Index of current target

	// View Matrix tells the shaders where the camera is (this is set by the user before rendering)
	D3DXMATRIX  m_matView;

	//---------------------------------------------------
	//Hold the color value that the frame is reset to
	D3DXCOLOR m_clearColor;

	//---------------------------------------------------
	//Determines whether or not v-sync is active
	bool m_vsync;
	//---------------------------------------------------

	//---------------------------------------------------
	//Graphics card descriptions
	unsigned int	m_videoMemSize;
	char			m_adapterDesc[128];

	int		m_refreshRate[2];
	//---------------------------------------------------

	//---------------------------------------------------
	//Shader object list
	std::vector<_CShader*>		m_ppGFXShader;
	//Active shader ID info
	_CShader*					m_pActiveShader;
	int							m_nActiveShaderIndex;
	//---------------------------------------------------

	ID3D11ShaderResourceView*   m_pDefaultTexture;
};

template<class T>
inline void CD3DGraphics::SafeRelease(T *& t)
{
	if (!t)
		return;

	t->Release();
	t = NULL;
}