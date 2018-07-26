#include "engine.h"
#include "world.h"
#include <sstream>
#include <cstdlib>
#include <iomanip>

std::string engine_input_gene;

namespace DGEBC
{
	typedef Engine::input_t input_t;
	typedef Engine::output_t output_t;
	typedef Engine::chrome_arr_t chrome_arr_t;
	static constexpr const int CHROME_LEN = Engine::CHROME_LEN;
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
		chrome_arr_t chrome;
		for(int i = 0; i < CHROME_LEN; i++)
			chrome[i] = 1. * rand() / RAND_MAX;
		return chrome_to_gene(chrome);
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
	void Engine::gene_to_chrome(const input_t &gene, chrome_arr_t chrome)
	{
		for(int i = 0; i < CHROME_LEN; i++)
		{
			unsigned tmp = stoll(gene.substr(i * 8, 8), NULL, 16);
			chrome[i] = *(float *) &tmp;
		}
	}
	input_t Engine::chrome_to_gene(chrome_arr_t chrome)
	{
		using namespace std;
		stringstream stream;
		stream << setfill('0') << setw(8) << hex;
		for(int i = 0; i < CHROME_LEN; i++)
			stream << *(unsigned *) &chrome[i];
		return stream.str();
	}
#endif // !DGEBC_ENGINE_NO_VISUALIZE
}
