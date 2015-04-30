#include <Engine/Texture2D.hpp>
#include <FreeImage.h>

Engine::Texture2D::Texture2D(void)
{
}

Engine::Texture2D::~Texture2D(void)
{
}

Engine::TextureType Engine::Texture2D::getType(void) const
{
	return TEXTURE_2D_TYPE;
}

void Engine::Texture2D::loadFromFile(const CHAR *path)
{
	FIBITMAP *image;
	FIBITMAP *tmp;
	ID3D11Texture2D *texture;

	if (_pShaderResourceView) _pShaderResourceView->Release();
	if (_pSamplerState) _pSamplerState->Release();

	image = FreeImage_Load(FreeImage_GetFileType(path), path);
	if (image == NULL)
	{
		std::string text = "Error while loading image: ";
		text.append(path);
		MessageBoxA(NULL, text.c_str(), __FILE__, MB_OK);
		abort();
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
	Device->CreateTexture2D(&descTexture, NULL, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResourceView;
	descShaderResourceView.Format = descTexture.Format;
	descShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descShaderResourceView.Texture2D.MostDetailedMip = 0;
	descShaderResourceView.Texture2D.MipLevels = descTexture.MipLevels;
	Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView);

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
	Device->CreateSamplerState(&descSampler, &_pSamplerState);

	FreeImage_Unload(image);
	texture->Release();
}