int LoopFusion_base(int N) {
    // Creazione dei due array
    int A[64];
    int B[64];

    // L1
    int i = 0;
    int j = 0;

    if(i < N){
        do {
            A[i] = (i * 3) + 7;
            i++;
        } while (i < N);
    }
    // L2 
    if(j < N){
        do {
            B[j] = A[j] + (j ^ 5);
            j++;
        } while (j < N);
    }

    return B[N-1];
}