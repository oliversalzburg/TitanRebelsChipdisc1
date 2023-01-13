#pragma once

class CGFxTemplate : public CGFx {
public:
	CGFxTemplate(void) {
		m_pVertexBufferVB=NULL;
		m_pIndexBufferIB=NULL;
		m_pDevice=NULL;
	}
	CGFxTemplate(LPDIRECT3DDEVICE9 device) {
		m_pVertexBufferVB=NULL;
		m_pIndexBufferIB=NULL;
		m_pDevice=device;

		CreateBuffers();
	}
	~CGFxTemplate(void) {
		SAFE_RELEASE(m_pVertexBufferVB);
		SAFE_RELEASE(m_pIndexBufferIB);
		return;
	}

	void CreateBuffers() {
		if (m_pDevice==NULL) return;

		//here you should create and fill your buffers

		/*
		D3DVERTEXXYZRGBAN aSphereVertices[(SPHEREPRECISION+1)*(SPHEREPRECISION/2)+(SPHEREPRECISION/2)];

		m_pDevice->CreateVertexBuffer(sizeof(aSphereVertices),D3DUSAGE_WRITEONLY,D3DFVF_XYZRGBAN,D3DPOOL_MANAGED,&m_pSphereVB,NULL);

		VOID* pData=NULL;
		m_pSphereVB->Lock(0,sizeof(pData),(void**)&pData,0);
		memcpy(pData,aSphereVertices,sizeof(aSphereVertices));
		m_pSphereVB->Unlock();
		*/

	}

	void Render(float fftval, float timeval) {
		if (m_pDevice==NULL) return;
	
		//do rendering shit here

		//set renderstates
		m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
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

		//set transformation
		m_pDevice->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);
		
		//set stream and stream-format and draw
		m_pDevice->SetFVF(D3DFVF_XYZRGBAN);
		m_pDevice->SetStreamSource(0,m_pVertexBufferVB,0,sizeof(D3DVERTEXXYZRGBAN));
		m_pDevice->SetIndices(m_pCubeIB);
		m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);
	}

	bool PlayIn() {
		return false;
	}

	bool PlayOut() {
		if (m_pDevice==NULL) return false;

		//if (done) return false;

		return true;
	}

private:
	//transformation data
	CNEVector3 m_Pos;
  CNEEuler m_Rot;
	CNEVector3 m_Scal;
	CNEPosition pos;
  CNEMatrix4 mat;

	LPDIRECT3DDEVICE9 m_pDevice; //pointer to the direct3d device

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBufferVB; //this effects vertex buffer
	LPDIRECT3DINDEXBUFFER9 m_pIndexBufferIB; //this effects index buffer
};
