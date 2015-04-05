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
	class TextureCube;
	class DepthMap;

	class DLLAPI Model : public Object
	{
	private:
		struct
		{
			XMMATRIX MVP;
			XMMATRIX projection;
			XMMATRIX view;
			XMMATRIX model;
			XMMATRIX normal;
		} _matrix;

		BOOL _isMirror;
		std::vector<Mesh *> *_tMesh;
		Buffer *_matrixBuffer;
		XMFLOAT3 *_position;
		XMFLOAT3 *_rotation;
		XMFLOAT3 *_scale;
		XMMATRIX *_modelMatrix;
		XMMATRIX *_normalMatrix;
		BOOL _needMatModel;
		BOOL _needMatNormal;
		TextureCube *_cubeTexture;
		ShaderProgram *_gProgram;
		ShaderProgram *_smProgram;
		ID3D11InputLayout *_pInputLayout;
		void genMatModel(void) const;
		void genMatNormal(void) const;
		void checkMatrix(void);
		void deleteMesh(void);
	public:
		Model(ShaderProgram *gProgram, ShaderProgram *smProgram);
		Model(Model *model, ShaderProgram *gProgram, ShaderProgram *smProgram);
		~Model(void);
		void addMesh(const UINT &numVertex, const Vertex *vertexArray,
				  const UINT &numIndex, const UINT *indexArray,
				  const CHAR *colorTexture, const CHAR *NMTexture,
				  const XMVECTOR &ambient, const XMVECTOR &diffuse, const XMVECTOR &specular, const FLOAT &shininess);
		void loadFromFile(const CHAR *szFileName, const CHAR *defaultTex, const CHAR *defaultNM);
		void sortMesh(void);
		void setPosition(const XMVECTOR &position);
		void setRotation(const XMVECTOR &rotation);
		void setScale(const XMVECTOR &scale);
		void setCubeTexture(TextureCube *cubeTex);
		XMVECTOR getPosition(void) const;
		XMVECTOR getRotation(void) const;
		XMVECTOR getScale(void) const;
		std::vector<Mesh *> getMeshVector(void) const;
		void display(GBuffer *gbuf, Camera *cam);
		void displayTransparent(GBuffer *gbuf, Camera *cam);
		void displayDepthMap(DepthMap *dmap, Camera *cam);
		void displayDepthMap(DepthMap *dmaps, DirLight *light);
		void displayDepthMap(DepthMap *dmap, SpotLight *light);
	};
}
    

#endif
