<?php
    require_once '../../utils/connect_database.php';

    // Check connection
    if(!$conn)
    {
        die("Connection failed");
    }
    else
    {
        $Est_Id = filter_input(INPUT_POST, 'Est_Id');
        $Cust_Id = filter_input(INPUT_POST, 'Cust_Id');

        if($query = "INSERT INTO VISITS (Est_Id, Cust_Id) VALUES (:Est_Id, :Cust_Id)")
        {
            if($query==null)
            {
                echo "Unable to insert due to violation.";
                die();
            }
            else
            {
                $stmt = $conn->prepare($query);
                $stmt->execute(array(':Est_Id' => $Est_Id, ':Cust_Id' => $Cust_Id));
                $rows = $stmt->fetchALL(PDO::FETCH_ASSOC);
                if($stmt)
                    echo 'New record inserted successfully.';
                else
                {
                    echo "Unable to insert due to violation. Please check your input and the table.";
                }
            }
        }

    }
    echo '<p><a href="javascript:history.go(-1)" title="return">&laquo; Return to Slash-Trash Homepage</a></p>';    
?>
