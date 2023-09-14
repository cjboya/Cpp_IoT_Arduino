<?php
$onoroff = $_GET["state"];
$textfile = "LEDstate.txt";
 
$fileLocation = "$textfile";
$fh = fopen($fileLocation, 'w   ') or die("Algo Fallo"); // Esto abre el archivo .txt para escribir y remplaza su contenido
$stringToWrite = "$onoroff"; // Escribe 1 o 0 dependiendo de la respuesta obtenida en index.html
fwrite($fh, $stringToWrite); // Escribe sobre el archivo .txt
fclose($fh);
 
header("Location: index.html"); // Regresa a (index.html)

    ?>