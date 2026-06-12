int LoopFusion_base(int N) {
  // Creazione dei due array
  int A[64];
  int B[64];

  // Inizializzazione dei due array
  for (int k = 0; k < 64; ++k) { 
    A[k] = 0;
    B[k] = 0;
  }

  if (N > 64 || N < 0)
    N = 64;

  // L1
  for (int i=0; i < N; i++) {
    A[i] = (i * 3) + 7;
  }
  // L2
  for (int j=0; j < N; j++) {
    B[j] = A[j] + (j ^ 5);
  }

  return B[N-1];
}