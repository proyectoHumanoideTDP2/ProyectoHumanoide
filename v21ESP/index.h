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
    #cover {
        background: #222;
 
        height: 100%;
        text-align: center;

        align-items: center;
        }

    .probando {
        background-color: blue;
    }
  </style>
<body>
    <div class="col">
        <div class="row text-white justify-content-center mt-2">
            <h1 class="display-4">B H A S K Y<h1></h1>
        </div>
        <br>
        <form action="" method="get" class="row justify-content-center">
            <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="1" method="get">Saludar</button> 
            <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="2" method="get">No</button> 
            <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="3" method="get">Dar la mano</button>
            <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="4" method="get">Caminar</button>  
            <button class="btn btn-lg btn-light mr-2 mt-2 col-md-2" type="submit" name="funcion" value="5" method="get">Estabilizar</button> 
        </form>
        <br>
        <div class="container">
              <form action="" class="row justify-content-center ">
                  <div class="form-group">
                      <label class="sr-only">Angulos</label>
                      <input type="text" class="form-control" name="angles" placeholder="Servo + Angulo">
                  </div>
                  <div class="form-group">
                      <button type="submit" class="btn btn-success form-control ml-2">Enviar</button>
                  </div>
              </form>
              
              <div class="row justify-content-center">
                  <button type="button" class="btn btn-primary " data-toggle="modal" data-target="#exampleModal2">
                      Info-Servos
                  </button>    
              </div>
              <br>
              <div class="container col-md-6 justify-content-center">
                  <form action="" method="get" class="row justify-content-center">
                          <h5 class="text-white ">Velocidad</h5>
                          <select class="form-control  " name="velocidad" id="exampleFormControlSelect1">
                              <option>1</option>
                              <option>2</option>
                              <option>3</option>
                              <option>4</option>
                              <option>5</option>
                          </select>
                          <input class="btn btn-primary mt-2 " type="submit" value="Actualizar Velocidad">
                  </form>   
              </div>
                         
       
        </div>
    </div>
    <script src="/bootstrap.min.js"></script>
</body>
</html>


)=====";
