#pragma once

#include <iostream>;


enum class Color : uint8_t
{
	Green,
	Red,
	Blue,
	Yellow,
	Reset
};

std::string ColorCode(Color pColor)
{
	switch (pColor)
	{
	case Color::Green:
		return "\x1b[32m";
	case Color::Red:
		return "\x1b[31m";
	case Color::Blue:
		return "\x1b[34m";
	case Color::Yellow:
		return "\x1b[33m";
	case Color::Reset:
		return "\033[0m";
	default:
		std::cerr << "Farbe nicht unterstützt";
		return "\x1b[37m";
	}
}
