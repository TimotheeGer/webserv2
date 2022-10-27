<!DOCTYPE html>
<html>
  <head>
    <style>
      @import url("https://fonts.googleapis.com/css2?family=Didact+Gothic&family=Hind:wght@400;700&display=swap");
      body {
        width: 90%;
        margin: 0 auto;
        background-color: #e9d5f1;
      }
      h1 {
        font-family: "Hind", sans-serif;
      }
      .box {
        margin: 0 auto;
        display: flex;
        flex-direction: column;
        /* height: 700px; */
        justify-content: center;
        justify-items: center;
		overflow: auto;
        /* border: 1px solid green; */
      }
      .text {
        opacity: 0.5;
        line-height: 150%;
        align-self: center;
        text-align: center;
        display: inline-block;
        /* border: 1px solid blue; */
      }
      .line {
        height: 1px;
        background-color: black;
        margin: 1rem;
        opacity: 0.3;
      }
      .img {
        width: auto;
        margin: 0 auto;
        /* border: 1px solid green; */
      }

	    body {
        font-family: "Hind", sans-serif;
        text-align: center;
        padding-top: 100px;
		width: 90%;
        margin: 0 auto;
        background-color: #e9d5f1;
      }
	  

    </style>
    <meta charset="utf-8" />
    <title>Webserv</title>
    <link
      href="https://profile.intra.42.fr/assets/42_logo_black-684989d43d629b3c0ff6fd7e1157ee04db9bb7a73fba8ec4e01543d650a1c607.png"
      rel="icon"
      type="image/png"
    />
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1" />
  </head>
  <body>
    <div class="box">
      <div class="img">
		<a href="./index.html">
        <img 
          src="./img/webserv.png"
          alt="logo"
          class="logo"
        /></a>
      </div>
      <div class="text">
        <h1>Welcome to WebservTest</h1>
        <h1>Here all the images on the serv</h1>
      </div>
    
	  <?php
	  $files = scandir('../images');
	  foreach ($files as $file) {
	  if ($file !== "." && $file !== "..") {
				  echo "<div id=\"text\">";
				  echo "<img src='../img/$file' alt='$file' style='max-width:30%' id=\"$file\"/> <br><br>\n\n";
				  echo "</div>";
				  echo "<br>";
			  }
		  } 
		  
		?>
  </body>
</html>
