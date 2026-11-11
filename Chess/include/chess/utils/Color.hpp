#pragma once

enum class Color
{
	White,
	Black
};

constexpr Color oppositeColor(Color color)
{
	return color == Color::White ? Color::Black : Color::White;
}
