#ifndef D3D_OBJECT
#define D3D_OBJECT

#include "ShaderProgram.hpp"

namespace engine
{
	class DLLAPI D3DObject : public Object
	{
		friend int comparD3DObject(const void *p1, const void *p2);
	private:
		struct uniform
		{
			FLOAT ambient[4];
			FLOAT diffuse[4];
			FLOAT specular[4];
		};

		ID3D11Resource *_pTexture;
		ID3D11ShaderResourceView *_pShaderResource;
		ID3D11SamplerState *_pSamplerState;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11Buffer *_pIndexBuffer;
		ID3D11InputLayout *_pVertexLayout;
		ID3D11Buffer *_pConstantBuffer;
		struct uniform *_material;
		ShaderProgram *_program;
		UINT _numElement;
		FLOAT shininess[1];
	public:
		D3DObject(void);
		~D3DObject(void);
		HRESULT setShaderProgram(ShaderProgram *program, ID3D11Device *pd3dDevice);
		void setTexture(ID3D11ShaderResourceView *pShaderResource, ID3D11SamplerState *pSamplerState);
		void setAmbient(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w);
		void setDiffuse(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w);
		void setSpecular(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w);
		void setShininess(const FLOAT &x);
		FLOAT getTransparency(void);
		HRESULT load(const UINT &sizeVertexArray, const FLOAT *vertexArray,
			const UINT &sizeIndexArray, const UINT *indexArray,
			ID3D11Device *pd3dDevice);
		void display(Window *win) const;
		//void displayOnGBuffer(GBuffer *g) const;
		//void displayShadow(Light *l) const;
	};
  
	int comparD3DObject(const void *p1, const void *p2);
  
}


#endif
