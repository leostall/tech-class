<?php
include("conexao.php");

// Receba o ID do dispositivo do formulário HTML
$dispositivo = $_POST["dispositivo"];
$acao = $_POST["acao"];

// Consulte o status atual no banco de dados
$sql = "SELECT `STATUS` FROM perifericos_3c WHERE ID = '$dispositivo'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    $status_atual = $row["STATUS"];
    
    // Atualize o status com base na ação recebida
    if ($dispositivo == "P001") {
        if ($acao == 2) {
            // Ligar
            $dado = "2";
        } elseif ($acao == 3) {
            // Desligar
            $dado = "3";
        } elseif ($acao == 4) {
            // Congelar
            $dado = "4";
        }
    } else {
        // Para outros dispositivos, inverta o status
        if ($status_atual == "LIGADO") {
            $dado = "0";
        } elseif ($status_atual == "DESLIGADO") {
            $dado = "1";
        } else {
            $dado = $status_atual;
        }
    }
    
    // Atualize a coluna "DADO" com o novo status
    $sql = "UPDATE perifericos_3c SET DADO = '$dado' WHERE ID = '$dispositivo'";
    
    if ($conn->query($sql) === TRUE) {
        // Retorne o novo status
        echo $dado;
    } else {
        echo "Erro ao atualizar o status: " . $conn->error;
    }
} else {
    echo "Dispositivo não encontrado.";
}

// Feche a conexão com o banco de dados
$conn->close();
?>