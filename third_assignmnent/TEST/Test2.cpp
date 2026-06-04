int licm_test2(int a, int b, int start) {
  int i = start;

  while (i < 10) {
    int base = a * a + 2 * b;
    int bias = (b - 4) * (b - 4);
    int t = base + i - bias;
    if (t & 1) {
      i += 1;
    } else {
      i += 2;
      if (t > 1000) return t;
    }
  }

  return i;
}