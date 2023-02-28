import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

public class SystemLogReaderServlet extends HttpServlet {
    // jdbc connectivity variables
    public static String jdbc_class = "com.mysql.cj.jdbc.Driver";
    public static String url = "jdbc:mysql://localhost:3306/systemlogs";
    public static String user = "root";
    public static String password = "";
    public static int count;

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException {
        try {
            PrintWriter out = response.getWriter();
            out.println("<html>");
            out.println("<head>");
            out.println("<title>System Log Reader</title>");
            out.println("<link rel='stylesheet' href='/style.css'>");
            out.println("<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js'></script>");
            out.println(
                    "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.16.0/umd/popper.min.js'></script>");
            out.println("<script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js'></script>");
            out.println("</head>");
            out.println("<body>");
            out.println("<h2>System Logs</h2>");
            // pagination
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
            SystemLog log = new SystemLog();
            ArrayList<SystemLog> logList = new ArrayList<SystemLog>();
            out.println("<main>");
            out.println("<table class='table table-bordered'>");
            out.println("<thead>");
            out.println("<tr>");
            out.println("<th scope='col'>Source</th>");
            out.println("<th scope='col'>Description</th>");
            out.println("</tr>");
            out.println("</thead>");
            out.println("<tbody>");
            while (rs.next()) {
                out.println("<tr>");
                out.println("<td>" + rs.getString("source") + "</td>");
                out.println("<td>" + rs.getString("description") + "</td>");
                out.println("</tr>");
                String source = rs.getString("source");
                String description = rs.getString("description");
                log.setSource(source);
                log.setDescription(description);
                logList.add(log);
            }
            int noOfRecords = logList.size();
            int noOfPages = (int) Math.ceil(noOfRecords * 1.0 / recordsPerPage);
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
