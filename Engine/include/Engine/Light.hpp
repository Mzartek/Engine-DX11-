#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include "Object.hpp"

namespace Engine
{
	class Buffer;
	class ShadowMap;
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
		} _mainInfo;

		Buffer *_vertexBuffer;
		Buffer *_mainInfoBuffer;
		Buffer *_lightInfoBuffer;
		ShaderProgram *_program;
		ID3D11InputLayout *_pInputLayout;
		ShadowMap *_shadow;
		XMMATRIX *_projectionMatrix;
		XMMATRIX *_viewMatrix;
		XMMATRIX *_VPMatrix;
	public:
		Light(ShaderProgram *program);
		~Light(void);
		virtual void configShadowMap(const UINT &width, const UINT &height) const = 0;
		virtual void clear(void) const = 0;
		virtual void display(GBuffer *gbuf, Camera *cam) = 0;
	};
}

#endif