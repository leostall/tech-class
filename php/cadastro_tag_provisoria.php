<?php
include("conexao.php");

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $matricula = $_POST['matricula'];
    $rfid_codigo = $_POST['rfid_codigo'];
    $validade = $_POST['validade'];

    $matriculaTag = $matricula . "*";

    $data_hora = date('Y-m-d H:i:s');
    $dataAtual = date("Y-m-d");

    $dataValidade = date("Y-m-d", strtotime($dataAtual . " + " . ($validade - 1) . " days"));

    $sql_aluno_busca = "SELECT * FROM 3infc WHERE MATRICULA = '$matricula'";
    $result_aluno_busca = $conn->query($sql_aluno_busca);

if ($result_aluno_busca) {
    if ($result_aluno_busca->num_rows > 0) {
        $row_aluno_busca = $result_aluno_busca->fetch_assoc();
        $nomeAluno = $row_aluno_busca['NOME'];
        
        $matriculaTag = $matricula . "*"; // Agora você pode criar a matriculaTag depois de buscar o nome do aluno
        
        $sql_aluno = "SELECT * FROM 3infc WHERE MATRICULA = '$matriculaTag'";
        $result_aluno = $conn->query($sql_aluno);

        if ($result_aluno->num_rows > 0) {
            // Aluno encontrado, atualize os dados existentes
            $sql_atualizacao = "UPDATE 3infc SET TAG = '$rfid_codigo', VALIDADE = '$dataValidade', `TIPO` = 'PROVISÓRIA',`STATUS` = 'AUSENTE', `TIME` = '$data_hora' WHERE MATRICULA = '$matriculaTag'";
            $sql_execucao = $conn->query($sql_atualizacao);

            if ($sql_execucao === TRUE) {
                echo "<script>alert('Dados ATUALIZADOS com sucesso!');"; echo "window.location.href = 'http://localhost/pagina_incial_opcoes.html';</script>";
            } else {
                echo "Erro ao atualizar dados: " . $conn->error;
            }
        } else {
            // Aluno não encontrado, insira novos dados
            $sql_insercao = "INSERT INTO 3infc (MATRICULA, NOME, TAG, VALIDADE, TIPO, `STATUS`, `TIME`, TURMA) VALUES ('$matriculaTag', '$nomeAluno', '$rfid_codigo', '$dataValidade', 'PROVISÓRIA', 'AUSENTE', '$data_hora', '3 INF C')";
            $sql_execucao = $conn->query($sql_insercao);

            if ($sql_execucao === TRUE) {
                echo "<script>alert('Dados CADASTRADOS com sucesso!');"; echo "window.location.href = 'http://localhost/pagina_incial_opcoes.html';</script>";
            } else {
                echo "Erro ao cadastrar dados: " . $conn->error;
            }
        }
    } else {
        echo "Aluno não encontrado.";
    }
} else {
    echo "Erro na busca do aluno: " . $conn->error;
}
    
    $conn->close();
}
?>