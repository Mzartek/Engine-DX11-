#include <Engine/SkyBox.hpp>
#include <FreeImage.h>

engine::SkyBox::SkyBox()
{
	_pTexture = NULL;
	_pShaderResourceView = NULL;
	_pSamplerState = NULL;
	_pVertexBuffer = NULL;
	_pIndexBuffer = NULL;
	_pInputLayout = NULL;
	_pMVPMatrixBuffer = NULL;
	_rotateMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_rotateMatrix, 16);
	_program = NULL;

	*_rotateMatrix = XMMatrixIdentity();
}

engine::SkyBox::~SkyBox()
{
	_aligned_free(_rotateMatrix);

	if (_pMVPMatrixBuffer)
		_pMVPMatrixBuffer->Release();
	if (_pInputLayout)
		_pInputLayout->Release();
	if (_pIndexBuffer)
		_pIndexBuffer->Release();
	if (_pVertexBuffer)
		_pVertexBuffer->Release();
	if (_pSamplerState)
		_pSamplerState->Release();
	if (_pShaderResourceView)
		_pShaderResourceView->Release();
	if (_pTexture)
		_pTexture->Release();
}

void engine::SkyBox::load(const TCHAR *posx, const TCHAR *negx,
	const TCHAR *posy, const TCHAR *negy,
	const TCHAR *posz, const TCHAR *negz,
	FLOAT dim, ShaderProgram *program, ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	UINT i;
	FIBITMAP *image[6];
	FIBITMAP *tmp = NULL;

	_program = program;

	image[0] = FreeImage_Load(FreeImage_GetFileType(posx), posx);
	image[1] = FreeImage_Load(FreeImage_GetFileType(negx), negx);
	image[2] = FreeImage_Load(FreeImage_GetFileType(posy), negy);
	image[3] = FreeImage_Load(FreeImage_GetFileType(negy), posy);
	image[4] = FreeImage_Load(FreeImage_GetFileType(posz), posz);
	image[5] = FreeImage_Load(FreeImage_GetFileType(negz), negz);

	D3D11_TEXTURE2D_DESC descTexture;
	descTexture.Width = FreeImage_GetWidth(image[0]);
	descTexture.Height = FreeImage_GetHeight(image[0]);
	descTexture.MipLevels = 1;
	descTexture.ArraySize = 6;
	descTexture.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	descTexture.SampleDesc.Count = 1;
	descTexture.SampleDesc.Quality = 0;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	descTexture.CPUAccessFlags = 0;
	descTexture.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SUBRESOURCE_DATA data[6];
	for (i = 0; i < 6; i++)
	{
		if (image == NULL)
		{
			MessageBox(NULL, "Fail to load an Image", "SkyBox", MB_OK);
			exit(1);
		}
		tmp = image[i];
		image[i] = FreeImage_ConvertTo32Bits(image[i]);
		FreeImage_Unload(tmp);

		data[i].pSysMem = FreeImage_GetBits(image[i]);
		data[i].SysMemPitch = 4 * descTexture.Width;
		data[i].SysMemSlicePitch = 0;
	}
	hr = pd3dDevice->CreateTexture2D(&descTexture, data, &_pTexture);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create the Cube Texture", "SkyBox", MB_OK);
		exit(1);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResourceView;
	descShaderResourceView.Format = descTexture.Format;
	descShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	descShaderResourceView.TextureCube.MostDetailedMip = 0;
	descShaderResourceView.TextureCube.MipLevels = descTexture.MipLevels;
	hr = pd3dDevice->CreateShaderResourceView(_pTexture, &descShaderResourceView, &_pShaderResourceView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create the ShaderResourceView", "SkyBox", MB_OK);
		exit(1);
	}

	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 1;
	descSampler.BorderColor[0] = 0.0f;
	descSampler.BorderColor[1] = 0.0f;
	descSampler.BorderColor[2] = 0.0f;
	descSampler.BorderColor[3] = 0.0f;
	descSampler.MinLOD = 0;
	descSampler.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pd3dDevice->CreateSamplerState(&descSampler, &_pSamplerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Error while creating the SamplerState", "SkyBox", MB_OK);
		exit(1);
	}

	// Create the Cube
	FLOAT vertexArray[] = {
		+dim, -dim, -dim,
		-dim, -dim, -dim,
		-dim, -dim, +dim,
		+dim, -dim, +dim,
		+dim, +dim, -dim,
		-dim, +dim, -dim,
		-dim, +dim, +dim,
		+dim, +dim, +dim
	};
	UINT indexArray[] = {
		0, 1, 2, 0, 2, 3,
		4, 7, 6, 4, 6, 5,
		0, 4, 5, 0, 5, 1,
		3, 2, 6, 3, 6, 7,
		1, 5, 6, 1, 6, 2,
		0, 3, 7, 0, 7, 4
	};
	_numElement = sizeof(indexArray) / sizeof(UINT);

	D3D11_BUFFER_DESC bd;
	// Create vertex buffer
	bd.ByteWidth = sizeof vertexArray;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	data[0].pSysMem = vertexArray;
	data[0].SysMemPitch = 0;
	data[0].SysMemSlicePitch = 0;
	hr = pd3dDevice->CreateBuffer(&bd, data, &_pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Vertex Buffer", "SkyBox", MB_OK);
		exit(1);
	}

	// Create Index Buffer
	bd.ByteWidth = sizeof indexArray;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	data[0].pSysMem = indexArray;
	hr = pd3dDevice->CreateBuffer(&bd, data, &_pIndexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Index Buffer", "SkyBox", MB_OK);
		exit(1);
	}

	// Create Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		_program->getEntryBufferPointer(), _program->getEntryBytecodeLength(),
		&_pInputLayout);

	// Create the matrix buffer
	bd.ByteWidth = sizeof XMMATRIX;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pMVPMatrixBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Matrix Buffer", "SkyBox", MB_OK);
		exit(1);
	}
}

void engine::SkyBox::rotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_rotateMatrix = XMMatrixRotationAxis(XMVectorSet(x, y, z, 1.0f), angle * ((FLOAT)XM_PI / 180)) * *_rotateMatrix;
}

void engine::SkyBox::display(GBuffer *g, Camera *cam)
{
	if (_program == NULL)
	{
		MessageBox(NULL, "Need to config the SkyBox before displaying", "SkyBox", MB_OK);
		exit(1);
	}
	if (g == NULL)
	{
		MessageBox(NULL, "Bad GBuffer", "SkyBox", MB_OK);
		exit(1);
	}
	if (cam == NULL)
	{
		MessageBox(NULL, "Bad Camera", "SkyBox", MB_OK);
		exit(1);
	}

	XMMATRIX pos = XMMatrixTranslation(cam->getPositionCamera().x, cam->getPositionCamera().y, cam->getPositionCamera().z);
	pos = *_rotateMatrix * pos;
	pos *= cam->getVPMatrix();

	g->enableDepthMask(FALSE);
	g->getContext()->VSSetShader(_program->getVertexShader(), NULL, 0);
	g->getContext()->GSSetShader(_program->getGeometryShader(), NULL, 0);
	g->getContext()->PSSetShader(_program->getPixelShader(), NULL, 0);
	g->getContext()->IASetInputLayout(_pInputLayout);
	g->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g->getContext()->UpdateSubresource(_pMVPMatrixBuffer, 0, NULL, &pos, 0, 0);
	g->getContext()->VSSetConstantBuffers(0, 1, &_pMVPMatrixBuffer);

	g->getContext()->PSSetShaderResources(0, 1, &_pShaderResourceView);
	g->getContext()->PSSetSamplers(0, 1, &_pSamplerState);

	// Vertex And Index Buffer
	UINT stride = 3 * sizeof(FLOAT), offset = 0;
	g->getContext()->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	g->getContext()->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	g->getContext()->DrawIndexed(_numElement, 0, 0);

	g->enableDepthMask(TRUE);

	g->executeDeferredContext();
}
