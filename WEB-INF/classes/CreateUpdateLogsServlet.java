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

    public static String jdbc_class = "com.mysql.cj.jdbc.Driver";
    public static String url = "jdbc:mysql://localhost:3306/systemlogs";
    public static String user = "root";
    public static String password = "";

    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        try {
            String logName = request.getParameter("logName");
            PrintWriter out = response.getWriter();
            out.println("<html>");
            out.println("<head>");
            out.println("<title>System Log Reader</title>");
            out.println("<body>");
            out.println("<h1>System Log Reader</h1>");
            System.out.println(logName);
            CreateUpdateLogsServlet reader = new CreateUpdateLogsServlet();
            SystemLog logs[] = reader.readSystemLog(logName);
            ArrayList<SystemLog> logList = new ArrayList<SystemLog>(Arrays.asList(logs));
            System.out.println("Log size : " + logList.size());
            Class.forName(jdbc_class);
            Connection con = DriverManager.getConnection(url, user, password);
            String sql = "CREATE TABLE IF NOT EXISTS logs (eventId INT, eventType INT, source VARCHAR(255), category INT, timeGenerated INT, description VARCHAR(255))";
            PreparedStatement ps = con.prepareStatement(sql);
            ps.executeUpdate();
            Thread t1 = new Thread() {
                public void run() {
                    System.out.println("Thread 1 started");
                    try {
                        for (int i = 0; i < logList.size() / 2; i++) {
                            SystemLog log = logList.get(i);
                            String sql = "SELECT * FROM logs WHERE source = ? AND description = ?";
                            PreparedStatement ps = con.prepareStatement(sql);
                            ps.setString(1, log.getSource());
                            ps.setString(2, log.getDescription());
                            ResultSet rs = ps.executeQuery();
                            if (rs.next()) {
                                continue;
                            }
                            sql = "INSERT INTO logs (eventId, eventType, source, category, timeGenerated, description) VALUES (?,?,?,?,?, ?)";
                            ps = con.prepareStatement(sql);
                            ps.setInt(1, log.getEventID());
                            ps.setInt(2, log.getEventType());
                            ps.setString(3, log.getSource());
                            ps.setInt(4, log.getCategory());
                            ps.setInt(5, log.gettimeGenerated());
                            ps.setString(6, log.getDescription());
                            ps.executeUpdate();
                        }
                    } catch (SQLException e) {
                        System.out.println("Error: " + e.getMessage());
                    }
                    System.out.println("Thread 1 ended");
                }
            };
            Thread t2 = new Thread() {
                public void run() {
                    System.out.println("Thread 2 started");
                    try {
                        for (int i = logList.size() / 2; i < logList.size(); i++) {
                            SystemLog log = logList.get(i);
                            String sql = "SELECT * FROM logs WHERE source = ? AND description = ?";
                            PreparedStatement ps = con.prepareStatement(sql);
                            ps.setString(1, log.getSource());
                            ps.setString(2, log.getDescription());
                            ResultSet rs = ps.executeQuery();
                            if (rs.next()) {
                                continue;
                            }
                            sql = "INSERT INTO logs (eventId, eventType, source, category, timeGenerated, description) VALUES (?,?,?,?,?, ?)";
                            ps = con.prepareStatement(sql);
                            ps.setInt(1, log.getEventID());
                            ps.setInt(2, log.getEventType());
                            ps.setString(3, log.getSource());
                            ps.setInt(4, log.getCategory());
                            ps.setInt(5, log.gettimeGenerated());
                            ps.setString(6, log.getDescription());
                            ps.executeUpdate();
                        }
                    } catch (SQLException e) {
                        System.out.println("Error: " + e.getMessage());
                    }
                    System.out.println("Thread 2 ended");
                }
            };
            t1.start();
            t2.start();
            t1.join();
            t2.join();
            out.println(
                    "<h2>Reading the system log is complete. <a href='index.jsp'>Click here</a> to go back to the home page.</h2>");
            out.println("</body>");
            out.println("</html>");
            con.close();
            out.close();
        } catch (IOException | ClassNotFoundException | SQLException | InterruptedException e) {
            System.out.println("Error: " + e.getMessage());

        }
    }
}