int LoopFusion_base(int N) {
  // Creazione dei due array
  int A[64];
  int B[64];

  // inizializzazione dei due array
  for (int k = 0; k < 64; ++k) { A[k] = k; B[k] = 0; }

  int i = 0;
  int j = 0;

  while (i < N) {
    A[i] = (i * 3) + 7;
    i++;
  }

  while (j < N) {
    B[j] = A[j] + (j ^ 5);
    j++;
  }

  return B[N-1];
}