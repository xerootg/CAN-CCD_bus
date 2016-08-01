#include "CAN.h"

class HaltechCAN: public CAN {
public:
	bool getUpdates();
	void busRecieve();
};