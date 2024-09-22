<?php

include("conexao.php");

$matricula = $_GET['matricula'];

$sql = "SELECT * FROM 3infc WHERE MATRICULA = '$matricula'";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();

    $aluno = array(
        'id' => $row['MATRICULA'],
        'nome' => $row['NOME'],
        'turma' => $row['TURMA'],
    );

    $alunos[] = $aluno;

    header('Content-Type: application/json');
    echo json_encode($alunos);

} else {
    echo "Nenhum aluno encontrado com essa matrícula.";
}

$conn->close();
?>