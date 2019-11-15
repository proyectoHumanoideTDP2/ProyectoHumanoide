const char MAIN_page[] PROGMEM = R"=====(
<html>
  <HEAD>
      <TITLE>B H A S K Y</TITLE>
      <meta name="viewport" content="user-scalable=no"/>
      
      <link href="/bootstrap.min.css" rel="stylesheet">

  </HEAD>
  <style>
    html,body{ 
        background:#222;
        
        }
    .content-angles{
      margin-top:15px;
    }
    .content-function{
      display: flex;
      justify-content: center;
      flex-direction: column;
      width: 100%;
    }
    .caminar-content{
      justify-content: center;
      display: flex;
    }
    h1{
      text-align: center;
      display: block;
      color:white;
      margin-top:15px;
      letter-spacing: 1ch;
      font-family: 'Times new Roman';
    }
    h2{
      color:white;
      display: block;
      margin-top:15px;
      font-family: 'Times new Roman';
      text-align: center;
    }
    .line{
      width: 80%;
      border:1px solid white;
    }
    .page-content{
      display:flex;
      justify-content: center;
      flex-direction: column;
      align-items: center;
    }
  </style>
<body>
    <div class="container page-content">
      <h1>BHASKY</h1>
      <div class="line"></div>
      <div class="row content-function">
        <h2>Movimientos</h2>
        <div class="movimientos-content">
            <form action="" method="get" class="row justify-content-center">
                <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="1" method="get">Saludar</button> 
                <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="2" method="get">No</button> 
                <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="3" method="get">Dar la mano</button>
                <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="5" method="get">Estabilizar</button> 
                <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="5" method="get">Dab</button> 
                <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="5" method="get">Onda</button> 
            </form>
        </div>
        <div class="caminar-content">
            <form action="" method="get" class="row justify-content-center">
                    <h2>Caminar</h5>
                    <select class="form-control" id="exampleFormControlSelect1">
                        <option>2</option>
                        <option>3</option>
                        <option>4</option>
                        <option>5</option>
                    </select>
                    <button class="btn btn-lg btn-light mr-2 mt-2 " name="caminar" type="submit">Caminar</button>
            </form>   
        </div>
      </div>
      <div class="line"></div>
      <div class="content-angles">
        <form action="" class="row justify-content-center ">
            <div class="form-group">
                <label class="sr-only">Angulos</label>
                <input type="text" class="form-control" name="angles" placeholder="Servo + Angulo">
            </div>
            <div class="form-group">
                <button type="submit" class="btn btn-success form-control ml-2">Enviar</button>
            </div>
        </form>
      </div>
    </div>
</body>
</html>


)=====";
