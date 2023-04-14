int next_floor(int k,int i) {
    return k/(k/i);
}

int next_ceil(int k,int i) {
    if(k-1<i) return i;
    return (k-1)/((k-1)/i);
}