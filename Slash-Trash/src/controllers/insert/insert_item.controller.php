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
        $IName = filter_input(INPUT_POST, 'IName');
        $Pt_Val = filter_input(INPUT_POST, 'Pt_Val');
        $Category = filter_input(INPUT_POST, 'Category');
        $Cust_Id = filter_input(INPUT_POST, 'Cust_Id');

        if($query = "INSERT INTO REUSABLE_ITEM(Item_Id, Category, Pt_Val, IName, Cust_Id) VALUES (:Item_Id, :Category, :Pt_Val, :IName, :Cust_Id)")
        {
            if($query==null)
            {
                echo "Unable to insert due to violation.";
                die();
            }
            else
            {
                $stmt = $conn->prepare($query);
                $stmt->execute(array(':Item_Id' => $Item_Id, ':Category' => $Category, ':Pt_Val' => $Pt_Val, ':IName' => $IName, ':Cust_Id' => $Cust_Id));
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
