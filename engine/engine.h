#ifndef DGEBC_ENGINE_H
#define DGEBC_ENGINE_H

#include <string>

#ifndef DGEBC_ENGINE_NO_VISUALIZE
#include <QLayout>
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
		input_t initial();
		input_t mutate(const input_t &in);
#ifndef DGEBC_ENGINE_NO_VISUALIZE
		QLayout *visualize(const input_t &in);
#endif // !DGEBC_ENGINE_NO_VISUALIZE

		static constexpr const int CHROME_LEN = 40;
		typedef float chrome_arr_t[40];
	private:
		void gene_to_chrome(const input_t &gene, chrome_arr_t chrome);
		input_t chrome_to_gene(chrome_arr_t chrome);
	};
}

#endif // !DGEBC_ENGINE_H
