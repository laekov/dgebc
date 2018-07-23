#ifndef DGEBC_ENGINE_H
#define DGEBC_ENGINE_H

#include <string>

#ifndef DGEBC_ENGINE_NO_VISUALIZE
#include <QWidget>
#endif // !DGEBC_ENGINE_NO_VISUALIZE

namespace DGEBC
{
	class Engine
	{
	public:
		typedef std::string input_t;
		typedef double output_t;
		
		output_t score(const input_t &in);
		input_t combine(const input_t &in1, const input_t &in2);
		input_t mutate(const input_t &in);
#ifndef DGEBC_ENGINE_NO_VISUALIZE
		QWidget *visualize(const input_t &in);
#endif // !DGEBC_ENGINE_NO_VISUALIZE
	};
}

#endif // !DGEBC_ENGINE_H
