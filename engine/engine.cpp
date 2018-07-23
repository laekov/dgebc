#include "engine.h"
#include <cstdlib>

#ifndef DGEBC_ENGINE_NO_VISUALIZE
#include <QLabel>
#endif // !DGEBC_ENGINE_NO_VISUALIZE

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
			return 1000 / (pow(x - 250, 2) / 1000 + 1);
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
	input_t Engine::initial()
	{
		return to_string(rand() * 1000. / RAND_MAX);
	}
	input_t Engine::mutate(const input_t &in)
	{
		output_t x = stold(in);
		return to_string(x - 10 + rand() * 20. / RAND_MAX);
	}
#ifndef DGEBC_ENGINE_NO_VISUALIZE
	QLayout *Engine::visualize(const input_t &in)
	{
		QVBoxLayout *lo = new QVBoxLayout();
		lo->addWidget(new QLabel(("Gene: " + in).c_str()));
		lo->addWidget(new QLabel(QString("Score: %1").arg(score(in))));
		lo->addStretch();
		return lo;
	}
#endif // !DGEBC_ENGINE_NO_VISUALIZE
}
