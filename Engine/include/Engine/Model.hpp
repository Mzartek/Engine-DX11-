#ifndef MODEL
#define MODEL

#include "Object.hpp"
//#include "Light.hpp"
//#include "GBuffer.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Window.hpp"

namespace engine
{
	class DLLAPI Model
	{
	private:
		struct uniform
		{
			FLOAT screenWidth;
			FLOAT screenHeight;
			DirectX::XMMATRIX MVP;
		};
		BOOL isMirror;
		std::vector<Object *> *_tObject;
		ID3D11Buffer *_pConstantBuffer;
		ShaderProgram *_program;
		struct uniform *_info;
		DirectX::XMMATRIX *_modelMatrix;
		void InitMesh(unsigned int i, const aiMesh* paiMesh);
	public:
		Model();
		~Model();
		void initObjectArray(void);
		void initObjectMirror(Model *m);
		HRESULT config(ShaderProgram *program, ID3D11Device *pd3dDevice);
		HRESULT createObject(const UINT &sizeVertexArray, const FLOAT *vertexArray,
				  const UINT &sizeIndexArray, const UINT *indexArray,
				  const std::string pathTexture,
				  const FLOAT *ambient, const FLOAT *diffuse, const FLOAT *specular, const FLOAT *shininess,
				  ID3D11Device *pd3dDevice);
		HRESULT loadFromFile(const std::string szFileName);
		void sortObject(void);
		void matIdentity(void);
		void matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matRotate(const FLOAT &angle, const BOOL &x, const BOOL &y, const BOOL &z);
		void matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		DirectX::XMFLOAT3 getPosition(void) const;
		Object *getObject(UINT num) const;
		void display(Window *win, Camera *cam);// , LBuffer *l) const;
		//void displayOnGBuffer(Camera *cam, GBuffer *g) const;
		//void displayShadow(Light *l) const;
	};
}
    

#endif
