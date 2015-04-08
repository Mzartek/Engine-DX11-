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
		Buffer *_vertexBuffer;
		Buffer *_indexBuffer;
		UINT _numElement;

		Material *_material;
		ID3D11ShaderResourceView *_pshr[10];
		ID3D11SamplerState *_psam[10];
	public:
		Mesh(void);
		~Mesh(void);
		void setMaterial(Material *material);
		Material *getMaterial(void) const;
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
