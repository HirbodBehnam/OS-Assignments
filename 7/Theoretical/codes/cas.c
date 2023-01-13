int compare_and_swap(int *a, int old, int new) {
    int temp = *a;
    if (*a == old)
        *a = new;
    return temp;
}