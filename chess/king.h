#pragma once

#include "chessmen.h"

class king : public chessmen{
	public:
		void move() override;
		void attack() override;
};