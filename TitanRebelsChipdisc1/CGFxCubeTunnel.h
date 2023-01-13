#pragma once

#define CUBECOUNT 100

class CGFxCubeTunnel : public CGFx {
public:
	CGFxCubeTunnel(void) {
		m_pDevice=NULL;
	}
	CGFxCubeTunnel(LPDIRECT3DDEVICE9 device) {
		m_pDevice=device;
		m_fGlobRot=0.f;
		m_IsInit=false;
		m_PlayinVal=0.f;
	}
	~CGFxCubeTunnel(void) {
		return;
	}

	void Render(float fftval, float timeval) {
		if (m_pDevice==NULL) return;

		if (g_pBassLib->GetIsPaused() || g_pBassLib->GetIsStopped())
			fftval=0.01f;

		//set renderstates
		m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

		//handle alpha
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);

		//set texturestages
		m_pDevice->SetTexture(0,NULL);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		
		m_pDevice->SetFVF(D3DFVF_XYZRGBA);
		m_pDevice->SetStreamSource(0,m_pCubeVB,0,sizeof(D3DVERTEXXYZRGBA));
		m_pDevice->SetIndices(m_pCubeIB);
		m_Rot.Set(0.f,0.f,0.f);

		float scale = .5f;
		if (fftval>.2f) scale = .6f;

		if (fftval>.1f) 
			for (int i=0; i<CUBECOUNT; i++)
				cubes[i].rotz += .1f;
		
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		scale+=.1f;
		for (int i=0; i<CUBECOUNT; i++) {			
			m_Pos.x = cubes[i].x;//-sin(m_PlayinVal)*20.f;
			m_Pos.y = cubes[i].y;
			m_Pos.z = cubes[i].z-(100-m_PlayinVal);

			m_Rot.Set(cubes[i].rotx,cubes[i].roty,cubes[i].rotz);

			pos.Set(m_Rot,m_Pos,scale);
			mat.Set(pos);
			m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
			
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);
		}
		
		scale-=.1f;
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		for (int i=0; i<CUBECOUNT; i++) {
			m_Pos.x = cubes[i].x;//+sin(m_PlayinVal)*20.f;
			m_Pos.y = cubes[i].y;
			m_Pos.z = cubes[i].z-(100-m_PlayinVal);

			m_Rot.Set(cubes[i].rotx,cubes[i].roty,cubes[i].rotz);

			pos.Set(m_Rot,m_Pos,scale);
			mat.Set(pos);
			m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
			
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);


			//away
			/*
			cubes[i].z += i*(fftval*.005f);

			if (cubes[i].z > 50.f) {
				cubes[i].z = -30.f+(((float)rand()/(float)RAND_MAX)*10.f);
				float val = -3.5f+(((float)rand()/(float)RAND_MAX)*7.f);;
				cubes[i].x = sin(val)*2.f;
				cubes[i].y = cos(val)*2.f;
				cubes[i].rotx = val;
				cubes[i].roty = val;
				cubes[i].rotz = val;
			}
			*/
			//towards
			cubes[i].z -= i*(fftval*.005f);

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

		/*
		if (fftval>.2f) {
			m_Pos.x = 0.f;
			m_Pos.y = 0.f;
			m_Pos.z = 0.f;
			pos.Set(m_Rot,m_Pos,10.f);
			mat.Set(pos);
			m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);
		}
		*/
	}

	bool PlayIn(float fftval, float timeval) {
		if (!m_IsInit) {
			for (int i=0; i<CUBECOUNT; i++) {
				cubes[i].x=(float)i;
				cubes[i].y=1000.f;
				//away
				//cubes[i].z=9999.f;
				//towards
				cubes[i].z = 0.f+(((float)rand()/(float)RAND_MAX)*100.f);
				float val = -3.5f+(((float)rand()/(float)RAND_MAX)*7.f);;
				cubes[i].x = sin(val)*3.f;
				cubes[i].y = cos(val)*2.f;
				cubes[i].rotx = val;
				cubes[i].roty = val;
				cubes[i].rotz = val;
			}
			m_IsInit=true;
		}

		m_PlayinVal+=timeval*0.1f;
		if (m_PlayinVal>=100.f) {
			m_PlayinVal=100.f;
			return false;
		}

		Render(fftval,timeval);

		return true;
	}

	bool PlayOut(float fftval, float timeval) {
		m_IsInit=false;
		if (m_pDevice==NULL) return false;

		for (int i=0; i<CUBECOUNT; i++) {
			cubes[i].y-=(float)(i+10)*0.01f;
		}

		if (cubes[0].y <= -10.f) {
			m_PlayinVal=0.f;
			return false;
		}

		Render(fftval,timeval);

		return true;
	}

	LPDIRECT3DVERTEXBUFFER9 m_pCubeVB;
	LPDIRECT3DINDEXBUFFER9	m_pCubeIB;

private:
	bool m_IsInit;
	float m_PlayinVal;

	float m_fGlobRot;
	mycube cubes[CUBECOUNT];

	CNEVector3 m_Pos;
  CNEEuler m_Rot;
	CNEVector3 m_Scal;
	CNEPosition pos;
  CNEMatrix4 mat;
	LPDIRECT3DDEVICE9 m_pDevice;
};
