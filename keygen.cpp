/**************************************************************
 *       Author: Joseph Kirk								  *
 *         Date: 05/12/17                                     *
 *         Name: Keygen.cpp                                   *
 *  Description: An implementation of the keygen function     *
 *               used by Coron et al (CRYPTO 2011) for use    *
 *               with Vivado HLS                              *
 *************************************************************/

#include "ap_int.h"
#include "hls_math.h"
#define AP_INT_MAX_W 5120


//structs to keep the keys in
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

typedef struct sk
{
	bool s0[12];
	bool s1[12];
} sk;

typedef struct keys
{
	pk pk;
	pkstar pkstar;
	sk sk;
} keys;

//returns a single PRNG bit
//adapted from Schneier's Applied Cryptography
//using a primitive polynomial mod 2 with taps on
//bits 128, 7, 2, 1, 0
bool lfsr(ap_uint<128> seed, bool seeded)
{
	static ap_uint<128> lfsr;
	lfsr = 1;

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


//checks if a number is prime
//(for use in generating primes)
bool isPrime(ap_uint<1764> num)
{
	if(num % 2 == 0 | num < 2)
		return false;

	ap_uint<882> root = sqrt(num);

	bool prime = true;

	for(int i = 3; i < root; i += 2)
	{
		if(num % i == 0)
		{
			prime = false;
			break;
		}
	}

	return prime;
}


//finds the nearest prime number to a given number
//(for use in generating primes)
ap_uint<1764> nearestPrime(ap_uint<1764> num)
{
	if(isPrime(num))
		return num;

	for(int i = (num % 2 + 1); i < num; i++)
	{
		if(isPrime(num+i))
			return num+i;

		if(isPrime(num-i))
			return num-i;
	}

	//if no prime was found, return 1
	return 1;
}


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
	ap_uint<3528> q0;
	ap_uint<1764> prime1 = 0;
	ap_uint<1764> prime2 = 0;

	for(int i = 0; i < 1764; i++)
	{
		prime1 += lfsr(0, 0);
		prime1 << 1;
		prime2 += lfsr(0, 0);
		prime2 << 1;
	}

	if(!isPrime(prime1))
		prime1 = nearestPrime(prime1);

	if(!isPrime(prime2))
		prime2 = nearestPrime(prime2);

	q0 = prime1*prime2;

	// let x0 = q0*p
	ap_int<4616> x0;
	x0 = q0*p;
}

