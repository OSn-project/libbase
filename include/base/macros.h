#ifndef BASE_MACROS_H_
#define BASE_MACROS_H_

#define b_ceil(N, D) (1 + ((N - 1) / D))

#define b_max(A, B) ((A) > (B) ? (A) : (B))
#define b_min(A, B) ((A) < (B) ? (A) : (B))
#define b_abs(A) ((A) > 0 ? (A) : -(A))

#endif /* INCLUDE_BASE_MACROS_H_ */
