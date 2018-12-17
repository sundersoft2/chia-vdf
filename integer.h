//note: gmp already has c++ bindings so could have just used those. oh well

const bool output_stats=false;

struct generic_stats {
    vector<int> entries;

    void add(int i) {
        entries.push_back(i);
    }

    void output(string name) {
        sort(entries.begin(), entries.end());
        vector<double> percentiles={0, 0.01, 0.1, 1, 10, 25, 50, 75, 90, 99, 99.9, 99.99, 100};

        print( "::", name );
        print( "    num =", entries.size() );

        if (entries.empty()) {
            return;
        }

        for (double c : percentiles) {
            int i=(c/100)*entries.size();
            if (i<0) {
                i=0;
            }
            if (i>=entries.size()) {
                i=entries.size()-1;
            }

            print( "    ", c, "    ->    ", entries.at(i) );
        }
    }
};

//typedef __mpz_struct mpz_t[1];
//__mpz_struct does not have a pointer to itself
struct integer {
    __mpz_struct impl;

    vector<__mpz_struct> cache;

    const __mpz_struct* get_impl() const {
        return &impl;
    }

    __mpz_struct* get_impl() {
        return &impl;
    }

    void do_init() {
        /*if (!cache.empty()) {
            impl=cache.back();
            cache.pop_back();
        } else {*/
            mpz_init(get_impl());
        //}
    }

    ~integer() {
        //cache.push_back(impl);
        mpz_clear(get_impl());
    }

    integer() {
        do_init();
        mpz_set_si(get_impl(), 0);
    }

    integer(const integer& t) {
        do_init();
        mpz_set(get_impl(), t.get_impl());
    }

    integer(integer&& t) {
        do_init();
        mpz_swap(get_impl(), t.get_impl());
    }

    explicit integer(int64 i) {
        do_init();
        mpz_set_si(get_impl(), i);
    }

    explicit integer(const string& s) {
        do_init();
        assert(mpz_set_str(get_impl(), s.c_str(), 0)==0);
    }

    explicit integer(const vector<uint64>& data) {
        do_init();
        mpz_import(get_impl(), data.size(), -1, 8, 0, 0, &data[0]);
    }

    vector<uint64> to_vector() const {
        vector<uint64> res;
        res.resize(mpz_sizeinbase(get_impl(), 2)/64 + 1, 0);

        size_t count;
        mpz_export(&res[0], &count, -1, 8, 0, 0, get_impl());
        res.resize(count);

        return res;
    }

    integer& operator=(const integer& t) {
        mpz_set(get_impl(), t.get_impl());
        return *this;
    }

    integer& operator=(integer&& t) {
        mpz_swap(get_impl(), t.get_impl());
        return *this;
    }

    integer& operator=(int64 i) {
        mpz_set_si(get_impl(), i);
        return *this;
    }

    integer& operator=(const string& s) {
        assert(mpz_set_str(get_impl(), s.c_str(), 0)==0);
    }

    USED string to_string() const {
        char* res_char=mpz_get_str(nullptr, 16, get_impl());
        string res_string="0x";
        res_string+=res_char;

        if (res_string.substr(0, 3)=="0x-") {
            res_string.at(0)='-';
            res_string.at(1)='0';
            res_string.at(2)='x';
        }

        free(res_char);
        return res_string;
    }

    integer& operator+=(const integer& t) {
        mpz_add(get_impl(), get_impl(), t.get_impl());
        return *this;
    }

    integer operator+(const integer& t) const {
        integer res;
        mpz_add(res.get_impl(), get_impl(), t.get_impl());
        return res;
    }

    integer& operator-=(const integer& t) {
        mpz_sub(get_impl(), get_impl(), t.get_impl());
        return *this;
    }

    integer operator-(const integer& t) const {
        integer res;
        mpz_sub(res.get_impl(), get_impl(), t.get_impl());
        return res;
    }

    integer& operator*=(const integer& t) {
        mpz_mul(get_impl(), get_impl(), t.get_impl());
        return *this;
    }

    integer operator*(const integer& t) const {
        integer res;
        //uint64 start_time=__rdtsc();
        mpz_mul(res.get_impl(), get_impl(), t.get_impl());
        //uint64 end_time=__rdtsc();
        //if (num_bits()>=100 && t.num_bits()>=100) print(end_time-start_time, num_bits(), t.num_bits());
        return res;
    }

    integer& operator<<=(int i) {
        assert(i>=0);
        mpz_mul_2exp(get_impl(), get_impl(), i);
        return *this;
    }

    integer operator<<(int i) const {
        assert(i>=0);
        integer res;
        mpz_mul_2exp(res.get_impl(), get_impl(), i);
        return res;
    }

    integer operator-() const {
        integer res;
        mpz_neg(res.get_impl(), get_impl());
        return res;
    }

    integer& operator/=(const integer& t) {
        mpz_fdiv_q(get_impl(), get_impl(), t.get_impl());
        return *this;
    }

    integer operator/(const integer& t) const {
        integer res;
        //uint64 start_time=__rdtsc();
        mpz_fdiv_q(res.get_impl(), get_impl(), t.get_impl());
        //uint64 end_time=__rdtsc();
        //if (num_bits()>=100 && t.num_bits()>=100 && num_bits()-t.num_bits()>=100) print(end_time-start_time, num_bits(), t.num_bits());
        return res;
    }

    integer& operator>>=(int i) {
        assert(i>=0);
        mpz_fdiv_q_2exp(get_impl(), get_impl(), i);
        return *this;
    }

    integer operator>>(int i) const {
        assert(i>=0);
        integer res;
        mpz_fdiv_q_2exp(res.get_impl(), get_impl(), i);
        return res;
    }
    integer& operator%=(const integer& t) {
        mpz_mod(get_impl(), get_impl(), t.get_impl());
        return *this;
    }

    integer operator%(const integer& t) const {
        integer res;
        mpz_mod(res.get_impl(), get_impl(), t.get_impl());
        return res;
    }

    bool prime() const {
        return mpz_probab_prime_p(get_impl(), 50)!=0;
    }

    bool operator<(const integer& t) const {
        return mpz_cmp(get_impl(), t.get_impl())<0;
    }

    bool operator<=(const integer& t) const {
        return mpz_cmp(get_impl(), t.get_impl())<=0;
    }

    bool operator==(const integer& t) const {
        return mpz_cmp(get_impl(), t.get_impl())==0;
    }

    bool operator>=(const integer& t) const {
        return mpz_cmp(get_impl(), t.get_impl())>=0;
    }

    bool operator>(const integer& t) const {
        return mpz_cmp(get_impl(), t.get_impl())>0;
    }

    bool operator!=(const integer& t) const {
        return mpz_cmp(get_impl(), t.get_impl())!=0;
    }

    bool operator<(int i) const {
        return mpz_cmp_si(get_impl(), i)<0;
    }

    bool operator<=(int i) const {
        return mpz_cmp_si(get_impl(), i)<=0;
    }

    bool operator==(int i) const {
        return mpz_cmp_si(get_impl(), i)==0;
    }

    bool operator>=(int i) const {
        return mpz_cmp_si(get_impl(), i)>=0;
    }

    bool operator>(int i) const {
        return mpz_cmp_si(get_impl(), i)>0;
    }

    bool operator!=(int i) const {
        return mpz_cmp_si(get_impl(), i)!=0;
    }

    int num_bits() const {
        return mpz_sizeinbase(get_impl(), 2);
    }
};

integer abs(const integer& t) {
    integer res;
    mpz_neg(res.get_impl(), t.get_impl());
    return res;
}

struct gcd_res {
    integer gcd;
    integer s;
    integer t;
};

//a*s + b*t = gcd ; gcd>=0
// abs(s) < abs(b) / (2 gcd)
// abs(t) < abs(a) / (2 gcd)
//(except if |s|<=1 or |t|<=1)
gcd_res gcd(const integer& a, const integer& b) {
    gcd_res res;

    /*uint64 start_time=__rdtsc();
    mpz_gcdext(res.gcd.get_impl(), res.s.get_impl(), nullptr, a.get_impl(), b.get_impl());
    uint64 end_time=__rdtsc();*/

    mpz_gcdext(res.gcd.get_impl(), res.s.get_impl(), res.t.get_impl(), a.get_impl(), b.get_impl());

    //print( "gmp gcd:", end_time-start_time );

    return res;
}

integer rand_integer(int num_bits) {
    static gmp_randstate_t state;
    static bool is_init=false;

    if (!is_init) {
        gmp_randinit_mt(state);
        gmp_randseed_ui(state, 0);
        is_init=true;
    }

    integer res;
    assert(num_bits>=0);
    mpz_urandomb(res.get_impl(), state, num_bits);
    return res;
}