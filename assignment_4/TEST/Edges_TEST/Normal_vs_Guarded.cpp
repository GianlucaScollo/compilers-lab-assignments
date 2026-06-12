int LoopFusion_normal_vs_guarded(int N) {
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

  int i = 0;
  
  // Loop 1 unguarded
  do {
    A[i] = A[i] * 2 + 1;
    i++;
  } while (i < N);

  // Loop 2 guarded (solo se N > 0)
  for (int i = 0; i < N; ++i) {
    B[i] = A[i] + 10;
  }

  return B[N-1];
}