
#pragma once
#include <systemc.h>
#include <vector>

constexpr int W = 1024;           // total bit width of input/output
constexpr int w = 64;             // word width
constexpr int s = W / w;          // number of words

SC_MODULE(MontgomeryMul) {
    // Ports 
    sc_in<bool> clk, start;
    sc_in<sc_biguint<W>> a, b, p;           // operands and modulus
    sc_in<sc_biguint<w>> p_prime;           // p' = -p^(-1) mod 2^w
    sc_out<sc_biguint<W>> result;
    sc_out<bool> ready;

	// Internal
    enum State {
        IDLE, INIT, MUL_LOOP_I, MUL_LOOP_J,
        MUL_FINAL_SUM, REDUCE_M, REDUCE_LOOP_J,
        REDUCE_FINAL, DONE
    };
    sc_signal<State> state;
    sc_biguint<w> A[s], B[s], P[s];

    sc_biguint<w> T[s + 2]; //accumulator
    int i = 0, j = 0; // Loop counters
    sc_biguint<w> C;  // carry
    sc_biguint<w> m;  // reduction multiplier, is mod 2basew, so witdh w should be fine

    
    SC_CTOR(MontgomeryMul) {
        SC_METHOD(fsm_process);
        sensitive << clk.pos();
        dont_initialize();
		state = IDLE;
    }

    void fsm_process();

};
