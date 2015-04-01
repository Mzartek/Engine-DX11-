#include <Engine/TextureCube.hpp>
#include <FreeImage.h>

Engine::TextureCube::TextureCube(void)
{
}

Engine::TextureCube::~TextureCube(void)
{
}

Engine::TextureType Engine::TextureCube::getType(void) const
{
	return TEXTURE_CUBE_TYPE;
}

void Engine::TextureCube::loadFromFiles(
	const CHAR *posx, const CHAR *negx,
	const CHAR *posy, const CHAR *negy,
	const CHAR *posz, const CHAR *negz)
{
	UINT i;
	FIBITMAP *image[6];
	FIBITMAP *tmp;
	ID3D11Texture2D *texture;

	const CHAR *tex[] = { posx, negx, negy, posy, posz, negz };
	image[0] = FreeImage_Load(FreeImage_GetFileType(tex[0]), tex[0]);
	image[1] = FreeImage_Load(FreeImage_GetFileType(tex[1]), tex[1]);
	image[2] = FreeImage_Load(FreeImage_GetFileType(tex[2]), tex[2]);
	image[3] = FreeImage_Load(FreeImage_GetFileType(tex[3]), tex[3]);
	image[4] = FreeImage_Load(FreeImage_GetFileType(tex[4]), tex[4]);
	image[5] = FreeImage_Load(FreeImage_GetFileType(tex[5]), tex[5]);

	if (_pShaderResourceView) _pShaderResourceView->Release();
	if (_pSamplerState) _pSamplerState->Release();

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
		if (image[i] == NULL)
		{
			std::string text = "Error while loading image: ";
			text.append(tex[i]);
			MessageBoxA(NULL, text.c_str(), __FILE__, MB_OK);
			exit(1);
		}

		tmp = image[i];
		image[i] = FreeImage_ConvertTo32Bits(image[i]);
		FreeImage_Unload(tmp);

		data[i].pSysMem = FreeImage_GetBits(image[i]);
		data[i].SysMemPitch = 4 * descTexture.Width;
		data[i].SysMemSlicePitch = 0;
	}
	Device->CreateTexture2D(&descTexture, data, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResourceView;
	descShaderResourceView.Format = descTexture.Format;
	descShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	descShaderResourceView.TextureCube.MostDetailedMip = 0;
	descShaderResourceView.TextureCube.MipLevels = descTexture.MipLevels;
	Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView);

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
	Device->CreateSamplerState(&descSampler, &_pSamplerState);

	texture->Release();
}