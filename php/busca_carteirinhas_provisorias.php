<?php
ini_set('memory_limit', '2097152'); 
set_time_limit(0);

include("conexao.php");

$sql = 'SELECT * FROM 3infc WHERE `TIPO` = "PROVISÓRIA"';

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $alunos = array();

    while ($row = $result->fetch_assoc()) {
        $aluno = array(
            'id' => $row['MATRICULA'],
            'nome' => $row['NOME'],
            'turma' => $row['TURMA'],
            'tipo' => $row['TIPO'],
            'cod' => $row['TAG'],
            'validade' => $row['VALIDADE'],
        );
        $alunos[] = $aluno;
    }

    // Ordenar o array de alunos pelo nome
    usort($alunos, function ($a, $b) {
        return strcmp($a['nome'], $b['nome']);
    });

    header('Content-Type: application/json');
    echo json_encode($alunos);
} else {
    echo 'Nenhum aluno presente encontrado.';
}

$conn->close();
?>