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
		EngineDevice _EngineDevice;
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
		Light(const EngineDevice &EngineDevice, ShaderProgram *program);
		~Light(void);
		void configShadowMap(const UINT &width, const UINT &height);
		XMMATRIX getProjectionMatrix(void) const;
		XMMATRIX getViewMatrix(void) const;
		XMMATRIX getVPMatrix(void) const;
		ShadowMap *getShadowMap(void);
		void clear(void) const;
		virtual void display(GBuffer *gbuf, Camera *cam) = 0;
	};
}

#endif