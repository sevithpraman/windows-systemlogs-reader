import java.io.PrintWriter;

public class SystemLogReader {

    static {
        System.loadLibrary("SystemLogReader");
    }

    public native void readSystemLog(PrintWriter out);
}

//javac -h . SystemLogReader.java
//gcc -I"C:\Program Files\Java\jdk1.8.0_333\include" -I"C:\Program Files\Java\jdk1.8.0_333\include\win32" -shared -o SystemLogReader.dll SystemLogReader.c
//java SystemLogReader