#pragma once

#include <d3dx9.h>
#include "Input.h"

enum TextureNum
{
	T_Player,
	T_Enemy0,
	T_Enemy1,
	T_ZiDan,
	T_Background
};

enum ObjectType
{
	OBJT_UnKnow,
	OBJT_Player,
	OBJT_Enemy,
	OBJT_ZiDanFromPlayer,
	OBJT_ZiDanFromEnemy,
	OBJT_Background
};

struct  Vertex
{
	Vertex(float _x,float _y,float _z, float _u, float _v)
	{
		x = _x;
		y = _y;
		z = _z;
		u = _u;
		v = _v;
	}

	float x,y,z;
	float u,v;
};

#define FVF (D3DFVF_XYZ | D3DFVF_TEX1)

struct BumpBox
{
	float xMax,xMin;
	float yMax,yMin;
};

class ObjectManager;

class Object
{
public:
	Object();
	~Object(void);

	virtual void Updata(Input *_input, float deltaTime, IDirect3DDevice9 *_device, ObjectManager *objManager) {}
	virtual void OnBump(ObjectType objType) {};
	virtual void OnUnBump(ObjectType objType) {};

	float x,y;
	float m_MP;
	int m_shangHai;
	int m_textureNum;
	float m_v;
	bool m_isAlive;
	bool isBump;
	ObjectType m_ObjectType;

	D3DXMATRIX m_worldMat;

	BumpBox m_bumpBox;

	IDirect3DVertexBuffer9 * m_VB;
    IDirect3DIndexBuffer9 *m_IB;
};

