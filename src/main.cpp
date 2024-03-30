#include <bits/types/FILE.h>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>
#include "sieve.hpp"
#include "array"
#include "random"
#include <fstream>
#include <filesystem>
#include <string>

using namespace std;

struct Keys {
	array<mpz_t, 2> publicKey;
	array<mpz_t, 2> privateKey;
};

bool is_in_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

int GetLine(std::ifstream& file, int n) {
	int i = 0;
	string sLine;
	while (i != n && getline(file, sLine)) {
		++i;
	}

	if (i == n) {
		return stoi(sLine);
	}
	else {
		return -1;	
	}
}

// TODO use more optimized algorithm?
long long mod_pow(long long b, long long e, long long m) {
	if (m == 1) {
		return 0;
	}
	long long c = 1;
	for (int e_prime = 0; e_prime < e; e_prime++) {
		c = (c * b) % m;
	}
	return c;
}

long long extendedEuclidean(long long a, long long n) {
	long long t = 0;
	long long newt = 1;
	long long r = n;
	long long newr = a;

	while (newr != 0) {
		long long quotiens = div(r, newr).quot;
		long long prov = newt;
		newt = t - quotiens * newt;
		t = prov;
		prov = newr;
		newr = r - quotiens * newr;
		r = prov;
	}

	if (r > 1) {
		cout << "How did we get here?" << endl;
		vector<int> hehe;
		hehe[1] = 3;
	}
	if (t < 0) {
		t += n;
	}

	return t;
}

// TODO use the gmp class interface instead (c style code is weird, I don't like it)
Keys RSAGeneration(){
	std::random_device r;
	std::random_device s;

    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0,  50847534);
	int a = uniform_dist(e1);
	std::default_random_engine e2(s());
    int b = uniform_dist(e2);

	//const filesystem::path dir = filesystem::current_path() / "list.txt";
	//cout << dir.c_str() << endl;
	ifstream primesIn {"list.txt"};
	if (is_in_empty(primesIn)) {
		primesIn.close();
		ofstream primesOut {"list.txt"};
		cout << "Sieving out prime numbers, will take a while, please wait" << endl;
		vector<unsigned long long> v = sieve(1000000000);
		for (unsigned long long i = 0; i < v.size(); i++) {
			//primesOut << "why\n";
			primesOut << v[i] << "\n";
		}
		primesOut.close();
		primesIn.open("list.txt");
		cout << "Sieving complete" << endl;
	}
	vector<unsigned long long> v;
	string sLine;
	int i = 0;
	while (i != 50847534 && getline(primesIn, sLine)) {
		v.push_back(stoull(sLine));
		++i;
	}
	mpz_t p;
	mpz_t q;
	mpz_init_set_si(p, v[a]);
	primesIn.close();
	mpz_init_set_si(q, v[b]);

	mpz_t n;
	mpz_init(n);
	mpz_mul(n, p, q);

	mpz_t pMinus1;
	mpz_t qMinus1;
	mpz_init(pMinus1);
	mpz_init(qMinus1);
	mpz_sub_ui(pMinus1, p, 1);
	mpz_sub_ui(qMinus1, q, 1);

	mpz_t lambda;
	mpz_init(lambda);
	mpz_lcm(lambda, pMinus1, qMinus1);

	mpz_t e;
	mpz_init_set_si(e, 65537);

	mpz_t d;
	mpz_init(d);
	mpz_t crimeAgainstHumanity;
	mpz_init_set_si(crimeAgainstHumanity, -1);
	mpz_powm(d, e, crimeAgainstHumanity, lambda);


	cout << "a: " << a << endl;
	cout << "b: " << b << endl;
	/*
	cout << "p: " << p << endl;
	cout << "q: " << q << endl;
	cout << "n: " << n << endl;
	cout << "lambda: " << lambda << endl;
	cout << "e: " << e << endl;
	cout << "d: " << d << endl;
	*/

	cout << "p: ";
	mpz_out_str(NULL, 10, p);
	cout << endl;
	cout << "q: ";
	mpz_out_str(NULL, 10, p);
	cout << endl;

	cout << "n: ";
	mpz_out_str(NULL, 10, n);
	cout << endl;

	mpz_t test;
	mpz_init(test);
	mpz_gcd(test, e, lambda);
	cout << "gcd: ";
	mpz_out_str(NULL, 10, test);
	cout << endl;

	Keys o;
	mpz_init_set(o.publicKey[0], n);
	mpz_init_set(o.publicKey[1], e);
	mpz_init_set(o.privateKey[0], n);
	mpz_init_set(o.privateKey[1], d);

	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(pMinus1);
	mpz_clear(qMinus1);
	mpz_clear(lambda);
	mpz_clear(e);
	mpz_clear(d);
	mpz_clear(crimeAgainstHumanity);

	return o;
}

mpz_ptr RSAEncryption(string in, array<mpz_t, 2> key) {
	static mpz_t M;
	mpz_init(M);
	for (int i = in.length() - 1; i >= 0; i--) {
		mpz_t e;
		mpz_init_set_ui(e, 256);
		mpz_pow_ui(e, e, in.length()-i-1);
		mpz_mul_si(e, e, (int)in[i]);
		mpz_add(M, M, e);
		mpz_clear(e);
		// to decript, use bitwise ands and <<
	}
	cout << "M: ";
	mpz_out_str(NULL, 10, M);
	cout << endl;
	mpz_powm(M, M, key[1], key[0]);
	return M;
}

// TODO possibly optimize it more
string RSADecryption(mpz_ptr c, array<mpz_t, 2> key) {
	mpz_t M;
	mpz_init(M);
	mpz_powm(M, c, key[1], key[0]);
	string out;

	// to get the order of magnitude in base 256
	mpfr_t u;
	mpfr_init(u);
	mpfr_set_z(u, M, MPFR_RNDN);

	// log of u with base 16
	mpfr_log2(u, u, MPFR_RNDN);
	double omOld = mpfr_get_d(u, MPFR_RNDN);
	omOld /= 8;

	int om = floor(omOld);

	for (int i = 0; i <= om; i++) {
		mpz_t t; 
		mpz_init_set(t, M);
		mpz_t comp;
		mpz_init_set_ui(comp, 255);
		mpz_mul_2exp(comp, comp, (8 * om  - 8 * i));
		mpz_and(t, t, comp);
		mpz_fdiv_q_2exp(t, t, (8 * om  - 8 * i));
		out.push_back((char)mpz_get_si(t));
		mpz_clear(t);
		mpz_clear(comp);
	}

	mpz_clear(M);
	mpfr_clear(u);
	return out;
}

int main(int argc, char const *argv[]) {
	Keys key = RSAGeneration();
	//cout << "Public key: (" << key.publicKey[0] << "," << key.publicKey[1] << ")" << endl;
	//cout << "Private key: (" << key.privateKey[0] << "," << key.privateKey[1] << ")" << endl;
	string i;
	getline(cin, i);
	mpz_ptr cripted;
	cripted = RSAEncryption(i, key.publicKey);
	//cout << "Crypted message: "<< cripted << endl;
	string decripted = RSADecryption(cripted, key.privateKey);	
	cout << "Decrypted message: " << decripted << endl;
	return 0;
}
