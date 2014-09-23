#include <Engine/Texture.hpp>
#include <FreeImage.h>

extern ID3D11Device *Device;
extern ID3D11DeviceContext *DeviceContext;

engine::Texture::Texture(void)
	: _pShaderResourceView(NULL), _pSamplerState(NULL)
{
}

engine::Texture::~Texture(void)
{
	if (_pShaderResourceView) _pShaderResourceView->Release();
	if (_pSamplerState) _pSamplerState->Release();
}

ID3D11ShaderResourceView *engine::Texture::getShaderResourceView(void)
{
	return _pShaderResourceView;
}

ID3D11SamplerState *engine::Texture::getSamplerState(void)
{
	return _pSamplerState;
}

void engine::Texture::load2DTextureFromFile(const TCHAR *path)
{
	HRESULT hr;
	FIBITMAP *image;
	FIBITMAP *tmp;
	ID3D11Texture2D *texture;

	if (_pShaderResourceView) _pShaderResourceView->Release();
	if (_pSamplerState) _pSamplerState->Release();

	image = FreeImage_Load(FreeImage_GetFileType(path), path);
	if (image == NULL)
	{
		std::string text = "Fail to load file: ";
		text.append(path);
		MessageBox(NULL, text.c_str(), "Texture", MB_OK);
		exit(1);
	}
	tmp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(tmp);

	D3D11_TEXTURE2D_DESC descTexture;
	descTexture.Width = FreeImage_GetWidth(image);
	descTexture.Height = FreeImage_GetHeight(image);
	descTexture.MipLevels = 9;
	descTexture.ArraySize = 1;
	descTexture.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	descTexture.SampleDesc.Count = 1;
	descTexture.SampleDesc.Quality = 0;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	descTexture.CPUAccessFlags = 0;
	descTexture.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hr = Device->CreateTexture2D(&descTexture, NULL, &texture);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Error while creating the Texture2D", "Texture", MB_OK);
		exit(1);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResourceView;
	descShaderResourceView.Format = descTexture.Format;
	descShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descShaderResourceView.Texture2D.MostDetailedMip = 0;
	descShaderResourceView.Texture2D.MipLevels = descTexture.MipLevels;
	hr = Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Error while creating the ShaderResourceView", "Texture", MB_OK);
		exit(1);
	}

	DeviceContext->UpdateSubresource(texture, 0, NULL, FreeImage_GetBits(image), 4 * descTexture.Width, 4 * descTexture.Width * descTexture.Height);
	DeviceContext->GenerateMips(_pShaderResourceView);

	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_ANISOTROPIC;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 16;
	descSampler.BorderColor[0] = 0.0f;
	descSampler.BorderColor[1] = 0.0f;
	descSampler.BorderColor[2] = 0.0f;
	descSampler.BorderColor[3] = 0.0f;
	descSampler.MinLOD = 0;
	descSampler.MaxLOD = D3D11_FLOAT32_MAX;
	hr = Device->CreateSamplerState(&descSampler, &_pSamplerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Error while creating the SamplerState", "Texture", MB_OK);
		exit(1);
	}

	FreeImage_Unload(image);
	texture->Release();
}

void engine::Texture::loadCubeTextureFromFiles(
	const TCHAR *posx, const TCHAR *negx,
	const TCHAR *posy, const TCHAR *negy,
	const TCHAR *posz, const TCHAR *negz)
{
	HRESULT hr;
	UINT i;
	FIBITMAP *image[6];
	FIBITMAP *tmp;
	ID3D11Texture2D *texture;

	if (_pShaderResourceView) _pShaderResourceView->Release();
	if (_pSamplerState) _pSamplerState->Release();

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
	descTexture.Usage = D3D11_USAGE_IMMUTABLE;
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
	hr = Device->CreateTexture2D(&descTexture, data, &texture);
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
	hr = Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView);
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
	hr = Device->CreateSamplerState(&descSampler, &_pSamplerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Error while creating the SamplerState", "SkyBox", MB_OK);
		exit(1);
	}

	texture->Release();
}