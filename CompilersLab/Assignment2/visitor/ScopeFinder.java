package visitor;

import java.util.*;

public class ScopeFinder{
	HashMap<String,String> H1 =new HashMap<String,String>();
	public String getvar(String key){
	return H1.get(key);
	}
	public void putvar(String varname, String funcname){
	//if(H1.containskey(varname)==false)
	H1.put(varname,funcname);
	
	}
}

