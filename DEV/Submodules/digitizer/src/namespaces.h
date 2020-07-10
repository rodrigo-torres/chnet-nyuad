/*
 * namespaces.h
 *
 *  Created on: Oct 24, 2019
 *      Author: Rodrigo Torres
 */

#ifndef SRC_NAMESPACES_H_
#define SRC_NAMESPACES_H_
#include <stdlib.h>
#include <iostream>
#include <random>

namespace conf
{
void set_params_channel1(CAEN_DGTZ_DPP_PHA_Params_t& DPPParams)
{
	DPPParams.thr[0]		= 880;	// Trigger Threshold
	DPPParams.k[0]			= 1100; // Trapezoid Rise Time (ns)
	DPPParams.m[0]			= 1000;	// Trapezoid Flat Top  (ns)
	DPPParams.M[0]			= 3100;	// Decay Time Constant (ns)
	DPPParams.ftd[0]		= 670;	// Peaking delay  (ns)
	DPPParams.a[0]			= 4; 	// Trigger Filter smoothing factor
	DPPParams.b[0]			= 200;	// Delay(b)
	DPPParams.trgho[0]		= 1300;	// Trigger Hold Off
	DPPParams.nsbl[0]		= 4;	// Baseline mean del trapezio (ordine di comparsa nel menu)
	DPPParams.nspk[0]		= 2;	// Peak mean (ordine di comparsa nel menu)
	DPPParams.pkho[0]		= 770;	// Peak holdoff
	DPPParams.blho[0]		= 100;	// Baseline holdoff del trapezio
	DPPParams.enf[0]		= 0.3;	// Energy Normalization Factor
	DPPParams.dgain[0]		= 0;	// Digital Gain (ordine di comparsa nel menu)
	DPPParams.decimation[0]	= 0;	// Decimation
}
void set_params_channel2(CAEN_DGTZ_DPP_PHA_Params_t& DPPParams)
{
	DPPParams.thr[1]		= 85;	// Trigger Threshold
	DPPParams.k[1]			= 810;// Trapezoid Rise Time (ns)
	DPPParams.m[1]			= 1000;	// Trapezoid Flat Top  (ns)
	DPPParams.M[1]			= 8000;	// Decay Time Constant (ns)
	DPPParams.ftd[1]		= 670;	// Peaking delay  (ns)
	DPPParams.a[1]			= 4; 	// Trigger Filter smoothing factor
	DPPParams.b[1]			= 100;	// Delay(b)
	DPPParams.trgho[1]		= 1300;	// Trigger Hold Off
	DPPParams.nsbl[1]		= 5;	// Baseline mean del trapezio (ordine di comparsa nel menu)
	DPPParams.nspk[1]		= 3;	// Peak mean (ordine di comparsa nel menu)
	DPPParams.pkho[1]		= 770;	// Peak holdoff
	DPPParams.blho[1]		= 100;	// Baseline holdoff del trapezio
	DPPParams.enf[1]		= 5;	// Energy Normalization Factor
	DPPParams.dgain[1]		= 0;	// Digital Gain (ordine di comparsa nel menu)
	DPPParams.decimation[1]	= 0;	// Decimation
}
}

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
