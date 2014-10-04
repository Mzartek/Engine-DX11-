#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include "Renderer.hpp"

namespace Engine
{
	class DLLAPI ShaderProgram : public Object
	{
	private:
		ID3D11VertexShader *_pVertexShader;
		ID3D11HullShader *_pHullShader;
		ID3D11DomainShader *_pDomainShader;
		ID3D11GeometryShader *_pGeometryShader;
		ID3D11PixelShader *_pPixelShader;
		ID3DBlob *_pBlob;
	public:
		ShaderProgram(const EngineDevice &EngineDevice, LPCWSTR vs, LPCWSTR hs, LPCWSTR ds, LPCWSTR gs, LPCWSTR ps);
		~ShaderProgram(void);
		ID3D11VertexShader *getVertexShader(void) const;
		ID3D11HullShader *getHullShader(void) const;
		ID3D11DomainShader *getDomainShader(void) const;
		ID3D11GeometryShader *getGeometryShader(void) const;
		ID3D11PixelShader *getPixelShader(void) const;
		void *getEntryBufferPointer(void);
		SIZE_T getEntryBytecodeLength(void);
	};
}

#endif
