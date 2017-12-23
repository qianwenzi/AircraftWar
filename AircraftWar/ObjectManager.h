#pragma once

#include <vector>
#include "Object.h"
#include "Input.h"

class ObjectManager
{
public:
	ObjectManager(IDirect3DDevice9 *_device);
	~ObjectManager(void);

	void Render(IDirect3DDevice9 *_device,  D3DXMATRIX *matWorld );
	void Updata(Input *_input, float deltaTime, IDirect3DDevice9 *_device);

	void AddObject(Object *obj);
	void RemoveObject(Object *obj);

	Object* GetPlayer() { for(UINT i = 0; i < m_objectList.size(); i++) return m_objectList[i];  return NULL;}

private:
	std::vector<Object*> m_objectList;
	IDirect3DTexture9 *m_texture[5];
};

