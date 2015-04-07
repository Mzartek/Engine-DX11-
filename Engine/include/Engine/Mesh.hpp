#ifndef MESH_HEADER
#define MESH_HEADER

#include "Object.hpp"

namespace Engine
{
	class Texture2D;
	class TextureCube;
	class Buffer;
	class Material;

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
	};

	class DLLAPI Mesh : public Object
	{
		friend struct CompareMesh;

	private:
		struct material
		{
			XMFLOAT4 ambient;
			XMFLOAT4 diffuse;
			XMFLOAT4 specular;
			FLOAT __declspec(align(16)) shininess;
		} _material;

		Texture2D *_colorTexture;
		Texture2D *_NMTexture;
		Buffer *_vertexBuffer;
		Buffer *_indexBuffer;
		Buffer *_materialBuffer;
		UINT _numElement;

		Material *_materia;
		ID3D11ShaderResourceView *_pshr[10];
		ID3D11SamplerState *_psam[10];
	public:
		Mesh(void);
		~Mesh(void);
		void loadColorTexture(const CHAR *path);
		void loadNMTexture(const CHAR *path);
		void setAmbient(const XMVECTOR &ambient);
		void setDiffuse(const XMVECTOR &diffuse);
		void setSpecular(const XMVECTOR &specular);
		void setShininess(const FLOAT &shininess);
		void setMaterial(Material *material);
		FLOAT getTransparency(void);
		void load(const UINT &numVertex, const Vertex *vertexArray,
			const UINT &numIndex, const UINT *indexArray);
		void display(void) const;
		void display(TextureCube *cubeTex) const;
		void displayShadow(void) const;
	};

	struct CompareMesh
	{
		bool operator() (const Mesh *first, const Mesh *second);
	};
}


#endif
