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
		struct material
		{
			FLOAT ambient[4];
			FLOAT diffuse[4];
			FLOAT specular[4];
			FLOAT shininess[1];
		};

		ID3D11Texture2D *_pTex;
		ID3D11ShaderResourceView *_pShaderResourceView;
		ID3D11SamplerState *_pSamplerState;
		ID3D11Buffer *_pMaterialBuffer;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11Buffer *_pIndexBuffer;
		ID3D11InputLayout *_pInputLayout;
		struct material *_material;
		ShaderProgram *_program;
		UINT _numElement;
	public:
		D3DObject(void);
		~D3DObject(void);
		HRESULT config(ShaderProgram *program, ID3D11Device *pd3dDevice);
		void setTexture(ID3D11Texture2D *ptex, ID3D11ShaderResourceView *pShaderResourceView, ID3D11SamplerState *pSamplerState);
		void setAmbient(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w);
		void setDiffuse(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w);
		void setSpecular(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w);
		void setShininess(const FLOAT &x);
		FLOAT getTransparency(void);
		HRESULT load(const UINT &sizeVertexArray, const FLOAT *vertexArray,
			const UINT &sizeIndexArray, const UINT *indexArray,
			ID3D11Device *pd3dDevice);
		void display(GBuffer *g) const;
	};
  
	int comparD3DObject(const void *p1, const void *p2);
  
}


#endif
