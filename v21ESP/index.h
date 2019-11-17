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
    .caminar-button{
      justify-content: center;
      display: block;
    }
    .title-text{
      text-align: center;
      display: block;
      color:white;
      font-family: 'Times new Roman';
    }
    .title{
      letter-spacing: 1ch;
      margin-top:15px;
    }
    .movimientos{
      margin-top:15px;
    }
    .pasos{
      margin-right:15px;
      margin-bottom:5px;
    }
    .text-radio{
      color:white;
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
      <h1 class="title title-text">BHASKY</h1>
      <div class="line"></div>
      <div class="row content-function">
        <h2 class="title-text movimientos">Movimientos</h2>
        <div class="movimientos-content">
            <form action="" method="get" class="row justify-content-center">
              <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="1" method="get">Estabilizar</button> 
              <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="2" method="get">Saludar</button> 
              <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="3" method="get">No</button> 
              <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="4" method="get">Dar la mano</button>
              <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="5" method="get">Dab</button> 
              <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="6" method="get">Onda</button> 
            </form>
        </div>
        <div class="caminar-content row">
          <form action="" method="get" class="justify-content-center">
              <h2 class="title-text">Caminar</h2>
                <div class="row"> 
                  <h4 class="title-text pasos"> Pasos </h4>
                  <div class="form-check form-check-inline">
                    <input class="form-check-input" type="radio" id="2pasos"  value='2' name="caminar" checked>
                    <label class="form-check-label text-radio" for="2pasos"> 2 </label>
                  </div>
                <div class="form-check form-check-inline">
                  <input class="form-check-input" type="radio" id="3pasos"  value='3' name="caminar">
                    <label class="form-check-label text-radio" for="3pasos"> 3 </label>
                </div>
                <div class="form-check form-check-inline">
                  <input class="form-check-input" type="radio" id="4pasos"  value='4' name="caminar">
                  <label class="form-check-label text-radio" for="4pasos"> 4 </label>
                </div>
                <div class="form-check form-check-inline">
                  <input class="form-check-input" type="radio" id="5pasos"  value='5' name="caminar">
                  <label class="form-check-label text-radio" for="5pasos"> 5 </label>
                </div>
              </div>
                <div class="row justify-content-center">
                    <button class="btn btn-lg btn-light mr-2 mt-2" type="submit">Caminar</button>
                </div>
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
