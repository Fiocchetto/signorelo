#include <array>
#include <cmath>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

// https://stackoverflow.com/a/1505791
int myPow(long x, unsigned long long p)
{
  if (p == 0) return 1;
  if (p == 1) return x;

  int tmp = myPow(x, p/2);
  if (p%2 == 0) return tmp * tmp;
  else return x * tmp * tmp;
}

std::vector<int> sieve(int n) {
    std::vector<int> o;
    if (n > 1) {
        std::vector<bool> A;
        A.push_back(false);
        A.push_back(false);

        //inizialize A
        for (int i = 2; i < n+1; i++) {
            A.push_back(true);
        }

        for (int i = 2; i <= sqrt(n); i++)  {
            if (A[i]) {
                for (int j = myPow(i, 2); j <= n; j += i) {
                    A[j] = false;
                }
            }
        }

        for (int i = 2; i < n+1; i++) {
            if (A[i]) {
                o.push_back(i);
            }
        }
    }
    else {
        std::cout << "n not valid" << std::endl;
    }
    return o;
}

/*
int main() {
    string i;
    cin >> i;
    size_t pos{};
    vector<int> v = sieve(stoi(i, &pos));
    for (int i = 0; i < v.size(); i++) {
            cout << v[i] << endl;
    }
    return 0;
}
*/