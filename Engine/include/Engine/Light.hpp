#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include "Object.hpp"

namespace Engine
{
	class Buffer;
	class DepthMap;
	class ShaderProgram;
	class GBuffer;
	class Camera;

	class DLLAPI Light : public Object
	{
	protected:
		struct
		{
			XMMATRIX IVPMatrix;
			XMUINT2 __declspec(align(16)) screen;
			XMVECTOR __declspec(align(16)) camPosition;
			INT withShadowMapping;
		} _mainInfo;

		Buffer *_vertexBuffer;
		Buffer *_mainInfoBuffer;
		Buffer *_lightInfoBuffer;
		ShaderProgram *_program;
		ID3D11InputLayout *_pInputLayout;
		XMMATRIX *_projectionMatrix;
		XMMATRIX *_viewMatrix;
		XMMATRIX *_VPMatrix;
	public:
		Light(ShaderProgram *program);
		~Light(void);
		virtual void display(GBuffer *gbuf, Camera *cam) = 0;
		virtual void display(GBuffer *gbuf, DepthMap *dmap, Camera *cam) = 0;
	};
}

#endif