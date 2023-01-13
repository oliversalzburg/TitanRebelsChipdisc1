#pragma once

struct myleg {
	float x,y,z;
	float rotx,roty,rotz;
	float scalx,scaly,scalz;
	CNEVector3 dir;
};

class CGFxSpider : public CGFx {
public:
	CGFxSpider(void) {
		m_pVertexBufferVB=NULL;
		m_pIndexBufferIB=NULL;
		m_pDevice=NULL;
	}
	CGFxSpider(LPDIRECT3DDEVICE9 device) {
		m_pVertexBufferVB=NULL;
		m_pIndexBufferIB=NULL;
		m_pDevice=device;

		m_PlayinVal = 0.f;

		CreateBuffers();
	}
	~CGFxSpider(void) {
		SAFE_RELEASE(m_pVertexBufferVB);
		SAFE_RELEASE(m_pIndexBufferIB);
		return;
	}

	void CreateBuffers() {
		if (m_pDevice==NULL) return;

		m_pDevice->CreateVertexBuffer(sizeof(g_aCubeVertices),D3DUSAGE_WRITEONLY,D3DFVF_XYZRGBA,D3DPOOL_MANAGED,&m_pVertexBufferVB,NULL);
		m_pDevice->CreateIndexBuffer(sizeof(g_aCubeIndices),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&m_pIndexBufferIB,NULL);

		VOID* pData=NULL;
		m_pVertexBufferVB->Lock(0,sizeof(pData),(void**)&pData,0);
		memcpy(pData,g_aCubeVertices,sizeof(g_aCubeVertices));
		m_pVertexBufferVB->Unlock();

		m_pIndexBufferIB->Lock(0,sizeof(pData),(void**)&pData,0);
		memcpy(pData,g_aCubeIndices,sizeof(g_aCubeIndices));
		m_pIndexBufferIB->Unlock();

		m_pSpiderLegs[0].dir.x = (float)RAND_MAX/(float)rand();
		m_pSpiderLegs[0].dir.y = (float)RAND_MAX/(float)rand();

		for (int i=0; i<100; i++) {
			/*
			m_pSpiderLegs[i].x = (((float)rand())/((float)RAND_MAX)-.5f)*20.f;
			m_pSpiderLegs[i].y = (((float)rand())/((float)RAND_MAX)-.5f)*10.f;
			m_pSpiderLegs[i].z = (((float)rand())/((float)RAND_MAX)+5.f)*2.f;
			*/
			m_pSpiderLegs[i].x = -10.f+i/5.f;
			m_pSpiderLegs[i].y = sin((float)i/10.f)*2.f;
			m_pSpiderLegs[i].z = (((float)rand())/((float)RAND_MAX)+5.f)*2.f;
			float scal=fabs(sin((float)i/5.f))+.2f;
			m_pSpiderLegs[i].scalx = scal;
			m_pSpiderLegs[i].scaly = scal;
			m_pSpiderLegs[i].scalz = scal;
		}

	}

	void Render(float fftval, float timeval) {
		if (m_pDevice==NULL) return;

		if (g_pBassLib->GetIsPaused() || g_pBassLib->GetIsStopped())
			fftval=0.01f;
	
		//do rendering shit here

		//set renderstates
		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

		//handle alpha
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//set texturestages
		m_pDevice->SetTexture(0,NULL);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_DIFFUSE);

		//set stream and stream-format
		m_pDevice->SetFVF(D3DFVF_XYZRGBA);
		m_pDevice->SetStreamSource(0,m_pVertexBufferVB,0,sizeof(D3DVERTEXXYZRGBA));
		m_pDevice->SetIndices(m_pIndexBufferIB);

		for (int i=0; i<100; i++) {
			float scal=fabs(sin(
				(((float)i*2)+(float)g_pTimer->GetTime()/1000.f)
				)/1.f)+fftval;//+(fftval*m_pSpiderLegs[i].y);
			m_pSpiderLegs[i].scalx = scal;
			m_pSpiderLegs[i].scaly = scal;
			m_pSpiderLegs[i].scalz = scal;

			m_pSpiderLegs[i].x = sin((float)g_pTimer->GetTime()/1000.f+i) * 5.f;
			m_pSpiderLegs[i].y = sin((float)g_pTimer->GetTime()/1000.f+i+(float)g_pTimer->GetTime()/1000.f) * 5.f;
			m_pSpiderLegs[i].z = cos((float)g_pTimer->GetTime()/1000.f+i) * 5.f + 5.f;
		}

		for (int i=(int)m_PlayoutVal; i<100; i++) {
			//set transformation
			m_Pos.x = m_pSpiderLegs[i].x+5.f+sin(10.f-m_PlayinVal)*10.f;
			m_Pos.y = m_pSpiderLegs[i].y+sin(10.f-m_PlayinVal)*10.f;
			m_Pos.z = m_pSpiderLegs[i].z+sin(10.f-m_PlayinVal)*10.f;
			//m_Rot.Set(g_pTimer->GetTime()/1000.f,g_pTimer->GetTime()/1000.f,g_pTimer->GetTime()/1000.f);
			m_Rot.Set(0.f,0.f,0.f);
			m_Scal.x = m_pSpiderLegs[i].scalx;
			m_Scal.y = m_pSpiderLegs[i].scaly;
			m_Scal.z = m_pSpiderLegs[i].scalz;

			pos.Set(m_Rot,m_Pos,m_Scal);
			mat.Set(pos);

			m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
					
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);
		}

		/*
		if (fftval>.2f) {
			m_Pos.x = 0.f;
			m_Pos.y = 0.f;
			m_Pos.z = 0.f;
			pos.Set(m_Rot,m_Pos,10.f);
			mat.Set(pos);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
			m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);
		}
		*/

	}

	bool PlayIn(float fftval, float timeval) {
		m_PlayoutVal=0.f;

		m_PlayinVal += timeval*.01f;
		m_PlayinVal -= m_PlayinVal*.01f;

		if (m_PlayinVal >= 10.f) {
			m_PlayinVal=10.f;
			return false;
		}

		Render(fftval,timeval);
		
		return true;
	}

	bool PlayOut(float fftval, float timeval) {
		if (m_pDevice==NULL) return false;

		m_PlayoutVal+=timeval*0.05f;
		
		if (m_PlayoutVal>= 100.f) {
			m_PlayinVal = 0.f;
			return false;
		}

		Render(fftval,timeval);

		return true;
	}

private:
	float m_PlayinVal;
	float m_PlayoutVal;

	//transformation data
	CNEVector3 m_Pos;
  CNEEuler m_Rot;
	CNEVector3 m_Scal;
	CNEPosition pos;
  CNEMatrix4 mat;

	LPDIRECT3DDEVICE9 m_pDevice; //pointer to the direct3d device

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBufferVB; //this effects vertex buffer
	LPDIRECT3DINDEXBUFFER9 m_pIndexBufferIB; //this effects index buffer

	myleg m_pSpiderLegs[100];
};
