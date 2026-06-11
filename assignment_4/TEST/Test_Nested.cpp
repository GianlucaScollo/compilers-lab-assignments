int LoopFusion_nested(int N) {
  // Creazione dei due array
  int A[64];
  int B[64];

  // inizializzazione dei due array
  for (int k = 0; k < 64; ++k) { A[k] = 1; B[k] = 0; }

  // Loop esterno 1
  for (int i = 0; i < N; ++i) {
    int acc = 0;
    for (int j = 0; j < 4; ++j) {
      acc += (i + j);
    }
    A[i] = acc;
  }

  // Loop esterno 2 (stesso trip count N)
  for (int i = 0; i < N; ++i) {
    int acc = 0;
    for (int j = 0; j < 4; ++j) {
      acc += (A[i] ^ j);
    }
    B[i] = acc + 3;
  }

  return B[N-1];
}