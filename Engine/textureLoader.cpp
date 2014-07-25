#include <Engine/DXHead.hpp>
#include <FreeImage.h>

HRESULT engine::loadTextureFromFile(const TCHAR *szFileName, ID3D11ShaderResourceView **ppshr, ID3D11SamplerState **ppsam, ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	FIBITMAP *image = NULL;
	FIBITMAP *tmp = NULL;

	image = FreeImage_Load(FreeImage_GetFileType(&szFileName[0]), &szFileName[0]);
	tmp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(tmp);

	D3D11_TEXTURE2D_DESC descTexture;
	descTexture.Width = FreeImage_GetWidth(image);
	descTexture.Height = FreeImage_GetHeight(image);
	descTexture.MipLevels = 1;
	descTexture.ArraySize = 1;
	descTexture.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	descTexture.SampleDesc.Count = 1;
	descTexture.SampleDesc.Quality = 0;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	descTexture.CPUAccessFlags = 0;
	descTexture.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = FreeImage_GetBits(image);
	data.SysMemPitch = 4 * descTexture.Width;
	data.SysMemSlicePitch = 4 * descTexture.Width * descTexture.Height;

	ID3D11Texture2D *tex;
	hr = pd3dDevice->CreateTexture2D(&descTexture, &data, &tex);
	FreeImage_Unload(image);
	if (FAILED(hr))
		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResourceView;
	descShaderResourceView.Format = descTexture.Format;
	descShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descShaderResourceView.Texture2D.MostDetailedMip = 0;
	descShaderResourceView.Texture2D.MipLevels = 1;
	hr = pd3dDevice->CreateShaderResourceView(tex, &descShaderResourceView, ppshr);
	if (FAILED(hr))
		return hr;

	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 1;
	descSampler.BorderColor[0] = 0.0f;
	descSampler.BorderColor[1] = 0.0f;
	descSampler.BorderColor[2] = 0.0f;
	descSampler.BorderColor[3] = 0.0f;
	descSampler.MinLOD = 0;
	descSampler.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pd3dDevice->CreateSamplerState(&descSampler, ppsam);

	return hr;
}