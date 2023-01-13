class CGTimer {
  public:
  CGTimer();
  ~CGTimer();

  double GetTime(); //Returns the current count divided by the frequency
  double GetTimeDiff(); //Returns time difference to last call in seconds

  //Music values
  void SetBPM(int bpm);
  void SetExternalTime(float time);
	float GetExternalTime();

  //These will return values based on the extern time source
  long GetBar();
  long GetQuarterBeat();
  long GetBeat();
  float GetBeatFloat();
	float GetBarFloat();
  //BeatValue will always run from 0 to 1 between every beat
  float GetBeatValue();
	//BarValue will always run from 0 to 1 between every bar
  float GetBarValue();

  private:
  double m_TimerFreq; //Holds Counts per Second Frequency
  LARGE_INTEGER m_OldTime; //Holds count of last call

  int m_iBPM;
  float m_fExTime;
};