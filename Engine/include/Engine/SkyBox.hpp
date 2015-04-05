#ifndef SKYBOX_HEADER
#define SKYBOX_HEADER

#define SKYBOX_DIM 10

#include "Object.hpp"

namespace Engine
{
	class TextureCube;
	class Buffer;
	class ShaderProgram;
	class GBuffer;
	class PerspCamera;

	class DLLAPI SkyBox : public Object
	{
	private:
		TextureCube *_cubeTexture;
		Buffer *_vertexBuffer;
		Buffer *_indexBuffer;
		Buffer *_MVPMatrixBuffer;
		ShaderProgram *_program;
		ID3D11InputLayout *_pInputLayout;
		UINT _numElement;
	public:
		SkyBox(ShaderProgram *program);
		~SkyBox(void);
		void load(const CHAR *posx, const CHAR *negx,
			const CHAR *posy, const CHAR *negy,
			const CHAR *posz, const CHAR *negz);
		TextureCube *getTexture(void) const;
		void display(GBuffer *gbuf, PerspCamera *cam);
	};
}


#endif
