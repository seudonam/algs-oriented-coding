#include <algorithm>
#include <print>
#include <string_view>
#include <vector>

// suffix array
auto suffix(std::string_view str) {
  std::vector<std::string_view> A;
  for (int i = 0; i < str.size(); i++) A.push_back(str.substr(i, str.size()));
  std::ranges::sort(A);
  return A;
}

// longest common prefix
std::string_view lcp(std::string_view S, std::string_view T) {
  int n = std::min(S.size(), T.size());
  for (int k = 0; k < n; k++)
    if (S.at(k) != T.at(k)) return S.substr(0, k);
  return S.substr(0, n);
}

// longest repeated substring
std::string_view lrs(std::string_view str) {
  std::string_view lrs;
  auto A{suffix(str)};
  for (int i = 1; i < A.size(); i++) {
    int n = std::min(A[i].size(), A[i - 1].size());
    int j{0};
    for (; j < n; j++)
      if (A[i].at(j) != A[i - 1].at(j)) break;
    if (j > lrs.size()) lrs = A[i].substr(0, j);
  }
  return lrs;
}

// longest common substring
std::string_view lcs(std::string_view S, std::string_view T) {
  std::string_view lcs;
  auto A{suffix(S)}, B{suffix(T)};
  int i{0}, j{0};
  while (i < S.size() && j < T.size()) {
    auto x{lcp(A[i], B[j])};
    if (x.size() > lcs.size()) lcs = x;
    if (A[i] < B[j])
      i++;
    else
      j++;
  }
  return lcs;
}

int main() {
  auto S{"itwasthebestoftimesitwastheworstoftimes"};
  auto T{"itwastheageofwisdomitwastheageoffoolishness"};
  auto U{"itwastheepochofbeliefitwastheepochofincredulity"};
  auto V{"itwastheseasonoflightitwastheseasonofdarkness"};

  std::print("{}\n", lrs(U));

  std::print("{}\n", lcs(S, T));

  std::print("{}\n", lcs(U, V));
}
