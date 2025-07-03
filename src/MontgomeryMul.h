
#pragma once
#include <systemc.h>

SC_MODULE(MontgomeryMul){
	//Ports


	//Internal


	SC_CTOR(MontgomeryMul){
		SC_THREAD(fsm);
		//sensitive << clk.pos();
	}

	void fsm();
};
