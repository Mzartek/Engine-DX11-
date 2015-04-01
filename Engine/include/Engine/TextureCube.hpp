#ifndef TEXTURECUBE_HEADER
#define TEXTURECUBE_HEADER

#include "Texture.hpp"

namespace Engine
{
	class DLLAPI TextureCube : public Texture
	{
	public:
		TextureCube(void);
		~TextureCube(void);
		TextureType getType(void) const;

		void loadFromFiles(
			const CHAR *posx, const CHAR *negx,
			const CHAR *posy, const CHAR *negy,
			const CHAR *posz, const CHAR *negz);
	};
}

#endif