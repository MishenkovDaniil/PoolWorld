#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "../pool_cont/pools.hpp"
#include "tests.hpp"


void simpleTest () {
    {
        Container cont;
        
        Pool& a = cont.AddPool();
        Pool& b = cont.AddPool();
        Pool& c = cont.AddPool();
        Pool& d = cont.AddPool();
        std::cout << "create 4 pools.\n";
        
#ifdef DEBUG
        std::cout << "real a_volume = " << a.volume_ << ".\n";
        std::cout << "real b_volume = " << b.volume_ << ".\n";
        std::cout << "real c_volume = " << c.volume_ << ".\n";
        std::cout << "real d_volume = " << d.volume_ << ".\n\n";
#endif
        cont.AddWater(a, 10);
        std::cout << "Add 10 Water to a pool.\n";
        cont.AddWater(b, 20);
        std::cout << "Add 20 Water to b pool.\n";
        cont.AddWater(c, 50);
        std::cout << "Add 50 Water to c pool.\n";

#ifdef DEBUG
        std::cout << "real a_volume = " << a.volume_ << ".\n";
        std::cout << "real b_volume = " << b.volume_ << ".\n";
        std::cout << "real c_volume = " << c.volume_ << ".\n";
        std::cout << "real d_volume = " << d.volume_ << ".\n\n";
#endif

        cont.Connect (a, b); //expect 15 in both
        
        std::cout << "connect a and b pools.\n";
        
#ifdef DEBUG
        std::cout << "real a_volume = " << a.volume_ << ".\n";
        std::cout << "real b_volume = " << b.volume_ << ".\n";
        std::cout << "real c_volume = " << c.volume_ << ".\n";
        std::cout << "real d_volume = " << d.volume_ << ".\n\n";
#endif
        
        cont.AddWater (a, 10); // expect 20 in a and b
        std::cout << "Add 10 Water to a pool.\n";

#ifdef DEBUG
        std::cout << "real a_volume = " << a.volume_ << ".\n";
        std::cout << "real b_volume = " << b.volume_ << ".\n";
        std::cout << "real c_volume = " << c.volume_ << ".\n";
        std::cout << "real d_volume = " << d.volume_ << ".\n\n";
#endif

        cont.Connect (b, c); // expect 30 in every
        std::cout << "connect c and b pools.\n";
        
#ifdef DEBUG
        std::cout << "real a_volume = " << a.volume_ << ".\n";
        std::cout << "real b_volume = " << b.volume_ << ".\n";
        std::cout << "real c_volume = " << c.volume_ << ".\n";
        std::cout << "real d_volume = " << d.volume_ << ".\n\n";
#endif

        size_t a_volume = cont.GetVolume (a);
        size_t b_volume = cont.GetVolume (b);
        size_t c_volume = cont.GetVolume (c);
        size_t d_volume = cont.GetVolume (d);

        std::cout << "a_volume = " << a_volume << ".\n";
        std::cout << "b_volume = " << b_volume << ".\n";
        std::cout << "c_volume = " << c_volume << ".\n";
        std::cout << "d_volume = " << d_volume << ".\n\n";
    }
    //maybe more simple tests...
}

void timeTest () {
    std::srand(std::time(nullptr));
    const size_t N = 25000000;
    const size_t K = 14000000;
    const size_t L = 27000000;
    const size_t M = 3000000;
    const size_t MinWater = 1;
    const size_t MaxWater = 500;


    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    {
        Container cont(N);
        
        printf ("1\n");
        for (size_t i = 0; i < N; ++i) {
            cont.AddPool ();
        }
        printf ("2\n");
        for (size_t i = 0; i < N; ++i) {
            cont.AddWater (i, std::rand() % MaxWater + MinWater);
        }

        printf ("3\n");
        for (size_t i = 0; i < K; ++i) {
            cont.Connect (std::rand() % N, std::rand() % N);
        }

        printf ("4\n");
        for (size_t i = 0; i < N; ++i) {
            cont.GetVolume (i);
        }

        printf ("5\n");
        for (size_t i = 0; i < L; ++i) {
            cont.AddWater (std::rand() % N, std::rand() % MaxWater + MinWater);
        }

        printf ("6\n");
        for (size_t i = 0; i < N; ++i) {
            cont.GetVolume (i);
        }

        printf ("7\n");
        for (size_t i = 0; i < M; ++i) {
            cont.RemoveConnection (std::rand() % N, std::rand() % N);
        }

        printf ("8\n");
        for (size_t i = 0; i < L; ++i) {
            cont.AddWater (std::rand() % N, std::rand() % MaxWater + MinWater);
        }

        printf ("9\n");
        for (size_t i = 0; i < N; ++i) {
            cont.GetVolume (i);
        }
        printf ("end\n");
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    size_t elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000;
    std::cout << "Elapsed time = " << elapsedTime << "seconds.\n";
}