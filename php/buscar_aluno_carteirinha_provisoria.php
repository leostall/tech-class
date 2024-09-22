<?php
include("conexao.php");

if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST['matricula'])) {
    $matricula = $_POST['matricula'];

    $sql = "SELECT * FROM 3infc WHERE MATRICULA = '$matricula'";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        $aluno = array(
            'nome' => $row['NOME'],
            'turma' => $row['TURMA']
        );
        header('Content-Type: application/json');
        echo json_encode($aluno);
    } else {
        echo json_encode(null); // Nenhum aluno encontrado
    }
}
?>
