#pragma once

#include "object.h"

class BackGround :
	public Object
{
public:
	BackGround(IDirect3DDevice9 *_device);
	~BackGround(void);

	virtual void Updata(Input *_input, float deltaTime, IDirect3DDevice9 *_device, ObjectManager *objManager);
};

