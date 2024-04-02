#include "array"
#include "random"
#include "sieve.hpp"
#include <bits/types/FILE.h>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <mpfr.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

struct Keys {
  array<mpz_t, 2> publicKey;
  array<mpz_t, 2> privateKey;
};

bool is_in_empty(std::ifstream &pFile) {
  return pFile.peek() == std::ifstream::traits_type::eof();
}

int GetLine(std::ifstream &file, int n) {
  int i = 0;
  string sLine;
  while (i != n && getline(file, sLine)) {
    ++i;
  }

  if (i == n) {
    return stoi(sLine);
  } else {
    return -1;
  }
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

Keys RSAGeneration() {
  std::random_device r;
  std::random_device s;

  std::default_random_engine e1(r());
  std::uniform_int_distribution<int> uniform_dist(0, 50847534);
  int a = uniform_dist(e1);
  std::default_random_engine e2(s());
  int b = uniform_dist(e2);

  ifstream primesIn{"list.txt"};
  if (is_in_empty(primesIn)) {
    primesIn.close();
    ofstream primesOut{"list.txt"};
    cout << "Sieving out prime numbers, will take a while, please wait" << endl;
    vector<mpz_class> v = sieve(1000000000);
    for (unsigned long long i = 0; i < v.size(); i++) {
      primesOut << v[i].get_ui() << "\n";
    }
    primesOut.close();
    primesIn.open("list.txt");
    cout << "Sieving complete" << endl;
  }
  vector<mpz_class> v;
  string sLine;
  int i = 0;
  while (i != 50847534 && getline(primesIn, sLine)) {
    mpz_class welp(sLine);
    v.push_back(welp);
    ++i;
  }
  mpz_class p = v[a];
  mpz_class q = v[b];
  primesIn.close();

  mpz_class n;
  n = p * q;

  mpz_class pMinus1 = p - 1;
  mpz_class qMinus1 = q - 1;

  mpz_class lambda = lcm(pMinus1, qMinus1);

  mpz_class e(65537);

  mpz_class d;
  mpz_class crimeAgainstHumanity(-1);
  mpz_powm(d.get_mpz_t(), e.get_mpz_t(), crimeAgainstHumanity.get_mpz_t(),
           lambda.get_mpz_t());

  Keys o;
  mpz_init_set(o.publicKey[0], n.get_mpz_t());
  mpz_init_set(o.publicKey[1], e.get_mpz_t());
  mpz_init_set(o.privateKey[0], n.get_mpz_t());
  mpz_init_set(o.privateKey[1], d.get_mpz_t());

  return o;
}

void RSAEncryptionUtility(unsigned int in, array<mpz_t, 2> key,
                          vector<mpz_class> &ihe) {
  mpz_class N(in);
  mpz_powm(N.get_mpz_t(), N.get_mpz_t(), key[1], key[0]);
  ihe.push_back(N);
}

void RSAEncryption(string in, array<mpz_t, 2> key,
                   vector<mpz_class> &iHateEveryone) {
  static mpz_class M;
  for (int i = in.length() - 1; i >= 0; i--) {
    mpz_class e(256);
    mpz_pow_ui(e.get_mpz_t(), e.get_mpz_t(), in.length() - i - 1);
    e *= (int)in[i];
    M += e;
  }

  // to get the order of magnitude in base 4294967295
  mpfr_t u;
  mpfr_init(u);
  mpfr_set_z(u, M.get_mpz_t(), MPFR_RNDN);

  // log of u with base 4294967295
  mpfr_log2(u, u, MPFR_RNDN);
  double omOld = mpfr_get_d(u, MPFR_RNDN);
  omOld /= 32;

  int om = floor(omOld);

  vector<unsigned int> v;

  for (int i = 0; i <= om; i++) {
    mpz_class t(M);
    mpz_class comp(4294967295);
    comp = comp << (32 * om - 32 * i);
    t = t & comp;
    t = t >> (32 * om - 32 * i);
    v.push_back(t.get_ui());
  }

  for (int i = 0; i < v.size(); i++) {
    RSAEncryptionUtility(v[i], key, iHateEveryone);
  }

  mpfr_clear(u);
}

string RSADecryptionUtility(mpz_class M, array<mpz_t, 2> key) {
  mpz_powm(M.get_mpz_t(), M.get_mpz_t(), key[1], key[0]);
  string out;

  // to get the order of magnitude in base 256
  mpfr_t u;
  mpfr_init(u);
  mpfr_set_z(u, M.get_mpz_t(), MPFR_RNDN);

  // log of u with base 256
  mpfr_log2(u, u, MPFR_RNDN);
  double omOld = mpfr_get_d(u, MPFR_RNDN);
  omOld /= 8;

  int om = floor(omOld);

  for (int i = 0; i <= om; i++) {
    mpz_class t(M);
    mpz_class comp(255);
    comp = comp << (8 * om - 8 * i);
    t = t & comp;
    t = t >> (8 * om - 8 * i);
    out.push_back((char)t.get_ui());
  }

  mpfr_clear(u);
  return out;
}

string RSADecryption(vector<mpz_class> c, array<mpz_t, 2> key) {
  string out;
  for (int i = 0; i < c.size(); i++) {
    out = out + RSADecryptionUtility(c[i], key);
  }
  return out;
}

int main(int argc, char const *argv[]) {
  Keys key = RSAGeneration();
  cout << "Input:" << endl;
  string i;
  getline(cin, i);
  vector<mpz_class> cripted;
  cout << "Crypted message: ";
  RSAEncryption(i, key.publicKey, cripted);
  for (int i = 0; i < cripted.size(); i++) {
    cout << cripted[i].get_str();
  }
  cout << endl;
  string decripted = RSADecryption(cripted, key.privateKey);
  cout << "Decrypted message: " << decripted << endl;
  return 0;
}
