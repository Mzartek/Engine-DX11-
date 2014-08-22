#ifndef D3D_OBJECT
#define D3D_OBJECT

#include "ShaderProgram.hpp"
#include "GBuffer.hpp"

namespace engine
{
	class DLLAPI D3DObject : public Object
	{
		friend int comparD3DObject(const void *p1, const void *p2);
	private:
		ID3D11Texture2D *_pTex;
		ID3D11ShaderResourceView *_pShaderResourceView;
		ID3D11SamplerState *_pSamplerState;
		ID3D11Buffer *_pMaterialBuffer;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11Buffer *_pIndexBuffer;
		ID3D11InputLayout *_pInputLayout;
		struct material
		{
			DirectX::XMFLOAT4 ambient;
			DirectX::XMFLOAT4 diffuse;
			DirectX::XMFLOAT4 specular;
			FLOAT shininess;
		} _material;
		UINT _numElement;
		// Do not delete it
		ShaderProgram *_gProgram;
		ID3D11Device *_pd3dDevice;
		ID3D11DeviceContext *_pContext;
	public:
		D3DObject(void);
		~D3DObject(void);
		HRESULT config(ShaderProgram *program, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		void setTexture(ID3D11Texture2D *ptex, ID3D11ShaderResourceView *pShaderResourceView, ID3D11SamplerState *pSamplerState);
		void setAmbient(const DirectX::XMFLOAT4 &ambient);
		void setDiffuse(const DirectX::XMFLOAT4 &diffuse);
		void setSpecular(const DirectX::XMFLOAT4 &specular);
		void setShininess(const FLOAT &shininess);
		FLOAT getTransparency(void);
		HRESULT load(const UINT &sizeVertexArray, const FLOAT *vertexArray,
			const UINT &sizeIndexArray, const UINT *indexArray);
		void display(GBuffer *g) const;
	};
  
	int comparD3DObject(const void *p1, const void *p2);
  
}


#endif
