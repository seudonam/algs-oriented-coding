#include <print>
#include <string_view>
#include <vector>

int naiveMethod(std::string_view pat, std::string_view txt) {
  int M = pat.size();
  int N = txt.size();
  // i <= ((N - 1) - M) + 1
  for (int i = 0; i <= N - M; i++) {
    int j;
    for (j = 0; j < M; j++)
      if (txt[i + j] != pat[j]) break;
    if (j == M) return i;
  }
  return N;
}

int bruteForce(std::string_view pat, std::string_view txt) {
  int i, N = txt.size();
  int j, M = pat.size();
  for (i = 0, j = 0; i < N && j < M; ++i) {
    if (txt[i] == pat[j])
      j++;
    else {
      // i = (i - (j + 1)) + 1
      i = i - j;
      j = 0;
    }
  }
  return j == M ? i - M : N;
}

struct KMP {
  static std::vector<int> next(std::string_view pat) {
    int m = pat.size(), j = 0;
    std::vector<int> N(m);
    int t = N[0] = -1;
    while (j < m - 1)
      if (t < 0 || pat[j] == pat[t])
        N[++j] = ++t;
      else
        t = N[t];
    return N;
  }
  static int search(std::string_view pat, std::string_view txt) {
    int n = txt.size(), i = 0;
    int m = pat.size(), j = 0;
    std::vector<int> N = next(pat);
    while (j < m && i < n)
      if (j < 0 || txt[i] == pat[j])
        ++i, ++j;
      else
        j = N[j];
    return i - j;
  }
};

struct ImprovedKMP {
  static std::vector<int> next(std::string_view pat) {
    int m = pat.size(), j = 0;
    std::vector<int> N(m);
    int t = N[0] = -1;
    while (j < m - 1)
      if (t < 0 || pat[j] == pat[t]) {
        ++j, ++t;
        if (pat[j] != pat[t])
          N[j] = t;
        else
          N[j] = N[t];
      } else
        t = N[t];
    return N;
  }
  static int search(std::string_view pat, std::string_view txt) {
    int n = txt.size(), i = 0;
    int m = pat.size(), j = 0;
    std::vector<int> N = next(pat);
    while (j < m && i < n)
      if (j < 0 || txt[i] == pat[j])
        ++i, ++j;
      else
        j = N[j];
    return i - j;
  }
};

struct BM {
  static std::vector<int> right(std::string_view pat) {
    std::vector<int> right(256);
    for (int c = 0; c < 256; c++) right[c] = -1;
    for (int j = 0; j < pat.size(); j++) right[pat[j]] = j;
    return right;
  }

  static int search(std::string_view pat, std::string_view txt) {
    int N = txt.length();
    int M = pat.length();
    std::vector<int> A(right(pat));
    int skip;
    for (int i = 0; i <= N - M; i += skip) {
      skip = 0;
      for (int j = M - 1; j >= 0; j--) {
        if (pat.at(j) != txt.at(i + j)) {
          skip = std::max(1, j - A[txt.at(i + j)]);
          break;
        }
      }
      if (skip == 0) return i;
    }
    return N;
  }
};

int main() {
  char txt[]{"BAAABAABBB"};
  char pat[]{"AAABAAB"};
  std::print("BF");
  int a{naiveMethod(pat, txt)};
  std::print("\n{}\t\n", txt);
  for (int i = 0; i < a; ++i) std::print(" ");
  std::print("{}\t\n\n", pat);
  // KMP
  std::print("KMP\t");
  std::vector<int> vec1{KMP::next(pat)};
  for (const auto &e : vec1) std::print("{}\t", e);
  int b{KMP::search(pat, txt)};
  std::print("\n{}\t\n", txt);
  for (int i = 0; i < b; ++i) std::print(" ");
  std::print("{}\t\n\n", pat);
  // ImprovedKMP
  std::print("IKMP\t");
  std::vector<int> vec2{ImprovedKMP::next(pat)};
  for (const auto &e : vec2) std::print("{}\t", e);
  int c{ImprovedKMP::search(pat, txt)};
  std::print("\n{}\t\n", txt);
  for (int i = 0; i < c; ++i) std::print(" ");
  std::print("{}\t\n\n", pat);
  // BM
  std::print("BM\t");
  int d{BM::search(pat, txt)};
  std::print("\n{}\t\n", txt);
  for (int i = 0; i < d; ++i) std::print(" ");
  std::print("{}\t\n", pat);
}