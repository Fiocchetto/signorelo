#include "sieve.hpp"
#include "random"

bool millerRabin(int n, int k) {
	int d = n - 1;
	int s;
	while ((d % 2) == 0) {
		d /= 2;
		s++;
	}

	for (int i = 0; i < k; i++) {
		std::random_device r;

    	// Choose a random mean between 1 and 6
    	std::default_random_engine e1(r());
    	std::uniform_int_distribution<int> uniform_dist(2,  n - 1);
    	int a = uniform_dist(e1);

		int x = myPow(a, d) % n;

		if (x == 1 || x == n - 1) continue;

		bool continueMaybe = false;

		for (int j = 0; j < s - 1; j++) {
			x = (x*x) % n;
			if (x == 1) return false;
			if (x == n - 1) {
				continueMaybe = true;
				break;
			}
		}
		if (continueMaybe) {
			continue;
		}

		return false;
	}
	return true;
}

int main() {
	std::string n;
    std::cin >> n;
    std::size_t pos{};

	//std::vector<int> v = sieve(stoi(n, &pos));

	//for (int i = 0; i < v.size(); i++) {
    	if (millerRabin(stoi(n, &pos), 20)) {
			std::cout << n << ": probably prime" << std::endl;
		}
		else {
			std::cout << n << ": composite" << std::endl;
		}
	//}
	return 0;
}