#ifndef TEXTURE2D_HEADER
#define TEXTURE2D_HEADER

#include "Texture.hpp"

namespace Engine
{
	class DLLAPI Texture2D : public Texture
	{
	public:
		Texture2D(void);
		~Texture2D(void);
		TextureType getType(void) const;

		void loadFromFile(const CHAR *path);
	};
}

#endif