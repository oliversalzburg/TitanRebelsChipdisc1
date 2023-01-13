#pragma once

#define CUBETRAILS 20

class CGFxSpinCube : public CGFx {
public:
	CGFxSpinCube(void) {
		m_pDevice=NULL;
	}
	CGFxSpinCube(LPDIRECT3DDEVICE9 device) {
		m_pDevice=device;
		m_pCubeLineIB=NULL;
		rot=0.f;
		zpos=0.f, ypos=0.f, xpos=0.f;
		direction=1.f;
		internaltimedif=0.f;
		m_PlayinVal=30.f;
		m_PlayoutVal=0.f;
		CreateBuffers();
	}
	~CGFxSpinCube(void) {
		return;
	}

	void CreateBuffers() {
		short aCubeLineIndices[] = {	1,2, 2,6, 6,7, 7,1,
																	1,0, 2,3, 6,5, 7,4,
																	0,3, 3,5, 5,4, 4,0};
		VOID* pData=NULL;

		SAFE_RELEASE(m_pCubeLineIB);
		m_pDevice->CreateIndexBuffer(sizeof(g_aCubeIndices),
                                     D3DUSAGE_WRITEONLY,
                                     D3DFMT_INDEX16,
                                     D3DPOOL_MANAGED,
                                     &m_pCubeLineIB,
                                     NULL);

		m_pCubeLineIB->Lock(0,sizeof(pData),(void**)&pData,0); //lock buffer
		memcpy(pData,aCubeLineIndices,sizeof(aCubeLineIndices));   //copy data
		m_pCubeLineIB->Unlock();                               //unlock buffer
	}

	void Render(float fftval, float timeval) {
		if (m_pDevice==NULL) return;

		if (g_pBassLib->GetIsPaused() || g_pBassLib->GetIsStopped())
			fftval=0.01f;

		if (fftval>.25f)
		direction *=-1.f;
	
		rot+=(0.01f+fftval/10.f*direction);
		
		zpos=sin(rot)*5.f+3.f;
		xpos=sin(rot/2.f)-.3f;
		ypos=sin(rot/3.f)/3.f;

		if (fftval>.1f) {
			m_Scal.x = .8f+barvals[2]*1.f;
			m_Scal.y = .8f+barvals[4]*1.f;
			m_Scal.z = .8f+barvals[6]*1.f;
		} else {
			m_Scal.x = .8f+fftval+barvals[2];
			m_Scal.y = .8f+fftval+barvals[4];
			m_Scal.z = .8f+fftval+barvals[6];
		}

		if (timeval+internaltimedif>100.f) {
			internaltimedif = 100.f-(timeval+internaltimedif);
			for (int i=CUBETRAILS-1; i>1; i--)
				cubetrail[i] = cubetrail[i-1];
		}
		internaltimedif+=timeval;

		for (int i=CUBETRAILS-1; i>0; i--) {
			cubetrail[i].z = cubetrail[i].z-(timeval*.01f);
			cubetrail[i].rotz = max(cubetrail[i].rotz+(timeval*.001f),0);
			//cubetrail[i].scalx = max(cubetrail[i].scalx+(timeval*.01f),0);
			//cubetrail[i].scaly = max(cubetrail[i].scaly+(timeval*.01f),0);
			//cubetrail[i].scalz = max(cubetrail[i].scalz+(timeval*.01f),0);
		}

		cubetrail[0].x = 1.5f+m_PlayoutVal;
		cubetrail[0].y = ypos;
		cubetrail[0].z = zpos+5.f+m_PlayinVal;
		cubetrail[0].rotx = rot;
		cubetrail[0].roty = rot;
		cubetrail[0].rotz = rot;

		cubetrail[0].scalx = m_Scal.x-m_PlayoutVal;
		cubetrail[0].scaly = m_Scal.y-m_PlayoutVal;
		cubetrail[0].scalz = m_Scal.z-m_PlayoutVal;

		if (cubetrail[0].scalx < 0.f) cubetrail[0].scalx = 0.f;
		if (cubetrail[0].scaly < 0.f) cubetrail[0].scaly = 0.f;
		if (cubetrail[0].scalz < 0.f) cubetrail[0].scalz = 0.f;

		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);

		for (int i=CUBETRAILS-1; i>=0; i--) {
			m_Pos.x = cubetrail[i].x;
			m_Pos.y = cubetrail[i].y;
			m_Pos.z = cubetrail[i].z;
			m_Rot.Set(cubetrail[i].rotx,cubetrail[i].roty,cubetrail[i].rotz);
			m_Scal.x = cubetrail[i].scalx;
			m_Scal.y = cubetrail[i].scaly;
			m_Scal.z = cubetrail[i].scalz;

			pos.Set(m_Rot,m_Pos,m_Scal);
			mat.Set(pos);

			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
			m_pDevice->SetRenderState(D3DRS_ZENABLE,true);

			m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTCOLOR);
			
			m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR,D3DCOLOR_ARGB(0,0,0,50));
			m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_MODULATE2X);
			m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG2, D3DTA_TFACTOR);

			m_pDevice->SetFVF(D3DFVF_XYZRGBA);
			m_pDevice->SetStreamSource(0,m_pCubeVB,0,sizeof(D3DVERTEXXYZRGBA));
			m_pDevice->SetIndices(m_pCubeLineIB);

			m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
			m_pDevice->DrawIndexedPrimitive(D3DPT_LINELIST,0,0,8,0,12);
		}

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
		
	}

	bool PlayIn(float fftval, float timeval) {
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

	mycube cubetrail[CUBETRAILS];

	float internaltimedif;
};
