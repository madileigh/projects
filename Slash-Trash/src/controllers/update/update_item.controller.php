<?php
    require_once '../../utils/connect_database.php';

    // Check connection
    if(!$conn) 
    {  
        die("Connection failed");
    }
    else 
    {
        $Item_Id = filter_input(INPUT_POST, 'Item_Id');
        $Pt_Val = filter_input(INPUT_POST, 'Pt_Val');

        if($query = "UPDATE REUSABLE_ITEM SET Pt_Val = :Pt_Val WHERE Item_Id = :Item_Id")
        {    
            if($query==null)
            {      
                echo "Unable to update due to violation.";      
                die();    
            }    
            else
            {
                $stmt = $conn->prepare($query);  
                $stmt->execute(array(':Item_Id' => $Item_Id, ':Pt_Val' => $Pt_Val));
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