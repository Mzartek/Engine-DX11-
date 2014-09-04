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
		ID3D11Texture2D *_pColorTex;
		ID3D11Texture2D *_pNMTex;
		ID3D11ShaderResourceView *_pColorTexSHR;
		ID3D11ShaderResourceView *_pNMTexSHR;
		ID3D11SamplerState *_pTexSamplerState;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11Buffer *_pIndexBuffer;
		ID3D11Buffer *_pMaterialBuffer;
		struct material
		{
			XMFLOAT4 ambient;
			XMFLOAT4 diffuse;
			XMFLOAT4 specular;
			FLOAT __declspec(align(16)) shininess;
		} _material;
		UINT _numElement;
	public:
		D3DObject(ID3D11Device *pd3dDevice);
		~D3DObject(void);
		void setColorTexture(ID3D11Texture2D *ptex, ID3D11ShaderResourceView *pShaderResourceView, ID3D11SamplerState *pSamplerState);
		void setNMTexture(ID3D11Texture2D *ptex, ID3D11ShaderResourceView *pShaderResourceView);
		void setAmbient(const XMFLOAT4 &ambient, ID3D11DeviceContext *pContext);
		void setDiffuse(const XMFLOAT4 &diffuse, ID3D11DeviceContext *pContext);
		void setSpecular(const XMFLOAT4 &specular, ID3D11DeviceContext *pContext);
		void setShininess(const FLOAT &shininess, ID3D11DeviceContext *pContext);
		FLOAT getTransparency(void);
		void load(const UINT &sizeVertexArray, const FLOAT *vertexArray,
			const UINT &sizeIndexArray, const UINT *indexArray,
			ID3D11Device *pd3dDevice);
		void display(ID3D11DeviceContext *pContext) const;
		void displayShadow(ID3D11DeviceContext *pContext) const;
	};
  
	int comparD3DObject(const void *p1, const void *p2);
  
}


#endif
