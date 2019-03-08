#define Base_Speed 170  //speed for moving forward
#define Kp 0.9          //proportional gain constant  
#define Kd 90           //derivative gain constant
#define Ki 1/80000        //integrative gain constant

const int left_motor = 5;
const int right_motor = 6;
const int left_motor_direction = 2;
const int right_motor_direction = 3;
static int last_error = 0 ;
int sum_error = 0;

void setup()
{
  pinMode(left_motor, OUTPUT);
  pinMode(right_motor, OUTPUT);
  pinMode(left_motor_direction, OUTPUT);
  pinMode(right_motor_direction, OUTPUT);
}

uint8_t readsensors()
{
  uint8_t input = 0;
  input |= digitalRead(21)<< 7;
  input |= digitalRead(8) << 6;
  input |= digitalRead(10)<< 5;
  input |= digitalRead(4) << 4;
  input |= digitalRead(19)<< 3;
  input |= digitalRead(20)<< 2;
  input |= digitalRead(18)<< 1;
  input |= digitalRead(9) << 0;
  
  return input;
}

int return_error(uint8_t input){
  int error = 0;

  switch(input)
  {
    case 1:   error = -7; break;
    case 3:   error = -6; break;
    case 2:   error = -5; break;
    case 6:   error = -4; break;
    case 4:   error = -3; break;
    case 12:  error = -2; break;
    case 8:   error = -1; break;
    case 24:  error =  0; break;
    case 16:  error =  1; break;
    case 48:  error =  2; break;
    case 32:  error =  3; break;
    case 96:  error =  4; break;
    case 64:  error =  5; break;
    case 192: error =  6; break;
    case 128: error =  7; break;
    default:  error = last_error;
  }
  return error;
}

void loop()
{    
    int LeftCommand = 0;
    int RightCommand = 0;
    
    uint8_t input = readsensors();
    
    int error = return_error(input);

    sum_error = sum_error + error;
    
    int power_difference = error * Kp + ( error - last_error) * Kd + sum_error * Ki;
    
    last_error = error;
    
    digitalWrite(left_motor_direction,LOW);
    digitalWrite(right_motor_direction,LOW);
    if(power_difference < 0){
        digitalWrite(left_motor_direction,HIGH);
        LeftCommand = 255;
        RightCommand = Base_Speed - 2*power_difference;
      }
     else if(power_difference > 0){
        digitalWrite(right_motor_direction,HIGH);
        RightCommand = 255;
        LeftCommand  = Base_Speed + 2*power_difference;
      }
      else if(power_difference == 0){
        RightCommand = Base_Speed;
        LeftCommand  = Base_Speed;
      }

    analogWrite(left_motor, LeftCommand);
    analogWrite(right_motor, RightCommand);
}
        

