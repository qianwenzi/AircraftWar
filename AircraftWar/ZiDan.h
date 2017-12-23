#pragma once

#include "Object.h"

class ZiDan : public Object
{
public:
	ZiDan(IDirect3DDevice9 *_device, float formWhich);
	~ZiDan(void);

	virtual void Updata(Input *_input, float deltaTime, IDirect3DDevice9 *_device, ObjectManager *objManager);
	virtual void OnBump(ObjectType objType);
	virtual void OnUnBump(ObjectType objType);

private:
	float m_fromWhich; // 来自何方，速度往哪
};

