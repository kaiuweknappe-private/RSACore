
#pragma once
#include <systemc.h>

const int W = 32; //total word length
const int WORD_WIDTH = 16;
const int NUM_WORDS = W / WORD_WIDTH;

SC_MODULE(MontgomeryMul){
	//Ports
	sc_in<bool> clk, start;
	sc_in<sc_biguint<W>> a, b, n; //with a,b < n ofc
	sc_in<sc_biguint<WORD_WIDTH>> n_prime;
	
	sc_out<sc_biguint<W>> result;
	sc_out<bool> ready;

	//Internal
	enum State {IDLE, LOAD, LOOP_I, REDUCE, CHECK_LOOP, FINAL_REDUCTION, DONE};
	sc_signal<State> state; //current state for fsm
	sc_biguint<W> A, B, N; 
	sc_biguint<2*W> T; //accumulator
	sc_biguint<WORD_WIDTH> a_i, m;
	int i; // index over words of A
	const sc_biguint<WORD_WIDTH + 1> R = sc_biguint<WORD_WIDTH + 1>(1) << WORD_WIDTH;

	SC_CTOR(MontgomeryMul){
		SC_METHOD(fsm_process);
		sensitive << clk.pos();
		dont_initialize(); //striktly starts with first rising edge
	}

	void fsm_process();

};
