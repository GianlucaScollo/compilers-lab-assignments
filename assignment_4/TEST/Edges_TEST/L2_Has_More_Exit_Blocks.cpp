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

  int i = 0;
  int j = 0;
  
  // L1
  do {
    A[i] = (i * 3) + 7;
    i++;
  } while (i < N);
  
  // L2
  do {
    if (j == 7) {
      break;
    }
    B[j] = A[j] + (j ^ 5);
    j++;
  } while (j < N);

  return B[N-1];
}