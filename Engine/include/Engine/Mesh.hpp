#ifndef MESH
#define MESH

#include "Object.hpp"

namespace engine
{
	class DLLAPI Mesh : public Object
	{
		friend int comparMesh(const void *p1, const void *p2);
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
		explicit Mesh(ID3D11Device *pd3dDevice);
		~Mesh(void);
		void setColorTexture(ID3D11Texture2D *ptex, ID3D11ShaderResourceView *pShaderResourceView, ID3D11SamplerState *pSamplerState);
		void setNMTexture(ID3D11Texture2D *ptex, ID3D11ShaderResourceView *pShaderResourceView);
		void setAmbient(const XMFLOAT4 &ambient);
		void setDiffuse(const XMFLOAT4 &diffuse);
		void setSpecular(const XMFLOAT4 &specular);
		void setShininess(const FLOAT &shininess);
		FLOAT getTransparency(void);
		void load(const UINT &sizeVertexArray, const FLOAT *vertexArray,
			const UINT &sizeIndexArray, const UINT *indexArray,
			ID3D11Device *pd3dDevice);
		void display(ID3D11DeviceContext *pContext) const;
		void displayShadow(ID3D11DeviceContext *pContext) const;
	};
  
	int comparMesh(const void *p1, const void *p2);
  
}


#endif
