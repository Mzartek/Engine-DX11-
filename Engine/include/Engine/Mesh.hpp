#ifndef MESH
#define MESH

#include "Object.hpp"

namespace Engine
{
	class Texture;
	class Buffer;

	class DLLAPI Mesh : public Object
	{
		friend int comparMesh(const void *p1, const void *p2);

	private:
		Texture *_colorTexture;
		Texture *_NMTexture;
		Buffer *_vertexBuffer;
		Buffer *_indexBuffer;
		Buffer *_materialBuffer;
		struct material
		{
			XMFLOAT4 ambient;
			XMFLOAT4 diffuse;
			XMFLOAT4 specular;
			FLOAT __declspec(align(16)) shininess;
		} _material;
		UINT _numElement;
	public:
		Mesh(void);
		~Mesh(void);
		void setColorTexture(const CHAR *path);
		void setNMTexture(const CHAR *path);
		void setAmbient(const XMFLOAT4 &ambient);
		void setDiffuse(const XMFLOAT4 &diffuse);
		void setSpecular(const XMFLOAT4 &specular);
		void setShininess(const FLOAT &shininess);
		FLOAT getTransparency(void);
		void load(const UINT &sizeVertexArray, const FLOAT *vertexArray,
			const UINT &sizeIndexArray, const UINT *indexArray);
		void display(void) const;
		void displayShadow(void) const;
	};

	int comparMesh(const void *p1, const void *p2);
}


#endif
