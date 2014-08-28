#ifndef MODEL
#define MODEL

#include "D3DObject.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Light.hpp"

namespace engine
{
	class DLLAPI Model : public Object
	{
	private:
		BOOL isMirror;
		std::vector<D3DObject *> *_tD3DObject;
		ID3D11Buffer *_pMVPMatrixBuffer;
		ID3D11Buffer *_pNormalMatrixBuffer;
		ID3D11InputLayout *_pInputLayout;
		XMMATRIX *_ModelMatrix;
		// Do not delete it
		ShaderProgram *_program;
		ID3D11Device *_pd3dDevice;
		ID3D11DeviceContext *_pContext;
	public:
		Model();
		~Model();
		void initD3DObjectArray(void);
		void initD3DObjectMirror(Model *m);
		void config(ShaderProgram *program, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		void createObject(const UINT &sizeVertexArray, const FLOAT *vertexArray,
				  const UINT &sizeIndexArray, const UINT *indexArray,
				  const TCHAR *pathTexture,
				  const XMFLOAT4 &ambient, const XMFLOAT4 &diffuse, const XMFLOAT4 &specular, const FLOAT &shininess);
		void loadFromFile(const TCHAR *szFileName);
		void sortD3DObject(void);
		void matIdentity(void);
		void matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matRotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		XMFLOAT3 getPosition(void) const;
		D3DObject *getD3DObject(UINT num) const;
		void display(GBuffer *g, Camera *cam) const;
		void displayShadow(Light *l) const;
	};
}
    

#endif
