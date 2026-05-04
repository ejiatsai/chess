#pragma once

class chessmen {
	public:
		virtual ~chessmen() {};
		virtual void move() = 0;
		virtual void attack() = 0;
};