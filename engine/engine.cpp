#include "engine.h"
#include <cstdlib>

namespace DGEBC
{
	typedef Engine::input_t input_t;
	typedef Engine::output_t output_t;
	using namespace std;
	output_t Engine::score(const input_t &in)
	{
		try
		{
			output_t x = stold(in);
			return 1 / (pow(x - 250, 2) + 1);
		}
		catch(exception e)
		{
			return 0;
		}
	}
	input_t Engine::combine(const input_t &in1, const input_t &in2)
	{
		output_t x = stold(in1), y = stold(in2);
		return to_string((x + y) / 2);
	}
	input_t Engine::mutate(const input_t &in)
	{
		output_t x = stold(in);
		return to_string(x - 10 + rand() * 20. / RAND_MAX);
	}
#ifndef DGEBC_ENGINE_NO_VISUALIZE
	QWidget *visualize(const input_t &in)
	{
		return new QWidget();
	}
#endif // !DGEBC_ENGINE_NO_VISUALIZE
}
