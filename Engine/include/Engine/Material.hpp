#ifndef MATERIAL_HEADER
#define MATERIAL_HEADER

#include "Object.hpp"

namespace Engine
{
	class Texture2D;
	class Buffer;

	class DLLAPI Material : public Object
	{
	private:
		struct mat
		{
			XMFLOAT3 __declspec(align(16)) diffuse;
			XMFLOAT3 __declspec(align(16)) specular;
			XMFLOAT3 __declspec(align(16)) ambient;
			XMFLOAT3 __declspec(align(16)) emissive;
			FLOAT shininess;
			FLOAT __declspec(align(16)) opacity;
		} _mat;

		struct state
		{
			INT __declspec(align(16)) hasDiffuseTexture;
			INT hasSpecularTexture;
			INT hasAmbientTexture;
			INT hasEmissiveTexture;
			INT __declspec(align(16)) hasShininessTexture;
			INT hasOpacityTexture;
			INT hasBumpMap;
			INT hasNormalMap;
			INT __declspec(align(16)) hasDisplacementMap;
			INT hasLightMap;
		} _state;

		Buffer *_matBuffer;
		Buffer *_stateBuffer;
		
		Texture2D *_diffuseTexture;
		Texture2D *_specularTexture;
		Texture2D *_ambientTexture;
		Texture2D *_emissiveTexture;
		Texture2D *_shininessTexture;
		Texture2D *_opacityTexture;
		Texture2D *_bumpMap;
		Texture2D *_normalMap;
		Texture2D *_displacementMap;
		Texture2D *_lightMap;

	public:
		Material(void);
		~Material(void);

		void setDiffuse(const XMFLOAT3 &color);
		void setSpecular(const XMFLOAT3 &color);
		void setAmbient(const XMFLOAT3 &color);
		void setEmissive(const XMFLOAT3 &color);
		void setShininess(const FLOAT &shininess);
		void setOpacity(const FLOAT &opacity);

		void setDiffuseTexture(Texture2D *tex);
		void setSpecularTexture(Texture2D *tex);
		void setAmbientTexture(Texture2D *tex);
		void setEmissiveTexture(Texture2D *tex);
		void setShininessTexture(Texture2D *tex);
		void setOpacityTexture(Texture2D *tex);
		void setBumpMap(Texture2D *map);
		void setNormalMap(Texture2D *map);
		void setDisplacementMap(Texture2D *map);
		void setLightMap(Texture2D *map);

		XMFLOAT3 getDiffuse(void) const;
		XMFLOAT3 getSpecular(void) const;
		XMFLOAT3 getAmbient(void) const;
		XMFLOAT3 getEmissive(void) const;
		FLOAT getShininess(void) const;
		FLOAT getOpacity(void) const;

		Texture2D *getDiffuseTexture(void) const;
		Texture2D *getSpecularTexture(void) const;
		Texture2D *getAmbientTexture(void) const;
		Texture2D *getEmissiveTexture(void) const;
		Texture2D *getShininessTexture(void) const;
		Texture2D *getOpacityTexture(void) const;
		Texture2D *getBumpMap(void) const;
		Texture2D *getNormalMap(void) const;
		Texture2D *getDisplacementMap(void) const;
		Texture2D *getLightMap(void) const;

		BOOL hasDiffuseTexture(void) const;
		BOOL hasSpecularTexture(void) const;
		BOOL hasAmbientTexture(void) const;
		BOOL hasEmissiveTexture(void) const;
		BOOL hasShininessTexture(void) const;
		BOOL hasOpacityTexture(void) const;
		BOOL hasBumpMap(void) const;
		BOOL hasNormalMap(void) const;
		BOOL hasDisplacementMap(void) const;
		BOOL hasLightMap(void) const;

		ID3D11Buffer *getMatBuffer(void) const;
		ID3D11Buffer *getStateBuffer(void) const;
	};
}

#endif