#include "engine.h"
#include "world.h"
#include <sstream>
#include <cstdlib>
#include <iomanip>

#ifndef DGEBC_ENGINE_NO_VISUALIZE
#include <QLabel>
#include "mainwindow.h"
#endif // !DGEBC_ENGINE_NO_VISUALIZE

namespace DGEBC
{
	typedef Engine::input_t input_t;
	typedef Engine::output_t output_t;
	typedef Engine::chrome_arr_t chrome_arr_t;
	static constexpr const int CHROME_LEN = Engine::CHROME_LEN;
	using namespace std;
	output_t Engine::score(const input_t &in, int* cnt_ptr)
	{
		static int tmp;
		if (!cnt_ptr) {
			cnt_ptr = &tmp;
		}
		try
		{
			World world(false, in);
			while(1) { 
				world.step(); 
				++ *cnt_ptr; 
			}
		}
		catch(float f)
		{
			return f;
		}
	}
	input_t Engine::combine(const input_t &in1, const input_t &in2)
	{
		chrome_arr_t c1, c2;
		gene_to_chrome(in1, c1);
		gene_to_chrome(in2, c2);
		int l = rand() % CHROME_LEN, r = rand() % CHROME_LEN;
		for(int i = l;; i = (i + 1) % CHROME_LEN)
		{
			c1[i] = c2[i];
			if(i == r) break;
		}
		return chrome_to_gene(c1);
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
		chrome_arr_t chrome;
		gene_to_chrome(in, chrome);
		chrome[rand() % CHROME_LEN] = 1. * rand() / RAND_MAX;
		return chrome_to_gene(chrome);
	}
#ifndef DGEBC_ENGINE_NO_VISUALIZE
	QLayout *Engine::visualize(const input_t &in)
	{
		QVBoxLayout *lo = new QVBoxLayout();
		lo->addWidget(new MainWindow(in, NULL));
		return lo;
	}
#endif // !DGEBC_ENGINE_NO_VISUALIZE
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
}
