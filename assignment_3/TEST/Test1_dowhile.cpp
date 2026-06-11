void licm_test1(int a, int b) {
  int i = 0;
  do {
    int s = a * 2;
    b = b - 3;
    if (b < 5) {
      i = i + 2;
    } else {
      s = s - b;
      if (s < -30) {
        return;
      }
    }
  }
  while (i < 20);
  return;
}
