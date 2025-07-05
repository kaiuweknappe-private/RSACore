
#include "MontgomeryMul.h"

void MontgomeryMul::fsm_process() {
	switch (state.read()) {
		case IDLE:
			if (start.read()) {
				i = 0; j = 0;
				C = 0; m = 0;
				for (int k = 0; k < s + 2; ++k)
					T[k] = 0;
				ready.write(false);
				state = INIT;
			}
			break;

		case INIT:
			for (int k = 0; k < s; ++k) {
				A[k] = a.read().range((k + 1) * w - 1, k * w);
				B[k] = b.read().range((k + 1) * w - 1, k * w);
				P[k] = p.read().range((k + 1) * w - 1, k * w);
			}
			state = MUL_LOOP_I;
			break;

		case MUL_LOOP_I:
			if (i < s) {
				j = 0;
				C = 0;
				state = MUL_LOOP_J;
			} else {
				state = DONE; 
			}
			break;

		case MUL_LOOP_J:
			if (j < s) {
				sc_biguint<2*w> mult = sc_biguint<2*w>(A[i]) * B[j];
				sc_biguint<2*w> temp = T[j] + mult + C;
				T[j] = temp.range(w - 1, 0);
				C = temp >> w;
				j++;
			} else {
				state = MUL_FINAL_SUM;
			}
			break;

		case MUL_FINAL_SUM: {
			sc_biguint<2*w> temp = T[s] + C;
			T[s] = temp.range(w - 1, 0);
			T[s + 1] = temp >> w;
			state = REDUCE_M;
			break;
		}

		case REDUCE_M: {
			m = (T[0] * p_prime.read()).range(w - 1, 0);
			C = 0;
			j = 0;
			state = REDUCE_LOOP_J;
			break;
		}

		case REDUCE_LOOP_J:
			if (j < s) {
				sc_biguint<2*w> mult = sc_biguint<w + 2>(m) * P[j];
				sc_biguint<2*w> temp = T[j] + mult + C;
				if (j > 0) T[j - 1] = temp.range(w - 1, 0);
				C = temp >> w;
				j++;
			} else {
				state = REDUCE_FINAL;
			}
			break;

		case REDUCE_FINAL: {
			sc_biguint<2*w> temp = T[s] + C;
			T[s - 1] = temp.range(w - 1, 0);
			T[s] = T[s + 1] + (temp >> w);
			i++;
			state = MUL_LOOP_I;
			break;
		}

		case DONE: {
			sc_biguint<W> result_word;
			for (int k = 0; k < s; ++k) {
				result_word.range((k + 1) * w - 1, k * w) = T[k].range(w - 1, 0);
			}
			result.write(result_word);
			ready.write(true);
			state = IDLE;
			break;
		}

		default:
			break;
	}
}
