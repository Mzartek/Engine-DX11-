#ifndef MODEL_HEADER
#define MODEL_HEADER

#include "Object.hpp"

namespace Engine
{
	struct Vertex;
	class Mesh;
	class Buffer;
	class ShaderProgram;
	class GBuffer;
	class Camera;
	class DirLight;
	class SpotLight;

	class DLLAPI Model : public Object
	{
	private:
		BOOL isMirror;
		std::vector<Mesh *> *_tMesh;
		Buffer *_matrixBuffer;
		Buffer *_cameraBuffer;
		XMMATRIX *_modelMatrix;
		XMMATRIX *_normalMatrix;
		// ShaderProgram
		ShaderProgram *_gProgram;
		ShaderProgram *_smProgram;
		ID3D11InputLayout *_pInputLayout;
	public:
		Model(ShaderProgram *gProgram, ShaderProgram *smProgram);
		~Model(void);
		void initMeshArray(void);
		void initMeshMirror(Model *m);
		void addMesh(const UINT &numVertex, const Vertex *vertexArray,
				  const UINT &numIndex, const UINT *indexArray,
				  const CHAR *colorTexture, const CHAR *NMTexture,
				  const XMVECTOR &ambient, const XMVECTOR &diffuse, const XMVECTOR &specular, const FLOAT &shininess);
		void loadFromFile(const CHAR *szFileName);
		void sortMesh(void);
		void matIdentity(void);
		void matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matRotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void genMatNormal(void);
		XMVECTOR getPosition(void) const;
		Mesh *getMesh(UINT num) const;
		void display(GBuffer *gbuf, Camera *cam);
		void displayTransparent(GBuffer *gbuf, Camera *cam);
		void displayShadowMap(DirLight *light);
		void displayShadowMap(SpotLight *light);
	};
}
    

#endif
