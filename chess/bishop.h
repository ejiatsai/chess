#pragma once

#include "chessmen.h"

class bishop : public chessmen {
	public:
		void move() override;
		void attack() override;
};