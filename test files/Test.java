import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.Arrays;

public class Test {
    static {
        System.loadLibrary("Test");
    }

    public native SystemLog[] readSystemLog(String logName);

    public static String jdbc_class = "com.mysql.cj.jdbc.Driver";
    public static String url = "jdbc:mysql://localhost:3306/systemlogs";
    public static String user = "root";
    public static String password = "";

    public static void main(String[] args) {
        Test reader = new Test();
        SystemLog logs[] = reader.readSystemLog("System");
        System.out.println(logs.length);

        // print the logs
        // for (SystemLog log : logs) {
        // System.out.println(log);
        // }
        // Convert the array of SystemLog objects to a ArrayList of SystemLog objects
        ArrayList<SystemLog> logList = new ArrayList<SystemLog>(Arrays.asList(logs));
        // jdbc connection
        try {
            Class.forName(jdbc_class);
            Connection con = DriverManager.getConnection(url, user, password);
            // create table logs if not exists
            String sql = "CREATE TABLE IF NOT EXISTS logs (eventId INT, eventType INT, source VARCHAR(255), category INT, timeGenerated INT, description VARCHAR(255))";
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
        } catch (Exception e) {
            System.out.println(e);
        }

    }
}
