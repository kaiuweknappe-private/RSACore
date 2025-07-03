
#pragma once 
#include <systemc.h>
#include "MontgomeryMul.h"

SC_MODULE(Testbench) {
	sc_clock clk{"clk", 10, SC_NS};
	sc_signal<bool> start, ready;
	sc_signal<sc_biguint<W>> a, b, n, result;
    sc_signal<sc_biguint<WORD_WIDTH>> n_prime;

	MontgomeryMul dut{"MontgomeryMul"};

	SC_CTOR(Testbench) {
		//connect dut
		dut.clk(clk);
		dut.a(a);
		dut.b(b);
		dut.n(n);
        dut.n_prime(n_prime);
		dut.start(start);
		dut.result(result);
		dut.ready(ready);

		SC_THREAD(drive);
	}

    //helper:
    template <int WORD_WIDTH>
    sc_biguint<WORD_WIDTH> compute_n_prime(sc_biguint<WORD_WIDTH> N){
        sc_biguint<WORD_WIDTH +1> R = 1 << WORD_WIDTH;

        for (sc_biguint<WORD_WIDTH+1> x = 1; x < R; ++x){
            if((N * x) % R == 1) {
                sc_biguint<WORD_WIDTH+1> neg_inv = R-x;
                return neg_inv.range(WORD_WIDTH -1, 0);
            }
        }
        throw std::runtime_error("No inverse found");
    }

	void drive() {
		wait(20, SC_NS);

		a.write(18);
		b.write(20);
		n.write(23); // should yield 7 imo
        //n_prime.write(compute_n_prime<WORD_WIDTH>(23));
        n_prime.write(14247); //should be n' to n mod 2base16

		start.write(true);
		wait(10, SC_NS);
		start.write(false);

		while(!ready.read()) wait(10, SC_NS);

		std::cout << "Result = " << result.read() << " at " << sc_time_stamp() << std::endl;
		sc_stop();
	}
};