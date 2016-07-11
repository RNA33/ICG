
#include "ICG.h"
#include <math.h> // using: sqrt ( ), log ( )

/**
 * Constructs an inversive congruential generator from the given parameters p, a, b and seed.
 *
 * The mathematical parameters p, a, b govern the generation according to the formula
 *
 *              next = ( a * inverse ( cur ) + b ) % p
 *
 * seed determines the start of the sequence, but will not itself be the first random value produced.
 *
 * @param a An unsigned long < p
 * @param b An unsigned long < p
 * @param p A prime integer >= 3
 * @param seed An unsigned long < p
 */
ICG :: ICG ( unsigned long p, unsigned long a, unsigned long b, unsigned long seed )
: generatorIsValid ( false ), p ( p ), a ( a ), b ( b ), seed ( seed ), curRand ( seed )
{
	checkGeneratorIsValid ( );
}


/**
 * Resets the generation parameters for this ICG.
 *
 * @param a An unsigned long < p
 * @param b An unsigned long < p
 * @param p A prime integer >= 3
 * @param seed An unsigned long < p
 * @return True iff the given parameters form a valid parameter combination for an ICG.
 */
bool ICG :: reparametrize ( unsigned long p, unsigned long a, unsigned long b, unsigned long seed ) {
	generatorIsValid = false;

	this -> p = p;
	this -> a = a;
	this -> b = b;
	this -> seed = seed;
	curRand = seed;

	checkGeneratorIsValid ( );
	return generatorIsValid;
}


/**
 * Resets the seed for this ICG and restarts pseudorandom number generation cycle at the new seed.
 *
 * @param newSeed unsigned long which must be less than the generator prime.
 * @return True if the generator state after successful reseeding is valid.
 */
bool ICG :: reseed ( unsigned long newSeed ) {
	generatorIsValid = false;
	
	seed = newSeed;
	curRand = seed;
	
	checkGeneratorIsValid ( );

	return generatorIsValid;
}
		

/**
 * Generates a pseudorandom unsigned integer between 0 and p-1 inclusive where p is the generator's prime number.
 *
 * The generated pseudorandom numbers will be roughly evenly distributed.
 *
 * @return A random unsigned integer in the range 0, 1, 2, ..., p-1
 */
unsigned long ICG :: rand ( ) {
	if ( !generatorIsValid ) return 0;
	
	if ( curRand == 0 ) { curRand = ( unsigned long ) b; return curRand; }
	
	// Basic generation formula:
	// next = ( a * inverse ( cur ) + b ) % p;

	// The class variables p, a, b are stored internally as unsigned long long values yet
	// never take on values greater than MAX ( unsigned long ).
	unsigned long long inv = inverse ( curRand );
	unsigned long long temp = ( a * inv + b ) % p;
	
	curRand = ( unsigned long ) ( temp );
	
	return curRand;
}


/**
 * Generates a pseudorandom unsigned integer between 0 and range-1 inclusive.
 *
 * The generated pseudorandom numbers will be roughly evenly distributed.
 *
 * @param range The largest generated number is given by range-1.
 * @return A random unsigned integer in the range 0, 1, 2, ..., range-1
 */
unsigned long ICG :: rand ( unsigned long range ) {
	return ( unsigned long ) ( rand01 ( ) * range );
}


/**
 * Generates a pseudorandom double precision floating point number in the intervall [0,1).
 *
 * The generated pseudorandom numbers will be roughly evenly distributed within the given interval.
 *
 * @return A random double in the interval [0,1).
 */
double ICG :: rand01 ( ) {
	if ( !generatorIsValid ) return 0;
	
	return ( ( double ) rand ( ) / ( double ) p );
}


/**
 * Generates a pseudorandom double precision floating point number in the interval [A,B).
 *
 * The generated pseudorandom numbers will be roughly evenly distributed within the given interval.
 *
 * @param A	Lower bound for the generated random numbers, rand >= A.
 * @param B Upper bound for the generated random numbers, rand < B.
 * @return A random double in the intervall [A,B).
 */
double ICG :: randInterval ( double A, double B ) {
	if ( !generatorIsValid ) return 0;

	if ( B == A ) return A;
	if ( B < A ) {
		double temp = A;
		A = B;
		B = temp;
	}
	
	return ( ( double ) rand ( ) / ( double ) p ) * ( B - A ) + A;
}


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
double ICG :: randNormal ( double mu, double ss ) {
	return sqrt ( ss ) * randStdNorm ( ) + mu;
}


/**
 * Generates pseudorandom numbers according to a standard normal distribution.
 *
 * Uses the Box-Muller method in polar form to produce standard normally distributed
 * numbers from evenly distributed ICG output.
 *
 * @return A roughly Z=N(0,1) distributed pseudorandom number.
 */
double ICG :: randStdNorm ( ) {
	// The Box-Muller method actually generates 2 random numbers, but
	// this method only returns one.
	// In order to avoid unnecessary calculation, we save the extra number (as a standard normal value)
	// in a class variable and return it on the next call.
	if ( useMullerNormal ) {
		useMullerNormal = false;
		return mullerNormal;
	}

	double u1 = 0.0, u2 = 0.0, q = 0.0;
	const double EPS = 0.0001;
	do {
		u1 = randInterval ( -1.0, 1.0 );
		u2 = randInterval ( -1.0, 1.0 );
		q = u1 * u1 + u2 * u2;

	} while ( q <= EPS || q > 1.0 );

	double r = sqrt ( -2.0 * log ( q ) / q );

	mullerNormal = r * u2;
	useMullerNormal = true;
	return r * u1;
}


/**
 * Determines if a number is prime.
 *
 * Private helper method. Uses naive algorithm, since only called once upon construction.
 *
 * @param pr A number to check for primeness.
 * @return True iff pr is a prime number.
 */
bool ICG :: isPrime ( unsigned long pr ) {
	if ( pr == 0 || pr == 1) return false;
	if ( pr == 2 || pr == 3 ) return true;
	if ( pr % 2 == 0 ) return false;

	unsigned long cur = 3, max = ( unsigned long ) ( sqrt ( pr ) );
	while ( cur <= max ) {
		if ( pr % cur == 0 ) return false;
		cur += 2;
	}

	return true;
}


/**
 * Calculates the inverse of an integer in the ring mod p.
 *
 * Private helper method.
 * If the passed integer is 0 or greater than p this function returns 0.
 * Uses the extended Euclidean algorithm to calculate the inverse of y such that
 *
 * 				( y * inverse ( y ) ) % p == 1
 *
 * @param y A nonzero unsigned long < p
 * @return An unsigned long integer z such that ( y*z % p ) == 1
 */
unsigned long ICG :: inverse ( unsigned long y ) {
	if ( y == 0 ) return 0;
	if ( y == 1 ) return 1;
	if ( y >= p ) return 0;
	
	
	unsigned long rn = p, rn1 = y, rn2 = rn % rn1;
	long long Rn = 0, Rn1 = 1, Rn2 = 0, q = 0;
	
	// a = ( a / b ) * b + a % b
	while ( rn2 != 0 ) {
		rn2 = rn % rn1;
		q = rn / rn1;
		
		Rn = Rn2 - q * Rn1;
		
		if ( rn2 != 0 ) {
			rn = rn1;
			rn1 = rn2;
			
			Rn2 = Rn1;
			Rn1 = Rn;
		}
	}
	
	while ( Rn1 < 0 ) Rn1 += p;
	return ( unsigned long ) Rn1;
}


/**
 * Sets the validity state of this ICG according to the current parameters.
 *
 * Private helper method.
 * In order to be a valid generator, the following conditions must be met:
 * 	 - p is prime and > 3
 * 	 - a < p
 * 	 - b < p
 * 	 - seed < p
 *
 */
void ICG :: checkGeneratorIsValid ( ) {
	generatorIsValid = ( p > 3 ) &&
					   ( isPrime ( p ) ) &&
					   ( a < p ) &&
					   ( b < p ) &&
					   ( seed < p );
}
