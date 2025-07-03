
#include "MontgomeryMul.h"

void MontgomeryMul::fsm_process() {

	switch (state.read()){
		case IDLE:
			if (start.read()) {
				A = a.read();
				B = b.read();
				N = n.read();
				T = 0;
				i = 0;
				ready.write(false);
				state = LOAD;
			}
			break;
			
		case LOAD:
			a_i = (A>>(i*WORD_WIDTH)).range(WORD_WIDTH-1,0);
			state = LOOP_I;
			break;

		case LOOP_I:
			T += a_i * B; 
			state = REDUCE;
			break;
		
		case REDUCE:
			m = (T * n_prime.read()) & (R - 1); // T * n' mod R 
			T += m * N;
			T = T >> WORD_WIDTH;
			state = CHECK_LOOP;
			break; 

		case CHECK_LOOP:
			i++;
			if(i < NUM_WORDS) {
				state = LOAD;
			} else {
				state = FINAL_REDUCTION;
			}
			break;

		case FINAL_REDUCTION:
			if (T >= N)
				T -= N;
			state = DONE;
			break;

		case DONE:
			result.write(T.range(W - 1, 0));
			ready.write(true);
			state = IDLE;
			break;

	}
}
