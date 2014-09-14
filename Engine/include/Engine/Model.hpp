#ifndef MODEL
#define MODEL

#include "Object.hpp"

namespace engine
{
	class Mesh;
	class ShaderProgram;
	class GBuffer;
	class Camera;
	class Light;

	class DLLAPI Model : public Object
	{
	private:
		BOOL isMirror;
		std::vector<Mesh *> *_tMesh;
		ID3D11Buffer *_pMVPMatrixBuffer;
		ID3D11Buffer *_pNormalMatrixBuffer;
		ID3D11InputLayout *_pInputLayout;
		XMMATRIX *_ModelMatrix;
		// ShaderProgram
		ShaderProgram *_gProgram;
		ShaderProgram *_smProgram;
		// Device
		ID3D11Device *_pd3dDevice;
		ID3D11DeviceContext *_pContext;
	public:
		Model();
		~Model();
		void initMeshArray(void);
		void initMeshMirror(Model *m);
		void config(ShaderProgram *gProgram, ShaderProgram *smProgram, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		void createMesh(const UINT &sizeVertexArray, const FLOAT *vertexArray,
				  const UINT &sizeIndexArray, const UINT *indexArray,
				  const TCHAR *colorTexture, const TCHAR *NMTexture,
				  const XMFLOAT4 &ambient, const XMFLOAT4 &diffuse, const XMFLOAT4 &specular, const FLOAT &shininess);
		void loadFromFile(const TCHAR *szFileName);
		void sortMesh(void);
		void matIdentity(void);
		void matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matRotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		XMFLOAT3 getPosition(void) const;
		Mesh *getMesh(UINT num) const;
		void display(GBuffer *gbuf, Camera *cam) const;
		void displayTransparent(GBuffer *gbuf, Camera *cam) const;
		void displayShadowMap(Light *light) const;
	};
}
    

#endif
