import java.util.Scanner;
import java.io.*;
import java.util.*;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;
import java.util.StringTokenizer;
// Class FastReader is a standard class used in the competitive programming community for fast input output in java.
// I used it to make the program run fast on large inputs.
// For more information check out https://www.geeksforgeeks.org/fast-io-in-java-in-competitive-programming/
class FastReader
{
    BufferedReader br;
    StringTokenizer st;

    public FastReader()
    {
        br = new BufferedReader(new InputStreamReader(System.in));
    }
    public FastReader(InputStreamReader isr)
    { 
        br = new BufferedReader(isr);
    }

    String next()
    {
        while (st == null || !st.hasMoreElements())
        {
            try
            {
                st = new StringTokenizer(br.readLine());
            }
            catch (IOException  e)
            {
                e.printStackTrace();
            }
        }
        return st.nextToken();
    }

    int nextInt()
    {
        return Integer.parseInt(next());
    }

    long nextLong()
    {
        return Long.parseLong(next());
    }

    double nextDouble()
    {
        return Double.parseDouble(next());
    }

    String nextLine()
    {
        String str = "";
        try
        {
            str = br.readLine();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        return str;
    }
}
 
class nodeinfo {
	int weight;
	int left = -1;
	int right = -1;
	long newweight = -1;
	int bias = -2;
}

class lockobj {
	int availthreads;
	lockobj(int at) {
		availthreads = at;
	}
	public int get() {
		return availthreads;
	}
	public void decrement() {
		availthreads--;
	}
}

class Graph {
	public nodeinfo[] G;
	//private AtomicInteger availthreads;
	private lockobj availthreads;
	private int N;
	Graph(nodeinfo[] G1, int n, lockobj at) {
      G = G1;
      availthreads = at;
      N = n;
    }
	public void setnewweight(int ind) {
		if(G[ind].newweight == -1) {
			int l = G[ind].left;
			int r = G[ind].right;
			G[ind].newweight = G[ind].weight;
			long diff = 0;
			if(l!=-1 && r == -1) {
				setnewweight(l);
				G[ind].newweight += G[l].newweight;
				diff += G[l].newweight;
			}
			else if(l==-1 && r != -1) {
				setnewweight(r);
				G[ind].newweight += G[r].newweight;
				diff -= G[r].newweight;
			}
			else if(l!=-1 && r!=-1){
	  			int flag = 0;
	  			synchronized (availthreads){
					if(availthreads.get()>1) {
						flag = 1;
						//System.out.println("Count is " + (index+1) + "::" +tcount.get());
						availthreads.decrement();
						//int val = availthreads.decrementAndGet();
						//System.out.println("Count is " + (ind+1) + "::" +val);
					}
				}
				if(flag==1) {
					ThreadImpl T1 = new ThreadImpl(this,l);
					T1.start();
					setnewweight(r);
					try { T1.join(); } catch ( Exception e) { System.out.println("Interrupted"); }
					synchronized(G[l]) {
						//System.out.println("weight is " + (l+1) + " " + G[l].newweight);
						if(G[l].newweight == -1) setnewweight(l);
					}
				}
				else {
					setnewweight(r);setnewweight(l);
				}
				G[ind].newweight += G[l].newweight;
				diff += G[l].newweight;
				G[ind].newweight += G[r].newweight;
				diff -= G[r].newweight;
			}
			if(diff > 0) {
				G[ind].bias = 1;
			}
			else if(diff==0) {
				G[ind].bias = 0;
			}
			else {
				G[ind].bias = -1;
			}
			//System.out.println("bias is " + (ind+1) + " "+ G[ind].bias);
		}
	}
	/*
	public void setnewweight2(int ind) {
		if(G[ind].newweight == -1) {
			int l = G[ind].left;
			int r = G[ind].right;
			G[ind].newweight = G[ind].weight;
			int diff = 0;
			if(l!=-1 && r == -1) {
				setnewweight(l);
				G[ind].newweight += G[l].newweight;
				diff += G[l].newweight;
			}
			else if(l==-1 && r != -1) {
				setnewweight(r);
				G[ind].newweight += G[r].newweight;
				diff -= G[r].newweight;
			}
			else if(l!=-1 && r!=-1){
				setnewweight(r);
				setnewweight(l);
				G[ind].newweight += G[l].newweight;
				diff += G[l].newweight;
				G[ind].newweight += G[r].newweight;
				diff -= G[r].newweight;
			}
			if(diff > 0) {
				G[ind].bias = 1;
			}
			else if(diff==0) {
				G[ind].bias = 0;
			}
			else {
				G[ind].bias = -1;
			}
		}
	}
	*/
	public void printweight() {		
		/*		
		for(int i = 0;i < N;i++) {
			System.out.println((i+1) + " " + G[i].bias);
		}*/
		OutputStream out = new BufferedOutputStream ( System.out );
        for (int i = 0; i < N; i++) {
        	try{ out.write(((i+1) + " " + G[i].bias + "\n").getBytes()); out.flush();} catch(Exception e) {;}
        }

		/*
		for(int i = 0;i < N;i++) {
			System.out.println((i+1) + " " + G[i].newweight);
		}*/

	}
}
class ThreadImpl extends Thread {
   private Thread t;
   private Graph grph;
   private int index;

   ThreadImpl(Graph gr1, int ind) {
      grph = gr1;
      index = ind; 
   }
   
   public void run() {
      synchronized(grph.G[index]) {
		  grph.setnewweight(index);
  	  }
   }
   public void start () {
      if (t == null) {
         t = new Thread (this);
         t.start ();
      }
   }
}
class P1
{ 
  public static void main(String args[])
  {
  	// The name of the file to open.
    String fileName = args[0];
	int nooft = Integer.parseInt(args[1]);
	lockobj availthreads = new lockobj(nooft);
	//AtomicInteger availthreads = new AtomicInteger(nooft);     
	try {
		InputStream inputStream  = new FileInputStream(fileName);
 	    FastReader in = new FastReader(new InputStreamReader(inputStream));
 
		int N = 0;     
		//Get input Integer
		N = in.nextInt();
		nodeinfo[] G = new nodeinfo[N];	
		//nodeinfo[] G2 = new nodeinfo[N];

		for(int i = 0;i < N;i++) {
			int ind = in.nextInt();
			G[ind-1] = new nodeinfo();
			//G2[ind-1] = new nodeinfo();			
			int temp = in.nextInt();
			G[ind-1].weight = temp;
			//G2[ind-1].weight = temp;
			temp = in.nextInt();
			G[ind-1].left = temp-1;
			//G2[ind-1].left = temp-1;
			temp = in.nextInt();
			G[ind-1].right = temp-1;
			//G2[ind-1].right = temp-1;
		}
		Graph grph = new Graph(G,N,availthreads);

		//long starttime = System.nanoTime();
	  	grph.setnewweight(0);
	  	//long endtime = System.nanoTime();

	  	//long timedone = endtime - starttime;
	  	//System.err.println(timedone/1000);

	  	/*
		Graph grph2 = new Graph(G2,N,availthreads);
		long starttime2 = System.nanoTime();
	  	grph2.setnewweight2(0);
	  	long endtime2 = System.nanoTime();

	  	long timedone2 = endtime2 - starttime2;
	  	System.out.println(timedone2/1000);
	  	*/
	  	grph.printweight();
	}
    catch(FileNotFoundException ex) {
        System.out.println("Unable to open file");                
    }
  }
}
