import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;

import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

//javac -h . CreateUpdateLogsServlet.java
//gcc -I"C:\Program Files\Java\jdk1.8.0_333\include" -I"C:\Program Files\Java\jdk1.8.0_333\include\win32" -shared -o SystemLogReader.dll CreateUpdateLogsServlet.c

public class CreateUpdateLogsServlet extends HttpServlet {
    static {
        System.loadLibrary("SystemLogReader");
    }

    public native SystemLog[] readSystemLog(String logName);

    // jdbc connectivity variables
    public static String jdbc_class = "com.mysql.cj.jdbc.Driver";
    public static String url = "jdbc:mysql://localhost:3306/systemlogs";
    public static String user = "root";
    public static String password = "";
    // public static int count;

    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        try {
            // Set the content type to plain text
            // response.setContentType("text/plain");
            String logName = request.getParameter("logName");
            // Create a new PrintWriter object
            PrintWriter out = response.getWriter();
            out.println("<h1>Processing...</h1>");
            System.out.println(logName);
            // Create a new CreateUpdateLogsServlet object
            CreateUpdateLogsServlet reader = new CreateUpdateLogsServlet();
            SystemLog logs[] = reader.readSystemLog(logName);

            // Convert the array of SystemLog objects to a ArrayList of SystemLog objects
            ArrayList<SystemLog> logList = new ArrayList<SystemLog>(Arrays.asList(logs));

            // print the logs size
            System.out.println("Log size : " + logList.size());

            // print the logs

            System.out.println(logList);

            for (SystemLog log : logList) {
                System.out.println(log.getSource() + " " + log.getDescription());
            }

            // jdbc connection
            Class.forName(jdbc_class);
            Connection con = DriverManager.getConnection(url, user, password);
            // create table logs if not exists
            String sql = "CREATE TABLE IF NOT EXISTS logs (source VARCHAR(255), description VARCHAR(255))";
            PreparedStatement ps = con.prepareStatement(sql);
            ps.executeUpdate();

            // check if the log already exists by checking the source and description, if
            // exists, skip it
            if (logList != null)
                for (SystemLog log : logList) {
                    sql = "SELECT * FROM logs WHERE source = ? AND description = ?";
                    ps = con.prepareStatement(sql);
                    ps.setString(1, log.getSource());
                    ps.setString(2, log.getDescription());
                    ResultSet rs = ps.executeQuery();
                    if (rs.next()) {
                        continue;
                    }

                    // insert the log into the database
                    sql = "INSERT INTO logs (source, description) VALUES (?, ?)";
                    ps = con.prepareStatement(sql);
                    ps.setString(1, log.getSource());
                    ps.setString(2, log.getDescription());
                    ps.executeUpdate();
                    // count++;
                }

            // Close the PrintWriter object
            out.close();

            // Close the connection
            con.close();

            out.println("Successfully Retrieved. Redirecting to home page in 2 seconds...");
            // redirect to home page
            response.setHeader("Refresh", "2; URL=index.jsp");

        } catch (IOException | ClassNotFoundException | SQLException e) {
            System.out.println("Error: " + e.getMessage());

        }
    }
}