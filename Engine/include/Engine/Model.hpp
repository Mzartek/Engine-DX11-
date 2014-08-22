#ifndef MODEL
#define MODEL

#include "D3DObject.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"

namespace engine
{
	class DLLAPI Model : public Object
	{
	private:
		BOOL isMirror;
		std::vector<D3DObject *> *_tD3DObject;
		ID3D11Buffer *_pMVPMatrixBuffer;
		ID3D11Buffer *_pModelMatrixBuffer;
		ID3D11Buffer *_pNormalMatrixBuffer;
		DirectX::XMMATRIX *_MVPMatrix;
		DirectX::XMMATRIX *_ModelMatrix;
		DirectX::XMMATRIX *_NormalMatrix;
		// Do not delete it
		ShaderProgram *_program;
		ID3D11Device *_pd3dDevice;
		ID3D11DeviceContext *_pContext;
	public:
		Model();
		~Model();
		void initD3DObjectArray(void);
		void initD3DObjectMirror(Model *m);
		HRESULT config(ShaderProgram *program, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		HRESULT createObject(const UINT &sizeVertexArray, const FLOAT *vertexArray,
				  const UINT &sizeIndexArray, const UINT *indexArray,
				  const TCHAR *pathTexture,
				  const DirectX::XMFLOAT4 &ambient, const DirectX::XMFLOAT4 &diffuse, const DirectX::XMFLOAT4 &specular, const FLOAT &shininess);
		HRESULT loadFromFile(const TCHAR *szFileName);
		void sortD3DObject(void);
		void matIdentity(void);
		void matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matRotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		DirectX::XMFLOAT3 getPosition(void) const;
		D3DObject *getD3DObject(UINT num) const;
		void display(GBuffer *g, Camera *cam);
	};
}
    

#endif
