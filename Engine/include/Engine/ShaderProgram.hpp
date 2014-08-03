#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include "Renderer.hpp"

namespace engine
{
	class DLLAPI ShaderProgram : public Object
	{
	private:
		ID3D11VertexShader* _pVertexShader;
		ID3D11GeometryShader* _pGeometryShader;
		ID3D11PixelShader* _pPixelShader;
		ID3DBlob* _pBlob;
	public:
		ShaderProgram(void);
		~ShaderProgram(void);
		HRESULT loadProgram(WCHAR *vs, WCHAR *gs, WCHAR *ps, ID3D11Device *pDevice);
		ID3D11VertexShader *getVertexShader(void);
		ID3D11GeometryShader *getGeometryShader(void);
		ID3D11PixelShader *getPixelShader(void);
		void *getEntryBufferPointer(void);
		SIZE_T getEntryBytecodeLength(void);
	};
}

#endif
