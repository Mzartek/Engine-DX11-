#include <Engine/Object.hpp>

INT Engine::Object::_memState = 0;
static std::vector<void *> _tmemNew;
static std::vector<void *> _tmemDelete;
static std::vector<void *> _tmem;

Engine::Object::Object(void)
{
}

Engine::Object::~Object(void)
{
}

void *Engine::Object::operator new(size_t sz)
{
	void *p = malloc(sz);
	if (p == NULL)
	{
		fprintf(stderr, "Error alloc\n");
		exit(1);
	}
	_memState++;
	_tmemNew.push_back(p);
	_tmem.push_back(p);

	return p;
}

void *Engine::Object::operator new[](size_t sz)
{
	void *p = malloc(sz);
	if (p == NULL)
	{
		fprintf(stderr, "Error alloc\n");
		exit(1);
	}
	_memState++;
	_tmemNew.push_back(p);
	_tmem.push_back(p);

	return p;
}

void Engine::Object::operator delete(void *p)
{
	UINT i;
	for (i = 0; i < _tmem.size(); i++)
	{
		if (_tmem[i] == p)
		{
			_tmemDelete.push_back(p);
			_tmem.erase(_tmem.begin() + i);
			_memState--;
			free(p);
			return;
		}
	}
	MessageBox(NULL, TEXT("Error Delete"), TEXT(__FILE__), MB_OK);
}

void Engine::Object::operator delete[](void *p)
{
	UINT i;
	for (i = 0; i < _tmem.size(); i++)
	{
		if (_tmem[i] == p)
		{
			_tmemDelete.push_back(p);
			_tmem.erase(_tmem.begin() + i);
			_memState--;
			free(p);
			return;
		}
	}
	MessageBox(NULL, TEXT("Error Delete"), TEXT(__FILE__), MB_OK);
}

INT Engine::Object::getMemoryState(void)
{
	return _memState;
}

void Engine::Object::saveMemoryInfo(const LPCTSTR filename)
{
	std::ofstream file(filename, std::ifstream::out);
	unsigned int i;

	file << "Memory Alloc:" << std::endl;
	for (i = 0; i < _tmemNew.size(); i++)
		file << _tmemNew[i] << std::endl;
	file << std::endl;

	file << "Memory Free:" << std::endl;
	for (i = 0; i < _tmemDelete.size(); i++)
		file << _tmemDelete[i] << std::endl;
	file << std::endl;

	file << "Memory Last:" << std::endl;
	for (i = 0; i < _tmem.size(); i++)
		file << _tmem[i] << std::endl;
	file << std::endl;

	file.close();
}