<?php
    $host = "localhost:3306";
    $dbusername = "root";
    $dbpassword = "";
    $dbname = "SLASHTRASH";
    // Create connection
    $conn = new PDO('mysql:host='.$host.';dbname='.$dbname, $dbusername, $dbpassword);
?>