#include "sieve.hpp"
#include "array"
#include "random"

using namespace std;

struct Keys {
	array<long long, 2> publicKey;
	array<long long, 2> privateKey;
};

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

Keys RSAGeneration(){
	std::random_device r;
	std::random_device s;

    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0,  20);
    int a = uniform_dist(e1);
	std::default_random_engine e2(s());
    int b = uniform_dist(e2);

	vector<int> v = sieve(1000000);
	int p = v[a];
	int q = v[b];

	long long n = p * q;

	long long lambda = lcm(p-1, q-1);

	int e = 65537;

	long long d = extendedEuclidean(e, lambda);

	cout << "p: " << p << endl;
	cout << "q: " << q << endl;
	cout << "n: " << n << endl;
	cout << "lambda: " << lambda << endl;
	cout << "e: " << e << endl;
	cout << "d: " << d << endl;

	Keys o;
	o.publicKey = {n, e};
	o.privateKey = {n, d};

	return o;
}

unsigned long long RSAEncryption(string in, array<long long, 2> key) {
	unsigned long long M;
	for (int i = in.length() - 1; i >= 0; i--) {
		M += (int)in[i] * myPow(256, in.length()-i-1);
		// to decript, use bitwise ands and <<
	}
	return mod_pow(M, key[1], key[0]);
}

// TODO make it work for more than 8 characters
string RSADecryption(unsigned long long c, array<long long, 2> key) {
	unsigned long long M = mod_pow(c, key[1], key[0]);
	string out;
	for (int i = 0; i < 8; i++) {
		int t = M & (255 << (64 - 8 * (i)));
		if (t != 0) out.insert(0, 1, (char)t);
	}
	return out;
}

int main(int argc, char const *argv[]) {
	Keys key = RSAGeneration();
	cout << "Public key: (" << key.publicKey[0] << "," << key.publicKey[1] << ")" << endl;
	cout << "Private key: (" << key.privateKey[0] << "," << key.privateKey[1] << ")" << endl;
	string i;
	getline(cin, i);
	if (i.size() <= 8) {
		unsigned long long cripted = RSAEncryption(i, key.publicKey);
		cout << "Crypted message: "<< cripted << endl;
		string decripted = RSADecryption(cripted, key.privateKey);
		cout << "Decrypted message: " << decripted << endl;
	}
	return 0;
}
