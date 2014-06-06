#ifndef SHADER_PROGRAM
#define SHADER_PROGRAM

#include "DXHead.hpp"
#include "Window.hpp"

namespace engine
{
	class DLLAPI ShaderProgram
	{
	private:
		ID3D11VertexShader* _pVertexShader;
		ID3D11PixelShader* _pPixelShader;
		ID3DBlob *_pVSBlob;
		ID3DBlob *_pPSBlob;
		HRESULT CompileShaderFromFile(WCHAR *szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob **ppBlobOut);
	public:
		ShaderProgram(void);
		~ShaderProgram(void);
		HRESULT loadProgram(WCHAR *vs, WCHAR *ps, ID3D11Device *pDevice);
		ID3D11VertexShader *getVertexShader(void);
		ID3D11PixelShader *getPixelShader(void);
		ID3DBlob *getVSBlob(void);
		ID3DBlob *getPSBlob(void);
	};
}

#endif
