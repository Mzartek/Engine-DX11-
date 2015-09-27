#pragma once

#include "GraphicsRenderer.h"

namespace Engine
{
	namespace Graphics
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

			void compiledShaderFromFile(const LPCWSTR &szFileName, const std::string &szEntryPoint, const std::string &szShaderModel, ID3DBlob **ppBlobOut);

		public:
			ShaderProgram(const LPCWSTR &vs, const LPCWSTR &hs, const LPCWSTR &ds, const LPCWSTR &gs, const LPCWSTR &ps);
			ShaderProgram(const LPCWSTR &vs, const LPCWSTR &hs, const LPCWSTR &ds, const LPCWSTR &gs, const LPCWSTR &ps, const D3D11_SO_DECLARATION_ENTRY *pDecl, const UINT &count);
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
}

