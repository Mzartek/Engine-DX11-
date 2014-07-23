#ifndef MODEL
#define MODEL

#include "D3DObject.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Window.hpp"

namespace engine
{
	class DLLAPI Model : public Object
	{
	private:
		struct uniform0
		{
			DirectX::XMMATRIX MVP;
			DirectX::XMMATRIX modelMatrix;
			DirectX::XMMATRIX normalMatrix;
		};
		struct uniform1
		{
			FLOAT screen[2];
		};
		BOOL isMirror;
		std::vector<D3DObject *> *_tObject;
		ID3D11Buffer *_pConstantBuffer0;
		ID3D11Buffer *_pConstantBuffer1;
		struct uniform0 *_matrix;
		struct uniform1 *_screen;
		ShaderProgram *_program;
	public:
		Model();
		~Model();
		void initObjectArray(void);
		void initObjectMirror(Model *m);
		HRESULT config(ShaderProgram *program, ID3D11Device *pd3dDevice);
		HRESULT createObject(const UINT &sizeVertexArray, const FLOAT *vertexArray,
				  const UINT &sizeIndexArray, const UINT *indexArray,
				  const TCHAR *pathTexture,
				  const FLOAT *ambient, const FLOAT *diffuse, const FLOAT *specular, const FLOAT *shininess,
				  ID3D11Device *pd3dDevice);
		HRESULT loadFromFile(const TCHAR *szFileName, ID3D11Device *pd3dDevice);
		void sortObject(void);
		void matIdentity(void);
		void matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matRotate(const FLOAT &angle, const BOOL &x, const BOOL &y, const BOOL &z);
		void matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		DirectX::XMFLOAT3 getPosition(void) const;
		D3DObject *getD3DObject(UINT num) const;
		void display(GBuffer *g, Camera *cam);
	};
}
    

#endif
