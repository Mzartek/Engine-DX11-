#ifndef SKYBOX
#define SKYBOX

#include "Object.hpp"

namespace engine
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
		ID3D11InputLayout *_pInputLayout;
		Buffer *_vertexBuffer;
		Buffer *_indexBuffer;
		Buffer *_MVPMatrixBuffer;
		XMMATRIX *_rotateMatrix;
		ShaderProgram *_program;
		UINT _numElement;
	public:
		SkyBox(void);
		~SkyBox(void);
		void load(const TCHAR *posx, const TCHAR *negx,
			const TCHAR *posy, const TCHAR *negy,
			const TCHAR *posz, const TCHAR *negz,
			FLOAT dim, ShaderProgram *program);
		void rotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void display(GBuffer *gbuf, Camera *cam);
	};
}


#endif
