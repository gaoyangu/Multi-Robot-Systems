#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>

#include <string>

using namespace std;

#define MIN_SCAN_ANGLE_RAD -45.0/180*M_PI
#define MAX_SCAN_ANGLE_RAD +45.0/180*M_PI

bool obstacleFound = false;

void readSensorCallback(const sensor_msgs::LaserScan::ConstPtr &sencor_msg);

int main(int argc, char **argv)
{
    if(argc < 2){
        ROS_ERROR("You must specify robot id.");
        return -1;
    }
    char *robot_name = argv[1];

    ROS_INFO("Moving robot %s", robot_name);
    ros::init(argc, argv, "random_walk");
    ros::NodeHandle nh;

    string cmd_vel_topic_name = robot_name;
    cmd_vel_topic_name += "/cmd_vel";
    ros::Publisher cmd_vel_pub = nh.advertise<geometry_msgs::Twist>(cmd_vel_topic_name, 10);

    string laser_topic_name =  robot_name;
    laser_topic_name += "/scan";
    ros::Subscriber base_scan_sub = nh.subscribe<sensor_msgs::LaserScan>(laser_topic_name, 1, &readSensorCallback);

    geometry_msgs::Twist moveForwardCommand;
    moveForwardCommand.linear.x = 0.2;

    geometry_msgs::Twist turnCommand;
    turnCommand.angular.z = 0.3;

    ros::Rate loop_rate(10.0);
    
    while (ros::ok()){
        if(obstacleFound){
            ROS_INFO("Turning around");
            cmd_vel_pub.publish(turnCommand);
        }
        else{
            ROS_INFO("Moving forward");
            cmd_vel_pub.publish(moveForwardCommand);
        }
        // let ROS process incoming messages
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}

void readSensorCallback(const sensor_msgs::LaserScan::ConstPtr &scan){
    bool isObstacle = false;
    int minIndex = ceil(MIN_SCAN_ANGLE_RAD - scan->angle_min) / scan->angle_increment;
    int maxIndex = floor(MAX_SCAN_ANGLE_RAD - scan->angle_min) / scan->angle_increment;

    for(int i = minIndex; i < maxIndex; i++){
        if(scan->ranges[i] < 0.5){
            isObstacle = true;
        }
    }
    if(isObstacle){
        ROS_INFO("Obstacle in front of you!");
        obstacleFound = true;
    }
    else{
        obstacleFound = false;
    }
}