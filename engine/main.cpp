#include <QDebug>
#include <cstdlib>
#include <vector>
using namespace std;
#include "engine.h"

vector<string> genes;

int main()
{
	srand(time(0));
	DGEBC::Engine e;
	std::string gene = e.initial();
	double best_score = e.score(gene);
	genes.push_back(gene);
	for(int _ = 0;; _++)
	{
		if(_ % 10 == 0) qDebug() << "iter" << _;
		std::string new_gene = e.mutate(genes[rand() % genes.size()]);
		double new_score = e.score(new_gene);
		if(new_score > best_score)
		{
			best_score = new_score;
			qDebug() << "mutate" << _ << best_score << new_gene.c_str();
			genes.push_back(new_gene);
			goto finish;
		}
		else for(const std::string &other: genes)
		{
			std::string combined_gene = e.combine(new_gene, other);
			double combined_score = e.score(combined_gene);
			if(combined_score > best_score)
			{
				best_score = combined_score;
				qDebug() << "combine" << _ << best_score << combined_gene.c_str();
				genes.push_back(combined_gene);
				goto finish;
			}
		}
		// genes.push_back(new_gene);
	finish:;
	}
}
