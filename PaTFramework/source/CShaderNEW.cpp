#include "CShaderNEW.h"

#include <stdio.h>
#include <fstream>

#include "CD3DGraphics.h"
#include "helpers/MemoryHelper.h"
#include "helpers/FileHelper.h"

#include "error.h"

#include "CLog.h"

using namespace SHADER;
using namespace std;

_CShader::_CShader(string name, string psFile, string vsFile, string gsFile, string hsFile, string dsFile)
	:
	m_pSDomain(NULL),
	m_pSGeometry(NULL),
	m_pSHull(NULL),
	m_pSVertex(NULL),
	m_pSPixel(NULL),
	m_curSS(-1),
	m_nBlendState(0),
	m_nRasterState(0),
	m_nDepthState(0)
{
	ZeroMemory(this, sizeof(_CShader));

	m_psFile = psFile;
	m_vsFile = vsFile;
	m_gsFile = gsFile;
	m_hsFile = hsFile;
	m_dsFile = dsFile;

	m_shader_name = name;
}

_CShader::~_CShader()
{
}

unsigned int _CShader::Init(CD3DGraphics * pDX, bool bISFromFile = true)
{
	unsigned int result = 0;
	if (!pDX)
	{
		return -1;
	}

	m_pDX = pDX;

	result = CompileShader();

	if (!CreateMatrixBuffer())
		return -1;
/*
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;
	D3DXVECTOR2 UV;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 bitangent;*/

	SetInputLayout("D3DXVECTOR3 POSITION \
		D3DXVECTOR4 COLOR\
		D3DXVECTOR2 TEXCOORD\
		D3DXVECTOR3 NORMAL\
		D3DXVECTOR3 TANGENT\
		D3DXVECTOR3 BINORMAL");

	InitDefaultSampleState();
	SetRequiredResources(0);
	return result;
}

unsigned int _CShader::SetInputLayout(char * vertex_def)
{
	std::vector<std::string> args;
	std::string cmd = vertex_def;

	int len = cmd.length();
	
	int start = 0;
	bool valid = false;
	for (int i = 0; i < len; i++)
	{
		if (!FILEH::LOAD::isWhitespace(cmd[i]))
		{
			if (!valid)
				start = i;
			valid = true;
		}
		else if (FILEH::LOAD::isWhitespace(cmd[i]) && valid)
		{
			cmd[i] = '\0';
			args.push_back(&cmd[start]);
			start = i + 1;
			valid = false;
		}

		if (i == len - 1)
		{
			if (valid)
			{
				args.push_back(&cmd[start]);
				break;
			}
		}
	}

	std::vector<SHADER_TYPE_DEF> IL;
	for (int i = 0; i < args.size() / 2; i++)
	{
		SHADER_TYPE_DEF def(SHADER::GetType(args[2*i + 1].c_str()),
			SHADER::GetTypeFormat(args[2*i].c_str()), args[2*i + 1].c_str());
		
		IL.push_back(def);
	}

	GenerateInputLayout(IL);

	return 0;
}

unsigned int _CShader::UpdateMatrixBuffer(D3DXMATRIX view, D3DXMATRIX projection, D3DXMATRIX world, D3DXMATRIX ortho)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE res;
	MatrixBuffer *data;

	if (FAILED(m_pDX->Context()->Map(m_pMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &res))) { return -1; }

	data = (MatrixBuffer*)res.pData;

	D3DXMatrixTranspose(&data->v, &view);
	D3DXMatrixTranspose(&data->o, &ortho);
	D3DXMatrixTranspose(&data->w, &world);
	D3DXMatrixTranspose(&data->p, &projection);

	m_pDX->Context()->Unmap(m_pMatrix, 0);
	m_pDX->Context()->VSSetConstantBuffers(0, 1, &m_pMatrix);
	return 0;
}

unsigned int _CShader::SetInputLayout(D3D11_INPUT_ELEMENT_DESC* dsc, int nElements)
{
	if (!m_pVsBuffer)
	{
		MessageBox(NULL, "Shader Error!", "Shader must be compiled before Creating Input Layout!",
			MB_OK || MB_ICONEXCLAMATION);
		return -1;
	}
	HRESULT r;
	r = m_pDX->Device()->CreateInputLayout(dsc, nElements, m_pVsBuffer->GetBufferPointer(), m_pVsBuffer->GetBufferSize(),
		&m_pILayout);
	if(FAILED(r)) return ERROR_DX_NOT_INITIALIZED;

	return 0;
}
bool _CShader::update_constant_buffer(int buffer_id, void * data)
{
	ID3D11Buffer* buffer = nullptr;
	int data_size = 0;
	for (int i = 0; i < m_pConstBuffer.size(); i++)
	{
		if (m_pConstBuffer[i]->_id == buffer_id)
		{
			buffer = m_pConstBuffer[i]->_d3dbuffer;
			data_size = m_pConstBuffer[i]->_size;
		}
	}

	if (!buffer)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "No shader constant buffer with id: %d exists", buffer_id);
		return false;
	}

	D3D11_MAPPED_SUBRESOURCE resData;
	m_pDX->Context()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resData);
	memcpy(resData.pData, data, data_size);
	m_pDX->Context()->Unmap(buffer, 0);

	return true;
}

int _CShader::add_constant_buffer(char shader_type, int index, ID3D11Buffer * buffer,int size)
{
	ConstantBuffer* cbuf = new ConstantBuffer;
	cbuf->_d3dbuffer = buffer;
	cbuf->_id = get_constbuffer_uniqueid();
	cbuf->_shader_index = index;
	cbuf->_shader_type = shader_type;
	cbuf->_size = size;

	m_pConstBuffer.push_back(cbuf);

	return cbuf->_id;
}

int _CShader::get_constbuffer_uniqueid()
{
	int id = rand()*MAXINT;
	for (int i = 0; i < m_pConstBuffer.size(); i++)
	{
		if (m_pConstBuffer[i]->_id == id)
			return get_constbuffer_uniqueid();
	}
	return id;
}

void _CShader::PassInputStruct(FILE * f)
{
	if (!f)
		return;

	std::string buf = "";
	std::vector<SHADER_TYPE_DEF> ISDef;
	int nISTypes = 0;

	do
	{
		buf = FILEH::LOAD::ScanSafe(f);
		if (buf[buf.length() - 1] == ';')
		{
			buf[buf.length() - 1] = '\0';
		}
		SHADER_TYPE_DEF t(GetType(buf.c_str()),GetTypeFormat(GetType(buf.c_str())),buf.c_str());
		switch (t.m_type)
		{
		case _UNKNOWN:
			break;
		default:
			ISDef.push_back(t);
			break;
		}

	} while (!strstr(buf.c_str(), "}"));

	ISDef.clear();
}


unsigned int _CShader::GenerateInputLayout(std::vector<SHADER::SHADER_TYPE_DEF> dataTypes)
{
	m_nInputElements = 0;
	MEMORY::SafeDeleteArr(m_pIDesc);

	for (int i = 0; i < dataTypes.size(); i++)
	{
		D3D11_INPUT_ELEMENT_DESC d = {dataTypes[i].m_semantic,0, dataTypes[i].m_format,
			0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 };
		if (i == 0)
		{
			d.AlignedByteOffset = 0;
		}

		MEMORY::LIST::AddToList(m_pIDesc, &d, m_nInputElements);
	}

	SetInputLayout(m_pIDesc, m_nInputElements);

	return 0;
}

void _CShader::SetPixelShader(string file)
{
	m_psFile = file;
}

void _CShader::SetVertiexShader(string file)
{
	m_vsFile = file;
}

void _CShader::SetHullShader(string file)
{
	m_hsFile = file;
}

void _CShader::SetGeometryShader(string file)
{
	m_gsFile = file;
}

void _CShader::SetDomainShader(string file)
{
	m_dsFile = file;
}

unsigned int _CShader::CompileShader()
{
	if (m_psFile.length())
		CompilePS();

	if (m_vsFile.length())
		CompileVS();

	if (m_dsFile.length())
		CompileDS();

	if (m_hsFile.length())
		CompileHS();

	if (m_gsFile.length())
		CompileGS();

	return 0;
}

void _CShader::SetSampleState(int index)
{
	m_curSS = index;
	if (index < 0 || index >= m_ppSampleState.size())
	{
		m_pDX->Context()->PSSetSamplers(0, 0, 0);
		m_curSS = index;
		return;
	}
}

void _CShader::AddSampleState(D3D11_SAMPLER_DESC desc)
{
	ID3D11SamplerState* state = 0;
	m_ppSampleState.resize(m_ppSampleState.size() + 1);

	m_pDX->Device()->CreateSamplerState(&desc, &m_ppSampleState[m_ppSampleState.size() - 1]);
}

unsigned int _CShader::Render(unsigned int nIndices, _CShader::MatrixBuffer mat, std::vector<ID3D11ShaderResourceView*> resource_list)
{
	if (!m_pSVertex || !m_pSPixel)
	{
		//A vertex and pixel shader are required as DirectX has no default shaders that it can use
		return ERROR_DX_SHADER_INVALID;
	}

	if (resource_list.size() < m_resource_count)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to render shader (%s) not enough resources passed to shader", m_shader_name);
		return ERROR_DX_INVALID_DIMENSIONS;
	}
	UpdateMatrixBuffer(mat.v, mat.p, mat.w, mat.o);
	m_pDX->Context()->PSSetShaderResources(0, m_resource_count, resource_list.data());
	m_pDX->Context()->DrawIndexed(nIndices, 0, 0);
	return 0;
}

void _CShader::SetGFXShader()
{
	if (!m_pSVertex || !m_pSPixel)
	{
		//A vertex and pixel shader are required as DirectX has no default shaders that it can use
		return;
	}

	m_pDX->Context()->VSSetShader(m_pSVertex, NULL, 0);
	m_pDX->Context()->PSSetShader(m_pSPixel, NULL, 0);
	if (m_curSS >= 0 || m_curSS < m_ppSampleState.size())
		m_pDX->Context()->PSSetSamplers(0, 1, &m_ppSampleState[m_curSS]);

	m_pDX->Context()->IASetInputLayout(m_pILayout);

	//Set render states
	m_pDX->SetBlendState((unsigned int)m_nBlendState);
	m_pDX->SetDepthState((unsigned int)m_nDepthState);
	m_pDX->SetRasterState((unsigned int)m_nRasterState);

	for (int i = 0; i < m_pConstBuffer.size(); i++)
	{
		if (!m_pConstBuffer[i])
			continue;
		switch (m_pConstBuffer[i]->_shader_type)
		{
		case 'p':
			m_pDX->Context()->PSSetConstantBuffers(m_pConstBuffer[i]->_shader_index, 1, &m_pConstBuffer[i]->_d3dbuffer);
			break;
		case 'v':
			m_pDX->Context()->VSSetConstantBuffers(m_pConstBuffer[i]->_shader_index, 1, &m_pConstBuffer[i]->_d3dbuffer);
			break;
		case 'h':
			m_pDX->Context()->HSSetConstantBuffers(m_pConstBuffer[i]->_shader_index, 1, &m_pConstBuffer[i]->_d3dbuffer);
			break;
		case 'd':
			m_pDX->Context()->DSSetConstantBuffers(m_pConstBuffer[i]->_shader_index, 1, &m_pConstBuffer[i]->_d3dbuffer);
			break;
		case 'g':
			m_pDX->Context()->GSSetConstantBuffers(m_pConstBuffer[i]->_shader_index, 1, &m_pConstBuffer[i]->_d3dbuffer);
			break;
		};
	}
}

void _CShader::SetShaderName(std::string name)
{
	m_shader_name = name;
}

std::string _CShader::GetName()
{
	return m_shader_name;
}

void _CShader::InitDefaultSampleState()
{
	D3D11_SAMPLER_DESC sampleDesc;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampleDesc.MipLODBias = 0.0f;
	sampleDesc.MaxAnisotropy = 1;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampleDesc.BorderColor[0] = 0;
	sampleDesc.BorderColor[1] = 0;
	sampleDesc.BorderColor[2] = 0;
	sampleDesc.BorderColor[3] = 0;

	AddSampleState(sampleDesc);
}

bool _CShader::CreateMatrixBuffer()
{
	HRESULT r;
	D3D11_BUFFER_DESC dsc;
	dsc.Usage = D3D11_USAGE_DYNAMIC;
	dsc.ByteWidth = sizeof(MatrixBuffer);
	dsc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dsc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	dsc.MiscFlags = 0;
	dsc.StructureByteStride = 0;

	if (FAILED(m_pDX->Device()->CreateBuffer(&dsc, NULL, &m_pMatrix))) { return false; }

	return true;
}

ID3D10Blob* _CShader::CompileFile(string file, char * entry, char * profile)
{
	ID3D10Blob* err = NULL;
	ID3D10Blob* buffer;

	if (!file.length())
	{
		return NULL;
	}

	if (FAILED(D3DX11CompileFromFile(file.c_str(), NULL, NULL, entry, profile, D3D10_SHADER_ENABLE_STRICTNESS,
		0, NULL, &buffer, &err, NULL)))
	{
		if (err)
		{
			OutputErrorMsg(err, file);
		}
		else
		{
			MessageBox(NULL, "Could not find Shader File!", file.c_str(), MB_OK);
		}

		return NULL;
	}

	return buffer;
}

unsigned int _CShader::CompilePS()
{
	ID3D10Blob* buf = CompileFile(m_psFile, "PSMain", "ps_5_0");
	if (!buf)
		return -1;

	if (FAILED(m_pDX->Device()->CreatePixelShader(buf->GetBufferPointer(), buf->GetBufferSize(), NULL, &m_pSPixel)))
	{
		return -1;
	}

	return 0;
}

unsigned int _CShader::CompileVS()
{
	m_pVsBuffer = CompileFile(m_vsFile, "VSMain", "vs_5_0");
	if (!m_pVsBuffer)
		return -1;

	if (FAILED(m_pDX->Device()->CreateVertexShader(m_pVsBuffer->GetBufferPointer(), m_pVsBuffer->GetBufferSize(),
		NULL, &m_pSVertex)))
	{
		return -1;
	}

	return 0;
}

unsigned int _CShader::CompileHS()
{
	ID3D10Blob* buf = CompileFile(m_hsFile, "HSMain", "ps_5_0");
	if (!buf)
		return -1;

	if (FAILED(m_pDX->Device()->CreateHullShader(buf->GetBufferPointer(), buf->GetBufferSize(), NULL, &m_pSHull)))
	{
		return -1;
	}

	return 0;
}

unsigned int _CShader::CompileGS()
{
	ID3D10Blob* buf = CompileFile(m_gsFile, "GSMain", "ps_5_0");
	if (!buf)
		return -1;

	if (FAILED(m_pDX->Device()->CreateGeometryShader(buf->GetBufferPointer(), buf->GetBufferSize(), NULL, &m_pSGeometry)))
	{
		return -1;
	}

	return 0;
}

unsigned int _CShader::CompileDS()
{
	ID3D10Blob* buf = CompileFile(m_dsFile, "DSMain", "ps_5_0");
	if (!buf)
		return -1;

	if (FAILED(m_pDX->Device()->CreateDomainShader(buf->GetBufferPointer(), buf->GetBufferSize(), NULL, &m_pSDomain)))
	{
		return -1;
	}

	return 0;
}

void _CShader::OutputErrorMsg(ID3D10Blob * msg, string file)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	compileErrors = (char*)(msg->GetBufferPointer());
	bufferSize = msg->GetBufferSize();

	fout.open("shader-error.txt");
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}
	fout.close();

	m_pDX->SafeRelease(msg);
	MessageBox(NULL, "Error compiling shader. Check sader-error.txt for message.", file.c_str(), MB_OK);

	return;
}

const char * SHADER::GetSemanticName(SHADER_IN_TYPE type)
{
	for (int i = 0; i < sizeof(_types) / sizeof(SHADER_TYPE_DEF); i++)
	{
		if (type == _types[i].m_type)
		{
			return _types[i].m_semantic;
		}
	}
	return nullptr;
}

DXGI_FORMAT SHADER::GetTypeFormat(SHADER_IN_TYPE type)
{
	for (int i = 0; i < sizeof(_types) / sizeof(SHADER_TYPE_DEF); i++)
	{
		if (type == _types[i].m_type)
		{
			return _types[i].m_format;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT SHADER::GetTypeFormat(const char* type)
{
	for (int i = 0; i < sizeof(_types) / sizeof(SHADER_TYPE_DEF); i++)
	{
		if (!strcmp(type,_types[i].m_semantic))
		{
			return _types[i].m_format;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}

SHADER_IN_TYPE SHADER::GetType(const char * type)
{
	for (int i = 0; i < sizeof(_types) / sizeof(SHADER_TYPE_DEF); i++)
	{
		if (!strcmp(_types[i].m_semantic, type))
			return _types[i].m_type;
	}

	//because texcoord semantic can have an index after it i need (e.g. TEXCOORD1)
	//to check specifically for the formatting of this sematic
	SHADER_IN_TYPE ret = SHADER_IN_TYPE::_UNKNOWN;

	const char* s = GetSemanticName(TEXCOORD);
	int l = strlen(s);
	for (int i = 0; i < l; i++)
	{
		if (s[i] != type[i])
		{
			ret = SHADER_IN_TYPE::_UNKNOWN;
			break;
		}

		ret = SHADER_IN_TYPE::TEXCOORD;
	}

	return ret;
}
