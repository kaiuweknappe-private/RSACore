
#pragma once 
#include <systemc.h>
#include "MontgomeryMul.h"

SC_MODULE(Testbench) {
	sc_clock clk{"clk", 10, SC_NS};
	sc_signal<bool> start, ready;
	sc_signal<sc_biguint<W>> a, b, p, result;
    sc_signal<sc_biguint<w>> p_prime;

	MontgomeryMul dut{"MontgomeryMul"};

	SC_CTOR(Testbench) {
		//connect dut
		dut.clk(clk);
		dut.a(a);
		dut.b(b);
		dut.p(p);
        dut.p_prime(p_prime);
		dut.start(start);
		dut.result(result);
		dut.ready(ready);

		SC_THREAD(drive);
	}

	void drive() {
		wait(20, SC_NS);

		a.write(9);
		b.write(10);
		p.write(23); 
        p_prime.write(3208129404123400281); 

		start.write(true);
		wait(10, SC_NS);
		start.write(false);

		while(!ready.read()) wait(10, SC_NS);

		//should yield 22 for this test case
		std::cout << "Result = " << result.read() << " at " << sc_time_stamp() << std::endl; 
		sc_stop();
	}
};