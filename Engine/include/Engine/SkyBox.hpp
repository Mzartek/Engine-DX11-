#ifndef SKYBOX_HEADER
#define SKYBOX_HEADER

#define SKYBOX_DIM 10

#include "Object.hpp"

namespace Engine
{
	class Texture;
	class Buffer;
	class ShaderProgram;
	class GBuffer;
	class Camera;

	class DLLAPI SkyBox : public Object
	{
	private:
		Texture *_cubeTexture;
		Buffer *_vertexBuffer;
		Buffer *_indexBuffer;
		Buffer *_MVPMatrixBuffer;
		XMMATRIX *_rotateMatrix;
		ShaderProgram *_program;
		ID3D11InputLayout *_pInputLayout;
		UINT _numElement;
	public:
		SkyBox(ShaderProgram *program);
		~SkyBox(void);
		void load(const CHAR *posx, const CHAR *negx,
			const CHAR *posy, const CHAR *negy,
			const CHAR *posz, const CHAR *negz);
		void rotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void display(GBuffer *gbuf, Camera *cam);
	};
}


#endif
