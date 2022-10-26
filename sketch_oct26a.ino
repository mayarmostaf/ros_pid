#include <ros.h>
#include <std_msgs/Float32.h>// we put after / the type of message , here its 
ros::NodeHandle n;//allows our program to create publishers and subscribers , from it we create our suband pub.
//control
//prev speed
 float p_speed;
//target speed
float t_speed;
//speed pin 
int motor_speed_pin=19;
int16_t motor_speed=0;
//kp,ki,kd
int kp=5;
int ki=5;
int kd=3;

//callback func
void messageCb(const std_msgs::Float32& msg)
{
  t_speed=msg.data;
 
}
//subscriber
ros::Subscriber<std_msgs::Float32> speed_sub("speed", &messageCb);
//control
void control_speed(float t_speed){
int16_t error=t_speed-p_speed;
int16_t integ=0;
int16_t last_error=0;
int16_t last_time=0;
while((error>=-2)&&(error<=2)){
 int16_t new_error = t_speed - p_speed;//current error measuring
 int16_t new_time=millis();//currnt time measuring
 int16_t delt_e=new_error-last_error;//calculating delta error(change in error)
 int16_t delt_t=new_time-last_time;//calculating delta time(change in time)
 integ+=(new_error*delt_t);//calculate accumulated integration
 p_speed=kp*new_error+ki*integ+kd*(delt_e/delt_t);
 //we should first use constrain before map to constrain speed to the known  max speed of motor,and also replace 32767 in the map with this value .  
  motor_speed = map((unsigned)p_speed, 0, 32767, 0, 5);
  analogWrite(motor_speed_pin,motor_speed);
  last_error =new_error;//updating error
  last_time=new_time;//updating time 
  }
  }
void setup() {
  //ros sub
n.initNode();
n.subscribe(speed_sub);
  // put your setup code here, to run once:
pinMode(motor_speed, OUTPUT);
}

void loop() {
 n.spinOnce();
}
