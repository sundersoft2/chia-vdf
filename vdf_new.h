void normalize(integer& a, integer& b, integer& c) {
    integer r = (a-b)/(a<<1);

    integer A = a;
    integer B = b + ((r*a)<<1);
    integer C = a*r*r + b*r + c;

    a=A;
    b=B;
    c=C;
}

void reduce_impl(integer& a, integer& b, integer& c) {
    integer s = (c+b)/(c<<1);

    if (debug_reduce) {
        cerr << s.to_string() << ", ";
    }

    integer A = c;
    integer B = ((s*c)<<1) - b;
    integer C = c*s*s - b*s+a;

    a=A;
    b=B;
    c=C;
}

void reduce(integer& a, integer& b, integer& c) {
    normalize(a, b, c);

    if (debug_reduce) {
        print( "===reduce vdf_new===" );
    }

    while (a>c || (a==c && b<0)) {
        reduce_impl(a, b, c);
    }

    if (debug_reduce) {
        print( "" );
        print( "===end reduce vdf_new===" );
    }

    normalize(a, b, c);
}

void generator_for_discriminant(const integer& d, integer& a, integer& b, integer& c) {
    a=2;
    b=1;

    c = (b*b - d)/(a<<2);

    reduce(a, b, c);
}

void square(integer& a, integer& b, integer& c) {
    //b<0:
    // -bs===1 mod a
    // b(-s)===1 mod a
    // negate sign of s
    //note: s and b are unsigned with a separate sign
    gcd_res r=gcd(b, a);

    //s<0:
    // c>0 so this is used as an argument to the divide operation
    integer u=(c/r.gcd*r.s)%a;

    //need to apply b's sign to b now

    integer A = a*a;
    integer B = b - ((a*u)<<1);
    integer C = u*u - (b*u-c)/a;

    a=A;
    b=B;
    c=C;

    //print(end_time-start_time);
}

integer arg_discriminant;
int arg_iterations;

void parse_args(int argc, char** argv) {
    arg_discriminant=integer(
        "-0xdc2a335cd2b355c99d3d8d92850122b3d8fe20d0f5360e7aaaecb448960d57bcddfee12a229bbd8d370feda5a17466fc725158ebb78a2a7d37d0a226d89b54434db9c3be9a9bb6ba2c2cd079221d873a17933ceb81a37b0665b9b7e247e8df66bdd45eb15ada12326db01e26c861adf0233666c01dec92bbb547df7369aed3b1fbdff867cfc670511cc270964fbd98e5c55fbe0947ac2b9803acbfd935f3abb8d9be6f938aa4b4cc6203f53c928a979a2f18a1ff501b2587a93e95a428a107545e451f0ac6c7f520a7e99bf77336b1659a2cb3dd1b60e0c6fcfffc05f74cfa763a1d0af7de9994b6e35a9682c4543ae991b3a39839230ef84dae63e88d90f457"
    );
    arg_iterations=1000;

    if (argc==1) {
    } else
    if (argc==2) {
        arg_iterations=from_string<int>(argv[1]);
    } else
    if (argc==3) {
        arg_discriminant=integer(argv[1]);
        arg_iterations=from_string<int>(argv[2]);
    } else {
        assert(false);
    }
}