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
public class GJDepthFirst<R,A> implements GJVisitor<R,A> {
   //
   // Auto class visitors--probably don't need to be overridden.
   //
  int flag = -1;
  int tempcount = 200;
  ArrayList <String>temparglist = new ArrayList<String>();
  HashMap <Node,String> exprtostring = new HashMap <Node,String>();
  String getTemp()
  {
    return "TEMP " + Integer.toString(tempcount++); 
  }
  String getTemp(int x)
  {
    return "TEMP " + Integer.toString(tempcount-x); 
  }
  String stringstore(String a,String b)
  {
    b = b+" "+a;
    return b;
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
            String a = (String)e.nextElement().accept(this,argu);
            if(a!=null)
            temparglist.add(a);
            _count++;
         }
         return _ret;
      }
      else
         return null;
   }

   public R visit(NodeOptional n, A argu) {
      if ( n.present() )
      {
         String a=(String)n.node.accept(this,argu);
         System.out.println(a);
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
      System.out.println("MAIN");
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      System.out.println("END");
      n.f2.accept(this, argu);
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
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
      flag = 1;
      String x = (String)n.f0.accept(this, argu);
      System.out.println(x);
      n.f1.accept(this, argu);
      System.out.println("[");
      String y = (String)n.f2.accept(this, argu);
      System.out.println(y);
      System.out.println("]");
      n.f3.accept(this, argu);
      flag = 1;
      n.f4.accept(this, argu);
      flag = -1;
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
      n.f0.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> "NOOP"
    */
   public R visit(NoOpStmt n, A argu) {
      R _ret=null;
      System.out.println("NOOP");
      n.f0.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> "ERROR"
    */
   public R visit(ErrorStmt n, A argu) {
      R _ret=null;
      System.out.println("ERROR");
      n.f0.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> "CJUMP"
    * f1 -> Exp()
    * f2 -> Label()
    */
   public R visit(CJumpStmt n, A argu) {
      R _ret=null;
      
        String temp1 = (String)n.f1.accept(this, argu);
        System.out.println("CJUMP");
        n.f0.accept(this, argu);
        System.out.println(temp1);
        String x = (String)n.f2.accept(this, argu);
        System.out.println(x);
      
      return _ret;
   }

   /**
    * f0 -> "JUMP"
    * f1 -> Label()
    */
   public R visit(JumpStmt n, A argu) {
      R _ret=null;
      System.out.println("JUMP");
      n.f0.accept(this, argu);
      String x = (String)n.f1.accept(this, argu);
      System.out.println(x);
      return _ret;
   }

   /**
    * f0 -> "HSTORE"
    * f1 -> Exp()
    * f2 -> IntegerLiteral()
    * f3 -> Exp()
    */
   public R visit(HStoreStmt n, A argu) {
      R _ret=null;
      String temp1 = (String)n.f1.accept(this, argu);
      String temp2 = (String)n.f3.accept(this, argu);
      System.out.println("HSTORE");
      n.f0.accept(this, argu);
      System.out.println(temp1);
      String x = (String)n.f2.accept(this, argu);
      System.out.println(x);
      System.out.println(temp2);
      return _ret;
   }

   /**
    * f0 -> "HLOAD"
    * f1 -> Temp()
    * f2 -> Exp()
    * f3 -> IntegerLiteral()
    */
   public R visit(HLoadStmt n, A argu) {
      R _ret=null;
      String temp1 = (String)n.f2.accept(this, argu);
      System.out.println("HLOAD");
      n.f0.accept(this, argu);
      String y = (String)n.f1.accept(this, argu);
      System.out.println(y);
      System.out.println(temp1);
      String x = (String)n.f3.accept(this, argu);
      System.out.println(x);
      return _ret;
   }

   /**
    * f0 -> "MOVE"
    * f1 -> Temp()
    * f2 -> Exp()
    */
   public R visit(MoveStmt n, A argu) {
      R _ret=null;
      String x = (String)n.f2.accept(this, argu);
      System.out.println("MOVE");
      n.f0.accept(this, argu);
      String z = (String)n.f1.accept(this, argu);
      System.out.println(z);
      //Exp y = n.f2;
      System.out.println(x);
      //n.f2.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> "PRINT"
    * f1 -> Exp()
    */
   public R visit(PrintStmt n, A argu) {
      R _ret=null;
      String temp1 = (String)n.f1.accept(this, argu);
      System.out.println("PRINT");
      n.f0.accept(this, argu);
      System.out.println(temp1);
      return _ret;
   }

   /**
    * f0 -> StmtExp()
    *       | Call()
    *       | HAllocate()
    *       | BinOp()
    *       | Temp()
    *       | IntegerLiteral()
    *       | Label()
    */
   public R visit(Exp n, A argu) {
      int branch1 = n.f0.which;
      String temp1 = getTemp();
      if((branch1>=1))
      {
          String tempf = (String)n.f0.accept(this,argu);
          System.out.println("MOVE "+temp1);
          System.out.println(tempf);
      }
      else
      {
        String x = (String)n.f0.accept(this,argu);
	System.out.println("MOVE "+temp1);
          System.out.println(x);
      }
      R _ret=(R)temp1;
      return _ret;
   }

   /**
    * f0 -> "BEGIN"
    * f1 -> StmtList()
    * f2 -> "RETURN"
    * f3 -> Exp()
    * f4 -> "END"
    */
   public R visit(StmtExp n, A argu) {
      R _ret=null;
      if(flag==-1)
      {
            
            n.f0.accept(this, argu);
            n.f1.accept(this, argu);
            n.f2.accept(this, argu);
	    String x = (String)n.f3.accept(this, argu);
            n.f4.accept(this, argu);
            _ret = (R)x;
      }
      else
      {     
            flag = -1;
            System.out.println("BEGIN");
            n.f0.accept(this, argu);
            n.f1.accept(this, argu);
            n.f2.accept(this, argu);
            String x = (String)n.f3.accept(this, argu);
            System.out.println("RETURN "+x);;
            n.f4.accept(this, argu);
            System.out.println("END");
            
      }
      return _ret;
   }

   /**
    * f0 -> "CALL"
    * f1 -> Exp()
    * f2 -> "("
    * f3 -> ( Exp() )*
    * f4 -> ")"
    */
   public R visit(Call n, A argu) {
      R _ret=null;
      String tempf = " ";
      temparglist.clear();
      String temp1 = (String)n.f1.accept(this, argu);
      //x = exprtostring.get(n.f1);
      //tempf = stringstore(x,tempf);
      NodeListOptional templist = n.f3;
      n.f3.accept(this,argu);
      //tempf = stringstore(templist.toString(),tempf);
      tempf = stringstore("CALL",tempf);
      n.f0.accept(this, argu);
      tempf = stringstore(temp1,tempf);
      tempf = stringstore("(",tempf);
      n.f2.accept(this, argu);
      int i=0;
      for(i=0;i<temparglist.size()-1;i++)
      {
        tempf = stringstore(temparglist.get(i),tempf);
      }
      if(temparglist.size()>0)
       tempf = stringstore(temparglist.get(i),tempf);
      //n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      tempf = stringstore(")",tempf);
      _ret = (R)tempf;
      return _ret;
   }

   /**
    * f0 -> "HALLOCATE"
    * f1 -> Exp()
    */
   public R visit(HAllocate n, A argu) {
      R _ret=null;
      String tempf = " ";
      String temp1 = (String)n.f1.accept(this, argu);
      //x = exprtostring.get(n.f1);
      //tempf = stringstore(x,tempf);
      tempf = stringstore("HALLOCATE",tempf);
      n.f0.accept(this, argu);
      tempf = stringstore(temp1,tempf);
      _ret = (R)tempf;
      return _ret;
   }

   /**
    * f0 -> Operator()
    * f1 -> Exp()
    * f2 -> Exp()
    */
   public R visit(BinOp n, A argu) {
      R _ret=null;
      String tempf = " ";
      String temp1 = (String)n.f1.accept(this, argu);
      // String x = exprtostring.get(n.f1);
      // tempf = stringstore(x,tempf);
       String temp2 = (String)n.f2.accept(this, argu);
      // x = exprtostring.get(n.f2);
      // tempf = stringstore(x,tempf);
      String x = (String)n.f0.accept(this, argu);
      tempf = stringstore(x+" "+temp1+" "+temp2,tempf);
      _ret = (R)tempf;
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
      //n.f0.accept(this, argu);
      String tempf = " ";
      int x = n.f0.which;
      switch(x)
      {
        case 0:tempf = stringstore("LE",tempf);break;
        case 1:tempf = stringstore("NE",tempf);break;
        case 2:tempf = stringstore("PLUS",tempf);break;
        case 3:tempf = stringstore("MINUS",tempf);break;
        case 4:tempf = stringstore("TIMES",tempf);break;
        case 5:tempf = stringstore("DIV",tempf);break;
      }
      //System.out.println("LE");
      _ret = (R)tempf;
      return _ret;
   }

   /**
    * f0 -> "TEMP"
    * f1 -> IntegerLiteral()
    */
   public R visit(Temp n, A argu) {
      R _ret=null;
      String tempf = " ";
      n.f0.accept(this, argu);
      tempf = stringstore("TEMP",tempf);
      String x = (String)n.f1.accept(this, argu);
      tempf = stringstore(x,tempf);
      _ret = (R)tempf;
      return _ret;
   }

   /**
    * f0 -> <INTEGER_LITERAL>
    */
   public R visit(IntegerLiteral n, A argu) {
      R _ret=null;
      String tempf = " ";
      String x = n.f0.toString();
      tempf = stringstore(x,tempf);
      _ret = (R)tempf;
      return _ret;
   }

   /**
    * f0 -> <IDENTIFIER>
    */
   public R visit(Label n, A argu) {
      R _ret = null;
      String tempf = " ";
      String x = n.f0.toString();
      tempf = stringstore(x,tempf);
      _ret = (R)tempf;
      return _ret;
   }

}
