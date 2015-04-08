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
	class PerspCamera;
	class DirLight;
	class SpotLight;
	class Texture2D;
	class TextureCube;
	class DepthMap;
	class Material;

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

		struct
		{
			XMVECTOR position;
			XMVECTOR target;
		} _camera;

		BOOL _isMirror;

		std::vector<Mesh *> *_tMesh;
		std::vector<Object *> *_tObject;

		Buffer *_matrixBuffer;
		Buffer *_cameraBuffer;

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
	public:
		Model(ShaderProgram *gProgram, ShaderProgram *smProgram);
		Model(Model *model, ShaderProgram *gProgram, ShaderProgram *smProgram);
		~Model(void);
		void addMesh(Mesh *mesh);
		void loadFromFile(const CHAR *szFileName);
		void sortMesh(void);
		void setPosition(const XMVECTOR &position);
		void setRotation(const XMVECTOR &rotation);
		void setScale(const XMVECTOR &scale);
		void setCubeTexture(TextureCube *cubeTex);
		XMVECTOR getPosition(void) const;
		XMVECTOR getRotation(void) const;
		XMVECTOR getScale(void) const;
		std::vector<Mesh *> getMeshVector(void) const;
		void display(GBuffer *gbuf, PerspCamera *cam);
		void displayTransparent(GBuffer *gbuf, PerspCamera *cam);
		void displayDepthMap(DepthMap *dmap, Camera *cam);
		void displayDepthMap(DepthMap *dmaps, DirLight *light);
		void displayDepthMap(DepthMap *dmap, SpotLight *light);
	};
}
    

#endif
