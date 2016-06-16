import processing.serial.*;

final int RED = 0;
final int BLUE = 1;
final int GREEN = 2;
final int NO_OBJECT = 3;

Serial port;

int saveMe = 0;
int startup = 0;
byte index=0;
int colours[] = new int[8];
int order[] = new int[6];
PImage carImage;
float carX = 190, carY = 195;
int visits = 0;
int goal = order[visits];
int done = 1;
int rotation = 0;
int returning = 0;
float speed = 0.5;

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
  Data For Testing!
  order[0] = 1;
  order[1] = 6;
  order[2] = 4;
  order[3] = 5;
  order[4] = 3;
  order[5] = 7;
  colours[0] = NO_OBJECT;
  colours[1] = RED;
  colours[2] = NO_OBJECT;
  colours[3] = RED;
  colours[4] = GREEN;
  colours[5] = BLUE;
  colours[6] = BLUE;
  colours[7] = GREEN;
  */
  
  //use this to check on the output monitor wich port is available on your machine
  //port = new Serial(this, Serial.list()[0], 9600);
  port = new Serial(this, "COM11", 9600); 
  port.bufferUntil(' '); 
  carImage = new PImage();
  carImage = loadImage(yellowCar + str(rotation) + suffix);
  carImage.resize(80, 80);
  port.write('Q');
}

void draw() {
  background(245);
  textSize(24);
  fill(0, 102, 153, 204);
  text("Group 3 GUI Demo", 25, 25); 
    if(visits < 6)
   {
     text("Running...", 180, 470);
   } else
   {
     text("Done", 190, 470);
   }
  
  textSize(16);
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
  
  if(go || goBack)
  {
    GetNextCarPos();
    image(carImage, carX, carY);
  } else if(stop) {
    image(carImage, carX, carY);
  } else {
    image(carImage, 190, 195);
  }
}

/* Data button used for getting Data when car is plugged 
  in at the end of demo */
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

/* Sends the go signal to start the demo */
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
    port.write('B'); //B for Begin :)
  }
}

/* Draw the circles in the corners */
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
    }
    ellipse(x, y, w, h);
    stroke(0);
  }
}

/* Calculates the next car position based on its current goal*/
void GetNextCarPos()
{
  switch(goal)
  {
    case 0:
      if(returning == 1)
      {
        carY += speed;
        rotation = 180;
      } else
      {
        rotation = 0;
        carY -= speed;
      }
      break;
    case 1:
      if(returning == 1)
      {
        carY += speed;
        carX -= speed;
        rotation = 225;
      } else
      {
        rotation = 45;
        carY -= speed;
        carX += speed;
      }
      break;
    case 2:
      if(returning == 1)
      {
        carX -= speed;
        rotation = 270;
      } else
      {
        carX += speed;
        rotation = 90;
      }
      break;
    case 3:
      if(returning == 1)
      {
        rotation = 315;
        carX -= speed;
        carY -= speed;
      } else
      {
        rotation = 135;
        carX += speed;
        carY += speed;
      }
      break;
    case 4:
      if(returning == 1)
      {
        carY -= speed;
        rotation = 0;
      } else
      {
        rotation = 180;
        carY += speed;
      }
      break;
    case 5:
      if(returning == 1)
      {
        rotation = 45;
        carY -= speed;
        carX += speed;
      } else
      {
        rotation = 225;
        carY += speed;
        carX -= speed;
      }
      break;
   case 6:
       if(returning == 1)
      {
        rotation = 90;
        carX += speed;
      } else
      {
        rotation = 270;
        carX -= speed;
      }
      break;
   case 7:
      if(returning == 1)
      {
        rotation = 135;
        carY += speed;
        carX += speed;
      } else
      {
        rotation = 315;
        carY -= speed;
        carX -= speed;
      }
      break;
    default:
      break;
  }
      
  if((carY < 50 || carX < 60) || (carY > 325 || carX > 330) && (returning == 0))
  {
    returning = 1;
    go = false;
    stop = true;
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
      goBack = false;
      stop = true;
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

/* Read data from the serial */
void getData()
{
  byte[] inBuffer = new byte[4];
  for(int i = 0; i < 4; i ++)
  {
    inBuffer[i] = 0;
  }
  
  while(port.available() > 0)
  {
    /* all messages end in E */
    port.readBytesUntil('E', inBuffer); 
    println(inBuffer);
  }
  
  if(inBuffer[0] != 0)
  { 
   switch(inBuffer[0])
   {
     case 'G': //for Go
       go = true;
       order[visits] = inBuffer[1] - 48;
       break;
     case 'S': //for Stop
       stop = true;
       go = false;
       break;
     case 'C': //for Colour
       colours[inBuffer[1] - 48] = inBuffer[2] - 48;
       break;
     case 'R': //for returning
       goBack = true;
       stop = false;
       break;
     default:
   }
  }
}