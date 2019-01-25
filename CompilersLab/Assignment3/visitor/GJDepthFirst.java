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
   int parsecount = 0;
   int tempIndex = 20;
   int labelIndex = 2;
   HashMap< String, HashMap<String, String> > classnameoffset = new HashMap<String, HashMap<String, String> >();
   HashMap< String, HashMap<String, String> > vartypedataclass = new HashMap<String, HashMap<String, String> >();
   HashMap<String,String> funcreturn =new HashMap<String,String>();
   HashMap< String, ArrayList<String> > classnamevars = new HashMap<String, ArrayList<String> >();
   HashMap< String, ArrayList<String> > classnamevartypes =  new HashMap<String, ArrayList<String> >();
   HashMap<String,String> funcfind =new HashMap<String,String>();
   HashMap<String,Integer> HVtable = new HashMap<String,Integer>();
   HashMap<String,String> H1 = new HashMap<String,String>();
   HashMap<String,String> H2 =new HashMap<String,String>();
   ArrayList<String> A1 = new ArrayList<String>();
   ArrayList<String> A2 = new ArrayList<String>();
   HashMap<String,String> classextend = new HashMap<String,String>();
   HashMap<String,String> funcvar = new HashMap<String,String>();
   ArrayList<String> argname = new ArrayList<String>();
   ArrayList<String> argtypes = new ArrayList<String>();
   //HashMap<Node,String> tempvars = new HashMap<Node,String>();
   HashMap<String,Integer> classnames = new HashMap<String,Integer>();
   String currentfunc = null;
   String currentclass = null;
   int funcnumber = 0;
   int varnumber = 0;
   String getTemp(){
    return "TEMP " + tempIndex++;
   }
   String getTemp(int x){
    return "TEMP " + (tempIndex-x);
   }
   String getLabel(){
    return "L" + labelIndex++;
   }
   String getLabel(int x){
    return "L" + (labelIndex-x);
   }
   String getoffset(String cname, String var)
   {  
      if(parsecount==0)return null;
      String temp1 = (classnameoffset.get(cname)).get(var);
      String cextname = classextend.get(cname);
      while(temp1==null)
      {
        if(cextname==null)break;
        temp1 = (classnameoffset.get(cextname)).get(var);
        cextname = classextend.get(cextname);
      }
      return temp1;
   }
   String checkifobjectorclass(String cname,String cnamecurr)
   {
      if(classnames.get(cname)==null)
      {
        String temp2 = funcfind.get(cname);
        if(temp2!=null)return temp2;
        String temp1 = vartypedataclass.get(cnamecurr).get(cname);
        String cextname = classextend.get(cnamecurr);
        while(temp1==null)
        {
          if(cextname==null)break;
          temp1 = (vartypedataclass.get(cextname)).get(cname);
          cextname = classextend.get(cextname);
        }
        if(temp1==null)
        return cname;
        else
        {
          return temp1;
        }
      }
      else
      {
        return cname;
      }
   }
   String findrettype(String cname, String fname)
   {
      String temp1 = cname+"_"+fname;
      String rettype = funcreturn.get(temp1);
      String cextname = classextend.get(cname);
      while(rettype==null)
      {
        if(cextname==null)break;
        temp1 = cextname+"_"+fname;
        rettype = funcreturn.get(temp1);
        cextname = classextend.get(cextname);
      }
      return rettype;
   }
   void print(String s)
   {
      if(parsecount>0)
      System.out.println(s);
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
         return _ret;
      }
      else
         return null;
   }

   public R visit(NodeOptional n, A argu) {
      if ( n.present() )
         return n.node.accept(this,argu);
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
    * f0 -> MainClass()
    * f1 -> ( TypeDeclaration() )*
    * f2 -> <EOF>
    */
   public R visit(Goal n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      if(parsecount==0)
      {
        
      }
      parsecount++;
      return _ret;
   }

   /**
    * f0 -> "class"
    * f1 -> Identifier()
    * f2 -> "{"
    * f3 -> "public"
    * f4 -> "static"
    * f5 -> "void"
    * f6 -> "main"
    * f7 -> "("
    * f8 -> "String"
    * f9 -> "["
    * f10 -> "]"
    * f11 -> Identifier()
    * f12 -> ")"
    * f13 -> "{"
    * f14 -> PrintStatement()
    * f15 -> "}"
    * f16 -> "}"
    */
   public R visit(MainClass n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      currentclass = (String)n.f1.accept(this, argu);
      classnames.put(currentclass,1);
      funcnumber = varnumber = 0;
      n.f2.accept(this, argu);
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      n.f5.accept(this, argu);
      n.f6.accept(this, argu);
      currentfunc = "main";
      funcnumber  = 4;
      if(parsecount>0)
      System.out.println("MAIN");
      n.f7.accept(this, argu);
      n.f8.accept(this, argu);
      n.f9.accept(this, argu);
      n.f10.accept(this, argu);
      n.f11.accept(this, argu);
      n.f12.accept(this, argu);
      n.f13.accept(this, argu);
      n.f14.accept(this, argu);
      n.f15.accept(this, argu);
      currentfunc = null;
      funcnumber = varnumber = 0;
      n.f16.accept(this, argu);
      currentclass = null;
      if(parsecount>0)
      System.out.println("END");
      return _ret;
   }

   /**
    * f0 -> ClassDeclaration()
    *       | ClassExtendsDeclaration()
    */
   public R visit(TypeDeclaration n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> "class"
    * f1 -> Identifier()
    * f2 -> "{"
    * f3 -> ( VarDeclaration() )*
    * f4 -> ( MethodDeclaration() )*
    * f5 -> "}"
    */
   public R visit(ClassDeclaration n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      currentclass = (String)n.f1.accept(this, argu);
      classnames.put(currentclass,1);
      funcnumber = varnumber = 0;
      n.f2.accept(this, argu);
      if(parsecount==0)
      {
        H1 = new HashMap<String,String>();
        H2 = new HashMap<String,String>();
        A1 = new ArrayList<String>();
        A2 = new ArrayList<String>();
        HVtable.put(currentclass,0);
      }
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      funcnumber = varnumber = 0;
      n.f5.accept(this, argu);
      if(parsecount==0)
      {
        classnameoffset.put(currentclass,H1);
        classnamevars.put(currentclass,A1);
        classnamevartypes.put(currentclass,A2);
        vartypedataclass.put(currentclass,H2);
      }
      currentclass = null;
      return _ret;
   }

   /**
    * f0 -> "class"
    * f1 -> Identifier()
    * f2 -> "extends"
    * f3 -> Identifier()
    * f4 -> "{"
    * f5 -> ( VarDeclaration() )*
    * f6 -> ( MethodDeclaration() )*
    * f7 -> "}"
    */
   public R visit(ClassExtendsDeclaration n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      currentclass = (String)n.f1.accept(this, argu);
      classnames.put(currentclass,1);
      funcnumber = varnumber = 0;
      n.f2.accept(this, argu);
      String pclass = (String)n.f3.accept(this, argu);
      if(parsecount==0)
      {
        classextend.put(currentclass,pclass);
      }
      n.f4.accept(this, argu);
      if(parsecount==0)
      {
        H1 = new HashMap<String,String>();
        A1 = new ArrayList<String>();
        A2 = new ArrayList<String>();
        H2 = new HashMap<String,String>();
        HVtable.put(currentclass,0);
      } 
      n.f5.accept(this, argu);
      n.f6.accept(this, argu);
      funcnumber = varnumber = 0;
      n.f7.accept(this, argu);
      if(parsecount==0)
      {
        classnameoffset.put(currentclass,H1);
        classnamevars.put(currentclass,A1);
        classnamevartypes.put(currentclass,A2);
        vartypedataclass.put(currentclass,H2);
      }
      currentclass = null;
      return _ret;
   }

   /**
    * f0 -> Type()
    * f1 -> Identifier()
    * f2 -> ";"
    */
   public R visit(VarDeclaration n, A argu) {
      R _ret=null;
      String vartype = (String)n.f0.accept(this, argu);
      String varname = (String)n.f1.accept(this, argu);
      //print(vartype+":"+varname+currentfunc);
      n.f2.accept(this, argu);        
      if(currentfunc==null)
      {
        if(parsecount==0)
        {  
          H1.put(varname,Integer.toString(varnumber*4+4));
          //if(varname.equals("number"))System.out.println(varnumber*4);
          A1.add(varname);
          A2.add(vartype);
          HVtable.put(currentclass,varnumber+1);
          H2.put(varname,vartype);
          //System.out.println(currentclass+currentfunc+vartype);
        }
        varnumber += 1;
      }
      else
      {
          //System.out.println("HEy"); 
          funcfind.put(varname,vartype);
          String x = getTemp();
          //System.out.println(x);
          funcvar.put(varname,x);
        
      }
      
      return _ret;
   }

   /**
    * f0 -> "public"
    * f1 -> Type()
    * f2 -> Identifier()
    * f3 -> "("
    * f4 -> ( FormalParameterList() )?
    * f5 -> ")"
    * f6 -> "{"
    * f7 -> ( VarDeclaration() )*
    * f8 -> ( Statement() )*
    * f9 -> "return")
    * f10 -> Expression()
    * f11 -> ";"
    * f12 -> "}"
    */
   public R visit(MethodDeclaration n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      String rettype = (String)n.f1.accept(this, argu);
      currentfunc = (String)n.f2.accept(this, argu);
      funcreturn.put(currentclass+"_"+currentfunc,rettype);
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      n.f5.accept(this, argu);
      print(currentclass+"_"+currentfunc+" ["+Integer.toString(argname.size()+1)+" ]");
      print("BEGIN");
      n.f6.accept(this, argu);
      n.f7.accept(this, argu);
      n.f8.accept(this, argu);
      n.f9.accept(this, argu);
      print("RETURN");
      n.f10.accept(this, argu);
      n.f11.accept(this, argu);
      n.f12.accept(this, argu);
      print("END");
      if(parsecount==0)
      {
        H1.put(currentfunc,Integer.toString(funcnumber));
        A1.add(currentfunc);
        A2.add("int");
      }
      funcvar.clear();
      funcnumber += 4;
      argname.clear();
      argtypes.clear();
      funcfind.clear();
      currentfunc=null;
      return _ret;
   }

   /**
    * f0 -> FormalParameter()
    * f1 -> ( FormalParameterRest() )*
    */
   public R visit(FormalParameterList n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      for(int i=0;i<argname.size();i++)
      {
        String tempname = argname.get(i);
        String temptype = argtypes.get(i);
        String s = "TEMP " + (i+1);
        funcvar.put(tempname,s);
        funcfind.put(tempname,temptype);
      }
      return _ret;
   }

   /**
    * f0 -> Type()
    * f1 -> Identifier()
    */
   public R visit(FormalParameter n, A argu) {
      R _ret=null;
      String argumtypes = (String)n.f0.accept(this, argu);
      String argumts = (String)n.f1.accept(this, argu);
      argname.add(argumts);
      argtypes.add(argumtypes);
      return _ret;
   }

   /**
    * f0 -> ","
    * f1 -> FormalParameter()
    */
   public R visit(FormalParameterRest n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> ArrayType()
    *       | BooleanType()
    *       | IntegerType()
    *       | Identifier()
    */
   public R visit(Type n, A argu) {
      R _ret=null;
      _ret = (R)(String)n.f0.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> "int"
    * f1 -> "["
    * f2 -> "array]"
    */
   public R visit(ArrayType n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      _ret = (R)"intarray";
      return _ret;
   }

   /**
    * f0 -> "boolean"
    */
   public R visit(BooleanType n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      _ret = (R)"boolean";
      return _ret;
   }

   /**
    * f0 -> "int"
    */
   public R visit(IntegerType n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      _ret = (R)"int";
      return _ret;
   }

   /**
    * f0 -> Block()
    *       | AssignmentStatement()
    *       | ArrayAssignmentStatement()
    *       | IfStatement()
    *       | WhileStatement()
    *       | PrintStatement()
    */
   public R visit(Statement n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> "{"
    * f1 -> ( Statement() )*
    * f2 -> "}"
    */
   public R visit(Block n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> Identifier()
    * f1 -> "="
    * f2 -> Expression()
    * f3 -> ";"
    */
   public R visit(AssignmentStatement n, A argu) {
      R _ret=null;
      String tempvar = (String)n.f0.accept(this, argu);
      String tempname = funcvar.get(tempvar);
      if(parsecount>0)
      {
        if(tempname!=null)
        print("MOVE "+tempname +" ");
        else
        {
            String offset = getoffset(currentclass,tempvar);
            print("HSTORE " +"TEMP 0 "+offset+" ");
        }
      }
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      n.f3.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> Identifier()
    * f1 -> "["
    * f2 -> Expression()
    * f3 -> "]"
    * f4 -> "="
    * f5 -> Expression()
    * f6 -> ";"
    */
   public R visit(ArrayAssignmentStatement n, A argu) {
      R _ret=null;
      String tempvar = (String)n.f0.accept(this, argu);
      String tempname = funcvar.get(tempvar);
      if(parsecount>0)
      {
        if(tempname!=null)
        {
          String temp1 = getTemp();
          String temp2 = getTemp();
          String temp3 = getTemp();
          String l1 = getLabel();
          String l2 = getLabel();
          print("MOVE "+temp1 +" PLUS "+tempname+" PLUS 4 TIMES 4");
          n.f1.accept(this, argu);
          n.f2.accept(this, argu);
          n.f3.accept(this, argu);
          print("CJUMP TIMES LE "+tempname+" "+temp1+" LE "+temp1);
          print("BEGIN\nHLOAD "+temp2+" "+tempname+" 0");
          print("MOVE "+temp3+" PLUS "+tempname+" PLUS 4 TIMES 4 "+temp2);
          print("RETURN "+temp3);
          print("END "+l1);
          print("HSTORE "+temp1+" 0 ");
          n.f4.accept(this, argu);
          n.f5.accept(this, argu);
          n.f6.accept(this, argu);
          print("JUMP "+l2);
          print(l1+" NOOP \n ERROR");
          print(l2+" NOOP");
          return _ret;
        }
        else
        {
          String temp1 = getTemp();
          String temp2 = getTemp();
          String temp3 = getTemp();
          String l1 = getLabel();
          String l2 = getLabel();
          String offset = getoffset(currentclass,tempvar);
          print("HLOAD "+temp1+" TEMP 0 "+offset);
          print("MOVE "+temp2+" ");
          n.f1.accept(this, argu);
          n.f2.accept(this, argu);
          n.f3.accept(this, argu);
          print("CJUMP TIMES LE MINUS 0 1 "+temp2+" LE "+temp2+"\nBEGIN\nHLOAD "+temp3+" "+temp1+" 0\nRETURN "+temp3);
          print("END");
          print(l1+ " HSTORE PLUS "+temp1+" PLUS 4 TIMES 4 "+temp2+" 0");
          n.f4.accept(this, argu);
          n.f5.accept(this, argu);
          n.f6.accept(this, argu);
          print("JUMP "+l2);
          print(l1+" NOOP \n ERROR");
          print(l2+" NOOP");
          return _ret;

        }
      }
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      n.f5.accept(this, argu);
      n.f6.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> IfthenElseStatement()
    *       | IfthenStatement()
    */
   public R visit(IfStatement n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> "if"
    * f1 -> "("
    * f2 -> Expression()
    * f3 -> ")"
    * f4 -> Statement()
    */
   public R visit(IfthenStatement n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      print("CJUMP ");
      String l1 = getLabel();
      n.f2.accept(this, argu);
      print(l1);
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      print(l1+" NOOP");
      return _ret;
   }

   /**
    * f0 -> "if"
    * f1 -> "("
    * f2 -> Expression()
    * f3 -> ")"
    * f4 -> Statement()
    * f5 -> "else"
    * f6 -> Statement()
    */
   public R visit(IfthenElseStatement n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      print("CJUMP ");
      String l1 = getLabel();
      n.f2.accept(this, argu);
      print(l1);
      String l2 = getLabel();
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      print("JUMP "+l2+"\n"+l1+" NOOP");
      n.f5.accept(this, argu);
      n.f6.accept(this, argu);
      print(l2+" NOOP");
      return _ret;
   }

   /**
    * f0 -> "while"
    * f1 -> "("
    * f2 -> Expression()
    * f3 -> ")"
    * f4 -> Statement()
    */
   public R visit(WhileStatement n, A argu) {
      R _ret=null;
      String l1 = getLabel();
      String l2 = getLabel();
      print(l1+" NOOP\nCJUMP ");
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      print(l2);
      //String l2 = getLabel();
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      print("JUMP "+l1);
      print(l2+" NOOP");
      return _ret;
   }

   /**
    * f0 -> "System.out.println"
    * f1 -> "("
    * f2 -> Expression()
    * f3 -> ")"
    * f4 -> ";"
    */
   public R visit(PrintStatement n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      if(parsecount>0)
      {
        System.out.println("PRINT ");
      }
      n.f2.accept(this, argu);
      n.f3.accept(this, argu);
      n.f4.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> OrExpression()
    *       | AndExpression()
    *       | CompareExpression()
    *       | neqExpression()
    *       | PlusExpression()
    *       | MinusExpression()
    *       | TimesExpression()
    *       | DivExpression()
    *       | ArrayLookup()
    *       | ArrayLength()
    *       | MessageSend()
    *       | PrimaryExpression()
    */
   public R visit(Expression n, A argu) {
      R _ret=null;
      _ret = (R)(String)n.f0.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "&&"
    * f2 -> PrimaryExpression()
    */
   public R visit(AndExpression n, A argu) {
      R _ret=null;
      String retval = getTemp();
      print("BEGIN");
      String l1 = getLabel();
      String l2 = getLabel();
      print("CJUMP");
      n.f0.accept(this, argu);
      print(l1);
      n.f1.accept(this, argu);
      print("CJUMP");
      n.f2.accept(this, argu);
      print(l1);
      print("MOVE "+retval+" 1");
      print("JUMP "+l2);
      print(l1+" NOOP");
      print("MOVE "+retval+" 0");
      print(l2+" NOOP");
      print("RETURN "+retval);
      print("END");
      _ret = (R)("boolean");
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "||"
    * f2 -> PrimaryExpression()
    */
   public R visit(OrExpression n, A argu) {
      R _ret=null;
      String retval = getTemp();
      print("BEGIN");
      String l1 = getLabel();
      String l2 = getLabel();
      print("CJUMP");
      print("MINUS 1");
      n.f0.accept(this, argu);
      print(l1);
      n.f1.accept(this, argu);
      print("CJUMP");
      print("MINUS 1");
      n.f2.accept(this, argu);
      print(l1);
      print("MOVE "+retval+" 0");
      print("JUMP "+l2);
      print(l1+" NOOP");
      print("MOVE "+retval+" 1");
      print(l2+" NOOP");
      print("RETURN "+retval);
      print("END");
      _ret = (R)("boolean");
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "<="
    * f2 -> PrimaryExpression()
    */
   public R visit(CompareExpression n, A argu) {
      R _ret=null;
      print("LE");
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      _ret = (R)("boolean");
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "!="
    * f2 -> PrimaryExpression()
    */
   public R visit(neqExpression n, A argu) {
      R _ret=null;
      print("NE");
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      _ret = (R)("boolean");
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "+"
    * f2 -> PrimaryExpression()
    */
   public R visit(PlusExpression n, A argu) {
      R _ret=null;
      print("PLUS");
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      _ret = (R)("int");
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "-"
    * f2 -> PrimaryExpression()
    */
   public R visit(MinusExpression n, A argu) {
      R _ret=null;
      print("MINUS");
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      _ret = (R)("int");
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "*"
    * f2 -> PrimaryExpression()
    */
   public R visit(TimesExpression n, A argu) {
      R _ret=null;
      print("TIMES");
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      _ret = (R)("int");
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "/"
    * f2 -> PrimaryExpression()
    */
   public R visit(DivExpression n, A argu) {
      R _ret=null;
      print("DIV");
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      _ret = (R)("int");
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "["
    * f2 -> PrimaryExpression()
    * f3 -> "]"
    */
   public R visit(ArrayLookup n, A argu) {
      R _ret=null;
      String temp1 = getTemp();
      String temp2 = getTemp();
      String temp3 = getTemp();
      String temp4 = getTemp();
      String l1 = getLabel();
      String l2 = getLabel();
      print("BEGIN");
      print("MOVE "+temp1+" ");
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      print("MOVE "+temp2+" ");
      n.f2.accept(this, argu);
      print("CJUMP TIMES LE MINUS 0 1 "+temp2+" LE "+temp2+" BEGIN HLOAD "+temp4+" "+temp1+" 0\nRETURN "+temp4+" END "+l1);
      print("HLOAD "+temp3+" PLUS "+temp1+" PLUS TIMES "+temp2+" 4 4 0");
      print("JUMP "+l2);
      print(l1+" NOOP");
      print("ERROR \n"+l2+" NOOP");
      print("RETURN "+temp3+" END"); 
      n.f3.accept(this, argu);
      _ret = (R)("int");
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "."
    * f2 -> "length"
    */
   public R visit(ArrayLength n, A argu) {
      R _ret=null;
      String temp1 = getTemp();
      print("BEGIN \n MOVE "+temp1);
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      String temp2 = getTemp();
      print("HLOAD "+temp2+" "+temp1+" 0");
      print("RETURN "+temp2);
      print("END");
      _ret = (R)("int");
      return _ret;
   }

   /**
    * f0 -> PrimaryExpression()
    * f1 -> "."
    * f2 -> Identifier()
    * f3 -> "("
    * f4 -> ( ExpressionList() )?
    * f5 -> ")"
    */
   public R visit(MessageSend n, A argu) {
      R _ret=null;
      ArrayList<String> params = new ArrayList<String>();
      String x1=null;
      if(parsecount>0)
      {
        System.out.println("CALL");
        System.out.println("BEGIN");
        x1 = getTemp();
        System.out.println("MOVE "+ x1);
      }
      String cname = (String)n.f0.accept(this, argu);
      //print(currentfunc+currentclass+cname);
      if(parsecount>0)
      cname = checkifobjectorclass(cname,currentclass);
      n.f1.accept(this, argu);
      String fname = (String)n.f2.accept(this, argu);
      n.f3.accept(this, argu);
      if(parsecount>0)
      {
        //print(currentfunc+currentclass+cname+fname+classnames.get(cname));
        String offset = (classnameoffset.get(cname)).get(fname);
        String x2 = getTemp();
        System.out.println("HLOAD "+x2 + " "+x1+" 0"); 
        String x3 = getTemp();
        System.out.println("HLOAD "+x3 + " "+x2+" "+offset);
        System.out.println("RETURN "+x3);
        System.out.println("END");
        System.out.println("( "+ x1+ " ");
      }
      n.f4.accept(this, argu);
      n.f5.accept(this, argu);
      if(parsecount>0)
      {
        System.out.println(")");
        String rettype = findrettype(cname,fname);
        //System.out.println("hey"+rettype);
        return (R)rettype;
      }
       _ret = (R)("int");
      return _ret;
   }

   /**
    * f0 -> Expression()
    * f1 -> ( ExpressionRest() )*
    */
   public R visit(ExpressionList n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> ","
    * f1 -> Expression()
    */
   public R visit(ExpressionRest n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> IntegerLiteral()
    *       | TrueLiteral()
    *       | FalseLiteral()
    *       | Identifier()
    *       | ThisExpression()
    *       | ArrayAllocationExpression()
    *       | AllocationExpression()
    *       | NotExpression()
    *       | BracketExpression()
    */
   public R visit(PrimaryExpression n, A argu) {
      R _ret=null;
      _ret = (R)(String)n.f0.accept(this, argu);
      if(n.f0.which==3)
      {
          String varname = (String)_ret;
          //print(currentclass+currentfunc+varname);
          String tempname = funcvar.get(varname);
          if(tempname!=null)
          print(tempname);
          else
          {
            String offset = getoffset(currentclass,varname);
            String newvar = getTemp();
            print("BEGIN \n HLOAD "+ newvar +" TEMP 0 "+offset+" \n RETURN "+newvar+" END  ");
          }
      }
      return _ret;
   }

   /**
    * f0 -> <INTEGER_LITERAL>
    */
   public R visit(IntegerLiteral n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      print(n.f0.toString());
      _ret = (R)("int");
      return _ret;
   }

   /**
    * f0 -> "true"
    */
   public R visit(TrueLiteral n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      print("1");
      _ret = (R)("boolean");
      return _ret;
   }

   /**
    * f0 -> "false"
    */
   public R visit(FalseLiteral n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      print("0");
      _ret = (R)("boolean");
      return _ret;
   }

   /**
    * f0 -> <IDENTIFIER>
    */
   public R visit(Identifier n, A argu) {
      R _ret=null;
      _ret=(R)(String)(n.f0.toString());
      return _ret;
   }

   /**
    * f0 -> "this"
    */
   public R visit(ThisExpression n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      print("TEMP 0");
      _ret = (R)(currentclass);
      return _ret;
   }

   /**
    * f0 -> "new"
    * f1 -> "int"
    * f2 -> "["
    * f3 -> Expression()
    * f4 -> "]"
    */
   public R visit(ArrayAllocationExpression n, A argu) {
      R _ret=null;
      String temp1 = getTemp();
      String temp2 = getTemp();
      String temp3 = getTemp();
      String l1 = getLabel();
      String l2 = getLabel();
      print("BEGIN");
      print("MOVE "+temp1);
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      n.f3.accept(this, argu);
      print("MOVE "+temp2+ " HALLOCATE PLUS TIMES "+temp1+" 4 4");
      print("HSTORE "+temp2+" 0 "+temp1);
      print("MOVE "+temp3+" 4");
      print(l1+" CJUMP LE "+temp3+" PLUS TIMES "+ temp1 + " 4 4 " + l2);
      print("HSTORE PLUS "+temp2+" "+temp3+" 0 0");
      print("MOVE "+temp3+" PLUS "+temp3+" 4");
      print("JUMP "+l1);
      print(l2 + " NOOP");
      print("RETURN "+temp2);
      print("END");
      n.f4.accept(this, argu);
      _ret = (R)("intarray");
      return _ret;
   }

   /**
    * f0 -> "new"
    * f1 -> Identifier()
    * f2 -> "("
    * f3 -> ")"
    */
   public R visit(AllocationExpression n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      String newtempvar = (String)n.f1.accept(this, argu);
      n.f2.accept(this, argu);
      n.f3.accept(this, argu);
      if(parsecount>0)
      {
        String x1 = getTemp();
        //tempvars.put(n,x1);
        System.out.println("BEGIN");
        String x2 = getTemp();
        int sizetemp = (classnamevars.get(newtempvar)).size();
        //print("sixe ="+ sizetemp);
        String x3 = getTemp();
        int sizetemp2 = HVtable.get(newtempvar);
        System.out.println("MOVE " + x2 + " HALLOCATE "+Integer.toString((sizetemp-sizetemp2)*4));
        System.out.println("MOVE " + x3 + " HALLOCATE "+Integer.toString(sizetemp2*4+4));
        for(int i=sizetemp2;i<sizetemp;i++)
        {
          String x4 = getTemp();
          String fname = classnamevars.get(newtempvar).get(i);
          fname = newtempvar+"_"+fname;
          System.out.println("HSTORE " + x2 + " " + Integer.toString((i-sizetemp2)*4)+" " + fname);
        }
        System.out.println("HSTORE " + x3 + " 0 " + x2);
        System.out.println("RETURN " + x3);
        System.out.println("END");
      }
      _ret = (R)(newtempvar);
      return _ret;
   }

   /**
    * f0 -> "!"
    * f1 -> Expression()
    */
   public R visit(NotExpression n, A argu) {
      R _ret=null;
      print("MINUS 1");
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      _ret = (R)("boolean");
      return _ret;
   }

   /**
    * f0 -> "("
    * f1 -> Expression()
    * f2 -> ")"
    */
   public R visit(BracketExpression n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      _ret = (R)(String)(n.f1.accept(this, argu));
      n.f2.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> Identifier()
    * f1 -> ( IdentifierRest() )*
    */
   public R visit(IdentifierList n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      return _ret;
   }

   /**
    * f0 -> ","
    * f1 -> Identifier()
    */
   public R visit(IdentifierRest n, A argu) {
      R _ret=null;
      n.f0.accept(this, argu);
      n.f1.accept(this, argu);
      return _ret;
   }

}
