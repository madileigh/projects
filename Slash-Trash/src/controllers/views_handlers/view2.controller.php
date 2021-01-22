<?php
    require_once '../../utils/connect_database.php';

    // Check connection
    if(!$conn) 
    {  
        die("Connection failed");
    }
    else 
    {
        $est_id = filter_input(INPUT_POST, 'Est_Id');
        $visit_date = filter_input(INPUT_POST, 'visit_date');
        $points = filter_input(INPUT_POST, 'points');

        $query = "UPDATE TRANSACTION_INFO SET Cust_Pts = Cust_Pts + ".$points." WHERE Trans_Date = \"".$visit_date."\" AND Est_Id = \"".$est_id."\";";

        $result = $conn->query($query);
        if ($result)
        {
            echo "<br /><b>";
            echo "Customer points have been updated successfully.";
            echo "</b>";
        }
        else
        {
            echo "Error:".$query."
            ".$conn->error;
        }
    }
    echo '<p><a href="javascript:history.go(-1)" title="return">&laquo; Return to Slash-Trash Homepage</a></p>';
?>