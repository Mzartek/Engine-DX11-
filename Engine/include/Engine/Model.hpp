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
		struct matrix
		{
			DirectX::XMMATRIX MVP;
			DirectX::XMMATRIX modelMatrix;
			DirectX::XMMATRIX normalMatrix;
		};
		BOOL isMirror;
		std::vector<D3DObject *> *_tD3DObject;
		ID3D11Buffer *_pMatrixBuffer;
		struct matrix *_matrix;
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
				  ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		HRESULT loadFromFile(const TCHAR *szFileName, 
			ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
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
