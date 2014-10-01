#ifndef MODEL
#define MODEL

#include "Object.hpp"

namespace Engine
{
	class Mesh;
	class Buffer;
	class ShaderProgram;
	class GBuffer;
	class Camera;
	class Light;

	class DLLAPI Model : public Object
	{
	private:
		BOOL isMirror;
		std::vector<Mesh *> *_tMesh;
		Buffer *_MVPMatrixBuffer;
		Buffer *_normalMatrixBuffer;
		XMMATRIX *_ModelMatrix;
		// ShaderProgram
		ShaderProgram *_gProgram;
		ShaderProgram *_smProgram;
		ID3D11InputLayout *_pInputLayout;
	public:
		Model(ShaderProgram *gProgram, ShaderProgram *smProgram);
		~Model(void);
		void initMeshArray(void);
		void initMeshMirror(Model *m);
		void addMesh(const UINT &sizeVertexArray, const FLOAT *vertexArray,
				  const UINT &sizeIndexArray, const UINT *indexArray,
				  const CHAR *colorTexture, const CHAR *NMTexture,
				  const XMFLOAT4 &ambient, const XMFLOAT4 &diffuse, const XMFLOAT4 &specular, const FLOAT &shininess);
		void loadFromFile(const CHAR *szFileName);
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
