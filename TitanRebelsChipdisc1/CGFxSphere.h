#pragma once

#define SPHEREPRECISION 20
#define TWOPI           6.283185307179586476925287f
#undef PI
#define PI              3.141592653589793238462643f
#define PID2            1.570796326794896619231322f

#define SPHERECOUNT 6

struct mysphere {
	float x,y,z;
	float fy;
	float dir;
	float val;
};

class CGFxSphere : public CGFx {
public:
	CGFxSphere(void) {
		m_pSphereVB=NULL;
	}
	CGFxSphere(LPDIRECT3DDEVICE9 device) {
		m_pSphereVB=NULL;
		m_pDevice=device;
		rot=0.f;
		zpos=0.f, ypos=0.f, xpos=0.f;
		direction=1.f;

		m_Spheres.AddElements(SPHERECOUNT);
		
		for (int i=0; i<SPHERECOUNT; i++) {
			m_Spheres[i].val = (float)i;
			m_DrawOrder[i] = i;
		}

		CreateBuffers();

		m_IsInit=false;
	}
	~CGFxSphere(void) {
		m_Spheres.DeleteAllElements();
		SAFE_RELEASE(m_pSphereVB);
		return;
	}

	void CreateBuffers() {
		if (m_pDevice==NULL) return;

		//D3DVERTEXXYZRGBAN aSphereVertices[(SPHEREPRECISION+1)*(SPHEREPRECISION/2)+(SPHEREPRECISION/2)];
		D3DVERTEXXYZRGBAN aSphereVertices[(SPHEREPRECISION)*(SPHEREPRECISION)+SPHEREPRECISION];

		//create sphere
		int i,j,v=0;
		double theta1,theta2,theta3;
		D3DVERTEXXYZRGBAN e,p;
		double r=1.0;

		for (j=0;j<SPHEREPRECISION/2;j++) {
      theta1 = j * TWOPI / SPHEREPRECISION - PID2;
      theta2 = (j + 1) * TWOPI / SPHEREPRECISION - PID2;

      for (i=0;i<=SPHEREPRECISION;i+=2) {
				theta3 = i * TWOPI / SPHEREPRECISION;

				e.x = (float)(cos(theta2) * cos(theta3));
				e.y = (float)sin(theta2);
				e.z = (float)(cos(theta2) * sin(theta3));
				p.x = (float)r * e.x;
				p.y = (float)r * e.y;
				p.z = (float)r * e.z;
				aSphereVertices[v].x = p.x;
				aSphereVertices[v].y = p.y;
				aSphereVertices[v].z = p.z;
				aSphereVertices[v].nx = e.x;
				aSphereVertices[v].ny = e.y;
				aSphereVertices[v].nz = e.z;
				aSphereVertices[v].rgba = D3DCOLOR_ARGB(180,180,180,180);
				v++;

				/*
				glNormal3f(e.x,e.y,e.z);
				glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
				glVertex3f(p.x,p.y,p.z);
				*/

				e.x = (float)(cos(theta1) * cos(theta3));
				e.y = (float)sin(theta1);
				e.z = (float)(cos(theta1) * sin(theta3));
				p.x = (float)r * e.x;
				p.y = (float)r * e.y;
				p.z = (float)r * e.z;
				aSphereVertices[v].x = p.x;
				aSphereVertices[v].y = p.y;
				aSphereVertices[v].z = p.z;
				aSphereVertices[v].nx = e.x;
				aSphereVertices[v].ny = e.y;
				aSphereVertices[v].nz = e.z;
				aSphereVertices[v].rgba = D3DCOLOR_ARGB(180,180,180,180);
				v++;

				/*
				glNormal3f(e.x,e.y,e.z);
				glTexCoord2f(i/(double)n,2*j/(double)n);
				glVertex3f(p.x,p.y,p.z);
				*/

      }
      //glEnd();
   }

		m_pDevice->CreateVertexBuffer(sizeof(aSphereVertices),D3DUSAGE_WRITEONLY,D3DFVF_XYZRGBAN,D3DPOOL_MANAGED,&m_pSphereVB,NULL);

		VOID* pData=NULL;
		m_pSphereVB->Lock(0,sizeof(pData),(void**)&pData,0);
		memcpy(pData,aSphereVertices,sizeof(aSphereVertices));
		m_pSphereVB->Unlock();

	}

	void Render(float fftval, float timeval) {
		if (m_pDevice==NULL) return;

		if (g_pBassLib->GetIsPaused() || g_pBassLib->GetIsStopped())
			fftval=0.0f;
	
		rot+=(0.01f+fftval/10.f*direction);
		
		zpos=sin(rot)*5.f+3.f;
		xpos=sin(rot/2.f)-.3f;
		ypos=sin(rot/3.f)/3.f;

		//m_Spheres[0].fy = fftval;

		//sort spheres by z order
		int temp;
		int switches = 0;
		do {
			switches = 0;
			for (int i=0; i<SPHERECOUNT-1; i++) {
				if (m_Spheres[m_DrawOrder[i]].z > m_Spheres[m_DrawOrder[i+1]].z) {
					temp = m_DrawOrder[i+1];
					m_DrawOrder[i+1] = m_DrawOrder[i];
					m_DrawOrder[i] = temp;
					switches++;
				}
			}
		} while (switches > 0);

		for (int i=0; i<SPHERECOUNT; i++) {
			//rotate
			m_Spheres[m_DrawOrder[i]].x = sin((float)g_pTimer->GetTime()/1000.f+m_Spheres[m_DrawOrder[i]].val) * 5.f;
			m_Spheres[m_DrawOrder[i]].z = cos((float)g_pTimer->GetTime()/1000.f+m_Spheres[m_DrawOrder[i]].val) * 5.f + 10.f;

			//own force
			m_Spheres[m_DrawOrder[i]].y += m_Spheres[m_DrawOrder[i]].fy + sin(m_PlayOutVal);// * m_Spheres[i].dir;

			//gravity
			//if (m_Spheres[i].y > 0.f) m_Spheres[i].y -= timeval/1000.f;

			//decrease force
			//if (m_Spheres[i].y >0.f) m_Spheres[i].fy -= (timeval/1000.f)*m_Spheres[i].dir;
			if (m_Spheres[m_DrawOrder[i]].y >0.f) m_Spheres[m_DrawOrder[i]].fy -= 0.001f*timeval/(fftval*10.f);
			
			/*
			if (m_Spheres[i].dir > 0.f) {
				if (m_Spheres[i].fy>0.f) m_Spheres[i].fy -= (timeval/10000.f);
				else m_Spheres[i].fy = 0.f;
			}
			*/
			
			//reverse direction on bump
			if (m_Spheres[m_DrawOrder[i]].y <= 0.f) {
				m_Spheres[m_DrawOrder[i]].dir*=-1.f;
				m_Spheres[m_DrawOrder[i]].y = 0.f;
			}

			//bounce ball if on ground
			if (m_Spheres[m_DrawOrder[i]].y <= 0.f) {
				//m_Spheres[i].fy = (abs(m_Spheres[i].fy)+((fftval+barvals[i+1])/2.f)-0.1f);
				m_Spheres[m_DrawOrder[i]].fy = 0.02f+((fftval+m_Spheres[m_DrawOrder[i]].val/10.f)/2.f);
			}
				

			static float trot=0.1f;
			trot+=.01f;
			m_Pos.x = m_Spheres[m_DrawOrder[i]].x+11.f+m_XOffset;
			m_Pos.y = m_Spheres[m_DrawOrder[i]].y-2.f;
			m_Pos.z = m_Spheres[m_DrawOrder[i]].z+4.f;
			m_Rot.Set(0.f,0.f,0.f);
			/*
			m_Scal.x = 1.f+fftval*3.f;
			m_Scal.y = 1.f+fftval*3.f;
			m_Scal.z = 1.f+fftval*3.f;
			*/
			float scal = 1.5f-((float)m_Spheres[m_DrawOrder[i]].val/(float)SPHERECOUNT)+.1f;
			scal -= m_PlayOutVal*0.08f;
			if (scal < 0.f) scal = 0.f;

			pos.Set(m_Rot,m_Pos,scal);
			mat.Set(pos);

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
			m_pDevice->SetStreamSource(0,m_pSphereVB,0,sizeof(D3DVERTEXXYZRGBAN));

			m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
			//m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,SPHEREPRECISION*(SPHEREPRECISION+1)-2);
			for (int j=0;j<SPHEREPRECISION/2;j++)
				m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,j*(SPHEREPRECISION+2),SPHEREPRECISION);
			
		}
		/*
		m_pDevice->SetRenderState(D3DRS_ZENABLE,false);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		if (fftval>.1f) {
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);
		}
		
		if (fftval>.2f) {
			pos.Set(m_Rot,m_Pos,10.f);
			mat.Set(pos);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTCOLOR);
			m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);
		}
		*/
		

	}

	bool PlayIn(float fftval, float timeval) {
		if (m_pDevice==NULL) return false;

		if (!m_IsInit) {
			for (int i=0; i<SPHERECOUNT; i++) {
				m_Spheres[i].x = i-2.5f;
				m_Spheres[i].y = 1.f;
				m_Spheres[i].z = i*2.f+2.f;
				m_Spheres[i].fy = .1f;
				m_Spheres[i].dir = 1.f;
				m_Spheres[i].val = (float)i;
			}
			m_XOffset=-30.f;
			m_PlayOutVal=0.f;
			m_IsInit = true;
		}

		m_XOffset+=timeval/100.f;

		Render(fftval, timeval);

		if (m_XOffset>=0.f) {
			m_XOffset=0.f;
			return false;
		}

		return true;
	}

	bool PlayOut(float fftval, float timeval) {
		m_IsInit=false;

		if (m_pDevice==NULL) return false;
		/*
		for (int i=0; i<SPHERECOUNT; i++) {
			m_Spheres[i].val = m_Spheres[i].val + timeval*0.005f;
			if (m_Spheres[i].val >= 17.f) 
				return false;
		}
		*/

		m_PlayOutVal+=timeval/100.f;
		if (m_PlayOutVal>=20.f) {
			m_PlayOutVal=0.f;
			return false;
		}
		
		Render(fftval, timeval);
		
		return true;
	}

	LPDIRECT3DVERTEXBUFFER9 m_pSphereVB;

private:
	float rot;
	float zpos, ypos, xpos;
	float direction;

	bool m_IsInit;
	float m_XOffset;
	float m_PlayOutVal;

	CNEVector3 m_Pos;
  CNEEuler m_Rot;
	CNEVector3 m_Scal;
	CNEPosition pos;
  CNEMatrix4 mat;
	LPDIRECT3DDEVICE9 m_pDevice;

	CGArray<mysphere> m_Spheres;
	int m_DrawOrder[SPHERECOUNT];
};
