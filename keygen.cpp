/**************************************************************
 *       Author: Joseph Kirk								  *
 *         Date: 05/12/17                                     *
 *         Name: Keygen.cpp                                   *
 *  Description: An implementation of the keygen function     *
 *               used by Coron et al (CRYPTO 2011) for use    *
 *               with Vivado HLS                              *
 *************************************************************/

#include "ap_int.h"
#define AP_INT_MAX_W 2048

//returns a single PRNG bit
//adapted from Schneier's Applied Cryptography
//using a primitive polynomial mod 2 with taps on
//bits 128, 7, 2, 1, 0
bool lfsr(ap_uint<128> seed, bool seeded)
{
	static ap_uint<128> lfsr;

	if(seeded == true)
		lfsr = seed;
	lfsr = ((((lfsr >> 127)
			^ (lfsr >> 6)
			^ (lfsr >> 1)
			^ (lfsr))
			& 0x00000001)
			<< 127)
			| (lfsr >> 1);

	return lfsr & 0x00000001;
}


typedef struct sk
{
	bool s0[12];
	bool s1[12];
} sk;

typedef struct pkstar
{
	ap_uint<1000> x0[160];
	ap_uint<1000> x[12][2][160];
	//12 pairs xi,0 xi,1, each 160000 bits
} pkstar;

typedef struct pk
{
	pkstar pkstar;
	ap_uint<128> seed;
	ap_uint<51> u11; //u[1][1] from the calculations
	ap_uint<1000> cs0[12][160]; //160000-bit integer array cs0[12]
	ap_uint<1000> cs1[12][160]; //160000-bit integer array cs1[12]
} pk;

typedef struct keys
{
	pk pk;
	pkstar pkstar;
	sk sk;
} keys;


keys main(void)
{
	// generate random, odd, 1088-bit p
	ap_uint<1088> p = 0;
	for(int i = 0; i < 1088; i++)
	{
		p += lfsr(0, 0);
		p << 1;
	}
	if(p % 2 == 0)
		p++;

	//pick 0 geq q0 geq 2^1.6x10^5 /p, product of two 1764-bit primes


}
