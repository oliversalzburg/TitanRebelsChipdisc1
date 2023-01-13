#pragma once

class CGBassLib {
public:
	CGBassLib(void);
	~CGBassLib(void);

	BOOL InitBass(HWND hparent);
	int PlayFile(char* filename);
	int PlaybackPause();
	int PlaybackStop();
	int PlaybackPlay();
	
	BOOL SetVolume(int volume);
	DWORD GetVolume();

	QWORD GetLength();
	QWORD GetPosition();


	//expects float[1024]
	void GetChannelData(float* fft);

	inline bool GetIsPaused() { return m_bIsPaused; };
	inline bool GetIsStopped() { return m_bIsStopped; };

private:
	DWORD m_dwChannel;
	bool m_bIsPaused;
	bool m_bIsStopped;

	int m_iVolume;
};
