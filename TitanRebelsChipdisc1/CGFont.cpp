#include "StdAfx.h"
#include "Main.h"

CGFont::CGFont(void) {
	ZeroMemory(&m_Letters,sizeof(m_Letters));
}

CGFont::~CGFont(void) {
}

int CGFont::LoadFontFromFile(unsigned char*& output, int& width, int& height, const char* filename) {
	FILE* fp=NULL;
	fp = fopen(filename,"r");


	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	fread(&fileheader,sizeof(BITMAPFILEHEADER),1,fp);
	fread(&infoheader,sizeof(BITMAPINFOHEADER),1,fp);
	
	unsigned char* data=NULL;
	long datasize=infoheader.biSizeImage;
	data=(unsigned char*)malloc(datasize);
	
	m_TexWidth = width = infoheader.biWidth;
	m_TexHeight = height = infoheader.biHeight;

	//fseek(fp,1024,SEEK_CUR);
	fseek(fp,fileheader.bfOffBits,SEEK_SET);
	
	fread(data,datasize,1,fp);

	//alloc target
	//long outputsize=datasize * infoheader.biBitCount;
	long outputsize=infoheader.biWidth * infoheader.biHeight * 4;
	output = (unsigned char*)malloc(outputsize);
	
	//bgra order
	long m=0;
	long l=0;
	for (int y=0; y<height; y++) {
		for (int x=0; x<width; x++) {
			//l = x+y*width;
			if (data[l] == 0xfd) {
				//transparent background
				output[m+0] = 255;
				output[m+1] = 255;
				output[m+2] = 255;
				output[m+3] = 0;
			} else if (data[l] == 0x00) {
				//black
				output[m+0] = 0;
				output[m+1] = 0;
				output[m+2] = 0;
				output[m+3] = 255;
			} else if (data[l] == 0xff) {
				//white
				output[m+0] = 255;
				output[m+1] = 255;
				output[m+2] = 255;
				output[m+3] = 255;
			} else {
				//markers (unprocessed)
				output[m+0] = 255;
				output[m+1] = 255;
				output[m+2] = 255;
				output[m+3] = 0;
			}
			m+=4;
			l+=infoheader.biBitCount/8;
		}
		//add padding
		l+=((width*infoheader.biBitCount/8)%4);
	}

	fclose(fp);

	return 0;
}

CGFONTDRAWRESULT CGFont::DrawText(LPDIRECT3DDEVICE9 device, const char* text, long length, float x, float y, DWORD color, int fadeoutthres) {
	CGFONTDRAWRESULT ret={0,0,0.f};

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

	//set renderstates
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	device->SetRenderState(D3DRS_TEXTUREFACTOR,color);

	//set texturestages
	device->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
	device->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	device->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TFACTOR);


	//handle alpha
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	long alpha = color>>24&0xff;
	long red = RGB_GETRED(color);
	long green = RGB_GETGREEN(color);
	long blue = RGB_GETBLUE(color);
	float offset=0.f;
	
	device->SetFVF(D3DFVF_CUSTOMVERTEXORTHO);

	for (long i=0; i<length; i++) {
		if (offset + x + (float)m_Letters[text[i]].width<0.f) {
			offset+=(float)m_Letters[text[i]].width;
			ret.lCharDrawnFirst++;
			ret.fOffsetFirstChar = offset;
			continue;
		}
		if (offset + x > 600.f) {
			ret.lCharDrawnLast = max(0,i-1);
			break;
		}

		D3DVERTEXORTHO vdata[4];
		vdata[0].x = offset + x;
		vdata[0].y = y;
		vdata[0].z = 0.5f;
		vdata[0].rhw = .5f;
		vdata[0].u = ((float)m_Letters[text[i]].xoffset -.5f) / (float)m_TexWidth;
		vdata[0].v = ((float)m_Letters[text[i]].yoffset -.5f) / (float)m_TexHeight;

		vdata[1].x = offset + x + (float)m_Letters[text[i]].width;
		vdata[1].y = y;
		vdata[1].z = 0.5f;
		vdata[1].rhw = .5f;
		vdata[1].u = ((float)m_Letters[text[i]].xoffset -.5f+ (float)m_Letters[text[i]].width) / (float)m_TexWidth;
		vdata[1].v = ((float)m_Letters[text[i]].yoffset -.5f) / (float)m_TexHeight;

		vdata[2].x = offset + x + (float)m_Letters[text[i]].width;
		vdata[2].y = y + (float)m_CharHeight;
		vdata[2].z = 0.5f;
		vdata[2].rhw = .5f;
		vdata[2].u = ((float)m_Letters[text[i]].xoffset -.5f + (float)m_Letters[text[i]].width) / (float)m_TexWidth;
		vdata[2].v = ((float)m_Letters[text[i]].yoffset -.5f + (float)m_CharHeight) / (float)m_TexHeight;

		vdata[3].x = offset + x;
		vdata[3].y = y + (float)m_CharHeight;
		vdata[3].z = 0.5f;
		vdata[3].rhw = .5f;
		vdata[3].u = ((float)m_Letters[text[i]].xoffset -.5f) / (float)m_TexWidth;
		vdata[3].v = ((float)m_Letters[text[i]].yoffset -.5f + (float)m_CharHeight) / (float)m_TexHeight;

		device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,&vdata,sizeof(D3DVERTEXORTHO));

		offset+=(float)m_Letters[text[i]].width;
		if (offset > fadeoutthres) {
			alpha = alpha-((long)offset-fadeoutthres);
			if (alpha<0) alpha = 0;
			//D3DCOLOR_ARGB();
			color=D3DCOLOR_ARGB(alpha,red,green,blue);
			device->SetRenderState(D3DRS_TEXTUREFACTOR,color);
		}
	}

	return ret;
}

int CGFont::DrawTextGDI(const char* text, long length, HDC src, HDC dest, int x, int y) {
	long offset=0; 

	for (long i=0; i<length; i++) {
		if (offset + x + (float)m_Letters[text[i]].width<90.f) {
			offset+=(float)m_Letters[text[i]].width;
			continue;
		}
		if (offset + x > 560.f) {
			break;
		}

		BitBlt(dest,x+offset,y,m_Letters[text[i]].width,(float)m_CharHeight,src,m_Letters[text[i]].xoffset,m_Letters[text[i]].yoffset,SRCPAINT );

		offset+=(float)m_Letters[text[i]].width;
	}
	return 0;
}

int CGFont::DrawSineText(LPDIRECT3DDEVICE9 device, const char* text, float x, float y, float sine, float width, float amp) {
	//set renderstates
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	//handle alpha
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);

	//set texturestages
	device->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	device->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TEXTURE);
	
	//set stream and stream-format and draw
	device->SetFVF(D3DFVF_XYZVU);

  CNEVector3 m_Pos;
	m_Pos.x = -15.f;
	m_Pos.y = 115.f;
	m_Pos.z = 20.f;
  
	CNEVector3 m_Scal;
	m_Scal.x = 1.f;
	m_Scal.y = 1.f;
	m_Scal.z = 1.f;

	CNEEuler m_Rot;
	m_Rot.Set(-PI,PI/2.f,0.f);

	CNEPosition pos;
	pos.Set(m_Rot,m_Pos,m_Scal);

	CNEMatrix4 mat;
	mat.Set(pos);

	device->SetTransform(D3DTS_WORLD,(D3DMATRIX*)&mat);

	float offset=0.f;
	float yoffset=0.f;
	for (int i=0; i<(int)strlen(text); i++) {
		yoffset = sin(sine+(float)i/width)*amp;
		D3DVERTEXXYZVU vdata[4];
		vdata[0].x = offset + x;
		vdata[0].y = yoffset + y;
		vdata[0].z = 0.5f;
		vdata[0].u = (float)m_Letters[text[i]].xoffset / (float)m_TexWidth;
		vdata[0].v = (float)m_Letters[text[i]].yoffset / ((float)m_TexHeight-.5f);

		vdata[1].x = offset + x + (float)m_Letters[text[i]].width;
		vdata[1].y = yoffset + y;
		vdata[1].z = 0.5f;
		vdata[1].u = ((float)m_Letters[text[i]].xoffset + (float)m_Letters[text[i]].width) / (float)m_TexWidth;
		vdata[1].v = (float)m_Letters[text[i]].yoffset / ((float)m_TexHeight-.5f);

		vdata[2].x = offset + x + (float)m_Letters[text[i]].width;
		vdata[2].y = yoffset + y + (float)m_CharHeight;
		vdata[2].z = 0.5f;
		vdata[2].u = ((float)m_Letters[text[i]].xoffset + (float)m_Letters[text[i]].width) / (float)m_TexWidth;
		vdata[2].v = ((float)m_Letters[text[i]].yoffset + (float)m_CharHeight) / ((float)m_TexHeight-.5f);

		vdata[3].x = offset + x;
		vdata[3].y = yoffset + y + (float)m_CharHeight;
		vdata[3].z = 0.5f;
		vdata[3].u = (float)m_Letters[text[i]].xoffset / (float)m_TexWidth;
		vdata[3].v = ((float)m_Letters[text[i]].yoffset + (float)m_CharHeight) / ((float)m_TexHeight-.5f);

		device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,&vdata,sizeof(D3DVERTEXXYZVU));

		offset+=(float)m_Letters[text[i]].width;
	}

	return 0;
}

void CGFont::SetLetter(char letter, float xoffset, float yoffset, float width) {
	m_Letters[letter].xoffset = xoffset;
	m_Letters[letter].yoffset = yoffset;
	m_Letters[letter].width = width;
	return;
}

long CGFont::GetStrSize(char* str) {
	long ret=0;
	for (int i=0; i<(int)strlen(str); i++)
		ret+=(long)m_Letters[str[i]].width;
	return ret;
}