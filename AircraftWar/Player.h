#pragma once
#include "object.h"
#include "ZiDan.h"
#include <vector>


class Player : public Object
{
public:
	Player(IDirect3DDevice9 *_device) ;
	~Player(void);

	virtual void Updata(Input *_input, float deltaTime, IDirect3DDevice9 *_device, ObjectManager *objManager);
	virtual void OnBump(ObjectType objType);
	virtual void OnUnBump(ObjectType objType);

public:
	float JiSha;
private:
	float startTime;
	float lastTime;
	
};

