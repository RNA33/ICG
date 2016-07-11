
#ifndef __ICGSTATIC_H__
#define __ICGSTATIC_H__

#include "ICG.h"

/**
 * This is a simple wrapper class for the pseudorandom inversive congruential generator defined in ICG.h
 * It is meant for the user who just wants to generate random numbers quickly without having
 * to worry about prime numbers, seeds and such.
 *
 * It uses a preselected prime and the standard function time ( NULL ) to construct a static generator object.
 * All methods are static and can be immediately called to produce random values.
 *
 */

/*
 * Usage example:
 *
 * 	#include "ICGStatic.h"
 *
 *	...
 *
 *  // 0 <= rand0To99 < 100, evenly distributed
 * 	unsigned long rand0To99 = ICGStatic :: rand ( 100 );
 *
 *  // 0.0 <= rand0To1 < 1.0, evenly distributed
 *  double rand0To1 = ICGStatic :: rand01 ( );
 *
 *  // 20.0 <= rand20To25 < 25.0, evenly distributed
 *  double rand20To25 = ICGStatic :: randInterval ( 20.0, 25.0 );
 *
 *  // -INF < randStdNorm < INF, normally distributed, mu=0.0, sigma^2=1.0
 *  double randStdNorm = ICGStatic :: randStdNorm ( );
 *
 *  // -INF < randStdNorm < INF, normally distributed, mu=5.0, sigma^2=2.0
 *  double randNorm = ICGStatic :: randNormal ( 5.0, 2.0 );
 *
 */
class ICGStatic {
	public:
		/**
		 * Generates a pseudorandom unsigned integer between 0 and range-1 inclusive.
		 *
		 * The generated pseudorandom numbers will be roughly evenly distributed.
		 *
		 * @param range The largest generated number is given by range-1.
		 * @return A random unsigned integer in the range 0, 1, 2, ..., range-1
		 */
		static unsigned long rand ( unsigned long range ) { return icg.rand ( range ); }

		/**
		 * Generates a pseudorandom double precision floating point number in the intervall [0,1).
		 *
		 * The generated pseudorandom numbers will be roughly evenly distributed within the given interval.
		 *
		 * @return A random double in the interval [0,1).
		 */
		static double rand01 ( ) { return icg.rand01 ( ); }

		/**
		 * Generates a pseudorandom double precision floating point number in the interval [A,B).
		 *
		 * The generated pseudorandom numbers will be roughly evenly distributed within the given interval.
		 *
		 * @param A	Lower bound for the generated random numbers, rand >= A.
		 * @param B Upper bound for the generated random numbers, rand < B.
		 * @return A random double in the intervall [A,B).
		 */
		static double randInterval ( double A, double B ) { return icg.randInterval ( A, B ); }

		/**
		 * Generates normally distributed pseudorandom numbers.
		 *
		 * Uses the Box-Muller method in polar form to produce normally distributed
		 * numbers from evenly distributed ICG output.
		 *
		 * @param mu The mean of the normal distribution.
		 * @param ss The variance of the normal distribution.
		 * @return A roughly N(mu,ss) distributed pseudorandom number.
		 */
		static double randNormal ( double mu, double ss ) { return icg.randNormal ( mu, ss ); }

		/**
		 * Generates pseudorandom numbers according to a standard normal distribution.
		 *
		 * Uses the Box-Muller method in polar form to produce standard normally distributed
		 * numbers from evenly distributed ICG output.
		 *
		 * @return A roughly Z=N(0,1) distributed pseudorandom number.
		 */
		static double randStdNorm ( ) { return icg.randStdNorm ( ); }

	private:
		static ICG icg;
};

#endif /* __ICGSTATIC_H__ */
