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
		void setbumpMap(Texture2D *map);
		void setNormalMap(Texture2D *map);
		void setDisplacementMap(Texture2D *map);
		void setLightMap(Texture2D *map);

		BOOL hasDiffuseTexture(void);
		BOOL hasSpecularTexture(void);
		BOOL hasAmbientTexture(void);
		BOOL hasEmissiveTexture(void);
		BOOL hasShininessTexture(void);
		BOOL hasOpacityTexture(void);
		BOOL hasBumpMap(void);
		BOOL hasNormalMap(void);
		BOOL hasDisplacementMap(void);
		BOOL hasLightMap(void);
		
		Texture2D *getDiffuseTexture(void);
		Texture2D *getSpecularTexture(void);
		Texture2D *getAmbientTexture(void);
		Texture2D *getEmissiveTexture(void);
		Texture2D *getShininessTexture(void);
		Texture2D *getOpacityTexture(void);
		Texture2D *getBumpMap(void);
		Texture2D *getNormalMap(void);
		Texture2D *getDisplacementMap(void);
		Texture2D *getLightMap(void);

		ID3D11Buffer *getMatBuffer(void);
		ID3D11Buffer *getStateBuffer(void);
	};
}

#endif