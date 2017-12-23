#pragma once

#include "Object.h"
#include <d3dx9.h>
#include "ZiDan.h"
#include "Player.h"
#include <vector>


class Enemy : public Object
{
public:
	Enemy(int _type, IDirect3DDevice9 *_device, Player *_player);
	~Enemy(void);

	virtual void Updata(Input *_input, float deltaTime, IDirect3DDevice9 *_device, ObjectManager *objManager);
	virtual void OnBump(ObjectType objType);

private:
	float startTime;
	float lastTime;
	Player *m_player;
};

