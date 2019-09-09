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
    <input style="height:20%;width:40%;font-size:70px" type="submit" value="Saludar" method="get" action="saludar1" id="saludar2" name="saludar3"> 
</CENTER>

</body>
</html>

)=====";

