import java.util.ArrayList;
import java.io.*;
import java.util.*;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicIntegerArray;
import java.util.concurrent.locks.ReentrantLock;

class AtomicBitSet{
	private AtomicIntegerArray arr;
	private int finlen;

	public AtomicBitSet(int len) {
        finlen = (len + 31)/32; 
        arr = new AtomicIntegerArray(finlen);
    }
    public boolean get(int index) {
    	int indinarr = index/32;
    	int num = arr.get(indinarr);
    	num = num >> (index%32);
    	//int pos = 1 << (index % 32);
    	//System.err.println(num%2);
    	return ((num%2) != 0);
    }
    public void set(int index, boolean val) {
    	int indinarr = index/32;
    	if(val) { 
	    	int pos = 1 << (index % 32);
	    	while(true) {
		    	int num = arr.get(indinarr);
		    	int newnum = num | pos ; if(newnum == num) return;
		    	if(arr.compareAndSet(indinarr, num, newnum)) {
		    		return;
		    	}
	    	}
    	}
    	else {
    		int poscomp = (0xFFFFFFFF ^ (1 << (index % 32)));
    		while(true) {
	    		int num = arr.get(indinarr);
	    		int newnum = num & poscomp ;if(newnum == num) return;
	    		if(arr.compareAndSet(indinarr, num, newnum)) {
	    			//if(this.get(index)) System.err.println("error");
		    		return;
		    	}
		    }
    	}
    }
    public void initone() {
    	for(int i = 0; i<finlen; i++) {
    		arr.set(i,0xFFFFFFFF);
    	}
    	return;
    }
}
class ThreadImpl implements Runnable {
   private Thread t;
   private AtomicBitSet bs;
   ArrayList<Long> primes;
   int tno;
   int tcount;
   long iterator;
   ReentrantLock lock;

   ThreadImpl(AtomicBitSet bs, int tno, int tcount, ArrayList<Long> primes, ReentrantLock lock, long iterator) {
      this.bs = bs; 
      this.tno = tno;
      this.tcount = tcount;
      this.primes = primes;
      this.iterator = iterator;
      this.lock = lock;
   }
   
   public void run() {
	for(long i = 2+tno; i <= iterator; i += tcount) {
		if(bs.get((int)i)) {
			for(long j = i*i; j <= iterator; j = j+i) {
				if(!bs.get((int)i)) break;
				bs.set((int)j,false);
			}
		}
	}
	/*
	for(long i = 2+tno; i <= iterator; i += tcount) {
		if(bs.get((int)i)) {
			lock.lock();
			primes.add(i);
			lock.unlock();
		}
	}
	*/
   }
   public void start () {
      if (t == null) {
         t = new Thread (this);
         t.start ();
      }
   }
   public void join () {
		try { t.join(); } catch ( Exception e) { System.out.println("Interrupted"); }
   }
}
class ThreadImpl2 implements Runnable {
   private Thread t;
   ArrayList<Long> primes;
   long maxprime;
   int tno;
   int tcount;
   long iterator;
   AtomicInteger flag;

   ThreadImpl2(int tno, int tcount, ArrayList<Long> primes, long maxprime, long iterator, AtomicInteger flag) { 
      this.tno = tno;
      this.tcount = tcount;
      this.primes = primes;
      this.maxprime = maxprime;
      this.iterator = iterator;
      this.flag = flag;
   }
   
   public void run() {
   	for(long i = tno; i < primes.size();i += tcount) {
   		if(flag.get() == 0) break;
   		if(iterator%primes.get((int)i) == 0) {
   			flag.set(0);
   		}
   	}
	for(long i = maxprime+2+2*tno; i*i <= iterator; i += 2*tcount) {
		if(flag.get() == 0) break;
		if(iterator%i==0) {
			flag.set(0);
		}
	}
   }
   public void start () {
      if (t == null) {
         t = new Thread (this);
         t.start ();
      }
   }
   public void join () {
		try { t.join(); } catch ( Exception e) { System.out.println("Interrupted"); }
   }
}

class P2
{ 
  public static void main(String args[])
  {
	int nooft = Integer.parseInt(args[0]);
	AtomicInteger availthreads = new AtomicInteger(nooft);
	Util u = new Util();
	ArrayList<Long> a = u.readInput(); // read input and start timer.

	long starttime = System.nanoTime();
	AtomicBitSet bs = new AtomicBitSet(100000010);

	long maxlong = 1;
	for (long l: a){
		if(maxlong < l) maxlong = l;
	}

	long upperbound;
	if(maxlong > 100000008) upperbound = 100000008;
	else upperbound = maxlong;

	long iterator = upperbound + 1;
	bs.initone();
	bs.set(0,false); bs.set(1,false);

	ArrayList<Long> primes = new ArrayList<Long>();
	ReentrantLock lock = new ReentrantLock();

	ThreadImpl[] threads = new ThreadImpl[nooft];
	for(int i = 1; i<nooft; i++) {
		threads[i] = new ThreadImpl(bs,i,nooft,primes,lock,iterator);
		threads[i].start();
	}

	for(long i = 2; i <= iterator; i += nooft) {
		if(bs.get((int)i)) {
			for(long j = i*i; j <= iterator; j = j+i) {
				if(!bs.get((int)i)) break;
				bs.set((int)j,false);
			}
		}
	}
	/*
	for(long i = 2; i <= iterator; i += nooft) {
		if(bs.get((int)i)) {
			lock.lock();
			primes.add(i);
			lock.unlock();
		}
	}
	*/
	for(int i = 1; i <nooft; i++) {
		threads[i].join();
	}
	//System.err.println("Here now");
	for(long i = 2; i <= iterator; i += 1) {
		if(bs.get((int)i)) {
			primes.add(i);
		}
	}

	long endtime = System.nanoTime();
	//System.err.println((endtime-starttime)/1000000);
	//System.err.println("This done");
	long maxprime = primes.get(primes.size()-1);
	//System.err.println("Max prime print "+maxprime);

	//System.err.println(maxprime);
	/*
	System.out.println("Primes :");
	for (long n:primes){
		System.out.println(n);
	}
	*/
	HashMap<Long,String> hm = new HashMap<Long,String>();  
	for (long l: a){
		/* Compute the primes.  */

		/* If l is a prime do: */
		if(l<=upperbound) {
			if(bs.get((int)l)) u.storePrime(l);
		}
		else {
			if(hm.containsKey(l)) continue;
			AtomicInteger flag = new AtomicInteger(1);
			ThreadImpl2[] threads2 = new ThreadImpl2[nooft];
			for(int i = 1; i<nooft; i++) {
				threads2[i] = new ThreadImpl2(i,nooft,primes,maxprime,l,flag);
				threads2[i].start();
			}
		   	for(long i = 0; i < primes.size();i += nooft) {
		   		if(flag.get() == 0) break;
		   		if(l%primes.get((int)i) == 0) {
		   			flag.set(0);
		   		}
		   	}
		   	/*
		   	if(l > maxprime*maxprime) {
		   		System.err.println("Here " + l);
		   	}
		   	*/
			for(long i = maxprime+2; i*i <= l; i += 2*nooft) {
				if(flag.get() == 0) break;
				if(l%i==0) {
					flag.set(0);
				}
			}
			for(int i = 1; i <nooft; i++) {
				threads2[i].join();
			}

			if(flag.get()==1) {
				hm.put(l,"1");
				u.storePrime(l);
			}
			else {
				hm.put(l,"0");
			}
		}
		/* Print the current set of primes for debugging purpose*/ 
		//u.printPrimesDebug();
	}

	/* Finish */
	u.printPrimes(); // end-timer, print output, and print time taken.
  }
}