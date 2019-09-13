const char MAIN_page[] PROGMEM = R"=====(
<html>
  <HEAD>
      <TITLE>Bhasky</TITLE>
      <meta name="viewport" content="user-scalable=no"/>
  </HEAD>

<body>

<CENTER>
    <font size="70px" face="verdana">
        <p>B H A S K Y</p>
    </font>
    <form action="" method="get">
      <font size="60px" face="verdana"> 
      Angulos <input style="height:20%;width:40%;font-size:70px;margin:10px" name="angles" type="text" placeholder="Servo Angulo" />
      <input style="height:20%;width:40%;font-size:70px" type="submit" value="Enviar">
      </font>
    </form>
    <form action="" method="get">
      <button style="height:20%;width:40%;font-size:70px" type="submit" name="funcion" value="1" method="get">Saludar</button> 
      <button style="height:20%;width:40%;font-size:70px" type="submit" name="funcion" value="2" method="get">No</button> 
      <button style="height:20%;width:40%;font-size:70px" type="submit" name="funcion" value="3" method="get">Dar la mano</button> 
      <button style="height:20%;width:40%;font-size:70px" type="submit" name="funcion" value="4" method="get">Estabilizar</button> 
    </form>
</CENTER>

</body>
</html>

)=====";
