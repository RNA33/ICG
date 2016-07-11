
#ifndef __ICG_H__
#define __ICG_H__

/**
 * Inversive congruential generator
 *
 * This class is a simple implementation of an inversive congruential generator (ICG).
 * An ICG is a pseudorandom number generator, which generates according to the formula
 *
 * NEXT_RAND = ( a * CUR_RAND^-1 + b ) % p
 *
 * where p is a prime number and a and b are integers less than p.
 * This sort of generator produces pseudorandom number sequences with useful statistical properties.
 * It can be used for easy generation of normally distributed numbers via the Box-Muller Method.
 *
 */

/*
 * Usage example:
 *
 * 	#include "ICG.h"
 * 	#include <time.h>
 *
 * 	...
 *
 * 	ICG icg ( 15485863, 213, 64, time ( NULL ) % 15485863 );  // ICG initialized with prime and parameters a, b
 *
 *  // 0 <= rand0To99 < 100, evenly distributed
 * 	unsigned long rand0To99 = icg.rand ( 100 );
 *
 *  // 0.0 <= rand0To1 < 1.0, evenly distributed
 *  double rand0To1 = icg.rand01 ( );
 *
 *  // 20.0 <= rand20To25 < 25.0, evenly distributed
 *  double rand20To25 = icg.randInterval ( 20.0, 25.0 );
 *
 *  // -INF < randStdNorm < INF, normally distributed, mu=0.0, sigma^2=1.0
 *  double randStdNorm = icg.randStdNorm ( );
 *
 *  // -INF < randStdNorm < INF, normally distributed, mu=5.0, sigma^2=2.0
 *  double randNorm = icg.randNormal ( 5.0, 2.0 );
 *
 */
class ICG {
	public:
		ICG ( unsigned long p, unsigned long a, unsigned long b, unsigned long seed );
		
		bool reparametrize ( unsigned long a, unsigned long b, unsigned long p, unsigned long seed );
		bool reseed ( unsigned long seed );

		unsigned long rand ( );
		unsigned long rand ( unsigned long range );

		double rand01 ( );
		double randInterval ( double A, double B );
		
		double randNormal ( double mu, double ss );
		double randStdNorm ( );

		/**
		 * Returns the validity state of the generator.
		 *
		 * A generator can be invalid due to inappropriate parameter combinations.
		 * An invalid generator cannot produce random numbers and all random generation methods
		 * will return 0 in this case.
		 *
		 * @return True iff this ICG is valid and can produce random numbers.
		 */
		bool isValid ( ) const { return generatorIsValid; }

		/**
		 * Returns this generator's prime.
		 *
		 * @return The prime parameter p.
		 */
		unsigned long get_p ( ) const { return p; }

		/**
		 * Returns this generator's "a" parameter.
		 *
		 * @return The parameter a.
		 */
		unsigned long get_a ( ) const { return a; }

		/**
		 * Returns this generator's "b" parameter.
		 *
		 * @return The parameter b.
		 */
		unsigned long get_b ( ) const { return b; }

	private:
		bool generatorIsValid;

		unsigned long long p, a, b, seed, curRand;
		
		double mullerNormal;
		bool useMullerNormal;

		void checkGeneratorIsValid ( );

		bool isPrime ( unsigned long pr );
		unsigned long inverse ( unsigned long y );
};

#endif // __ICG_H__
