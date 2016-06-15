import processing.serial.*;

final int RED = 0;
final int BLUE = 1;
final int GREEN = 2;
final int NO_OBJECT = 3;

Serial port;

color cor;
int saveMe = 0;
int startup=0;
byte index=0;
int colours[] = new int[8];
int order[] = new int[6];
PImage carImage;
int carX = 190, carY = 195;
int visits = 0;
int goal = order[visits];
int done = 1;
int rotation = 0;
int returning = 0;

boolean go = false;
boolean stop = false;
boolean goBack = false;

String redCar = "Sprites/carRed";
String yellowCar = "Sprites/carYellow";
String blueCar = "Sprites/carBlue";
String greenCar = "Sprites/carGreen";
String suffix = ".png";

void setup() {
  size(500, 500);

  for(int i = 0; i < 8; i++)
  {
    colours[i] = NO_OBJECT;
  }
  println("Available serial ports:");
  println(Serial.list());
  order[0] = -1;
  /*
  order[0] = 1;
  order[1] = 6;
  order[2] = 4;
  order[3] = 5;
  order[4] = 3;
  order[5] = 7;
  
  // Keep track of the objects
  colours[0] = NO_OBJECT;
  colours[1] = RED;
  colours[2] = NO_OBJECT;
  colours[3] = RED;
  colours[4] = GREEN;
  colours[5] = BLUE;
  colours[6] = BLUE;
  colours[7] = GREEN;
  */
  // check on the output monitor wich port is available on your machine
  port = new Serial(this, Serial.list()[1], 9600);
  port.bufferUntil(' '); 
  carImage = new PImage();
  carImage = loadImage(yellowCar + str(rotation) + suffix);
  carImage.resize(80, 80);
    port.write('Q');
    
}

void draw() {
  
  //port.read();
  
  
  background(245);
  textSize(24);
  fill(0, 102, 153, 204);
  text("Group 3 GUI Demo", 25, 25); 
  textSize(16);
  /*
  text(order[0], 255, 25);
  text(order[1], 275, 25);
  text(order[2], 295, 25);
  text(order[3], 315, 25);
  text(order[4], 335, 25);
  text(order[5], 355, 25);
  */
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
  
  if(visits < 6)
  {
    getData();
  }
  DrawCircles();
  goButton();
  dataButton();

  if(go || goBack)
  {
    GetNextCarPos();
    image(carImage, carX, carY);
  } else if(stop) {
    image(carImage, carX, carY);
  } else {
    image(carImage, 190, 195);
  }
 // println("loop");
}

void dataButton()
{
  textSize(16);
  fill(40);
  rect(110, 450, 75, 25, 5);
  fill(255);
  text("Get Data", 115, 468);
  if(mousePressed==true && mouseX>110 && mouseX<185 && mouseY>450 && mouseY<475)
  {
    println("PRESSED!");
    getData();
  }
}

void goButton()
{
  textSize(16);
  fill(40);
  rect(250, 450, 50, 25, 5);
  fill(255);
  text("Go", 265, 468);
  if(mousePressed==true && mouseX>250 && mouseX<300 && mouseY>450 && mouseY<475)
  {
    println("PRESSED");
    //println(order[0]);
    if(order[0] != -1) done = 0;
  }
}

void DrawCircles()
{
  int x, y, w = 25, h = 25;
  for(int i = 0; i < 8; i++)
  {
    //calculate the x
    if(i > 4) x = 50;
    else if(i == 0 || i == 4) x = 230; 
    else x = 410; 
    //calculate the y
    if(i == 7 || i < 2) y = 50;
    else if(i == 6 || i == 2) y = 230; 
    else y = 410; 
    
    //if(colours[i] == 0) continue;
    switch(colours[i])
    {
      case NO_OBJECT:
        stroke(245);
        fill(245, 255);
        break;
      case RED:
        fill(255, 0, 0, 255);
      break;
      case GREEN:
        fill(0, 255, 0, 255);
      break;
      case BLUE:
        fill(0, 0, 255, 255);
      break;
      default:
     // println("invalid case");
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

void GetNextCarPos()
{
  switch(goal)
  {
    case 0:
      if(returning == 1)
      {
        carY += 1;
        rotation = 180;
      } else
      {
        rotation = 0;
        carY -= 1;
      }
      break;
    case 1:
      if(returning == 1)
      {
        carY += 1;
        carX -= 1;
        rotation = 225;
      } else
      {
        rotation = 45;
        carY -= 1;
        carX += 1;
      }
      break;
    case 2:
      if(returning == 1)
      {
        carX -= 1;
        rotation = 270;
      } else
      {
        carX += 1;
        rotation = 90;
      }
      break;
    case 3:
      if(returning == 1)
      {
        rotation = 315;
        carX -= 1;
        carY -= 1;
      } else
      {
        rotation = 135;
        carX += 1;
        carY += 1;
      }
      break;
    case 4:
      if(returning == 1)
      {
        carY -= 1;
        rotation = 0;
      } else
      {
        rotation = 180;
        carY += 1;
      }
      break;
    case 5:
      if(returning == 1)
      {
        rotation = 45;
        carY -= 1;
        carX += 1;
      } else
      {
        rotation = 225;
        carY += 1;
        carX -= 1;
      }
      break;
   case 6:
       if(returning == 1)
      {
        rotation = 90;
        carX += 1;
      } else
      {
        rotation = 270;
        carX -= 1;
      }
      break;
   case 7:
      if(returning == 1)
      {
        rotation = 135;
        carY += 1;
        carX += 1;
      } else
      {
        rotation = 315;
        carY -= 1;
        carX -= 1;
      }
      break;
    default:
      break;
  }
      
  if((carY < 50 || carX < 60) || (carY > 325 || carX > 330) && (returning == 0))
  {
    returning = 1;
    
  }
    
  if(returning == 0)
  {
    carImage = loadImage(yellowCar + str(rotation) + suffix);
    carImage.resize(80, 80);
  } else
  {
    switch(colours[goal])
    {
      case RED: 
        carImage = loadImage(redCar + str(rotation) + suffix);
        break;
      case GREEN:
        carImage = loadImage(greenCar + str(rotation) + suffix);
        break;
      case BLUE:
        carImage = loadImage(blueCar + str(rotation) + suffix);
        break;
      default:
        break;
    }
    carImage.resize(80, 80);
  }
  
  if((returning == 1) && (carX < 192 && carX > 187) && (carY < 198 && carY > 192))
  {
      visits++;
      returning = 0;
      carY = 195;
      carX = 190;
  }
  
  if(visits > 5) 
  {
    done = 1;
    carX = 190;
    carY = 195;
  }else
  {
    goal = order[visits];
  }
}

void DrawCar()
{
  PImage carImage = new PImage();
  carImage = loadImage("car.png");
  carImage.resize(50, 50);
  image(carImage, 205, 205);
}
void getData()
{
  byte[] inBuffer = new byte[4];
  for(int i = 0; i < 4; i ++)
  {
    inBuffer[i] = 0;
  }
  
  while(port.available() > 0)
  {
    port.readBytesUntil(69, inBuffer);
    println(inBuffer);
  }
  
  if(inBuffer[0] != 0)
  {
  // if(inBuffer[1] ==  null) inBuffer[1] = 0;
    
   switch(inBuffer[0])
   {
     case 'G':
       go = true;
       order[visits] = inBuffer[1] - 48;
       break;
     case 'S':
       stop = true;
       go = false;
       break;
     case 'C':
       colours[inBuffer[1] - 48] = inBuffer[2] - 48;
       break;
     case 'R':
       goBack = true;
       stop = false;
       break;
     default:
   }
  }
}

void getAllData()
{
  byte[] inBuffer = new byte[14];
  for(int i = 0; i < 14; i ++)
  {
    inBuffer[i] = 0;
  }
  port.write('Q');
  while(port.available() > 0)
  {
    inBuffer = port.readBytes();
    println(inBuffer);
  }
  
  for(int i = 0; i < 6; i ++)
  {
    order[i] = inBuffer[i];
  }
  for(int i = 6; i < 14; i ++)
  {
    colours[i - 6] = inBuffer[i];
  }
}
void getDataOld()
{
  port.write('Q');
  while(port.available() != 0)
  {
    switch(GetFromSerial())
    {
      case 'A':
        println("read A");
        order[0] = GetFromSerial();
        println(order[0]);
        break;
      case 'B':
        println("read B");
        order[1] = GetFromSerial();
        println(order[1]);
        break;
      case 'C':
      println("read C");
        order[2] = GetFromSerial();
        println(order[2]);
        break;
      case 'D':
      println("read D");
        order[3] = GetFromSerial();
        println(order[3]);
        break;
      case 'E':
      println("read E");
        order[4] = GetFromSerial();
        println(order[4]);
        break;
      case 'F':
      println("read F");
        order[5] = GetFromSerial();
        println(order[5]);
        break;
      case 'G':
      println("read G");
        colours[0] = GetFromSerial();
        println(colours[0]);
        break;
      case 'H':
      println("read H");
        colours[1] = GetFromSerial();
        println(colours[1]);
        break;
      case 'I':
      println("read I");
        colours[2] = GetFromSerial();
        println(colours[2]);
        break;
      case 'J':
      println("read J");
        colours[3] = GetFromSerial();
        println(colours[3]);
        break;
      case 'K':
      println("read K");
        colours[4] = GetFromSerial();
        println(colours[4]);
        break;
      case 'L':
      println("read L");
        colours[5] = GetFromSerial();
        println(colours[5]);
        break;
      case 'M':
      println("read M");
        colours[6] = GetFromSerial();
        println(colours[6]);
        break;
      case 'N':
      println("read N");
        colours[7] = GetFromSerial();
        println(colours[7]);
        break;
    }
  }
}

int GetFromSerial()
{
  while (port.available()==0) {
   delay(10);
  }
  return port.read();
}