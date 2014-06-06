#include <Engine/Model.hpp>

engine::Model::Model(void)
{
	isMirror = GL_FALSE;
	_tObject = NULL;
	_program = NULL;
	matIdentity();
}

engine::Model::~Model(void)
{
	GLuint i;
	if(_tObject != NULL && isMirror == GL_FALSE)
	{		
		for(i=0 ; i<_tObject->size(); i++)
			delete (*_tObject)[i];
		delete _tObject;
	}
}

void engine::Model::initObjectArray(void)
{
	GLuint i;
	if(_tObject != NULL && isMirror == GL_FALSE)
	{		
		for(i=0 ; i<_tObject->size(); i++)
			delete (*_tObject)[i];
		delete _tObject;
	}
	_tObject = new std::vector<Object *>;
}

void engine::Model::initObjectMirror(Model *m)
{
	isMirror = GL_TRUE;
	_tObject = m->_tObject;
}

void engine::Model::config(ShaderProgram *program)
{
	_program = program;
	_screenWidthLocation = glGetUniformLocation(_program->getId(), "screenWidth");
	_screenHeightLocation = glGetUniformLocation(_program->getId(), "screenHeight");
	_MVPLocation = glGetUniformLocation(_program->getId(), "MVP");
}

void engine::Model::createObject(const GLsizei &sizeVertexArray, const GLfloat *vertexArray,
				 const GLsizei &sizeIndexArray, const GLuint *indexArray,
				 const std::string pathTexture,
				 const GLfloat *ambient, const GLfloat *diffuse, const GLfloat *specular, const GLfloat *shininess)
{
	Object *newone = new Object();
	GLuint texture;
  
	loadTextureFromFile(pathTexture, &texture);

	newone->setShaderProgram(_program);
	newone->setTexture(texture);
	newone->setAmbient(ambient[0], ambient[1], ambient[2], ambient[3]);
	newone->setDiffuse(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	newone->setSpecular(specular[0], specular[1], specular[2], specular[3]);
	newone->setShininess(shininess[0]);
	newone->load(sizeVertexArray, vertexArray,
		     sizeIndexArray, indexArray);
  
	_tObject->push_back(newone);
}

void engine::Model::sortObject(void)
{
	qsort(&(*_tObject)[0], _tObject->size(), sizeof (*_tObject)[0], comparObject);
}

void engine::Model::matIdentity(void)
{
	matrixLoadIdentity(_modelMatrix);
}

void engine::Model::matTranslate(const GLfloat &x, const GLfloat &y, const GLfloat &z)
{
	matrixTranslate(_modelMatrix, x, y, z);
}

void engine::Model::matRotate(const GLfloat &angle, const GLfloat &x, const GLfloat &y, const GLfloat &z)
{
	matrixRotate(_modelMatrix, angle, x, y, z);
}

void engine::Model::matScale(const GLfloat &x, const GLfloat &y, const GLfloat &z)
{
	matrixScale(_modelMatrix, x, y, z);
}

engine::Vector3D<GLfloat> engine::Model::getPosition(void) const
{
	engine::Vector3D<GLfloat> tmp;
	tmp.x = _modelMatrix[12];
	tmp.y = _modelMatrix[13];
	tmp.z = _modelMatrix[14];
	return tmp;
}

engine::Object *engine::Model::getObject(GLuint num) const
{
	if(num>=_tObject->size())
	{
		std::cerr << "Bad num Object" << std::endl;
		return NULL;
	}
	return (*_tObject)[num];
}
  
void engine::Model::display(Window *win, Camera *cam, LBuffer *l) const
{
	GLuint i;
	GLfloat tmp[16];
  
	if(_program == NULL)
	{
		std::cerr << "You need to configure the Model before" << std::endl;
		return;
	}
	if(cam == NULL)
	{
		std::cerr << "Bad Camera" << std::endl;
		return;
	}
  
	glUseProgram(_program->getId());

	glUniform1f(_screenWidthLocation, (GLfloat)win->getWidth());
	glUniform1f(_screenHeightLocation, (GLfloat)win->getHeight());
  
	matrixMultiply(tmp, cam->getMatrix(), _modelMatrix);
	glUniformMatrix4fv(_MVPLocation, 1, GL_FALSE, tmp);
  
	glUseProgram(0);
  
	for(i=0 ; i<_tObject->size(); i++)
		(*_tObject)[i]->display(l);
}

void engine::Model::displayOnGBuffer(Camera *cam, GBuffer *g) const
{
	GLuint i;
	GLfloat tmp[16];
	
	if(cam == NULL)
	{
		std::cerr << "Bad Camera" << std::endl;
		return;
	}
	if(g == NULL)
	{
		std::cerr << "Bad GBuffer!" << std::endl;
		return;
	}
        
	glUseProgram(g->getProgramId());
	
	matrixMultiply(tmp, cam->getMatrix(), _modelMatrix);
	glUniformMatrix4fv(g->getMVPLocation(), 1, GL_FALSE, tmp);
	glUniformMatrix4fv(g->getModelMatrixLocation(), 1, GL_FALSE, _modelMatrix);
	matrixNormalFromModel(tmp, _modelMatrix);
	glUniformMatrix3fv(g->getNormalMatrixLocation(), 1, GL_FALSE, tmp);
	
	glUseProgram(0);
	
	for(i=0 ; i<_tObject->size(); i++)
		(*_tObject)[i]->displayOnGBuffer(g);
}

void engine::Model::displayShadow(Light *l) const
{
	GLuint i;
	GLfloat tmp[16];

	if(l == NULL)
	{
		std::cerr << "Bad Light!" << std::endl;
		return;
	}
	if(l->getShadowMap() == NULL)
	{
		std::cerr << "You need to config the ShadowMap before!" << std::endl;
		return;
	}
	
	glUseProgram(l->getShadowMap()->getProgramId());
	
	matrixMultiply(tmp, l->getMatrix(), _modelMatrix);
	glUniformMatrix4fv(l->getShadowMap()->getMVPLocation(), 1, GL_FALSE, tmp);
	
	glUseProgram(0);
	for(i=0 ; i<_tObject->size(); i++)
		(*_tObject)[i]->displayShadow(l);
}
