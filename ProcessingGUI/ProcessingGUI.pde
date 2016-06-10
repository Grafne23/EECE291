import processing.serial.*;
Serial port;

color cor;
int saveMe = 0;
int startup=0;
byte index=0;
int rVal, gVal, bVal, wVal;
int colours[] = new int[8];

void setup() {
  size(500, 500);

  println("Available serial ports:");
  println(Serial.list());
  
  /* Keep track of the objects
  * 0 - no object
  * 1 - Red
  * 2 - Blue
  * 3 - Green
  */
  
  colours[0] = 0;
  colours[1] = 1;
  colours[2] = 0;
  colours[3] = 1;
  colours[4] = 3;
  colours[5] = 2;
  colours[6] = 2;
  colours[7] = 3;
  
  // check on the output monitor wich port is available on your machine
  port = new Serial(this, Serial.list()[3], 9600);
  port.bufferUntil(' ');  
}

void draw() {
  background(245);
  textSize(24);
  fill(0, 102, 153, 204);
  text("Arduino GUI Learing is sub-optimal", 25, 25); 
  textSize(16);
  
  text(rVal, 10, 70);
  text(gVal, 50, 70);
  text(bVal, 90, 70);
  text(wVal, 130, 70);
  text(startup, 170, 70);
  
  strokeWeight(2);
  line(50, 60, 50, 400); //left
  line(60, 410, 400, 410); //bottom
  line(410, 400, 410, 60); //right
  line(60, 50, 400, 50); //top

  line(60, 60, 400, 400); //slash
  line(60, 400, 400, 60); //backslash
  line(230, 60, 230, 400); //vertical
  line(60, 230, 400, 230); //horizontal
  strokeWeight(1);
  
  DrawCircles();
  goButton();
}

void goButton()
{
  textSize(16);
  fill(40);
  rect(225, 450, 50, 25, 5);
  fill(255);
  text("Go", 240, 468);
  if(mousePressed==true && mouseX>225 && mouseX<275 && mouseY>450 && mouseY<475)
  {
    //println("PRESSED!");
    getData();
  }
}

void getData()
{
  port.write('Q');
  while(port.available() != 0)
  {
    switch(GetFromSerial())
    {
      case 'R':
        rVal=GetFromSerial();
        println(rVal);
        break;
      case 'G':
        gVal=GetFromSerial();
        println(gVal);
        break;
      case 'B':
        bVal=GetFromSerial();
        println(bVal);
        break;
      case 'W':
        wVal=GetFromSerial();
        println(wVal);
        break;
    }
  }
}

int GetFromSerial()
{
  while (port.available()==0) {
  }
  return port.read();
}

void DrawCircles()
{
  int x, y, w = 25, h = 25;
  for(int i = 0; i < 8; i++)
  {
    //calculate the x
    if(i < 3) x = 50;
    else if(i < 5) x = 230; 
    else x = 410; 
    //calculate the y
    if((i % 3) == 1) y = 50;
    else if((i % 3) == 2) y = 230; 
    else y = 410; 
    
    //if(colours[i] == 0) continue;
    switch(colours[i])
    {
      case 0:
        stroke(245);
        fill(245, 255);
        break;
      case 1:
        fill(255, 0, 0, 255);
      break;
      case 2:
        fill(0, 255, 0, 255);
      break;
      case 3:
        fill(0, 0, 255, 255);
      break;
      default:
      println("invalid case");
       break;
    }
    ellipse(x, y, w, h);
    stroke(0);
   //print(i);print(" : ");print(x);print(", ");println(y);
  }
  /*
  ellipse(50, 53, 25, 25); //x, y, width, height
  ellipse(50, 230, 25, 25);
  ellipse(50, 410, 25, 25); 
  ellipse(230, 50, 25, 25); 
  //ellipse(230, 230, 25, 25); 
  ellipse(230, 410, 25, 25); 
  ellipse(410, 50, 25, 25); 
  ellipse(410, 230, 25, 25); 
  ellipse(410, 410, 25, 25); 
  */
}