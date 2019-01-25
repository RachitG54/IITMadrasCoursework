import syntaxtree.*;
import visitor.*;

public class P3 {
   public static void main(String [] args) {
      try {
         Node root = new MiniJavaParser(System.in).Goal();
         //System.out.println("Program parsed successfully");
         GJDepthFirst<String,String> parseobject = new GJDepthFirst<String,String>();
         root.accept(parseobject,null); // Your assignment part is invoked here.
         root.accept(parseobject,null);
      }
      catch (ParseException e) {
         System.out.println(e.toString());
      }
   }
} 



