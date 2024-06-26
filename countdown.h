#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string> 
#include <algorithm>
#include <stack>
#include <ctype.h>
#include <locale>
#include <cmath>
#include <limits>
 

#define N 6

using namespace std;


class CountdownSolution{
  
protected:
    std::string solution;
    int value;
	std::string rpn;

public:

CountdownSolution() : solution(), value(0) {}

CountdownSolution(const std::string & solutionIn, const int valueIn)
    : solution(solutionIn), value(valueIn) {
}

const std::string & getSolution() const {
    return solution;
}

int getValue() const {
    return value;
}
};

string rpn;

double evaluateCountdown(const string rp)
{
	rpn = rp;


	stack<double> st;
	size_t* p = new size_t;
	*p = 0;

	double x, a, b;
	char ch;
	for (unsigned int c = 0; c < rp.size();)
	{
		ch = rp[c];

		if (ch == ' ') { c++; } //c++ 

		else if (isdigit(ch))
		{
			x = stod(rp.substr(c), p);
			c += *p;
			st.push(x);
		}

		else if (st.size() >= 2)
		{
			b = st.top(); st.pop();
			a = st.top(); st.pop();
			switch (ch)
			{
			case '+': st.push(a + b); break;
			case '-': st.push(a - b); break;
			case '*': st.push(a * b); break;
			case '/': 
				if (b == 0) { return NAN; }
				else { st.push(a / b); }
				break;
			default: {throw "evaluation failed"; };
			}
			c++;
		}

		else { cout << rp; throw "evaluation failed"; }
	}

	if (st.size() != 1) { throw "evaluation failed"; }

	return st.top();
}


double evalpatternwithnumbers(string pattern, int n, int target)
{
	const int bound = (1 << (2 * (n - 1)));
	int kk;
	double x, y;
	double m = numeric_limits<double>::infinity();
	
	for (int k = 0; k < bound; k++) {
		
		kk = k;
		string s = pattern;
		for (int i = 0; i < n - 1; i++)
		{
			size_t index = s.find('&');
			switch (kk & 3) {
			case 0: s.replace(index, 1, 1, '+'); break;
			case 1: s.replace(index, 1, 1, '-'); break;
			case 2: s.replace(index, 1, 1, '*'); break;
			case 3: s.replace(index, 1, 1, '/'); break;
			}
			kk >>= 2;
		}

		// evaluate the rp
		x = evaluateCountdown(s);
		if (isnan(x)) { continue; }
		if (abs(x - target) < m)
		{
			m = abs(x - target);
			y = x;
			if (m == 0) { break; } // we can do no better
		}		
	}

	return y;
}


double evalpattern(vector<int> numbers, string pattern, int n, int target)
{
	double x, y;
	double m;

	// permutation code
	// generates all permutations of the set of given numbers, of length n, in lexicographic order

	int* indices = new int[N];

	// cycles is a factoriadic number
	// https://en.wikipedia.org/wiki/Factorial_number_system
	int* cycles = new int[n];
	
	for (int i = 0; i < N; i++) { indices[i] = i; }
	for (int i = 0; i < n; i++) { cycles[i] = N - i; }

	// eval the trivial permutation here
	string s = "";
	int a = 0;
	for (const char& c : pattern)
	{
		if (c == '1')
		{
			s += to_string(numbers[a]);
			s += " ";
			a++;
		}
		else if (c == '&') {
			s += "& ";
		}
	}
	x = evalpatternwithnumbers(s, n, target);
	y = x;
	m = abs(x - target);
	// end of eval

	int i, j, k;
	bool finished = false;

	while (!finished)
	{
		finished = true;

		for (i = n - 1; i >= 0; i--)
		{
			if (cycles[i] == 1)
			{
				cycles[i] = n - i;
			}
			else {
				finished = false;
				break;
			}
		}

		if (finished) { continue; }

		reverse(indices + i + 1, indices + N);
		reverse(indices + i + 1, indices + N - n + i + 1);

		cycles[i]--;

		j = cycles[i];
		k = indices[i];
		indices[i] = indices[N - j];
		indices[N - j] = k;

		// eval the permutation here
		s = "";
		int a = 0;
		for (const char& c : pattern)
		{
			if (c == '1')
			{
				s += to_string(numbers[indices[a]]);
				s += " ";
				a++;
			}
			else if (c == '&') {
				s += "& ";
			}
		}
		double x = evalpatternwithnumbers(s, n, target);
		if (abs(x - target) < m)
		{
			y = x;
			m = abs(x - target);
			if (m == 0) { break; } // we can do no better
		}
		// end of eval
	}

	return y;
}


double searchpatterns(vector<int> numbers, int target, string root, int n, int k)
{
	double x, y;
	double m = numeric_limits<double>::infinity();
	if (n == k + 1)
	{
		// eval the pattern here
		x = evalpattern(numbers, root, n, target);
		if (abs(x - target) < m) { 
			y = x; m = abs(x - target); 
			if (m == 0) { return x; }
		}
	}
	else if (n > k + 1)
	{
		x = searchpatterns(numbers, target, root + '&', n, k + 1);;
		if (abs(x - target) < m) {
			y = x; m = abs(x - target);
			if (m == 0) { return x; }
		}
		
	}
	
	if (n != N)
	{
		x = searchpatterns(numbers, target, root + '1', n + 1, k);
		if (abs(x - target) < m) {
			y = x; m = abs(x - target);
		}
	}
	return y;
}

    CountdownSolution solveCountdownProblem(vector<int> numbers, int target){
	
	double sol = searchpatterns(numbers, target, "11", 2, 0);
	return CountdownSolution(rpn, sol);
    }


#endif
