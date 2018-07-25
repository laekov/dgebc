#include "engine.h"
#include "world.h"

std::string engine_input_gene;

namespace DGEBC
{
	typedef Engine::input_t input_t;
	typedef Engine::output_t output_t;
	using namespace std;
	output_t Engine::score(const input_t &in)
	{
		engine_input_gene = in;
		try
		{
			World world;
			while(1) world.step();
		}
		catch(float f)
		{
			return f;
		}
	}
	input_t Engine::combine(const input_t &in1, const input_t &in2)
	{
		return in1;
	}
	input_t Engine::initial()
	{
		return "3f7e34b73de1f23f3f2e82c83f020de03ea46e673f43cea53eea79fa3f3afb513f07c6be3e3e5e1d3f2026903f1472463d5ef3c23e9355a53e4d8d753e0ba1453ee648843e002f3b3f1481213f40c9dc3f5ec4cb3db678e93e78b8233f2c98ec3e01f3753f1301163e9ada783ec0051c3f5975043f1cd1b43ecb93543df34e603e73405f3f79c6c33edbcce53e6aa3103f67d6c73e80684e3f240dc33f1687f5";
	}
	input_t Engine::mutate(const input_t &in)
	{
		return in;
	}
#ifndef DGEBC_ENGINE_NO_VISUALIZE
	QLayout *Engine::visualize(const input_t &in)
	{
		return new QVBoxLayout();
	}
#endif // !DGEBC_ENGINE_NO_VISUALIZE
}
