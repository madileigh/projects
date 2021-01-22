<?php
    require_once '../../utils/connect_database.php';

    // Check connection
    if(!$conn) 
    {  
        die("Connection failed");
    }
    else 
    {
        $Order_Id = filter_input(INPUT_POST, 'Order_Id');
        $Pts = filter_input(INPUT_POST, 'Pts');
        $Item_Id = filter_input(INPUT_POST, 'Item_Id');

        if($query = "UPDATE ORDERS SET Pts = :Pts, Item_Id = :Item_Id WHERE Order_Id = :Order_Id")
        {    
            if($query==null)
            {      
                echo "Unable to update due to violation.";      
                die();    
            }    
            else
            {
                $stmt = $conn->prepare($query);  
                $stmt->execute(array(':Order_Id' => $Order_Id, ':Pts' => $Pts, ':Item_Id' => $Item_Id));
                $rows = $stmt->fetchALL(PDO::FETCH_ASSOC);
                if($stmt)
                    echo 'Record updated successfully.';
                else
                {
                    echo "Unable to update due to violation. Please check your input and the table."; 
                }
            }
        }

    }
    echo '<p><a href="javascript:history.go(-1)" title="return">&laquo; Return to Slash-Trash Homepage</a></p>';    
?>