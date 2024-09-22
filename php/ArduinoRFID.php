<?php

include("conexao.php");

$code_tag = $_GET['TAG_NFC'];
$modulo = $_GET['MODULO'];

date_default_timezone_set('America/Sao_Paulo');
$data_hora = date('Y-m-d H:i:s');
$dataAtual = date("Y-m-d");

$sql = "SELECT * FROM 3infc WHERE TAG = '$code_tag'";

$result = $conn->query($sql);

if ($result->num_rows > 0) { // SOMENTE PARA ALUNOS

  //echo("ALUNO LOCALIZADO / ");

  if($modulo == 1){

    $row = $result->fetch_assoc();
    $status = $row["STATUS"];
    $validade = $row["VALIDADE"];

    if($dataAtual <= $validade){

      if ($status == "AUSENTE"){
      
        $sql = "UPDATE 3infc SET `STATUS` = 'PRESENTE', `TIME` = '$data_hora'  WHERE TAG ='$code_tag'";
        
        if ($conn->query($sql) === TRUE){
          //echo "STATUS ALTERADO PARA: PRESENTE";
          $response = 1; 
        } 
      } else {
        //echo "ALUNO PRESENTE EM SALA";
        $response = 0;
      }
    }else{
      $response = 0;
      //VALIDADE INVALIDA
    }
  } elseif($modulo == 2){

    $row = $result->fetch_assoc();
    $status = $row["STATUS"];
    $validade = $row["VALIDADE"];

    if($dataAtual <= $validade){

    if ($status == "PRESENTE"){
      
      $sql = "UPDATE 3infc SET `STATUS` = 'AUSENTE', `TIME` = '$data_hora'  WHERE TAG ='$code_tag'";
        
      if ($conn->query($sql) === TRUE){
        //echo "STATUS ALTERADO PARA: AUSENTE";
        $response = 1;
      } 
    } else {
      //echo "ALUNO AUSENTE EM SALA";
      $response = 0;
    }

  }else{
    $response = 0;
    //VALIDADE INVALIDA
  }
  $conn->close();
}

} else { //BUSCA DE COLABORADORES
  $response = 0;
  //echo("ALUNO NÃO LOCALIZADO - BUSCANDO NA BASE DE DADOS: COLABORADORES / ");

  $sql = "SELECT * FROM colaboradores WHERE TAG = '$code_tag'";

  $result = $conn->query($sql);

  if ($result->num_rows > 0) { //ACHOU
    
    if($modulo == 1){
      
      $row = $result->fetch_assoc();
      $status = $row["STATUS"];

      if ($status == "AUSENTE") {

        $sql = "UPDATE colaboradores SET `STATUS` = 'PRESENTE', `TURMA`  = '3 INF C', `TIME` = '$data_hora'  WHERE TAG ='$code_tag'";

        if ($conn->query($sql) === TRUE) {
          //echo "STATUS ATUALIZADO DO PARA PRESENTE EM 3°INFC";
          $response = 1;
        } 
      } else {
        //echo "ERRO AO ATUALIZAR (POSSIVEL CAUSA: COLABORADOR PRESENTE EM SALA)";
        $response = 0;
      }
    } elseif($modulo == 2){
      
      $row = $result->fetch_assoc();
      $status = $row["STATUS"];

      if ($status == "PRESENTE") {

        $sql = "UPDATE colaboradores SET `STATUS` = 'AUSENTE', `TURMA`  = 'N/A', `TIME` = '$data_hora'  WHERE TAG ='$code_tag'";

        if ($conn->query($sql) === TRUE) {
          //echo "STATUS ATUALIZADO DO PARA AUSENTE";
          $response = 1;
        } 
      } else {
        //echo "ERRO AO ATUALIZAR (POSSIVEL CAUSA: COLABORADOR AUSENTE DA SALA)";
        $response = 0;
      }
    } elseif($modulo == 3){

      $sql = "SELECT * FROM `colaboradores` WHERE TAG = '$code_tag' AND `STATUS` = 'PRESENTE'";

      $result = $conn->query($sql);

      if ($result->num_rows > 0) {
        //echo "ACESSO AOS SISTEMAS: LIBERADO";
        $response = 1;

      } else {
        //echo "ACESSO AOS SISTEMAS: NEGADO (POSSIVEL CAUSA: COLABORADOR AUSENTE DA SALA)";
        $response = 0;
      }
    }
  } else { // NÃO ACHOU
    //echo "ATENÇÃO: COLABORADOR NÃO ENCONTRADO NA BASE DE DADOS";
    $response = 0;
  }

  $conn->close();
}

echo ($response);
?>