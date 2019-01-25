//
// Generated by JTB 1.3.2
//

package visitor;
import syntaxtree.*;
import java.util.*;

/**
 * Provides default methods which visit each node in the tree in depth-first
 * order.  Your visitors may extend this class.
 */
class inter
{
  String name;
  int start;
  int end;
  int active;
}
class func
{
  int[] procedurelabels = new int[3]; 
  String[] registers = new String[24];
  HashMap <String,Integer> livein = new HashMap<String,Integer>();
  HashMap <String,Integer> liveout = new HashMap<String,Integer>();
  HashMap <String,Integer> temptoreg = new HashMap<String,Integer>();
  HashMap <String,Integer> labels = new HashMap <String,Integer>();
  HashMap <Integer, ArrayList<String> > in = new HashMap <Integer, ArrayList<String>>();
  HashMap <Integer, ArrayList<String> > out = new HashMap <Integer, ArrayList<String>>();
  int bigvarcount = 0;
}
public class GJDepthFirst<R,A> implements GJVisitor<R,A> {
   //
   // Auto class visitors--probably don't need to be overridden.
   //
  int parsecount = 0;
  int statementcount = 0;
  int flag3 = -1;
  int flagdef =-1;
  int flagsimple = -1;
  String currentprocedure;
  String[] registers = new String[24];
  HashMap <String, func> globalmap = new HashMap <String,func>();
  HashMap <Integer, ArrayList<String> > use = new HashMap <Integer, ArrayList<String>>();
  HashMap <Integer, ArrayList<String> > def = new HashMap <Integer, ArrayList<String>>();
  HashMap <Integer, ArrayList<Integer> > nextstatement = new HashMap <Integer, ArrayList<Integer>>();
  HashMap <Integer, String> nextlabel1 = new HashMap <Integer, String>();
  HashMap <Integer, String> nextlabel2 = new HashMap <Integer, String>();
  HashMap <SimpleExp, String> finalheadache = new HashMap <SimpleExp, String>(); 
  ArrayList <String> opentemps = new ArrayList <String>();
  func tempfunction = new func();
  ArrayList <String> tempargs = new ArrayList();


  void print(String s)
  {
    if(parsecount==1)
    System.out.println(s);
  }
  void print2(String s)
  {
    if(parsecount==1)
    System.out.print(s);
  }

  void setnext(int curr)
  {
    if(parsecount==0)
      {
        ArrayList <Integer> templist;
        templist = nextstatement.get(curr);
        if(templist==null)templist = new ArrayList <Integer>();
        templist.add(curr+1);

          String z = nextlabel1.get(curr);
          if(z!=null)
          {
            int y = tempfunction.labels.get(z);
            templist.add(y);
          }
          z = nextlabel2.get(curr);
          if(z!=null)
          {
            int y = tempfunction.labels.get(z);
            templist.clear();
            templist.add(y);
          }
        nextstatement.put(curr,templist); 
      }
  }
  void analysislive()
  {
    if(parsecount==0)
    {
      int livenesscount = 0;
      while(livenesscount<10)
      {
        livenesscount++;
        for(int i=statementcount-1;i>=0;i--)
        {
          ArrayList<Integer> tempnextstmts = nextstatement.get(i);
          for(int j=0;j<tempnextstmts.size();j++)
          {
            ArrayList<String> a1 = tempfunction.in.get(tempnextstmts.get(j));
            if(a1!=null)
            {
               a1 = union( tempfunction.out.get(i),a1);
               tempfunction.out.put(i,a1);
            }
          }

          
          if(def.get(i)!=null)
          {
              ArrayList<String> a2 = tempfunction.out.get(i);
              a2 = union( a2,def.get(i));
              tempfunction.out.put(i,a2);
            
          }
          ArrayList<String> tempin = new ArrayList<String>();
          tempin.addAll( tempfunction.out.get(i));
          if(def.get(i)!=null)
          tempin.removeAll(def.get(i));
          if(use.get(i)!=null)
          tempin = union(tempin,use.get(i));
          tempfunction.in.put(i,tempin); 
        }
      
      }
    }
  }
  void intervals()
  {
    if(parsecount==0)
    {
      for(int i=0;i<statementcount;i++)
      {
        ArrayList<String> tempin = tempfunction.in.get(i);
        ArrayList<String> tempout = tempfunction.out.get(i);
        //tempin = union(tempout,tempin);
        for(int j=0;j<tempout.size();j++)
        {
          if(tempfunction.livein.get(tempout.get(j))==null)
          tempfunction.livein.put(tempout.get(j),i);
          else
          {
            int x = tempfunction.livein.get(tempout.get(j));
            if(i<x)tempfunction.livein.put(tempout.get(j),i);
          }
          if(tempfunction.liveout.get(tempout.get(j))==null)
          tempfunction.liveout.put(tempout.get(j),i);
          else
          {
            int x = tempfunction.liveout.get(tempout.get(j));
            if(i>x)tempfunction.liveout.put(tempout.get(j),i);
          }
        }
        //tempin = union(tempout,tempin);
        for(int j=0;j<tempin.size();j++)
        {
          if(tempfunction.livein.get(tempin.get(j))==null)
          tempfunction.livein.put(tempin.get(j),i);
          else
          {
            int x = tempfunction.livein.get(tempin.get(j));
            if(i<x)tempfunction.livein.put(tempin.get(j),i);
          }
          if(tempfunction.liveout.get(tempin.get(j))==null)
          tempfunction.liveout.put(tempin.get(j),i);
          else
          {
            int x = tempfunction.liveout.get(tempin.get(j));
            if(i>x)tempfunction.liveout.put(tempin.get(j),i);
          }
        }
      }
      Set<String> hs = new HashSet<>();
      hs.addAll(opentemps);
      opentemps.clear();
      opentemps.addAll(hs);
    }
  }
  int linearscan(int stackcount)
  {
    if(parsecount==0)
    {
      ArrayList <inter> inters = new ArrayList <inter>();
      for(int i=0;i<opentemps.size();i++)
      {
         inter tempinter = new inter();
         tempinter.name = opentemps.get(i);
         if(tempfunction.livein.get(tempinter.name)==null)
         {
          tempinter.start = 0;
         }
         else
         {
           tempinter.start = tempfunction.livein.get(tempinter.name);
         }
         if(tempfunction.liveout.get(tempinter.name)==null)
         {
          tempinter.end = stackcount;
         }
         else
         {
           tempinter.end = tempfunction.liveout.get(tempinter.name);
         }
         tempinter.active = -1;
         inters.add(tempinter);
      }
      Collections.sort(inters, new Comparator(){

        public int compare(Object o1, Object o2) {
            inter p1 = (inter) o1;
            inter p2 = (inter) o2;
            if(p1.start<p2.start)
            return -1;
            else if(p1.start==p2.start)
            return 0;
            else return 1;
        }
      });
      int activelength = 0;
      for(int i=0;i<inters.size();i++) {
        for(int k=0;k<inters.size();k++) {
          if(inters.get(k).active==1) {
            if(inters.get(k).end<inters.get(i).start) {
              //System.out.println("Deassigning temp"+inters.get(k).name);
              int index = tempfunction.temptoreg.get(inters.get(k).name);
              tempfunction.registers[index] = null;
              inters.get(k).active = -1;
              activelength--;
            }
          }
        }
        if(activelength>18) {
          tempfunction.temptoreg.put(inters.get(i).name,-1-stackcount);
          stackcount++;
        } 
        else {
          activelength++;
          int j;
          for(j=4;j<22;j++){
            if(tempfunction.registers[j]==null)break;
          }
          tempfunction.registers[j] = inters.get(i).name;
          inters.get(i).active = 1;
          tempfunction.temptoreg.put(inters.get(i).name,j);
          //System.out.println("Assigned temp"+inters.get(i).name+" value "+j);
        } 
      }
      if(tempfunction.procedurelabels[0]<4&&tempfunction.procedurelabels[2]<4)
      tempfunction.procedurelabels[1] = stackcount+18;
      else
      {
        tempfunction.procedurelabels[1] = stackcount+18+tempfunction.procedurelabels[0]+tempfunction.procedurelabels[2]-3;
      }
    }
    return stackcount;
  }
  String indextoreg(int index)
  {
    if(index<14)
    {
      return "t"+(index-4);
    }
    else if(index<22)
    {
      return "s"+(index-14);
    }
    return null;
  }


  int tempcheck(Temp x)
  {
    if(parsecount==1)
      {
        IntegerLiteral nodetemp = x.f1;
        String y = nodetemp.f0.toString();
        return globalmap.get(currentprocedure).temptoreg.get(y);
      }
    return 0;
  }
  ArrayList<String> union(ArrayList<String> list1, ArrayList<String> list2) {
        Set<String> set = new HashSet<String>();

        set.addAll(list1);
        set.addAll(list2);

        return new ArrayList<String>(set);
    }
    ArrayList<String> intersection(ArrayList<String> list1, ArrayList<String> list2) {
        ArrayList<String> list = new ArrayList<String>();

        for (String t : list1) {
            if(list2.contains(t)) {
                list.add(t);
            }
        }

        return list;
      }
  
   public R visit(NodeList n, A argu) {
      R _ret=null;
      int _count=0;
      for ( Enumeration<Node> e = n.elements(); e.hasMoreElements(); ) {
         e.nextElement().accept(this,argu);
         _count++;
      }
      return _ret;
   }

   public R visit(NodeListOptional n, A argu) {
      if ( n.present() ) {
         R _ret=null;
         int _count=0;
         for ( Enumeration<Node> e = n.elements(); e.hasMoreElements(); ) {
            e.nextElement().accept(this,argu);
            _count++;
         }
         if(tempfunction.procedurelabels[2]<_count && flag3==1 &&parsecount==0)
         tempfunction.procedurelabels[2] = _count;
         return _ret;
      }
      else
         return null;
   }

   public R visit(NodeOptional n, A argu) {
      if ( n.present() )
      {
         if(parsecount==0)
         {
           tempfunction.labels.put((String)n.node.accept(this,argu),statementcount);
         }
         return n.node.accept(this,argu);
      }
      else
         return null;
   }

   public R visit(NodeSequence n, A argu) {
      R _ret=null;
      int _count=0;
      for ( Enumeration<Node> e = n.elements(); e.hasMoreElements(); ) {
         e.nextElement().accept(this,argu);
         _count++;
      }
      return _ret;
   }


   public R visit(NodeToken n, A argu) { return null; }

   //
   // User-generated visitor methods below
   //

   

   /**
    * f0 -> "MAIN"
    * f1 -> StmtList()
    * f2 -> "END"
    * f3 -> ( Procedure() )*
    * f4 -> <EOF>
    */
   public R visit(Goal n, A argu) {
      R _ret=null;
      if(parsecount==0)
      {
        statementcount = 0;
        tempfunction = new func();
        tempfunction.procedurelabels[0] = tempfunction.procedurelabels[1] = tempfunction.procedurelabels[2] = 0;
        use.clear();
        def.clear();
        nextstatement.clear();
        nextlabel1.clear();
        nextlabel2.clear();
      }
      n.f0.accept(this, argu);
      currentprocedure = "MAIN";
      if(parsecount==1){
        print2(currentprocedure + "    ["+globalmap.get(currentprocedure).procedurelabels[0]+"]");
        print(" ["+globalmap.get(currentprocedure).procedurelabels[1]+"] ["+globalmap.get(currentprocedure).procedurelabels[2]+"]");
      }
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      print("END");
      if(parsecount==0)
      {
        for(int i=0;i<statementcount;i++)
        {
          setnext(i);

          ArrayList<String> templist = new ArrayList<String>();
           tempfunction.out.put(i,templist);
          templist = new ArrayList<String>();
           tempfunction.in.put(i,templist);
        }
        analysislive();
        intervals();
        tempfunction.bigvarcount = linearscan(0);
      }
      if(parsecount==0)
      {
        
        globalmap.put(currentprocedure,tempfunction);
      }
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      print("// Number of  vars after packing = 0; Number of Spilled vars = 0");
      parsecount++;
      return _ret;
   }

   /**
    * f0 -> ( ( Label() )? Stmt() )*
    */
   public R visit(StmtList n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> Label()
    * f1 -> "["
    * f2 -> IntegerLiteral()
    * f3 -> "]"
    * f4 -> StmtExp()
    */
   public R visit(Procedure n, A argu) {
      R _ret=null;
      if(parsecount==0)
      {
        statementcount = 0;
        tempfunction = new func();
        tempfunction.bigvarcount = 0;
        tempfunction.procedurelabels[0] = tempfunction.procedurelabels[1] = tempfunction.procedurelabels[2] = 0;
        use.clear();
        def.clear();
        nextstatement.clear();
        opentemps.clear();        
        nextlabel1.clear();
        nextlabel2.clear();
      }
      currentprocedure = (String)n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      IntegerLiteral nodetemp = n.f2;
      String x = nodetemp.f0.toString();
      if(parsecount==0)
      {
        tempfunction.procedurelabels[0] = Integer.parseInt(x);
      }
      n.f3.accept(this, argu);
      if(parsecount==1){
        print2("   ["+globalmap.get(currentprocedure).procedurelabels[0]+"]");
        print(" ["+globalmap.get(currentprocedure).procedurelabels[1]+"] ["+globalmap.get(currentprocedure).procedurelabels[2]+"]");
        int noofargs = globalmap.get(currentprocedure).procedurelabels[0];
        for(int i=0;i<noofargs;i++)
        {
          if(i<4)
          print("MOVE "+indextoreg(globalmap.get(currentprocedure).temptoreg.get(Integer.toString(i)))+" a"+i);
          else
          {
            print("ALOAD "+indextoreg(globalmap.get(currentprocedure).temptoreg.get(Integer.toString(i)))+" SPILLEDARG "+(i-4)); 
          }
        }

      }
      n.f4.accept(this, argu);
      for(int i=0;i<tempfunction.procedurelabels[0];i++)
      {
        if(parsecount==0)
        opentemps.add(Integer.toString(i));
      }
      if(parsecount==0)
      {
        for(int i=0;i<statementcount;i++)
        {
          setnext(i);
          ArrayList<String> templist = new ArrayList<String>();
           tempfunction.out.put(i,templist);
          templist = new ArrayList<String>();
           tempfunction.in.put(i,templist);
        }
        analysislive();
        intervals();
        tempfunction.bigvarcount = linearscan(0);
      }
      if(parsecount==0)
      {
        /*for(int i=0;i<statementcount;i++)
        {
          ArrayList<String> templist = tempfunction.in.get(i);
          ArrayList<String> templist2 = tempfunction.out.get(i);
          if(templist!=null)
          {
            for(int j=0;j<templist.size();j++)
            {
              System.out.print("i is "+i+" in is "+templist.get(j)+" ");
            }
            System.out.println(" ");
          }
          if(templist2!=null)
          {
            for(int j=0;j<templist2.size();j++)
            {
              System.out.print("i is "+i+" out is "+templist2.get(j)+" ");
            }
            System.out.println(" ");
          }
        }
        System.out.println(currentprocedure);
        for(int i=0;i<opentemps.size();i++)
        {
           String a = opentemps.get(i);
           System.out.println("tempvar is temp"+a+" in is "+tempfunction.livein.get(a)+" out is "+tempfunction.liveout.get(a));
           System.out.println("Assigned temp"+a+" is "+tempfunction.temptoreg.get(a));
        }*/
        globalmap.put(currentprocedure,tempfunction);
      }
      return _ret;
   }

   /**
    * f0 -> NoOpStmt()
    *       | ErrorStmt()
    *       | CJumpStmt()
    *       | JumpStmt()
    *       | HStoreStmt()
    *       | HLoadStmt()
    *       | MoveStmt()
    *       | PrintStmt()
    */
   public R visit(Stmt n, A argu) {
      R _ret=null;
      String x = (String)n.f0.accept(this, argu);
      if(parsecount==0)
      {
        if(n.f0.which==2)
        {
          nextlabel1.put(statementcount,x);
        }
        else if(n.f0.which==3)
        {
          nextlabel2.put(statementcount,x); 
        } 
        statementcount++;
      }
      return _ret;
   }

   /**
    * f0 -> "NOOP"
    */
   public R visit(NoOpStmt n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      print("NOOP");
      return _ret;
   }

   /**
    * f0 -> "ERROR"
    */
   public R visit(ErrorStmt n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      print("ERROR");
      return _ret;
   }

   /**
    * f0 -> "CJUMP"
    * f1 -> Temp()
    * f2 -> Label()
    */
   public R visit(CJumpStmt n, A argu) {
      R _ret=null;
      int flagtemp;
      if(parsecount==1)
      {
        flagtemp = tempcheck(n.f1);
        if(flagtemp<0)
        {
          int argsinstack = globalmap.get(currentprocedure).procedurelabels[0];
          if(argsinstack<4)argsinstack = 0;
          else argsinstack-=4;
          print("ALOAD v1 SPILLEDARG "+(18+argsinstack-flagtemp-1));
          print2("CJUMP v1 ");
          _ret = n.f2.accept(this, argu);
          print(" ");
        }
        else
        {
          n.f0.accept(this, argu);
          print2("CJUMP ");
          n.f1.accept(this, argu);
          _ret = n.f2.accept(this, argu);
          print(" ");
        }
      }
      else
      {
        n.f0.accept(this, argu);
        print2("CJUMP ");
        n.f1.accept(this, argu);
        _ret = n.f2.accept(this, argu);
        print(" "); 
      }
      return _ret;
   }

   /**
    * f0 -> "JUMP"
    * f1 -> Label()
    */
   public R visit(JumpStmt n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      print2("JUMP ");
      _ret = n.f1.accept(this, argu);
      print(" ");
      return _ret;
   }

   /**
    * f0 -> "HSTORE"
    * f1 -> Temp()
    * f2 -> IntegerLiteral()
    * f3 -> Temp()
    */
   public R visit(HStoreStmt n, A argu) {
      R _ret=null;
      int flagtemp;
      int flagtemp2;
      if(parsecount==1)
      {
        flagtemp = tempcheck(n.f1);
        flagtemp2 = tempcheck(n.f3);
        if(flagtemp<0)
        {
          int argsinstack = globalmap.get(currentprocedure).procedurelabels[0];
          if(argsinstack<4)argsinstack = 0;
          else argsinstack-=4;
          print("ALOAD v1 SPILLEDARG "+(18+argsinstack-flagtemp-1));;
        }
        if(flagtemp2<0)
        {
          int argsinstack = globalmap.get(currentprocedure).procedurelabels[0];
          if(argsinstack<4)argsinstack = 0;
          else argsinstack-=4;
          print("ALOAD v0 SPILLEDARG "+(18+argsinstack-flagtemp2-1));
        }
          n.f0.accept(this, argu);
          print2("HSTORE ");
          if(flagtemp>=0)
          n.f1.accept(this, argu);
          else
          print2("v1 ");
          n.f2.accept(this, argu);
          if(flagtemp2>=0)
          n.f3.accept(this, argu);
          else
          print2("v0 ");

          print(" ");
        
      }
      else
      {   
        n.f0.accept(this, argu);
        print2("HSTORE ");
        n.f1.accept(this, argu);
        n.f2.accept(this, argu);
        n.f3.accept(this, argu);
        print(" "); 
      }
      return _ret;
   }

   /**
    * f0 -> "HLOAD"
    * f1 -> Temp()
    * f2 -> Temp()
    * f3 -> IntegerLiteral()
    */
   public R visit(HLoadStmt n, A argu) {
      R _ret=null;
      int flagtemp;
      int flagtemp2;
      if(parsecount==1)
      {
        flagtemp = tempcheck(n.f1);
        flagtemp2 = tempcheck(n.f2);
        if(flagtemp2<0)
        {
          int argsinstack = globalmap.get(currentprocedure).procedurelabels[0];
          if(argsinstack<4)argsinstack = 0;
          else argsinstack-=4;
          print("ALOAD v0 SPILLEDARG "+(18+argsinstack-flagtemp2-1));
        }
          n.f0.accept(this, argu);
          print2("HLOAD ");
          if(flagtemp>=0)
          n.f1.accept(this, argu);
          else
          print2("v1 ");
          if(flagtemp2>=0)
          n.f2.accept(this, argu);
          else
          print2("v0 ");
          n.f3.accept(this, argu);
          print(" ");
          if(flagtemp<0)
          {
            int argsinstack = globalmap.get(currentprocedure).procedurelabels[0];
            if(argsinstack<4)argsinstack = 0;
            else argsinstack-=4;
            print("ASTORE SPILLEDARG "+(18+argsinstack-flagtemp-1)+" v1");
          }
        
      }
      else
      {   
        n.f0.accept(this, argu);
        print2("HLOAD ");
        flagdef = 1;
        n.f1.accept(this, argu);
        flagdef =-1;
        n.f2.accept(this, argu);
        n.f3.accept(this, argu);
        print(" "); 
      }
      return _ret;
   }

   /**
    * f0 -> "MOVE"
    * f1 -> Temp()
    * f2 -> Exp()
    */
   public R visit(MoveStmt n, A argu) {
      R _ret=null;
      int flagtemp;
      int flagtemp2;
      if(parsecount==1)
      {
        flagtemp = tempcheck(n.f1);
          Exp x = n.f2;
          if(x.f0.which==0)
          {
              String y = (String) n.f2.accept(this, argu);
            print2("MOVE ");
            flagdef = 1;
            if(flagtemp>=0)
            n.f1.accept(this, argu);
            else
            print2("v1 ");
            flagdef = -1;
            print("v0");
          }
          else if(x.f0.which==2)
          {
            n.f2.accept(this,argu);
            print2("MOVE ");
            flagdef=1;
            if(flagtemp>=0)
            n.f1.accept(this, argu);
            else
            print2("v1 ");
            flagdef=-1;
            print("v0 ");
          }
          else if(x.f0.which==1)
          {
            n.f2.accept(this,argu);
            print2("MOVE ");
            flagdef=1;
            if(flagtemp>=0)
            n.f1.accept(this, argu);
            else
            print2("v1 ");
            flagdef=-1;
            print("v0 ");
          }
          else
          {
            n.f0.accept(this, argu);
            print2("MOVE ");
            flagdef=1;
            if(flagtemp>=0)
            n.f1.accept(this, argu);
            else
            print2("v1 ");
            flagdef=-1;
            n.f2.accept(this, argu);
            print(" ");
          }
          if(flagtemp<0)
          {
            int argsinstack = globalmap.get(currentprocedure).procedurelabels[0];
            if(argsinstack<4)argsinstack = 0;
            else argsinstack-=4;
            print("ASTORE SPILLEDARG "+(18+argsinstack-flagtemp-1)+" v1");
          }
        
      }
      else
      {   
        Exp x = n.f2;
        if(x.f0.which==0)
        {
            String y = (String) n.f2.accept(this, argu);
          print2("MOVE ");
          flagdef = 1;
          n.f1.accept(this, argu);
          flagdef = -1;
          print("v0");
        }
        else if(x.f0.which==2)
        {
          n.f2.accept(this,argu);
          print2("MOVE ");
          flagdef=1;
          n.f1.accept(this, argu);
          flagdef=-1;
          print("v0 ");
        }
        else
        {
          n.f0.accept(this, argu);
          print2("MOVE ");
          flagdef=1;
          n.f1.accept(this, argu);
          flagdef=-1;
          n.f2.accept(this, argu);
          print(" ");
        } 
      }
      return _ret;
   }

   /**
    * f0 -> "PRINT"
    * f1 -> SimpleExp()
    */
   public R visit(PrintStmt n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      argu = (A)("1");
      n.f1.accept(this, argu);
      print2("PRINT v0");
      print(" ");
      return _ret;
   }

   /**
    * f0 -> Call()
    *       | HAllocate()
    *       | BinOp()
    *       | SimpleExp()
    */
   public R visit(Exp n, A argu) {
      R _ret=null;
      tempargs.clear();
      n.f0.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> "BEGIN"
    * f1 -> StmtList()
    * f2 -> "RETURN"
    * f3 -> SimpleExp()
    * f4 -> "END"
    */
   public R visit(StmtExp n, A argu) {
      R _ret=null;
      if(parsecount==1)
      {
        for(int i=14;i<22;i++)
        {
          if(globalmap.get(currentprocedure).registers[i]!=null||true)
          {
            if(globalmap.get(currentprocedure).procedurelabels[0]<4)
            print("ASTORE SPILLEDARG "+(globalmap.get(currentprocedure).bigvarcount+i-4)+" "+indextoreg(i));
            else
            {
              print("ASTORE SPILLEDARG "+(globalmap.get(currentprocedure).bigvarcount+i-4
                +globalmap.get(currentprocedure).procedurelabels[0]-3)+" "+indextoreg(i));
            }
          }
        }
      }
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      print2("MOVE v0 ");
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      print(" ");
      if(parsecount==1)
      {
        for(int i=14;i<22;i++)
        {
          
        if(globalmap.get(currentprocedure).registers[i]!=null||true)
          {
            if(globalmap.get(currentprocedure).procedurelabels[0]<4)
            print("ALOAD "+indextoreg(i)+" SPILLEDARG "+(globalmap.get(currentprocedure).bigvarcount+i-4));
            else
            {
              print("ALOAD "+indextoreg(i)+" SPILLEDARG "+(globalmap.get(currentprocedure).bigvarcount+i-4+
                globalmap.get(currentprocedure).procedurelabels[0]-3));
            }
          }
        }
      }
      print("\nEND");
      statementcount++;
      return _ret;
   }

   /**
    * f0 -> "CALL"
    * f1 -> SimpleExp()
    * f2 -> "("
    * f3 -> ( Temp() )*
    * f4 -> ")"
    */
   public R visit(Call n, A argu) {
      R _ret=null;
      flag3 = 1;
      n.f3.accept(this, argu);
      flag3 = -1;
      
      if(parsecount==1)
      {
        for(int i=4;i<14;i++)
        {
          if(globalmap.get(currentprocedure).registers[i]!=null||true)
          {
            if(globalmap.get(currentprocedure).procedurelabels[0]<4)
            print("ASTORE SPILLEDARG "+(globalmap.get(currentprocedure).bigvarcount+i-4)+" "+indextoreg(i));
            else
            {
              print("ASTORE SPILLEDARG "+(globalmap.get(currentprocedure).bigvarcount+i-4
                +globalmap.get(currentprocedure).procedurelabels[0]-4)+" "+indextoreg(i));
            }
          }
        }
      }
      for(int i=0;i<tempargs.size();i++)
      {
        if(parsecount==1)
        {
          if(i<4)
          {
            String tempvarno = tempargs.get(i);
            print2("MOVE a"+i+" ");
            print2(indextoreg(globalmap.get(currentprocedure).temptoreg.get(tempvarno))+" ");
            print(" ");
          }
          else
          {
            String tempvarno = tempargs.get(i);
            print2("PASSARG "+(i-3)+" ");
            print2(indextoreg(globalmap.get(currentprocedure).temptoreg.get(tempvarno))+" ");
            print(" ");
          }
        }
      }
      n.f0.accept(this, argu);
      print2("CALL ");
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      print(" ");
      n.f4.accept(this, argu);
      if(parsecount==1)
      {
        for(int i=4;i<14;i++)
        {
          if(globalmap.get(currentprocedure).registers[i]!=null||true)
          {
          if(globalmap.get(currentprocedure).procedurelabels[0]<4)
            print("ALOAD "+indextoreg(i)+" SPILLEDARG "+(globalmap.get(currentprocedure).bigvarcount+i-4));
            else
            {
              print("ALOAD "+indextoreg(i)+" SPILLEDARG "+(globalmap.get(currentprocedure).bigvarcount+i-4+
                globalmap.get(currentprocedure).procedurelabels[0]-4));
            }
          }
        }
      }
      return _ret;
   }

   /**
    * f0 -> "HALLOCATE"
    * f1 -> SimpleExp()
    */
   public R visit(HAllocate n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      argu = (A)("1");
      n.f1.accept(this, argu);
      print2("MOVE v0 HALLOCATE v0");
      print(" ");
      return _ret;
   }

   /**
    * f0 -> Operator()
    * f1 -> Temp()
    * f2 -> SimpleExp()
    */
   public R visit(BinOp n, A argu) {
      R _ret=null;
      Operator x = n.f0;
      argu = (A)("2");
      n.f2.accept(this, argu);
      if(x.f0.which==0)
      {
        print2("MOVE ");
        n.f1.accept(this, argu);
        print2("MINUS ");
        n.f1.accept(this, argu);
        print2("1\nMOVE v0 ");
        n.f0.accept(this, argu);
        n.f1.accept(this, argu);
        print2("a0 ");
        print(" ");
        print2("MOVE ");
        n.f1.accept(this, argu);
        print2("PLUS ");
        n.f1.accept(this, argu);
        print("1");
      }
      else if(x.f0.which==1)
      {
        print2("MOVE v1 ");
        print2("a0 ");
        print2("\nMOVE v0 LT v1 ");
        n.f1.accept(this, argu);
        print2("\nMOVE v1 LT ");
        n.f1.accept(this, argu);
        print2("a0 ");
        print2("\nMOVE v0 PLUS v0 v1\n");
      }
      else
      {
        print2("MOVE v0 ");
        n.f0.accept(this, argu);
        n.f1.accept(this, argu);
        print2("a0 ");
        print(" ");
      }
      return _ret;
   }

   /**
    * f0 -> "LE"
    *       | "NE"
    *       | "PLUS"
    *       | "MINUS"
    *       | "TIMES"
    *       | "DIV"
    */
   public R visit(Operator n, A argu) {
      R _ret=null;
      int x = n.f0.which;
      switch(x)
      {
        case 0:
        {
          print2("LT ");break;
        }
        case 1:print2("NE ");break;
        case 2:print2("PLUS ");break;
        case 3:print2("MINUS ");break;
        case 4:print2("TIMES ");break;
        case 5:print2("DIV ");break;
      }
      return _ret;
   }

   /**
    * f0 -> Temp()
    *       | IntegerLiteral()
    *       | Label()
    */
   public R visit(SimpleExp n, A argu) {
      R _ret=null;
      flagsimple = 1;
      String tempstr = (String)n.f0.accept(this, argu);
      flagsimple = -1;
      return _ret;
   }

   /**
    * f0 -> "TEMP"
    * f1 -> IntegerLiteral()
    */
   public R visit(Temp n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      IntegerLiteral nodetemp = n.f1;
      String x = nodetemp.f0.toString();
      if(parsecount==0)
        opentemps.add(x);
          
      if(parsecount==1)
      {
        if(flag3==-1)
        {
          if(flagsimple==-1)print2(indextoreg(globalmap.get(currentprocedure).temptoreg.get(x))+" ");
          else
          {
            String argstr = (String)(argu);
            if(argstr!=null && argstr.equals("1"))
            {
              int flagtemp2 = tempcheck(n);
              if(flagtemp2<0)
              {
                int argsinstack = globalmap.get(currentprocedure).procedurelabels[0];
                if(argsinstack<4)argsinstack = 0;
                else argsinstack-=4;
                print("ALOAD v0 SPILLEDARG "+(18+argsinstack-flagtemp2-1));
              }
              else
              {
                print2("MOVE v0 ");
                print(indextoreg(globalmap.get(currentprocedure).temptoreg.get(x)));   
              }
            }
            else if(argstr!=null && argstr.equals("2"))
            {
              int flagtemp2 = tempcheck(n);
              if(flagtemp2<0)
              {
                int argsinstack = globalmap.get(currentprocedure).procedurelabels[0];
                if(argsinstack<4)argsinstack = 0;
                else argsinstack-=4;
                print("ALOAD a0 SPILLEDARG "+(18+argsinstack-flagtemp2-1));
              }
              else
              {
                print2("MOVE a0 ");
                print(indextoreg(globalmap.get(currentprocedure).temptoreg.get(x)));   
              }
            }
            else
            {
              print2(indextoreg(globalmap.get(currentprocedure).temptoreg.get(x))+" ");
              _ret = (R)(indextoreg(globalmap.get(currentprocedure).temptoreg.get(x))+" ");
            }
          }
        }
        else
        {
          tempargs.add(x);
        }
      }
      //Calculating use and def
      if(parsecount==0)
      {
        if(flagdef==-1){
          ArrayList <String> templist;
          templist = use.get(statementcount);
          if(templist==null)
          {
            templist = new ArrayList <String>();
            templist.add(x);
            use.put(statementcount,templist); 
          }
          else
          {
            templist.add(x);
          }  
        }
        else
        {
          ArrayList <String> templist;
          templist = def.get(statementcount);
          if(templist==null)
          {
            templist = new ArrayList <String>();
            templist.add(x);
            def.put(statementcount,templist); 
          }
          else
          {
            templist.add(x);
          }
        }
      }
      return _ret;
   }

   /**
    * f0 -> <INTEGER_LITERAL>
    */
   public R visit(IntegerLiteral n, A argu) {
      R _ret=null;
      _ret=(R)(String)(n.f0.toString());
      print2((String)_ret + " ");
      return _ret;
   }

   /**
    * f0 -> <IDENTIFIER>
    */
   public R visit(Label n, A argu) {
      R _ret=null;
      _ret=(R)(String)(n.f0.toString());
      print2((String)_ret +" ");
      return _ret;
   }


}
