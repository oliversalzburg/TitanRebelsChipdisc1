#pragma once

class CGFx {
public:

	CGFx(void) {}
public:

	virtual ~CGFx(void){}

	virtual void Render(float fftval, float timeval) { return; };
	virtual bool PlayIn(float fftval, float timeval) { return false; };
	virtual bool PlayOut(float fftval, float timeval) { return false; };
};
