#ifndef MAX6675_H_
#define MAX6675_H_

#include "Arduino.h"

class Max6675
{
public:
	Max6675(uint8_t pin_dt, uint8_t pin_ss, uint8_t pin_clk, int offset = 0);
	virtual ~Max6675();
	float getCelsius();
	float getFahrenheit();
	float getKelvin();
	int getValue();
	void setOffset(int offset);

private:
	const uint8_t _pin_so;
	const uint8_t _pin_cs;
	const uint8_t _pin_clk;
	int _offset;


};

#endif /* MAX6675_H_ */
