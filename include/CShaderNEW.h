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

#include <stdarg.h>
#include <vector>
#include <string>

class CD3DGraphics;

namespace SHADER
{
	enum SHADER_IN_TYPE
	{
		_UNKNOWN,
		POSITION,
		TEXCOORD,
		COLOR,
		NORMAL,
		TANGENT,
		BITANGENT
	};

	struct SHADER_TYPE_DEF
	{
		SHADER_TYPE_DEF(SHADER_IN_TYPE type, DXGI_FORMAT format, const char* semantic)
			:
			m_type(type), m_format(format), m_semantic(semantic)
		{}

		SHADER_IN_TYPE	m_type;
		DXGI_FORMAT		m_format;
		const char*		m_semantic;
	};

	const char* GetSemanticName(SHADER_IN_TYPE type);
	DXGI_FORMAT GetTypeFormat(SHADER_IN_TYPE type);
	DXGI_FORMAT GetTypeFormat(const char* type_def);
	SHADER_IN_TYPE GetType(const char* type);

	static SHADER::SHADER_TYPE_DEF _types[] = {
		//General semantic formats (probably not needed - will be removed)
		{ POSITION, DXGI_FORMAT_R32G32B32_FLOAT,"POSITION" },
		{ TEXCOORD, DXGI_FORMAT_R32G32_FLOAT,"TEXCOORD" },
		{ COLOR, DXGI_FORMAT_R32G32B32A32_FLOAT,"COLOR" },
		{ NORMAL, DXGI_FORMAT_R32G32B32_FLOAT,"NORMAL" },
		{ TANGENT, DXGI_FORMAT_R32G32B32_FLOAT,"TANGENT" },
		{ BITANGENT, DXGI_FORMAT_R32G32B32_FLOAT,"BITANGENT" },

		//Direct X Type def format
		{ _UNKNOWN, DXGI_FORMAT_R32G32_FLOAT, "D3DXVECTOR2" },
		{ _UNKNOWN, DXGI_FORMAT_R32G32B32_FLOAT, "D3DXVECTOR3"},
		{ _UNKNOWN, DXGI_FORMAT_R32G32B32A32_FLOAT, "D3DXVECTOR4" }
	};
}

//A flexible shader class which allows you to specify the shader files and it handles the initialisation of those files

class _CShader
{
public:
	struct MatrixBuffer
	{
		D3DXMATRIX w;
		D3DXMATRIX v;
		D3DXMATRIX p;
		D3DXMATRIX o;
	};

public:
	_CShader(std::string name, std::string psFile, std::string vsFile, std::string gsFile = "", std::string hsFile = "", std::string dsFile = "");
	~_CShader();

	unsigned int Init(CD3DGraphics* pDX, bool bISFromFile);
	
	//--------------------------------------------------------
	// Pass the vertex struct as a c-string to be passed. An
	// Input struct is then generated using this string
	// 
	// The format of this definition is [DATATYPE] [HLSL SEMANTIC]
	// The definition must be structured exactly the same as the 
	// vertex struct
	// e.g. 
	// 
	// VERTEX STRUCT -
	//
	// struct SimpleVertex
	// {
	//		D3DXVECTOR3 pos;
	//		D3DXVECTOR2 tex;
	// }
	//
	// VERTEX STRUCT DEFINITION = "D3DXVECTOR3 POSITION D3DXVECTOR2 TEXTURE"]
	unsigned int SetInputLayout(char* vertex_def);
	//--------------------------------------------------------

	//--------------------------------------------------------
	//
	unsigned int UpdateMatrixBuffer(D3DXMATRIX view, D3DXMATRIX projection, D3DXMATRIX world, D3DXMATRIX ortho);
	//--------------------------------------------------------

	//--------------------------------------------------------
	//
	unsigned int SetInputLayout(D3D11_INPUT_ELEMENT_DESC* dsc, int nElements);
	//--------------------------------------------------------

	//--------------------------------------------------------
	unsigned int GenerateInputLayout(std::vector<SHADER::SHADER_TYPE_DEF> dataTypes);
	//--------------------------------------------------------

	//--------------------------------------------------------
	//
	void SetPixelShader(std::string file);
	//--------------------------------------------------------

	//--------------------------------------------------------
	//
	void SetVertiexShader(std::string file);
	//--------------------------------------------------------

	//--------------------------------------------------------
	//
	void SetHullShader(std::string file);
	//--------------------------------------------------------

	//--------------------------------------------------------
	//
	void SetGeometryShader(std::string file);
	//--------------------------------------------------------

	//--------------------------------------------------------
	//
	void SetDomainShader(std::string file);
	//--------------------------------------------------------

	//--------------------------------------------------------
	//
	unsigned int CompileShader();
	//--------------------------------------------------------
	
	void SetSampleState(int index);
	void AddSampleState(D3D11_SAMPLER_DESC desc);

	void SetRequiredResources(int count) { m_resource_count = count; }

	unsigned int Render(unsigned int nIndices, _CShader::MatrixBuffer mat, std::vector<ID3D11ShaderResourceView*> resource_list);

	void SetGFXShader();

	void SetShaderName(std::string name);
	std::string GetName();

	void set_depthstate(int i) { m_nDepthState = i; }
	void set_blendstate(int i) { m_nBlendState = i; }
	void set_rasterstate(int i) { m_nRasterState = i; }

	void set_depthstate(bool b) { set_depthstate((int)b); }
	void set_blendstate(bool b) { set_blendstate((int)b); }
private:
	void PassInputStruct(FILE * f);

	void InitDefaultSampleState();
	bool CreateMatrixBuffer();

	ID3D10Blob* CompileFile(std::string file, char* entry, char* profile);

	unsigned int CompilePS();
	unsigned int CompileVS();
	unsigned int CompileHS();
	unsigned int CompileGS();
	unsigned int CompileDS();

	//--------------------------------------------------------
	//
	void OutputErrorMsg(ID3D10Blob* msg, std::string file);
	//--------------------------------------------------------

	ID3D11PixelShader*		m_pSPixel;
	ID3D11VertexShader*		m_pSVertex;
	ID3D11HullShader*		m_pSHull;
	ID3D11GeometryShader*	m_pSGeometry;
	ID3D11DomainShader*		m_pSDomain;

	ID3D10Blob*				m_pVsBuffer;

	std::string				m_vsFile;
	std::string				m_psFile;
	std::string				m_gsFile;
	std::string				m_hsFile;
	std::string				m_dsFile;

	ID3D11InputLayout*		m_pILayout;
	ID3D11Buffer*			m_pMatrix;

	D3D11_INPUT_ELEMENT_DESC*	m_pIDesc;
	int							m_nInputElements;

	CD3DGraphics*				m_pDX;

	std::vector<ID3D11SamplerState*>	m_ppSampleState;
	int									m_curSS;

	int m_resource_count;

	int m_nDepthState;
	int m_nBlendState;
	int m_nRasterState;

	//Used to identify individual shaders
	std::string				m_shader_name;
};