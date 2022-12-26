while (true) {
    L:
    flag [i] = 1
    while (turn != i) {
        if (flag[turn] == 0) {
            turn = i;
        }
    }
    flag [i] = 2;
    for (int j = 0 ; j < n; j++) {
        if (j == i) {
            continue;
        }
        if (flag[j] == 2) {
            goto L;
        }
    }
    // critical section
    flag [i] = 0; 
}
