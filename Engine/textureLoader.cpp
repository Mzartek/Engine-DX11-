#include <Engine/DXHead.hpp>
#include <FreeImage.h>
#include "DirectXTK/DDSTextureLoader.h"

HRESULT engine::loadTextureFromFile(const TCHAR *szFileName, ID3D11ShaderResourceView **pptex, ID3D11SamplerState **ppsam, ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	FIBITMAP *image = NULL;
	FIBITMAP *tmp = NULL;

	image = FreeImage_Load(FreeImage_GetFileType(&szFileName[0]), &szFileName[0]);
	tmp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(tmp);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = FreeImage_GetWidth(image);
	texDesc.Height = FreeImage_GetHeight(image);
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = FreeImage_GetBits(image);
	initData.SysMemPitch = 4 * texDesc.Width;
	initData.SysMemSlicePitch = 4 * texDesc.Width * texDesc.Height;

	ID3D11Texture2D *tex = NULL;
	hr = pd3dDevice->CreateTexture2D(&texDesc, &initData, &tex);
	FreeImage_Unload(image);
	if (FAILED(hr))
		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format = texDesc.Format;
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSRV.Texture2D.MipLevels = 1;
	hr = pd3dDevice->CreateShaderResourceView(tex, &descSRV, pptex);
	tex->Release();
	if (FAILED(hr))
		return hr;

	D3D11_SAMPLER_DESC sampDesc;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = 0;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pd3dDevice->CreateSamplerState(&sampDesc, ppsam);

	return hr;
}

HRESULT engine::loadDDSTexture(const WCHAR *szFileName, ID3D11ShaderResourceView **pptex, ID3D11SamplerState **ppsam, ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	hr = DirectX::CreateDDSTextureFromFile(pd3dDevice, szFileName, NULL, pptex);
	if (FAILED(hr))
		return hr;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = 0;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pd3dDevice->CreateSamplerState(&sampDesc, ppsam);

	return hr;
}