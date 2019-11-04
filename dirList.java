//The program shown here illustrates how to use list( ) to examine the contents of a directory:

// Using directories.
import java.io.*;
import java.io.File;
public class dirList {
    final static String in="input";//"Training";//"input"; //change this to input if you want to list folders named input
	public static void main(String args[]) {
		String dirname = args[0];
               
		File f1 = new File(dirname);
                recList(f1);

	}
	static void recList(File f){

		//System.out.println("Directory of " + f.getAbsolutePath());
			String s[] = f.list();
			for (int i=0; i < s.length; i++) {
				File f1 = new File(f.getAbsolutePath(),s[i]);
				//System.out.println(s[i]);
				if (f1.isDirectory()) {
                                        if(dirList.in.equals(f1.getName()))
                                         {
                                              String snew=f1.getAbsolutePath();
                                              System.out.println(snew);
	                                      //snew=snew.replace("dataset\\","model\\");
					      snew=snew.replace("input","model");
					      //System.out.println(snew);
                                              new File(snew).mkdirs();
					      
                                         }
					recList(f1);
				}
				
			}

	}
}
