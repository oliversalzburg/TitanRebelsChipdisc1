#include "StdAfx.h"
#include "Main.h"

CGBassLib::CGBassLib(void) {
	m_bIsPaused=false;
	m_bIsStopped=true;
	m_iVolume=0;
}

CGBassLib::~CGBassLib(void) {
	BASS_Free();
}

BOOL CGBassLib::InitBass(HWND hparent) {
	BOOL retval = BASS_Init(1,44100,0,hparent,NULL);
	return retval;
}

int CGBassLib::PlayFile(char* filename) {
	PlaybackStop();

	m_dwChannel = BASS_MusicLoad(FALSE, filename, 0, 0, BASS_SAMPLE_LOOP|BASS_MUSIC_RAMP|BASS_MUSIC_PRESCAN, 0);
	SetVolume(m_iVolume);

	if (m_dwChannel==0) {
		DWORD error=BASS_ErrorGetCode();
		int sdf=0;
	}

	BASS_ChannelPlay(m_dwChannel,FALSE);
	m_bIsStopped=false;
	m_bIsPaused=false;
	return 0;
}

int CGBassLib::PlaybackPause() {
	if (m_dwChannel!=0) {
		if (!m_bIsStopped) {
			if (m_bIsPaused) {
				 BASS_ChannelPlay(m_dwChannel,false);
				 m_bIsPaused = false;
			} else {
				BASS_ChannelPause(m_dwChannel);
				m_bIsPaused=true;
			}
		}
	}
	return 0;
}
int CGBassLib::PlaybackStop() {
	if (m_dwChannel!=0) {
		BASS_ChannelStop(m_dwChannel);
		//BASS_MusicFree(m_dwChannel);
	}
	m_bIsStopped = true;
	return 0;
}
int CGBassLib::PlaybackPlay() {
	if (m_dwChannel!=0) {
		if (m_bIsPaused)
			BASS_ChannelPlay(m_dwChannel,false);
		else
			BASS_ChannelPlay(m_dwChannel,true);

		m_bIsStopped=false;
		m_bIsPaused=false;
	}
	return 0;
}

BOOL CGBassLib::SetVolume(int volume) {
	m_iVolume=volume;
	return BASS_ChannelSetAttributes(m_dwChannel,-1,volume,-101);
}

DWORD CGBassLib::GetVolume() {
	DWORD volume=-1;
	BASS_ChannelGetAttributes(m_dwChannel,NULL,&volume,NULL);
	return volume;
}

void CGBassLib::GetChannelData(float* fft) {
	BASS_ChannelGetData(m_dwChannel,fft,BASS_DATA_FFT2048);
	return;
}

QWORD CGBassLib::GetLength() {
	return BASS_ChannelGetLength(m_dwChannel);
}

QWORD CGBassLib::GetPosition() {
	return BASS_ChannelGetPosition(m_dwChannel);
}