#ifndef OBJECT
#define OBJECT

#include "DXHead.hpp"
#include "ShaderProgram.hpp"
//#include "Light.hpp"
//#include "GBuffer.hpp"

namespace engine
{
	class DLLAPI Object
	{
		friend int comparObject(const void *p1, const void *p2);
	private:
		struct uniform
		{
			FLOAT ambient[4];
			FLOAT diffuse[4];
			FLOAT specular[4];
		};

		ID3D11ShaderResourceView *_pTexture;
		ID3D11SamplerState *_pSampler;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11Buffer *_pIndexBuffer;
		ID3D11InputLayout *_pVertexLayout;
		ID3D11Buffer *_pConstantBuffer;
		ShaderProgram *_program;
		UINT _numElement;
		struct uniform _material;
		FLOAT shininess[1];
	public:
		Object(void);
		~Object(void);
		HRESULT setShaderProgram(ShaderProgram *program, ID3D11Device *pd3dDevice);
		void setTexture(ID3D11ShaderResourceView *pTexture, ID3D11SamplerState *pSampler);
		void setAmbient(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w);
		void setDiffuse(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w);
		void setSpecular(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w);
		void setShininess(const FLOAT &x);
		FLOAT getTransparency(void);
		HRESULT load(const UINT &sizeVertexArray, const FLOAT *vertexArray,
			const UINT &sizeIndexArray, const UINT *indexArray,
			ID3D11Device *pd3dDevice);
		void display(Window *win) const;//LBuffer *l) const;
		//void displayOnGBuffer(GBuffer *g) const;
		//void displayShadow(Light *l) const;
	};
  
	int comparObject(const void *p1, const void *p2);
  
}


#endif
