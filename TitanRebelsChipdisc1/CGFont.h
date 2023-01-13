#pragma once

struct CGFontLetter {
	float xoffset;
	float yoffset;
	float width;
};

struct CGFONTDRAWRESULT {
	long lCharDrawnFirst; //First character actually drawn
	long lCharDrawnLast; //Last character actually drawn
	float fOffsetFirstChar; //Drawing offset for the first character actually drawn
};

class CGFont {
public:
	CGFont(void);
	~CGFont(void);

	int LoadFontFromFile(unsigned char*& output, int& width, int& height, const char* filename);
	CGFONTDRAWRESULT DrawText(LPDIRECT3DDEVICE9 device, const char* text, long length, float x, float y, DWORD color, int fadeoutthres=65000);
	int DrawTextGDI(const char* text, long length, HDC src, HDC dest, int x, int y);
	int DrawSineText(LPDIRECT3DDEVICE9 device, const char* text, float x, float y, float sine, float width, float amp);
	void SetLetter(char letter, float xoffset, float yoffset, float width);
	long GetStrSize(char* str);

	int m_CharHeight;
	CGFontLetter m_Letters[256];

	int m_TexWidth;
	int m_TexHeight;
};
