#include <Engine/Model.hpp>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

engine::Model::Model(void)
{
	_tObject = NULL;
	_pConstantBuffer = NULL;
	_program = NULL;
	_info = (struct uniform *)_aligned_malloc(sizeof *_info, 16);
	_modelMatrix = (DirectX::XMMATRIX *) _aligned_malloc(sizeof *_modelMatrix, 16);

	matIdentity();
}

engine::Model::~Model(void)
{
	UINT i;

	if(_tObject != NULL && isMirror == FALSE)
	{		
		for(i=0 ; i<_tObject->size(); i++)
			delete (*_tObject)[i];
		delete _tObject;
	}

	if (_pConstantBuffer)
		_pConstantBuffer->Release();

	_aligned_free(_info);
	_aligned_free(_modelMatrix);
}

void engine::Model::initObjectArray(void)
{
	UINT i;
	if(_tObject != NULL && isMirror == FALSE)
	{		
		for(i=0 ; i<_tObject->size(); i++)
			delete (*_tObject)[i];
		delete _tObject;
	}
	isMirror = FALSE;
	_tObject = new std::vector<Object *>;
}

void engine::Model::initObjectMirror(Model *m)
{
	isMirror = TRUE;
	_tObject = m->_tObject;
}

HRESULT engine::Model::config(ShaderProgram *program, ID3D11Device *pd3dDevice)
{
	HRESULT hr;

	_program = program;

	// Create uniform
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(*_info);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pConstantBuffer);

	return hr;
}

HRESULT engine::Model::createObject(const UINT &sizeVertexArray, const FLOAT *vertexArray,
	const UINT &sizeIndexArray, const UINT *indexArray,
	const std::string pathTexture,
	const FLOAT *ambient, const FLOAT *diffuse, const FLOAT *specular, const FLOAT *shininess,
	ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	Object *newone = new Object;
	ID3D11ShaderResourceView *ptex;
	ID3D11SamplerState *psam;
  
	hr = loadTextureFromFile(pathTexture, &ptex, &psam, pd3dDevice);
	if (FAILED(hr))
		return hr;

	hr = newone->setShaderProgram(_program, pd3dDevice);
	if (FAILED(hr))
		return hr;

	newone->setTexture(ptex, psam);
	newone->setAmbient(ambient[0], ambient[1], ambient[2], ambient[3]);
	newone->setDiffuse(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	newone->setSpecular(specular[0], specular[1], specular[2], specular[3]);
	newone->setShininess(shininess[0]);
	hr = newone->load(sizeVertexArray, vertexArray,
		sizeIndexArray, indexArray,
		pd3dDevice);
	if (FAILED(hr))
		return hr;
  
	_tObject->push_back(newone);

	return S_OK;
}

void engine::Model::InitMesh(UINT i, const aiMesh* paiMesh)
{

}

HRESULT engine::Model::loadFromFile(const std::string szFileName)
{
	Assimp::Importer Importer;
	const aiScene *pScene = NULL;
	UINT numMeshes, numMaterials, i;

	pScene = Importer.ReadFile(szFileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (!pScene)
		return E_FAIL; 
	
	numMeshes = pScene->mNumMeshes;
	numMaterials = pScene->mNumMaterials;

	for (i = 0; i < numMeshes; i++) 
	{
		const aiMesh *paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return S_OK;
}

void engine::Model::sortObject(void)
{
	qsort(&(*_tObject)[0], _tObject->size(), sizeof (*_tObject)[0], comparObject);
}

void engine::Model::matIdentity(void)
{
	*_modelMatrix = XMMatrixTranspose(DirectX::XMMatrixIdentity());
}

void engine::Model::matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_modelMatrix *= XMMatrixTranspose(DirectX::XMMatrixTranslation(x, y, z));
}

void engine::Model::matRotate(const FLOAT &angle, const BOOL &x, const BOOL &y, const BOOL &z)
{
	if (x)
		*_modelMatrix *= XMMatrixTranspose(DirectX::XMMatrixRotationX(angle));
	if (y)
		*_modelMatrix *= XMMatrixTranspose(DirectX::XMMatrixRotationY(angle));
	if (z)
		*_modelMatrix *= XMMatrixTranspose(DirectX::XMMatrixRotationZ(angle));
}

void engine::Model::matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_modelMatrix *= XMMatrixTranspose(DirectX::XMMatrixScaling(x, y, z));
}

DirectX::XMFLOAT3 engine::Model::getPosition(void) const
{
	DirectX::XMFLOAT3 tmp;
	tmp.x = DirectX::XMVectorGetZ(_modelMatrix->r[0]);
	tmp.y = DirectX::XMVectorGetZ(_modelMatrix->r[1]);
	tmp.z = DirectX::XMVectorGetZ(_modelMatrix->r[2]);

	return tmp;
}

engine::Object *engine::Model::getObject(UINT num) const
{
	if(num>=_tObject->size())
	{
		MessageBox(NULL, "Bad num Object!", "Error", MB_OK);
		exit(1);
	}
	return (*_tObject)[num];
}
  
void engine::Model::display(Window *win, Camera *cam)// , LBuffer *l) const
{
	UINT i;
  
	if(_program == NULL)
	{
		MessageBox(NULL, "Bad num Object!", "Error", MB_OK);
		exit(1);
	}
	if(cam == NULL)
	{
		MessageBox(NULL, "Bad Camera!", "Error", MB_OK);
		exit(1);
	}

	_info->screenWidth = (FLOAT)win->getWidth();
	_info->screenHeight = (FLOAT)win->getHeight();
	_info->MVP = *cam->getMatrix() * *_modelMatrix;
  
	win->getImmediateContext()->UpdateSubresource(_pConstantBuffer, 0, NULL, _info, 0, 0);
	win->getImmediateContext()->VSSetConstantBuffers(1, 1, &_pConstantBuffer);
  
	for(i=0 ; i<_tObject->size(); i++)
		(*_tObject)[i]->display(win);

	// For Deffered Lighting
	// (*_tObject)[i]->display(win, l);
}

/*void engine::Model::displayOnGBuffer(Camera *cam, GBuffer *g) const
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
}*/
