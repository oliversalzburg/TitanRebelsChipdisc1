#include "StdAfx.h"
#ifdef _DEBUG
#include "mmgr/mmgr.h"
#else
#include "mmgr/nommgr.h"
#endif
#include "Main.h"

CGTimer::CGTimer() {
  m_iBPM=0;
  m_fExTime=0.f;

  LARGE_INTEGER tmpFreq;
  QueryPerformanceFrequency(&tmpFreq);

  m_TimerFreq = (double)tmpFreq.QuadPart;
}

CGTimer::~CGTimer() {
	return;
}

double CGTimer::GetTime() {
  LARGE_INTEGER NewTime;
  QueryPerformanceCounter(&NewTime);

  return ((double)NewTime.QuadPart)/(m_TimerFreq/1000.0);
}

double CGTimer::GetTimeDiff() {
  LARGE_INTEGER NewTime, DiffTime;
  QueryPerformanceCounter(&NewTime);

  if (m_OldTime.QuadPart == 0) DiffTime.QuadPart = 0;
  else DiffTime.QuadPart = NewTime.QuadPart - m_OldTime.QuadPart;
  m_OldTime.QuadPart = NewTime.QuadPart;

  return ((double)DiffTime.QuadPart)/(m_TimerFreq/1000.0);
}

void CGTimer::SetBPM(int bpm) {
  m_iBPM = bpm;
  return;
}

void CGTimer::SetExternalTime(float time) {
  m_fExTime = time;
  return;
}

float CGTimer::GetExternalTime() {
	return m_fExTime;
}

long CGTimer::GetQuarterBeat() {
  return (long)(m_fExTime/60.f*((float)(m_iBPM*4)));
}

long CGTimer::GetBeat() {
  return (long)(m_fExTime/60.f*((float)(m_iBPM)));
}

float CGTimer::GetBeatFloat() {
  return (m_fExTime/60.f*((float)(m_iBPM)));
}

float CGTimer::GetBarFloat() {
  return (m_fExTime/60.f*((float)(m_iBPM))/4.f);
}

long CGTimer::GetBar() {
  return (long)(m_fExTime/60.f*((float)m_iBPM)/4.f);
}

float CGTimer::GetBeatValue() {
  return (m_fExTime/60.f*((float)(m_iBPM)))-((float)GetBeat());
}

float CGTimer::GetBarValue() {
  return (m_fExTime/60.f*((float)(m_iBPM))/4.f)-((float)GetBar());
}