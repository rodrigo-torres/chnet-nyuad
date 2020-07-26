/*
 * performance.h
 *
 *  Created on: Oct 24, 2019
 *      Author: Rodrigo Torres
 */

#ifndef SRC_NAMESPACES_H_
#define SRC_NAMESPACES_H_
#include <stdlib.h>
#include <iostream>
#include <random>


class multidet
{
public:
	multidet(double, double);
	int distribute(int);
private:
	double jlo;
	double jhi;
	uint n_overlap;
	const double m;
	const double c;
	std::random_device r;
	std::default_random_engine e1;
	std::discrete_distribution<int> discrete_dist;
};

multidet::multidet(double _m, double _c) : m(_m),c(_c)
{
	jlo = 0;
	jhi = 1;
	n_overlap = ceil(jhi) - floor(jlo);
	std::default_random_engine e1(r());
	std::discrete_distribution<int> discrete_dist({1, 1});
}

int multidet::distribute(int j)
{
	jlo = (double)(j) * m + c;
	jhi = jlo + m;
	n_overlap = ceil(jhi) - floor(jlo);
	if (n_overlap == 1)
	{
		return (int)jlo;
	}

	std::vector<double> weights;
	weights.push_back(ceil(jlo) - jlo);

	while (--n_overlap)
	{
		//We construct a probability array { p_begin, p_mid, p_mid, ..., p_end }
		switch(n_overlap)
		{
		case 1:
			weights.push_back(jhi - floor(jhi));
			break;
		default:
			weights.push_back(1);
			break;
		}
	}
	std::discrete_distribution<int> discrete_dist(weights.begin(), weights.end());
	std::vector<double> p = discrete_dist.probabilities();
	return (int)jlo + discrete_dist(e1);
}



#endif /* SRC_NAMESPACES_H_ */

