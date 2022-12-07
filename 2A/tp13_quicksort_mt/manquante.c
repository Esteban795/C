void *parallel_qsort_1(void *arg){
    slice_t slice = *(slice_t*)arg;
    if (slice.len <= 1) return NULL;
    int pivot = partition(slice.start, slice.len);
    slice_t lo = {.start = slice.start, .len = pivot};
    slice_t hi = {.start = slice.start + pivot + 1,
                  .len = slice.len - pivot - 1};
    pthread_t t_lo, t_hi;
    pthread_create(&t_lo, NULL, parallel_qsort_1, &lo);
    pthread_create(&t_hi, NULL, parallel_qsort_1, &hi);
    pthread_join(t_lo, NULL);
    pthread_join(t_hi, NULL);
    return NULL;
}