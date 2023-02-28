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

            // Set the content type to plain text
            // response.setContentType("text/plain");
            // Create a new PrintWriter object
            PrintWriter out = response.getWriter();
            out.println("<html>");
            out.println("<head>");
            out.println("<title>System Log Reader</title>");
            out.println(
                    "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css'>");
            out.println("<link rel='stylesheet' href='/style.css'>");
            out.println("<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js'></script>");
            out.println(
                    "<script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.16.0/umd/popper.min.js'></script>");
            out.println("<script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js'></script>");
            out.println("</head>");
            out.println("<body>");
            // out.println("<header>");
            // out.println("System Log Reader");
            // out.println("</header>");
            out.println("<h2>System Logs</h2>");
            // pagination
            int page = 1;
            int recordsPerPage = 10;
            if (request.getParameter("page") != null)
                page = Integer.parseInt(request.getParameter("page"));

            // jdbc connection
            Class.forName(jdbc_class);
            Connection con = DriverManager.getConnection(url, user, password);

            // retrive data by limit
            String sql = "SELECT * FROM logs limit 10 offset "
                    + (page - 1) * recordsPerPage;
            PreparedStatement ps = con.prepareStatement(sql);
            ResultSet rs = ps.executeQuery();

            // Systemlog object
            SystemLog log = new SystemLog();
            // ArrayList of SystemLog objects
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
               
                // for (SystemLog log1 : logList) {
                out.println("<tr>");
                out.println("<td>" + rs.getString("source") + "</td>");
                out.println("<td>" + rs.getString("description") + "</td>");
                out.println("</tr>");
                // }
                // get details from db
                String source = rs.getString("source");
                String description = rs.getString("description");

                // set details to SystemLog object
                log.setSource(source);
                log.setDescription(description);

                // add SystemLog object to ArrayList
                logList.add(log);
            }
            // for(SystemLog log1 : logList) {
            // System.out.print(log1.getSource()+" ");
            // System.out.println(log1.getDescription());
            // }

            // do paginaiton for the logs
            int noOfRecords = logList.size();
            int noOfPages = (int) Math.ceil(noOfRecords * 1.0 / recordsPerPage);
            int start = (page - 1) * recordsPerPage;
            int end = start + recordsPerPage;
            if (end > noOfRecords) {
                end = noOfRecords;
            }
            // out.println("<main>");
            // out.println("<table class='table table-bordered'>");
            // out.println("<thead>");
            // out.println("<tr>");
            // out.println("<th scope='col'>Source</th>");
            // out.println("<th scope='col'>Description</th>");
            // out.println("</tr>");
            // out.println("</thead>");
            // out.println("<tbody>");
            // for (SystemLog log1 : logList) {
            // out.println("<tr>");
            // out.println("<td>" + log1.getSource() + "</td>");
            // out.println("<td>" + log1.getDescription() + "</td>");
            // out.println("</tr>");
            // }
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
            // out.println("<footer>");
            // out.println("<p>By Shravana Tirtha</p>");
            // out.println("</footer>");
            out.println("</body>");
            out.println("</html>");

            // Close the PrintWriter object
            out.close();

        } catch (IOException | SQLException | ClassNotFoundException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }
}
