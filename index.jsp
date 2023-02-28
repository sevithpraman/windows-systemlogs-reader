<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>System Log Reader</title>
    <link rel="stylesheet" href="style.css" />
    <link
      rel="stylesheet"
      href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css"
      integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65"
      crossorigin="anonymous"
    />
    <script
      src="https://kit.fontawesome.com/ef565604ce.js"
      crossorigin="anonymous"
    ></script>
  </head>
  <body>
    <header>
      <h1>System Log Reader</h1>
    </header>
    <main>
      <form class="loadForm" action="logs" method="get">
        <a class="btn btn-lg btn-dark" href="CreateUpdateLogsServlet?logName=System" name="logName">
          Load System Logs
        </a>
      </form>
      <form class="readForm" action="logs" method="get">
        <a class="btn btn-lg btn-dark" href="logs?page=1" name="logName">
          Read System Logs
        </a>
      </form>
    </main>
    <footer>
      <p>By Shravana Tirtha</p>
    </footer>
  </body>
</html>
