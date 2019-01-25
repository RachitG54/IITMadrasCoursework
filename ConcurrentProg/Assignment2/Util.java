import java.util.Scanner;
import java.util.ArrayList;
import java.io.IOException;
/* Do not modify this file. Extend this class if required. */

class Util{
	final private ArrayList<Long> outArr  = new ArrayList<Long>();
	final private ArrayList<Long> emirps  = new ArrayList<Long>();
	long startTime;

	/* Reads input from stdin and returns the same in an ArrayList */
	final public ArrayList<Long> readInput(){
		ArrayList<Long> inpArr  = new ArrayList<Long>();
		Scanner in = new Scanner(System.in);
		while (in.hasNext()){
			Long num = in.nextLong();
			inpArr.add(num);
		}
		init();
		return inpArr;
	}
	/* A deliberate constraint: We can only store one prime at a time */
	final public void storePrime(long n){
		int i;
		for (i = 0;i<outArr.size(); ++i){
			if (outArr.get(i) < n) continue;
			break;
		}
		outArr.add(i,n);
		updateEmirpPairs(n);
	}
	/* Checks if n is an emirp and there is a corresponding emirp
	 * already present.*/

	final private void updateEmirpPairs(long n){
		if (emirps.contains(n)) return; // Don't store duplicates.

		StringBuilder xb = new StringBuilder(Long.toString(n));
		String x = xb.toString();
		StringBuilder xbr = xb.reverse();
		String xr = xbr.toString();
		if (xr.equals(x)) return; // a palindrome don't store. 

		int i;
		for (i = 0;i<outArr.size(); ++i){
			long m = outArr.get(i);
			if (Long.toString(m).equals(xr)) {
				emirps.add(m);
				emirps.add(n);
				break;
			}
		}

	}
	/* Use this code for printing the primes computed so far. Use this for debugging */
	final public void printPrimesDebug(){
		for (long n:outArr){
			System.out.println("Debug: " + n);
		}
	}
	/* Use this code to print the primes computed. Use this for final printing. */
	final public void printPrimes(){
		long endTime = System.nanoTime();
		System.out.println("Primes :");
		for (long n:outArr){
			System.out.println(n);
		}
		System.out.println("Emirp Pairs :");
		for (long n:emirps){
			System.out.println(n);
		}
		System.out.println("Time taken (sec) to finish computation = " + (endTime-startTime)/1000000000.0);
	}
	final private void init(){
		startTime = System.nanoTime();
	}

	
}
