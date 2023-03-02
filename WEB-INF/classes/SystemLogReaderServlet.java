import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

public class SystemLogReaderServlet extends HttpServlet {
    public static String jdbc_class = "com.mysql.cj.jdbc.Driver";
    public static String url = "jdbc:mysql://localhost:3306/systemlogs";
    public static String user = "root";
    public static String password = "";

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        try {
            PrintWriter out = response.getWriter();
            out.println("<html>");
            out.println("<head>");
            out.println("<title>System Log Reader</title>");
            out.println("<link rel='stylesheet' href='../style.css'>");
            out.println(
                    "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css'>");
            out.println(
                    "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.6.1/jquery.min.js'></script>");
            out.println("</head>");

            out.println("</head>");
            out.println("<body>");
            out.println("<h2>System Logs</h2>");
            int page = 1;
            int recordsPerPage = 10;
            if (request.getParameter("page") != null)
                page = Integer.parseInt(request.getParameter("page"));
            Class.forName(jdbc_class);
            Connection con = DriverManager.getConnection(url, user, password);
            String sql = "SELECT * FROM logs limit 10 offset "
                    + (page - 1) * recordsPerPage;
            PreparedStatement ps = con.prepareStatement(sql);
            ResultSet rs = ps.executeQuery();
            out.println("<main>");
            out.println("<table class='table table-bordered'>");
            out.println("<thead>");
            out.println("<tr>");
            out.println("<th scope='col'>Event ID</th>");
            out.println("<th scope='col'>Event Type</th>");
            out.println("<th scope='col'>Source</th>");
            out.println("<th scope='col'>Category</th>");
            out.println("<th scope='col'>Time Generated</th>");
            out.println("<th scope='col'>Description</th>");
            out.println("</tr>");
            out.println("</thead>");
            out.println("<tbody>");
            while (rs.next()) {
                out.println("<tr>");
                out.println("<td>" + rs.getInt("eventID") + "</td>");
                out.println("<td>" + rs.getInt("eventType") + "</td>");
                out.println("<td>" + rs.getString("source") + "</td>");
                out.println("<td>" + rs.getInt("category") + "</td>");
                int time = rs.getInt("timeGenerated");
                int seconds = time % 60;
                int minutes = (time / 60) % 60;
                int hours = (time / 3600) % 12;
                String am_pm = "AM";
                if (hours > 12) {
                    hours -= 12;
                    am_pm = "PM";
                }
                hours = hours % 12;
                String timeGenerated1 = String.format("%02d:%02d:%02d", hours, minutes, seconds);
                out.println("<td>" + timeGenerated1 + " " + am_pm + "</td>");
                out.println("<td>" + rs.getString("description") + "</td>");
                out.println("</tr>");
            }
            String sql1 = "SELECT COUNT(*) FROM logs";
            PreparedStatement ps1 = con.prepareStatement(sql1);
            ResultSet rs1 = ps1.executeQuery();
            rs1.next();
            int noOfRecords = rs1.getInt(1);
            System.out.println(noOfRecords);
            int noOfPages = (int) Math.floor(noOfRecords * 1.0 / recordsPerPage);
            System.out.println(noOfPages);
            int start = (page - 1) * recordsPerPage;
            int end = start + recordsPerPage;
            if (end > noOfRecords) {
                end = noOfRecords;
            }
            out.println("</tbody>");
            out.println("</table>");
            out.println("<nav aria-label='Page navigation example'>");
            out.println("<ul class='pagination justify-content-center'>");
            for (int i = 1; i <= noOfPages + 1; i++) {
                out.println(
                        "<li class='page-item'><a class='page-link' href='logs?page=" + i + "'>" + i
                                + "</a></li>");
            }
            out.println("</ul>");
            out.println("</nav>");
            out.println("</main>");
            out.println("</body>");
            out.println("</html>");
            out.close();
        } catch (IOException | SQLException | ClassNotFoundException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }
}
