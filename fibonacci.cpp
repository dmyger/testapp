/**
 * @author Dmitriy Gertsog <dmyger@gmail.com>
 *
 * @details
 *
 */
#include <iostream>
using namespace std;

typedef unsigned long long fib_size;

fib_size fibonacci_r(int i, fib_size *memo)
{
	if(i == 0 || i == 1) return i;

	if(memo[i] == 0) {
		memo[i] = fibonacci_r(i - 1, memo) + fibonacci_r(i - 2, memo);
		// cout << i << ": " << memo[i] << endl;
	}
	return memo[i];
}

fib_size fibonacci_i(int n)
{
	if(n == 0) return 0;
	fib_size a = 0;
	fib_size b = 1;
	for(int i = 2; i < n; i++) {
		fib_size c = a + b;
		a = b;
		b = c;
	}
	return a + b;
}

int main(int argc, char *argv[])
{
	int n = 1;
	do {
		cout << "N=?";
		cin >> n;
		cout << fibonacci_i(n) << endl;
		// fib_size fib[n + 1] = { 0 };
		// cout << fibonacci_r(n, fib) << endl;
	} while(n);
	return 0;
}
