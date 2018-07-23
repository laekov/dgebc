#include "engine.h"

namespace DGEBC
{
	typedef Engine::input_t input_t;
	typedef Engine::output_t output_t;
	output_t Engine::score(const input_t &in)
	{
		return 0;
	}
	input_t Engine::combine(const input_t &in1, const input_t &in2)
	{
		return in1;
	}
	input_t Engine::mutate(const input_t &in)
	{
		return in;
	}
#ifndef DGEBC_ENGINE_NO_VISUALIZE
	QWidget *visualize(const input_t &in)
	{
		return new QWidget();
	}
#endif // !DGEBC_ENGINE_NO_VISUALIZE
}
