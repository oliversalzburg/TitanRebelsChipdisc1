#pragma once

class CGFxStars : public CGFx {
public:
	CGFxStars(void) {
		m_pDevice=NULL;
	}
	CGFxStars(LPDIRECT3DDEVICE9 device) {
		m_pDevice=device;
		rot=0.f;
		zpos=0.f, ypos=0.f, xpos=0.f;
		direction=1.f;

		m_pStarVB=NULL;
		m_pStarIB=NULL;

		CreateBuffers();
	}
	~CGFxStars(void) {
		SAFE_RELEASE(m_pStarVB);
		SAFE_RELEASE(m_pStarIB);
		return;
	}

	void CreateBuffers() {
		if (m_pDevice==NULL) return;

		D3DVERTEXXYZRGBA aStarVertices[] = {{120.0f,  0.0f, -1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{156.0f, 76.0f, -1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{238.0f, 87.0f, -1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{177.0f,145.0f, -1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{191.0f,227.0f, -1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{120.0f,187.0f, -1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{ 46.0f,226.0f, -1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{ 60.0f,145.0f, -1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{  0.0f, 86.0f, -1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{ 83.0f, 76.0f, -1.0f,D3DCOLOR_ARGB(128,80,80,80)},
		
																				{120.0f,  0.0f, 1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{156.0f, 76.0f, 1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{238.0f, 87.0f, 1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{177.0f,145.0f, 1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{191.0f,227.0f, 1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{120.0f,187.0f, 1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{ 46.0f,226.0f, 1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{ 60.0f,145.0f, 1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{  0.0f, 86.0f, 1.0f,D3DCOLOR_ARGB(128,80,80,80)},
																				{ 83.0f, 76.0f, 1.0f,D3DCOLOR_ARGB(128,80,80,80)}};

		short aStarIndices[] = {9, 1, 7, 
														1, 3, 7,
														7, 3, 5,
														0, 1, 9, 
														1, 2, 3, 
														3, 4, 5,
														7, 5, 6,
														8, 9, 7,
														
														19, 11, 17, 
														11, 13, 17,
														17, 13, 15,
														10, 11, 19, 
														11, 12, 13, 
														13, 14, 15,
														17, 15, 16,
														18, 19, 17,
		
														 0, 10,  1,
														10, 11,  1,
														 1, 11,  2,
													  11, 12,  2,
														 2, 12,  3,
														12, 13,  3,
														 3, 13,  4,
													  13, 14,  4,
														 4, 14,  5,
														14, 15,  5,
														 5, 15,	 6,
														15, 16,  6,
														 6, 16,  7,
														16, 17,  7,
														 7, 17,  8,
														17, 18,  8,
														 8, 18,  9,
														18, 19,  9,
														 9, 19,  0,
														19, 10,  0};

		//center star
		for (int i=0; i<20; i++) {
			aStarVertices[i].x -= 120.f;
			aStarVertices[i].y -= 115.f;
			aStarVertices[i].x /= 100.f;
			aStarVertices[i].y /= 100.f;
		}

		m_pDevice->CreateVertexBuffer(sizeof(aStarVertices),D3DUSAGE_WRITEONLY,D3DFVF_XYZRGBA,D3DPOOL_MANAGED,&m_pStarVB,NULL);
		m_pDevice->CreateIndexBuffer(sizeof(aStarIndices),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pStarIB,NULL);

		VOID* pData=NULL;
		m_pStarVB->Lock(0,sizeof(pData),(void**)&pData,0);
		memcpy(pData,aStarVertices,sizeof(aStarVertices));
		m_pStarVB->Unlock();

		m_pStarIB->Lock(0,sizeof(pData),(void**)&pData,0);
		memcpy(pData,aStarIndices,sizeof(aStarIndices));
		m_pStarIB->Unlock();
	}

	void Render(float fftval, float timeval) {
		if (m_pDevice==NULL) return;

		if (g_pBassLib->GetIsPaused() || g_pBassLib->GetIsStopped())
			fftval=0.01f;

		D3DVERTEXXYZRGBA* aStarVertices;

		//set renderstates
		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

		//handle alpha
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);

		//set texturestages
		m_pDevice->SetTexture(0,NULL);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_DIFFUSE);

		//set stream and stream-format and draw
		m_pDevice->SetFVF(D3DFVF_XYZRGBA);
		m_pDevice->SetStreamSource(0,m_pStarVB,0,sizeof(D3DVERTEXXYZRGBA));
		m_pDevice->SetIndices(m_pStarIB);

		//first we draw the star tunnel
		if (true) {
			m_pStarVB->Lock(0,20*sizeof(D3DVERTEXXYZRGBA),(void**)&aStarVertices,0);
			
			for (int i=0; i<10; i++)
				//aStarVertices[i].z = 1.f+fftval*-200.f;
				aStarVertices[i].z = fabs(sin(((float)GetTickCount())/5000.f))*-100.f;
			m_pStarVB->Unlock();
			
			for (int i=0; i<20; i++) {
				if (fftval>.25f)
				direction *=-1.f;
			
				rot+=0.0001f+((fftval*fftval)/2.f);

				m_Pos.x = sin((float)i)*2.f;
				m_Pos.y = cos((float)i)*2.f;
				m_Pos.z = 50.f-(float)i;
				
				m_Rot.Set(0.f,0.f,rot);

				pos.Set(m_Rot,m_Pos,1.f);
				mat.Set(pos);

				m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
				m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,20,0,36);
			}
		}
		

		//then the star starfield :D
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);

		m_pStarVB->Lock(0,20*sizeof(D3DVERTEXXYZRGBA),(void**)&aStarVertices,0);
		for (int i=00; i<10; i++)
			aStarVertices[i].z = 1.f;
		for (int i=10; i<20; i++)
			aStarVertices[i].z = 1.5f;
		m_pStarVB->Unlock();

		float scale = .5f;
		if (fftval>.2f) scale = .6f;

		if (fftval>.1f) 
			for (int i=0; i<CUBECOUNT; i++) {
				cubes[i].rotz += .1f;
			}
		
		scale-=.1f;
		
		for (int i=0; i<CUBECOUNT; i++) {
			m_Pos.x = cubes[i].x;
			m_Pos.y = cubes[i].y;
			m_Pos.z = cubes[i].z;

			m_Rot.Set(0.f,0.f,cubes[i].rotz);

			pos.Set(m_Rot,m_Pos,scale);
			mat.Set(pos);
			m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
			
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,20,0,36);

			//towards
			cubes[i].z -= (i+1)*(fftval*.005f);

			if (cubes[i].z < -5.f) {
				cubes[i].z = 100.f+(((float)rand()/(float)RAND_MAX)*10.f);
				float val = -3.5f+(((float)rand()/(float)RAND_MAX)*7.f);;
				cubes[i].x = sin(val)*3.f;
				cubes[i].y = cos(val)*2.f;
				cubes[i].rotx = val;
				cubes[i].roty = val;
				cubes[i].rotz = val;
			}
		}
		
	}

	bool PlayIn(float fftval, float timeval) {
		for (int i=0; i<CUBECOUNT; i++) {
			cubes[i].x=(float)i;
			cubes[i].y=1000.f;
			//towards
			cubes[i].z = 10.f+(((float)rand()/(float)RAND_MAX)*100.f);
			float val = -3.5f+(((float)rand()/(float)RAND_MAX)*7.f);;
			cubes[i].x = sin(val)*3.f;
			cubes[i].y = cos(val)*2.f;
			cubes[i].rotx = val;
			cubes[i].roty = val;
			cubes[i].rotz = val;
		}
		return false;
	}

	bool PlayOut(float fftval, float timeval) {
		if (m_pDevice==NULL) return false;

		m_Pos.y -= .1f;
		if (m_Pos.y < -5.f) return false;

		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,false);

		m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTCOLOR);

		m_pDevice->SetFVF(D3DFVF_XYZRGBA);
		m_pDevice->SetStreamSource(0,m_pStarVB,0,sizeof(D3DVERTEXXYZRGBA));
		m_pDevice->SetIndices(m_pStarIB);

		pos.Set(m_Rot,m_Pos,m_Scal);
		mat.Set(pos);

		m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,10,0,8);

		return true;
	}

private:
	float rot;
	float zpos, ypos, xpos;
	float direction;

	CNEVector3 m_Pos;
  CNEEuler m_Rot;
	CNEVector3 m_Scal;
	CNEPosition pos;
  CNEMatrix4 mat;
	LPDIRECT3DDEVICE9 m_pDevice;

	LPDIRECT3DVERTEXBUFFER9 m_pStarVB;
	LPDIRECT3DINDEXBUFFER9	m_pStarIB;

	mycube cubes[CUBECOUNT];
};
