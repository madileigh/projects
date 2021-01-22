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
        $EName = filter_input(INPUT_POST, 'EName');
        $Waste_Pts = filter_input(INPUT_POST, 'Waste_Pts');
        $Est_Address = filter_input(INPUT_POST, 'Est_Address');
        $Type = filter_input(INPUT_POST, 'Type');

        if($query = "INSERT INTO ESTABLISHMENT(Est_Id, EName, Est_Address, Waste_Pts, Type) VALUES (:Est_Id, :EName, :Est_Address, :Waste_Pts, :Type)")
        {
            if($query==null)
            {
                echo "Unable to insert due to violation.";
                die();
            }
            else
            {
                $stmt = $conn->prepare($query);
                $stmt->execute(array(':Est_Id' => $Est_Id, ':EName' => $EName, ':Est_Address' => $Est_Address, ':Waste_Pts' => $Waste_Pts, ':Type' => $Type));
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
