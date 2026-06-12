int LoopFusion_both_guarded(int N) {
  // Creazione dei due array
  int A[64];
  int B[64];

  // Inizializzazione dei due array
  for (int k = 0; k < 64; ++k) { 
    A[k] = 0; 
    B[k] = 0; 
  }

  // Loop 1 guarded (solo se N > 0)
  if (N > 0) {
    for (int i = 0; i < N; ++i) {
      A[i] = (i + 2) * (i + 2);
    }
  }

  // Loop 2 guarded (solo se N > 0)
  if (N > 0) {
    for (int i = 0; i < N; ++i) {
      B[i] = A[i] + (N - i);
    }
  }

  if (N > 0) {
    return B[N-1];
  }
  return 0;
}