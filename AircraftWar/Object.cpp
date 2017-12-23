#include "Object.h"


Object::Object(void)
{
	x=0;
	y=0;
	m_textureNum = 0;
	m_MP = 0;
	m_shangHai = 1;
	m_v = 10;
	m_isAlive = true;
	m_ObjectType = ObjectType::OBJT_UnKnow;

	m_VB = NULL;
	m_IB = NULL;

	m_bumpBox.xMax = x + 1.0f;
	m_bumpBox.xMin = x - 1.0f;
	m_bumpBox.yMax = y + 1.0f;
	m_bumpBox.yMin = y - 1.0f;

	isBump = true;
}


Object::~Object(void)
{
	if(m_IB)
		m_IB->Release();
	if(m_VB)
		m_VB->Release();
}

