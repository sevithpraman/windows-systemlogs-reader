import java.io.IOException;
import java.io.PrintWriter;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.RequestDispatcher;

public class SystemLogReaderServlet extends HttpServlet {
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws  IOException {
        try {

            // Set the content type to plain text
            // response.setContentType("text/plain");
            String logName = request.getParameter("logName");
            logName = "System";
            // // Create a new PrintWriter object
            PrintWriter out = response.getWriter();

            out.println("Hello World!");

            // // Create a new SystemLogReader object
            // SystemLogReader reader = new SystemLogReader();

            // // Call the readSystemLog() method with the PrintWriter object as a parameter
            // reader.readSystemLog(out);

            // // Close the PrintWriter object
            // out.close();
            // RequestDispatcher dispatcher =
            // request.getRequestDispatcher("SystemLogReader.jsp");
            // dispatcher.forward(request, response);
        } catch (IOException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }
}
