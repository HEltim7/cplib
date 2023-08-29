LL binmul(LL a, LL b, LL m) {
    LL c = (LL)a * b - (LL)((long double) a / m * b + 0.5L) * m;
    return c < 0 ? c + m : c;
}