#pragma once

#include "chessmen.h"

class pawn : public chessmen {
	public:
		void move() override;
		void attack() override;
};