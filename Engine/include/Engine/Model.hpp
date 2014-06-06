#ifndef MODEL
#define MODEL

#include <vector>

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
		BOOL isMirror;
		std::vector<Object *> *_tObject;
		FLOAT _modelMatrix[16];
		ShaderProgram *_program;
		INT _screenWidthLocation;
		INT _screenHeightLocation;
		INT _MVPLocation;
	public:
		Model();
		~Model();
		void initObjectArray(void);
		void initObjectMirror(Model *m);
		void config(ShaderProgram *program);
		void createObject(const UINT &sizeVertexArray, const FLOAT *vertexArray,
				  const UINT &sizeIndexArray, const GLuint *indexArray,
				  const std::string pathTexture,
				  const FLOAT *ambient, const FLOAT *diffuse, const FLOAT *specular, const FLOAT *shininess);
		void sortObject(void);
		void matIdentity(void);
		void matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matRotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		Vector3D<FLOAT> getPosition(void) const;
		Object *getObject(UINT num) const;
		void display(Window *win, Camera *cam, LBuffer *l) const;
		void displayOnGBuffer(Camera *cam, GBuffer *g) const;
		void displayShadow(Light *l) const;
	};
}
    

#endif
