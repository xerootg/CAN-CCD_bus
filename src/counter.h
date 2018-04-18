#include <stdint.h>

class Counter {
public:
	void init();
	Counter* pointer()       { return this; }
	const Counter* pointer() const { return this; }
	int getPulses();
  void setCount();
	void setCount(int pulses);
	float getDistance_miles();
	void resetDistance();
	float getSpeed_MPH(int ms);
	float getSpeed_MPH();
	void setTime(int ms);
	int getTime();
	void resetTime();
private:
	float getDistance_miles(int pulses);
  void doUpdates();
	friend void ftm0_isr(void);
	int time;
	int distancePulses;
	int speedPulses;
};
