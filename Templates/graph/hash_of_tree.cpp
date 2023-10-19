struct Hasher {
    ULL rnd;

    ULL operator()(ULL x) {
        x^=rnd;
        x^=x<<13;
        x^=x>>7;
        x^=x<<13;
        x^=rnd;
        return x;
    }

    Hasher() {
        mt19937 gen(random_device{}());
        rnd=gen();
    }
} f;