#pragma once

#define CUBES 10

class CGFxSpinCubes : public CGFx {
public:
	CGFxSpinCubes(void) {
		m_pDevice=NULL;
	}
	CGFxSpinCubes(LPDIRECT3DDEVICE9 device) {
		m_pDevice=device;
		m_pCubeLineIB=NULL;
		m_pCubeVB=NULL;
		rot=0.f;
		zpos=0.f, ypos=0.f, xpos=0.f;
		direction=1.f;
		internaltimedif=0.f;
		m_PlayinVal=30.f;
		m_PlayoutVal=0.f;
		CreateBuffers();
	}
	~CGFxSpinCubes(void) {
		SAFE_RELEASE(m_pCubeVB);
		return;
	}

	void CreateBuffers() {
		SAFE_RELEASE(m_pCubeVB);
		m_pDevice->CreateVertexBuffer(sizeof(g_aCubeVerticesN),D3DUSAGE_WRITEONLY,D3DFVF_XYZRGBAN,D3DPOOL_MANAGED,&m_pCubeVB,NULL);

		VOID* pData=NULL;
		m_pCubeVB->Lock(0,sizeof(pData),(void**)&pData,0); //lock buffer
		memcpy(pData,g_aCubeVerticesN,sizeof(g_aCubeVerticesN)); //copy data
		m_pCubeVB->Unlock();                               //unlock buffer
	}

	void Render(float fftval, float timeval) {
		if (m_pDevice==NULL) return;

		if (g_pBassLib->GetIsPaused() || g_pBassLib->GetIsStopped())
			fftval=0.1f;

		if (fftval>.25f)
		direction *=-1.f;
	
		rot+=(0.01f+fftval/10.f*direction);
		
		zpos=sin(rot)*5.f+3.f;
		xpos=sin(rot/2.f)-.3f;
		ypos=sin(rot/3.f)/3.f;

		if (fftval>.1f) {
			m_Scal.x = 1.f+barvals[2]*1.f;
			m_Scal.y = 1.f+barvals[4]*1.f;
			m_Scal.z = 1.f+barvals[6]*1.f;
		} else {
			m_Scal.x = 1.f+fftval+barvals[2];
			m_Scal.y = 1.f+fftval+barvals[4];
			m_Scal.z = 1.f+fftval+barvals[6];
		}

		for (int i=0; i<CUBES; i++) {
			cubetrail[i].x = 2.5f;
			cubetrail[i].y = 0.f;
			cubetrail[i].z = 8.f;
			cubetrail[i].rotx = rot+(i*PI+i*PI*(i/10.f));
			cubetrail[i].roty = rot+(i*PI+i*PI*(i/10.f));
			cubetrail[i].rotz = rot+(i*PI+i*PI*(i/10.f));
			cubetrail[i].scalx = m_Scal.x+(fftval*5.f);
			cubetrail[i].scaly = m_Scal.y+(fftval*5.f);
			cubetrail[i].scalz = m_Scal.z+(fftval*5.f);
			if (cubetrail[i].scalx < 0.f) cubetrail[i].scalx = 0.f;
			if (cubetrail[i].scaly < 0.f) cubetrail[i].scaly = 0.f;
			if (cubetrail[i].scalz < 0.f) cubetrail[i].scalz = 0.f;
		}

		m_pDevice->SetRenderState(D3DRS_LIGHTING, true);

		for (int i=0; i<CUBES; i++) {
			m_Pos.x = cubetrail[i].x;
			m_Pos.y = cubetrail[i].y;
			m_Pos.z = cubetrail[i].z;
			m_Rot.Set(cubetrail[i].rotx,cubetrail[i].roty,cubetrail[i].rotz);
			m_Scal.x = cubetrail[i].scalx;
			m_Scal.y = cubetrail[i].scaly;
			m_Scal.z = cubetrail[i].scalz;

			pos.Set(m_Rot,m_Pos,m_Scal);
			mat.Set(pos);
		
			m_pDevice->LightEnable(2,true);
			m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
			m_pDevice->SetRenderState(D3DRS_ZENABLE,true);

			m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTCOLOR);

			m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			

			m_pDevice->SetFVF(D3DFVF_XYZRGBAN);
			m_pDevice->SetStreamSource(0,m_pCubeVB,0,sizeof(D3DVERTEXXYZRGBAN));
			//m_pDevice->SetIndices(m_pCubeIB);

			m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
			//m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);
			m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,12);
		}

		/*
		m_pDevice->SetRenderState(D3DRS_ZENABLE,false);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		if (fftval>.1f) {
			m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR,D3DCOLOR_ARGB(180,255,255,255));
			m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_MODULATE4X);
			m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG2, D3DTA_TFACTOR);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pDevice->SetIndices(m_pCubeIB);
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);
			cubetrail[1]=cubetrail[0];
		}
		*/
		
	}

	bool PlayIn(float fftval, float timeval) {
		return false;

		m_PlayoutVal=0.f;
		m_PlayinVal -= timeval*0.01f;
		
		if (m_PlayinVal <= 0.f) {
			m_PlayinVal=0.f;
			return false;
		}
		
		Render(fftval,timeval);
		return true;
	}

	bool PlayOut(float fftval, float timeval) {
		return false;

		m_PlayoutVal += timeval*0.0005f;
		if (m_PlayoutVal >= 1.f) {
			m_PlayoutVal=0.f;
			m_PlayinVal=30.f;
			return false;
		}

		Render(fftval,timeval);
		return true;
	}

	LPDIRECT3DVERTEXBUFFER9 m_pCubeVB;
	LPDIRECT3DINDEXBUFFER9	m_pCubeIB;
	LPDIRECT3DINDEXBUFFER9	m_pCubeLineIB;

private:
	float m_PlayinVal;
	float m_PlayoutVal;

	float rot;
	float zpos, ypos, xpos;
	float direction;

	CNEVector3 m_Pos;
  CNEEuler m_Rot;
	CNEVector3 m_Scal;
	CNEPosition pos;
  CNEMatrix4 mat;
	LPDIRECT3DDEVICE9 m_pDevice;

	mycube cubetrail[CUBES];

	float internaltimedif;
};
