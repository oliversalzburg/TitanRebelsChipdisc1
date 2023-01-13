#pragma once
#undef UNICODE
#undef _UNICODE

#define SAFE_FREE(p)    { if(p) { free   (p);     (p)=NULL; } }
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define RGB_GETRED(rgb)         (((rgb) >> 16) & 0xff)
#define RGB_GETGREEN(rgb)       (((rgb) >> 8) & 0xff)
#define RGB_GETBLUE(rgb)        ((rgb) & 0xff)

#define WM_SELFUPDATE WM_USER+1

#define D3DFVF_CUSTOMVERTEXORTHO ( D3DFVF_XYZRHW | D3DFVF_TEX1 )
struct D3DVERTEXORTHO {
  float x, y, z, rhw;
  float u, v;        
};

#define D3DFVF_CUSTOMVERTEXORTHORGBA ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )
struct D3DVERTEXORTHORGBA {
  float x, y, z, rhw;
	DWORD rgba;    
};

#define D3DFVF_CUSTOMVERTEXORTHOCOL ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
struct D3DVERTEXORTHOCOL {
  float x, y, z, rhw;
  DWORD rgba;
	float u, v;
};

#define D3DFVF_XYZRGBA ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
struct D3DVERTEXXYZRGBA {
  float x, y, z;
  DWORD rgba;
};

#define D3DFVF_XYZRGBAN ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE )
struct D3DVERTEXXYZRGBAN {
  float x, y, z;
	float nx, ny, nz;
  DWORD rgba;
};

#define D3DFVF_XYZVU ( D3DFVF_XYZ | D3DFVF_TEX1 )
struct D3DVERTEXXYZVU {
  float x, y, z;
  float u, v;
};

extern float barvals[7];

struct mycube {
	float x,y,z;
	float rotx,roty,rotz;
	float scalx,scaly,scalz;
	mycube() { x=0.f;y=0.f;z=99999.f; }
};

#define CUBECOLOR D3DCOLOR_ARGB(128,5,5,5)
#define CUBECOLORN D3DCOLOR_ARGB(128,150,150,150)

static const D3DVERTEXXYZRGBA g_aCubeVertices[] = {{-1.0f,-1.0f,-1.0f,CUBECOLOR},
																									 {-1.0f, 1.0f,-1.0f,CUBECOLOR},
																									 { 1.0f, 1.0f,-1.0f,CUBECOLOR},
																									 { 1.0f,-1.0f,-1.0f,CUBECOLOR},
																									 {-1.0f,-1.0f, 1.0f,CUBECOLOR},
																									 { 1.0f,-1.0f, 1.0f,CUBECOLOR},
																									 { 1.0f, 1.0f, 1.0f,CUBECOLOR},
																									 {-1.0f, 1.0f, 1.0f,CUBECOLOR}};

static const D3DVERTEXXYZRGBAN g_aCubeVerticesN[] = {
	 {-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,-1.0f,   CUBECOLORN},  //Front face
   {-1.0f, 1.0f,-1.0f,   0.0f, 0.0f,-1.0f,   CUBECOLORN},
   { 1.0f, 1.0f,-1.0f,   0.0f, 0.0f,-1.0f,   CUBECOLORN},
   { 1.0f, 1.0f,-1.0f,   0.0f, 0.0f,-1.0f,   CUBECOLORN},
   { 1.0f,-1.0f,-1.0f,   0.0f, 0.0f,-1.0f,   CUBECOLORN},
   {-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,-1.0f,   CUBECOLORN},

   { 1.0f,-1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   CUBECOLORN},  //Back face
   { 1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   CUBECOLORN},
   {-1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   CUBECOLORN},
   {-1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   CUBECOLORN},
   {-1.0f,-1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   CUBECOLORN},
   { 1.0f,-1.0f, 1.0f,   0.0f, 0.0f, 1.0f,   CUBECOLORN},

   {-1.0f, 1.0f,-1.0f,   0.0f, 1.0f, 0.0f,   CUBECOLORN},  //Top face
   {-1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   CUBECOLORN},
   { 1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   CUBECOLORN},
   { 1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   CUBECOLORN},
   { 1.0f, 1.0f,-1.0f,   0.0f, 1.0f, 0.0f,   CUBECOLORN},
   {-1.0f, 1.0f,-1.0f,   0.0f, 1.0f, 0.0f,   CUBECOLORN},

   { 1.0f,-1.0f,-1.0f,   0.0f,-1.0f, 0.0f,   CUBECOLORN},  //Bottom face
   { 1.0f,-1.0f, 1.0f,   0.0f,-1.0f, 0.0f,   CUBECOLORN},
   {-1.0f,-1.0f, 1.0f,   0.0f,-1.0f, 0.0f,   CUBECOLORN},
   {-1.0f,-1.0f, 1.0f,   0.0f,-1.0f, 0.0f,   CUBECOLORN},
   {-1.0f,-1.0f,-1.0f,   0.0f,-1.0f, 0.0f,   CUBECOLORN},
   { 1.0f,-1.0f,-1.0f,   0.0f,-1.0f, 0.0f,   CUBECOLORN},

   {-1.0f,-1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,   CUBECOLORN},  //Left face
   {-1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,   CUBECOLORN},
   {-1.0f, 1.0f,-1.0f,  -1.0f, 0.0f, 0.0f,   CUBECOLORN},
   {-1.0f, 1.0f,-1.0f,  -1.0f, 0.0f, 0.0f,   CUBECOLORN},
   {-1.0f,-1.0f,-1.0f,  -1.0f, 0.0f, 0.0f,   CUBECOLORN},
   {-1.0f,-1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,   CUBECOLORN},

   { 1.0f,-1.0f,-1.0f,   1.0f, 0.0f, 0.0f,   CUBECOLORN},  //Right face
   { 1.0f, 1.0f,-1.0f,   1.0f, 0.0f, 0.0f,   CUBECOLORN},
   { 1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   CUBECOLORN},
   { 1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   CUBECOLORN},
   { 1.0f,-1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   CUBECOLORN},
   { 1.0f,-1.0f,-1.0f,   1.0f, 0.0f, 0.0f,   CUBECOLORN},};


static const short g_aCubeIndices[] = {	0,1,2, 2,3,0, 4,5,6,
																				6,7,4, 0,3,5, 5,4,0,
																				3,2,6, 6,5,3, 2,1,7,
																				7,6,2, 1,0,4, 4,7,1	};

#include "resource.h"
#include "CGBassLib.h"
#include "CGTimer.h"
extern CGTimer* g_pTimer;
extern CGBassLib*	g_pBassLib;

#include "CGFont.h"
#include "CGFx.h"
#include "CGFxCubeTunnel.h"
#include "CGFxSpinCube.h"
#include "CGFxStars.h"
#include "CGFxSphere.h"
#include "CGFxSpider.h"
#include "CGFxSpinCubes.h"