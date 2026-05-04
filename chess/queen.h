#pragma once

#include "chessmen.h"

class queen : public chessmen {
	public:
		void move() override;
		void attack() override;
};