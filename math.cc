#include <cstdio>
#include <string>
#include <vector>

int absolute(int a) { return ((a >> 31) ^ a) - (a >> 31); }

// littile endian high precision addition
std::string addition(std::string_view A, std::string_view B) {
  int n(A.size()), m(B.size());
  if (n < m) return addition(B, A);
  std::string S;
  char x{0};
  for (int i = 0; i < n; i++) {
    x = x + (A[i] - '0');
    if (i < m) x = x + (B[i] - '0');
    S.push_back(x % 10 + '0');
    x = x / 10;
  }
  if (x > 0) S.push_back(x + '0');
  return S;
}

std::string multiplication(std::string_view A, std::string_view B) {
  int n = A.size(), m = B.size();

  std::string P(n + m, 0);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      P[i + j] = P[i + j] + (A[i] - '0') * (B[j] - '0');
      P[i + j + 1] = P[i + j + 1] + P[i + j] / 10;
      P[i + j] = P[i + j] % 10;
    }
  }
  if (!P.empty() && !P.back()) P.pop_back();

  for (auto &e : P) e = e + '0';

  return P;
}

// trial division
bool is_prime(int num) {
  if (num < 2) return false;
  for (int i = 2; i * i <= num; i++)
    if (num % i == 0) return false;
  return true;
}

bool is_palindrome(std::string_view S) {
  if (S.empty())
    return true;
  else if (S.front() != S.back())
    return false;
  else
    return is_palindrome(S.substr(1, S.size() - 2));
}

// towers of hanoi
void hanoi(int n, int from, int to, int spare) {
  if (n == 1)
    printf("move from %i to %i\n", from, to);
  else {
    hanoi(n - 1, from, spare, to);
    hanoi(1, from, to, spare);
    hanoi(n - 1, spare, to, from);
  }
}

int main() {
  printf("%d\t%d\n", addition("99", "99") == "891",
         multiplication("99", "99") == "1089");

  printf("%d\t%d\t%d\t%d\n", is_palindrome("civic"), is_palindrome("refer"),
         is_palindrome("kayak"), is_palindrome("rotor"));

  printf("towers of hanoi\n");
  hanoi(3, 0, 1, 2);
  printf("prime\t");
  for (int i = 0; i < 32; i++)
    if (is_prime(i)) printf("%i\t", i);
  printf("\n");

  std::vector<int> fib{0, 1};
  for (int i = 0; fib.back() < 50; i++) fib.push_back(fib[i] + fib[i + 1]);
  printf("fib\t");
  for (auto e : fib) printf("%i\t", e);
  printf("\n");
}