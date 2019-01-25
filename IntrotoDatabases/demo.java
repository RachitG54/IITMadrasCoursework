//STEP 1. Import required packages
import java.sql.*;
import java.util.Scanner;
import java.util.ArrayList;

public class demo {
   // JDBC driver name and database URL
   static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
   static final String DB_URL = "jdbc:mysql://localhost/PADBMS";

   //  Database credentials
   static final String USER = "rachit";
   static final String PASS = "5x*M";
   
   static void giveuserchoice(Connection conn) {
      System.out.println("\t\t\t\t\tWelcome to the Console Level Interface of CS14B050!!!!!\n");
      System.out.println("Choose the option that you want to perform:");
      System.out.println("1.FIND TABLES");
      System.out.println("2.DESCRIBE");
      System.out.println("3.ADD");
      System.out.println("4.DELETE");
      System.out.println("5.UPDATE or MODIFY");
      System.out.println("6.DISPLAY");
      System.out.println("7.DELETE TABLE");
      System.out.println("8.EXIT\n");
      System.out.println("Enter Your Choice:");
      Scanner reader = new Scanner(System.in);
      int choice = reader.nextInt();
      switch(choice) {
        case 1: 
          iftables(conn);
          break;
        case 2:
          describe(conn);
          break;
        case 3:
          add(conn);
          break;
        case 4:
          delete(conn);
          break;
        case 5:
          update(conn);
          break;
        case 6:
          query(conn);
        case 7:
          deletetable(conn);
        case 8:
        default:
          System.exit(1);
      }
      //System.exit(1);
   }
   static void iftables(Connection conn) {
    while(true){ 
      System.out.println("Do you want to know which tables are present in database?(Enter y/n)");
      Scanner reader = new Scanner(System.in);
      char c = reader.next().charAt(0);
      if (c =='n') break;
      executequery(conn,"SHOW TABLES"); 
      System.out.println("Do you want to return back to main menu?(Enter y/n)");
      c = reader.next().charAt(0);
      if(c=='y')break;
    }
    giveuserchoice(conn);    
   }
   static void describe(Connection conn) {
    while(true){ 
      System.out.println("Do you want to know description of a table which is present in database?(Enter y/n)");
      Scanner reader = new Scanner(System.in);
      char c = reader.next().charAt(0);
      if (c =='n') break;
      System.out.println("Enter valid table name:");
      Scanner reader1 = new Scanner(System.in);
      String s = reader1.nextLine();
      executequery(conn,"DESCRIBE "+s); 
      System.out.println("Do you want to return back to main menu?(Enter y/n)");
      c = reader.next().charAt(0);
      if(c=='y')break;
    }
    giveuserchoice(conn);    
   }
   static void query(Connection conn) {
    while(true){ 
      System.out.println("Do you want to query on a table?(Enter y/n)");
      Scanner reader = new Scanner(System.in);
      char c = reader.next().charAt(0);
      if (c =='n') break;
      System.out.println("Enter valid table name:");
      Scanner reader1 = new Scanner(System.in);
      String s1 = reader1.nextLine();

      System.out.println("Enter Column names that u want to view(Seperated by a comma)(Enter * for all columns):");
      String s2 = reader1.nextLine();

      ArrayList<String> wherecond = new ArrayList<String>();
      while(true)
      {
        System.out.println("Do you want to impose Conditions on selection:(Enter y/n)");  
        String s = reader1.nextLine();
        if(s.equals("n"))break;
        System.out.println("Enter Condition that u want to impose:");
        s = reader1.nextLine();
        wherecond.add(s);
      }
      int no = wherecond.size();
      String finalwhere = "\0";
      if(no>0)
      {
        finalwhere = wherecond.get(0);if(no!=1)finalwhere+= " AND ";
      }
      for(int i=1;i<no;i++)
      {
        if(i!=no-1)
        finalwhere = finalwhere + wherecond.get(i)+" AND ";
        else 
        finalwhere = finalwhere + wherecond.get(i);
      }


      if(no==0)
      executequery(conn,"SELECT "+s2+" FROM "+s1);
      else
      executequery(conn,"SELECT "+s2+" FROM "+s1 +" WHERE "+finalwhere);

      System.out.println("Do you want to return back to main menu?(Enter y/n)");
      c = reader.next().charAt(0);
      if(c=='y')break;
    }
    giveuserchoice(conn);    
   }

   static void add(Connection conn) {
    while(true){ 
      System.out.println("Do you want to add values to a table?(Enter y/n)");
      Scanner reader = new Scanner(System.in);
      char c = reader.next().charAt(0);
      if (c =='n') break;
      System.out.println("Enter valid table name:");
      Scanner reader1 = new Scanner(System.in);
      String s1 = reader1.nextLine();

      System.out.println("Enter Column names that u want to add on (Seperated by a comma)(Enter * for all columns):");
      String s = reader1.nextLine();
      System.out.println("Enter Values that you want to add(Seperated by a comma):");
      String s2 = reader1.nextLine();
      if(!s.equals("*"))
      executeupdate(conn,"INSERT INTO "+s1+"( "+s+" ) VALUES ( "+s2+" )");
      else
      executeupdate(conn,"INSERT INTO "+s1+" VALUES ( "+s2+" )");
        
      executequery(conn,"SELECT * FROM "+s1);
      System.out.println("Do you want to return back to main menu?(Enter y/n)");
      c = reader.next().charAt(0);
      if(c=='y')break;
    }
    giveuserchoice(conn);    
   }

   static void delete(Connection conn) {
    while(true){ 
      System.out.println("Do you want to delete values of a table?(Enter y/n)");
      Scanner reader = new Scanner(System.in);
      char c = reader.next().charAt(0);
      if (c =='n') break;
      System.out.println("Enter valid table name:");
      Scanner reader1 = new Scanner(System.in);
      String s1 = reader1.nextLine();
      ArrayList<String> wherecond = new ArrayList<String>();
      while(true)
      {
        System.out.println("Do you want to impose Conditions on delete:(Enter y/n)");  
        String s = reader1.nextLine();
        if(s.equals("n"))break;
        System.out.println("Enter Condition that u want to impose on delete:");
        s = reader1.nextLine();
        wherecond.add(s);
      }
      int no = wherecond.size();
      String finalwhere = "\0";
      if(no>0)
      {
        finalwhere = wherecond.get(0);if(no!=1)finalwhere+= " AND ";
      }
      for(int i=1;i<no;i++)
      {
        if(i!=no-1)
        finalwhere = finalwhere + wherecond.get(i)+" AND ";
        else 
        finalwhere = finalwhere + wherecond.get(i);
      }
      if(no==0)
        executeupdate(conn,"DELETE FROM "+s1);
      else 
        executeupdate(conn,"DELETE FROM "+s1+" WHERE " + finalwhere);

      executequery(conn,"SELECT * FROM "+s1);
      System.out.println("Do you want to return back to main menu?(Enter y/n)");
      c = reader.next().charAt(0);
      if(c=='y')break;
    }
    giveuserchoice(conn);    
   }

   static void update(Connection conn) {
    while(true){ 
      System.out.println("Do you want to update values to a table?(Enter y/n)");
      Scanner reader = new Scanner(System.in);
      char c = reader.next().charAt(0);
      if (c =='n') break;
      System.out.println("Enter valid table name:");
      Scanner reader1 = new Scanner(System.in);
      String s1 = reader1.nextLine();

      System.out.println("Enter Column names and their corresponding values that u want to update(Seperated by a comma):");
      String s2 = reader1.nextLine();

      ArrayList<String> wherecond = new ArrayList<String>();
      while(true)
      {
        System.out.println("Do you want to impose Conditions on update:(Enter y/n)");  
        String s = reader1.nextLine();
        if(s.equals("n"))break;
        System.out.println("Enter Condition that u want to impose on update:");
        s = reader1.nextLine();
        wherecond.add(s);
      }
      int no = wherecond.size();
      String finalwhere = "\0";
      if(no>0)
      {
        finalwhere = wherecond.get(0);if(no!=1)finalwhere+= " AND ";
      }
      for(int i=1;i<no;i++)
      {
        if(i!=no-1)
        finalwhere = finalwhere + wherecond.get(i)+" AND ";
        else 
        finalwhere = finalwhere + wherecond.get(i);
      }
      //System.out.println("UPDATE "+s1+" SET "+s2+" WHERE "+finalwhere);
      if(no!=0)
      executeupdate(conn,"UPDATE "+s1+" SET "+s2+" WHERE "+finalwhere);
      else
      executeupdate(conn,"UPDATE "+s1+" SET "+s2);
        
      executequery(conn,"SELECT * FROM "+s1);
      System.out.println("Do you want to return back to main menu?(Enter y/n)");
      c = reader.next().charAt(0);
      if(c=='y')break;
    }
    giveuserchoice(conn);    
   }

   static void deletetable(Connection conn) {
    while(true){ 
      System.out.println("Do you want to delete a table?(Enter y/n)");
      Scanner reader = new Scanner(System.in);
      char c = reader.next().charAt(0);
      if (c =='n') break;
      System.out.println("Enter valid table name:");
      Scanner reader1 = new Scanner(System.in);
      String s1 = reader1.nextLine();
      executeupdate(conn,"DROP TABLE "+s1);
      System.out.println("Do you want to return back to main menu?(Enter y/n)");
      c = reader.next().charAt(0);
      if(c=='y')break;
    }
    giveuserchoice(conn);    
   }


   static void executequery(Connection conn,String sql){
      Statement smt = null;
    try{

        smt = conn.createStatement();
        ResultSet rs = smt.executeQuery(sql);
        ResultSetMetaData rsmd = rs.getMetaData();
        int columnsNumber = rsmd.getColumnCount();
        //STEP 5: Extract data from result set
        for (int i = 1; i <= columnsNumber; i++) {
          if (i > 1) System.out.print(",  ");
          System.out.print(rsmd.getColumnName(i));
         }
         System.out.println("");
        while(rs.next()){
           //Retrieve by column name
           
           for (int i = 1; i <= columnsNumber; i++) {
          if (i > 1) System.out.print(",  ");
          String columnValue = rs.getString(i);
          System.out.print(columnValue);
          }
          System.out.println("");
        }
        //STEP 6: Clean-up environment
        rs.close();
        smt.close();
      }
      catch(SQLException se){
      //Handle errors for JDBC
      se.printStackTrace();
     }catch(Exception e){
        //Handle errors for Class.forName
        e.printStackTrace();
     }finally{
        //finally block used to close resources
        try{
           if(smt!=null)
              smt.close();
        }catch(SQLException se2){
        }// nothing we can do
        
     }//end try 
   }

   static void executeupdate(Connection conn,String sql){
      Statement smt = null;
    try{

        smt = conn.createStatement();
        smt.executeUpdate(sql);
        
        smt.close();
      }
      catch(SQLException se){
      //Handle errors for JDBC
      se.printStackTrace();
     }catch(Exception e){
        //Handle errors for Class.forName
        e.printStackTrace();
     }finally{
        //finally block used to close resources
        try{
           if(smt!=null)
              smt.close();
        }catch(SQLException se2){
        }// nothing we can do
        
     }//end try 
   }






   public static void main(String[] args) {
   Connection conn = null;
   Statement stmt = null;

   try{
      //STEP 2: Register JDBC driver
      Class.forName("com.mysql.jdbc.Driver");

      //STEP 3: Open a connection
      System.out.println("Connecting to database...");
      conn = DriverManager.getConnection(DB_URL,USER,PASS);

      giveuserchoice(conn);
      //STEP 4: Execute a query
      System.out.println("Creating statement...");
      stmt = conn.createStatement();
      String sql;
      sql = "SELECT * FROM Staff";
      ResultSet rs = stmt.executeQuery(sql);
      ResultSetMetaData rsmd = rs.getMetaData();
      int columnsNumber = rsmd.getColumnCount();
      //STEP 5: Extract data from result set
      for (int i = 1; i <= columnsNumber; i++) {
        if (i > 1) System.out.print(",  ");
        System.out.print(rsmd.getColumnName(i));
       }
       System.out.println("");
      while(rs.next()){
         //Retrieve by column name
         
         for (int i = 1; i <= columnsNumber; i++) {
        if (i > 1) System.out.print(",  ");
        String columnValue = rs.getString(i);
        System.out.print(columnValue);
        }
        System.out.println("");
      }
      //STEP 6: Clean-up environment
      rs.close();
      stmt.close();
      conn.close();
   }catch(SQLException se){
      //Handle errors for JDBC
      se.printStackTrace();
   }catch(Exception e){
      //Handle errors for Class.forName
      e.printStackTrace();
   }finally{
      //finally block used to close resources
      try{
         if(stmt!=null)
            stmt.close();
      }catch(SQLException se2){
      }// nothing we can do
      try{
         if(conn!=null)
            conn.close();
      }catch(SQLException se){
         se.printStackTrace();
      }//end finally try
   }//end try
   System.out.println("Goodbye!");
}//end main
}//end FirstExample