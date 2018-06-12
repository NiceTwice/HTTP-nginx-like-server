<?php
    session_start();
?>
<!DOCTYPE html>
<html lang="fr">
<head>
  <title>Test page with get and post</title>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">

  <!-- Viewport (for responsive) -->
  <meta name="viewport" content="width=device-width, initial-scale=1, minimum-scale=1, user-scalable=no">
  <meta content="text/html; charset=UTF-8" http-equiv="content-type"/>
</head>
<body>

  <?php
    if (empty($_POST) && (!isset($_SESSION) || !isset($_SESSION['user']))) {
  ?>

    <form id="log" name="login" method="POST" accept-charset="utf-8">
      <span>
        <input id="login" name="user[login]" placeholder="Identifiant" class="form-control" type="text" autofocus required/>
        <i class="icon-user"></i>
      </span>
      <span>
        <input name="user[password]" placeholder="Mot de passe" class="form-control" type="password" id="Password" required/>
        <i class="icon-lock"></i>
      </span>
      <button id="connect" class="btn btn-sm btn-success">
        <i class="icon-sign-in"></i>
        Connexion
      </button>
    </form>

  <?php
    }
    else {
      if (!isset($_SESSION['user']))
        $_SESSION['user'] = $_POST['user'];
  ?>

    <pre>

      <?php
        print_r($_SESSION);
      ?>

    </pre>

  <?php
    }
  ?>

</body>
</html>
