#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// C是抽象程度最低的高层语言
// 逻辑结构,思维中的想象结构
// 物理结构,计算机中的存储结构
// 数学归纳,递归基,递归链
// 复杂度,数操作

int test_and_set(int *p, int new_value) {
  int actual = *p;
  *p = new_value;
  return actual;
}

int compare_and_swap(int *p, int expected, int new_value) {
  int actual = *p;
  if (actual == expected)
    *p = new_value;
  return actual;
}

// add %src, %dst
int add(int a, int b) {
  asm volatile("add %1,%0" : "=r"(b) : "r"(a), "0"(b));
  return b;
}

// sub %src, %dst
int sub(int a, int b) {
  asm volatile("sub %1,%0" : "+r"(a) : "r"(b));
  return a;
}

// "r"只读 "=r"只写 "+r"既读又写
int main(void) {
  srand(time(NULL));
  for (int i = 0; i < 8; i++) {
    int a = rand() % 8, b = rand() % 8;
    printf("%d + %d = ", a, b);
    printf("%d\n", add(a, b));
  }
  for (int i = 0; i < 8; i++) {
    int a = rand() % 8, b = rand() % 8;
    printf("%d - %d = ", a, b);
    printf("%d\n", sub(a, b));
  }
}
