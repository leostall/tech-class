<?php

include("conexao.php");

$username = $_POST['username'];
$password = $_POST['password'];

$sql = "SELECT * FROM colaboradores WHERE EMAIL = '$username' AND SENHA = '$password'";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    header("Location: pagina_incial_opcoes.html");
} else {
    echo "<script>alert('Credenciais inválidas. Tente novamente.');</script>";
}

$conn->close();
?>
