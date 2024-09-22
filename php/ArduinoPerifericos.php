<?php
    //   localhost/ArduinoPerifericos.php?valV001=38.2&valV002=37.9

    include("conexao.php");

/****************************************************************** INSERE A TEMPERATURA ATUAL DA SALA*/

    $valV001 = $_GET['valV001'];

    $updateSql = "UPDATE perifericos_3c SET VAL2 = '$valV001' WHERE ID = 'V001'";
    if ($conn->query($updateSql) === TRUE) {}

/****************************************************************** FAZ O UPDATE (DADO E STATUS) DOS VENTILADORES COM BASE NA TEMPERATURA MEDIA */
    $sql_presenca = "SELECT COUNT(*) as total FROM 3infc WHERE `STATUS` = 'PRESENTE'";
    $result_presenca = $conn->query($sql_presenca);

    if ($result_presenca) {
        $row_presenca = $result_presenca->fetch_assoc();
        $total_pessoas = $row_presenca['total'];

        $corteSql = "SELECT VAL1 FROM perifericos_3c WHERE ID = 'V001'";
        $result = $conn->query($corteSql);

        if ($result->num_rows > 0) {
            $row = $result->fetch_assoc();
            $corte = $row['VAL1'];

            if ($total_pessoas > 0) {
                if ($corte < $valV001) {
                    $updateDadosSql = "UPDATE perifericos_3c SET DADO = '1', `STATUS` = 'LIGADO' WHERE ID = 'V001'";
                    if ($conn->query($updateDadosSql) === TRUE) {}
                } else {
                    $updateDadosSql = "UPDATE perifericos_3c SET DADO = '0', `STATUS` = 'DESLIGADO' WHERE ID = 'V001'";
                    if ($conn->query($updateDadosSql) === TRUE) {}
                }
            } else {
                $updateDadosSql = "UPDATE perifericos_3c SET DADO = '0', `STATUS` = 'DESLIGADO' WHERE ID = 'V001'";
                if ($conn->query($updateDadosSql) === TRUE) {}
            }
        }
    }   

/***********************************************************************************************************/

// Atualiza o status do computador
$sql = "SELECT DADO FROM perifericos_3c WHERE ID = 'C001'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    $val1 = $row['DADO'];

    if ($val1 == 1) {
        $updateSql = "UPDATE perifericos_3c SET `STATUS` = 'LIGADO' WHERE ID = 'C001'";
    } else {
        $updateSql = "UPDATE perifericos_3c SET STATUS = 'DESLIGADO' WHERE ID = 'C001'";
    }
    if ($conn->query($updateSql) === TRUE) {}
}

// Atualiza o status das luzes laterais
$sql = "SELECT DADO FROM perifericos_3c WHERE ID = 'L008'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    $val3 = $row['DADO'];

    if ($val3 == 1) {
        $updateSql = "UPDATE perifericos_3c SET STATUS = 'LIGADO' WHERE ID = 'L008'";
    } else {
        $updateSql = "UPDATE perifericos_3c SET STATUS = 'DESLIGADO' WHERE ID = 'L008'";
    }
    if ($conn->query($updateSql) === TRUE) {}
}

/******************************************************************* TESTE AUTOMATIZAÇÃO LUZES*/

$sql_presenca = "SELECT COUNT(*) as total FROM 3infc WHERE `STATUS` = 'PRESENTE'";
$result_presenca = $conn->query($sql_presenca);

if ($result_presenca) {
    $row_presenca = $result_presenca->fetch_assoc();
    $total_pessoas = $row_presenca['total'];

    if ($total_pessoas > 0) {
        // Há pessoas na sala, ligue as lâmpadas
        $sql_ligar_lampadas = "UPDATE perifericos_3c SET  DADO = '1', STATUS = 'LIGADO' WHERE ID = 'L008'";  
        if ($conn->query($sql_ligar_lampadas) === TRUE) {}
    
    } else {
        // Não há pessoas na sala, desligue as lâmpadas
        $sql_desligar_lampadas = "UPDATE perifericos_3c SET  DADO = '0', STATUS = 'DESLIGADO' WHERE ID = 'L008'";  
        $result_desligar_lampadas = $conn->query($sql_desligar_lampadas);
    }
}

/******************************************************************* LUZES CENTRAIS CONFORME O STATUS DA PROJEÇÃO*/

$sql_presenca = "SELECT COUNT(*) as total FROM 3infc WHERE `STATUS` = 'PRESENTE'";
$result_presenca = $conn->query($sql_presenca);

if ($result_presenca) {
    $row_presenca = $result_presenca->fetch_assoc();
    $total_pessoas = $row_presenca['total'];

    $projetor1 = "SELECT * FROM perifericos_3c WHERE ID = 'P001'";
    $result = $conn->query($projetor1);

    $row = $result->fetch_assoc();
    $valStatus = $row['STATUS'];

    if ($total_pessoas > 0) {
        if ($valStatus == 'LIGADO') {
            $updateSql = "UPDATE perifericos_3c SET  DADO = '0', STATUS = 'DESLIGADO' WHERE ID = 'L004'";
            if ($conn->query($updateSql) === TRUE) {}

            $updateSql3 = "UPDATE perifericos_3c SET  DADO = '1', STATUS = 'LIGADO' WHERE ID = 'L001'";
            if ($conn->query($updateSql3) === TRUE) {}
        }
        if ($valStatus == 'DESLIGADO') {
            $updateSql = "UPDATE perifericos_3c SET  DADO = '1', STATUS = 'LIGADO' WHERE ID = 'L004'";
            if ($conn->query($updateSql) === TRUE) {}

            $updateSql3 = "UPDATE perifericos_3c SET  DADO = '0', STATUS = 'DESLIGADO' WHERE ID = 'L001'";
            if ($conn->query($updateSql3) === TRUE) {}
        }
    
    } else{
        $updateSql = "UPDATE perifericos_3c SET  DADO = '0', STATUS = 'DESLIGADO' WHERE ID = 'L004'";
        if ($conn->query($updateSql) === TRUE) {}

        $updateSql1 = "UPDATE perifericos_3c SET  DADO = '0', STATUS = 'DESLIGADO' WHERE ID = 'L001'";
        if ($conn->query($updateSql1) === TRUE) {}
    }

}

/****************************************************************** PRINTA OS VALORES PARA O ARDUINO*/

    $ids = ['C001', 'L001', 'L004', 'L008', 'V001', 'V002'];
    $query = "SELECT * FROM perifericos_3c WHERE ID IN ('" . implode("','", $ids) . "')";

    $result = $conn->query($query);

    if ($result->num_rows > 0) {
        while ($row = $result->fetch_assoc()) {
            echo $row['ID'] . ":" . $row['DADO'] . ";";
        }
    }

/****************************************************************** PRINTA OS VALORES PARA O ARDUINO*/

    $query = "SELECT * FROM perifericos_3c WHERE ID = 'P001'";

    $result = $conn->query($query);

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        echo $row['ID'] . ":" . $row['DADO'] . ";ONOFF:" . $row['VAL1'] . ";FREEZE:" . $row['VAL2'] . ";";
    } else {
        //echo "Nenhum resultado encontrado.";
    }

/****************************************************************** ATUALIZA OS VALORES DO PROJETOR DEPOIS DE PRINTAR PARA O ARDUINO*/

$projetor = "SELECT * FROM perifericos_3c WHERE ID = 'P001'";
    $result = $conn->query($projetor);

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        $valDado = $row['DADO'];
        $valSatatus = $row['STATUS'];

        if($valDado == '2'){
            $updateDadosSql2 = "UPDATE perifericos_3c SET DADO = '1', `STATUS` = 'LIGADO' WHERE ID = 'P001'";
            if ($conn->query($updateDadosSql2) === TRUE){/* LIGA O PROJETOR */}
        }

        if ($valDado == '3') {
            $updateDadosSql3 = "UPDATE perifericos_3c SET DADO = '0', `STATUS` = 'DESLIGADO' WHERE ID = 'P001'";
            if ($conn->query($updateDadosSql3) === TRUE) {/* DESLIGA O PROJETOR */}
        }
        
        if($valDado == '4' && $valSatatus === "LIGADO"){
            $updateDadosSql4 = "UPDATE perifericos_3c SET DADO = '1' WHERE ID = 'P001'";
            if ($conn->query($updateDadosSql4) === TRUE) {/* CONGELA O PROJETOR */}
        } else{
            $updateDadosSql5 = "UPDATE perifericos_3c SET DADO = '0' WHERE ID = 'P001'";
            if ($conn->query($updateDadosSql5) === TRUE) {/* CONGELA O PROJETOR (ENVIA O 0 POR QUE ESTAVA DESLIGADO)*/}
        }
    }

/*******************************************************************/

    $conn->close();
?>