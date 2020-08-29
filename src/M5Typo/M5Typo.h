#include "Arduino.h"

class M5Typo{
private: 
	int fontSize;

public:
	void setFontSize(int fSize);
	String getAlignedString(String input);
};