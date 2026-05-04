#pragma once

#include "chessmen.h"

class knight : public chessmen {
public:
	void move() override;
	void attack() override;
};