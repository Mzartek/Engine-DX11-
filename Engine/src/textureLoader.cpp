#include <Engine/EngineHead.hpp>
#include <FreeImage.h>

void engine::loadTextureFromFile(const TCHAR *szFileName, 
	ID3D11Texture2D **pptex, ID3D11ShaderResourceView **ppshr, ID3D11SamplerState **ppsam,
	ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	HRESULT hr;
	FIBITMAP *image = NULL;
	FIBITMAP *tmp = NULL;

	image = FreeImage_Load(FreeImage_GetFileType(szFileName), szFileName);
	if (image == NULL)
	{
		std::string text = "Fail to load file: ";
		text.append(szFileName);
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

	hr = pd3dDevice->CreateTexture2D(&descTexture, NULL, pptex);
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
	hr = pd3dDevice->CreateShaderResourceView(*pptex, &descShaderResourceView, ppshr);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Error while creating the ShaderResourceView", "Texture", MB_OK);
		exit(1);
	}

	pContext->UpdateSubresource(*pptex, 0, NULL, FreeImage_GetBits(image), 4 * descTexture.Width, 4 * descTexture.Width * descTexture.Height);
	pContext->GenerateMips(*ppshr);

	if (ppsam != NULL)
	{
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
		hr = pd3dDevice->CreateSamplerState(&descSampler, ppsam);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Error while creating the SamplerState", "Texture", MB_OK);
			exit(1);
		}
	}

	FreeImage_Unload(image);
}