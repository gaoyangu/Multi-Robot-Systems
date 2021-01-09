// ros includes 
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "multi_sync/RobotStatus.h"

// std includes
#include <string>
using namespace std;

#define MAX_ROBOTS_NUM 20
#define FORWARD_SPEED 0.2

int robot_id;
ros::Publisher cmd_vel_pub;
ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;
bool teamReady = false;

void publishReadyStatus();
void waitForTeam();
void move_forward();
void teamStatusCallback(const multi_sync::RobotStatus::ConstPtr& status_msgs);

int main(int argc, char **argv)
{
    if(argc < 2){
        ROS_ERROR("You must specify robot id.");
        return -1;
    }
    char *id = argv[1];
    robot_id = atoi(id);

    //Check that robot id is between 0 and MAX_ROBOTS_NUM
    if(robot_id > MAX_ROBOTS_NUM || robot_id < 0){
        ROS_ERROR("The robot's ID must be an integer number between 0 and 19");
        return -1;
    }
    ROS_INFO("moving robot no. %d", robot_id);

    //Creat a unique node name
    string node_name = "move_robot_";
    node_name += id;
    cout << node_name;

    ros::init(argc, argv, node_name);
    ros::NodeHandle nh;

    //cmd_vel_topic = "robot_X/cmd_vel"
    string cmd_vel_topic_name = "robot_";
    cmd_vel_topic_name += id;
    cmd_vel_topic_name += "/cmd_vel";

    cmd_vel_pub = nh.advertise<geometry_msgs::Twist>(cmd_vel_topic_name, 10);

    team_status_pub = nh.advertise<multi_sync::RobotStatus>("team_status", 10);
    team_status_sub = nh.subscribe("team_status", 1, &teamStatusCallback);

    publishReadyStatus();
    waitForTeam();

    move_forward();
    return 0;
}

void publishReadyStatus(){
    multi_sync::RobotStatus status_msgs;
    status_msgs.header.stamp = ros::Time::now();
    status_msgs.robot_id = robot_id;
    status_msgs.is_ready = true;

    //Wait for the publisher to conect to subsribers
    sleep(1.0);
    team_status_pub.publish(status_msgs);
    ROS_INFO("Robot %d publish ready status", robot_id);
}

void waitForTeam(){
    ros::Rate loopRate(1);
    //Wait until all robots are ready...
    while (!teamReady){
        ROS_INFO("Robot %d: waiting for team", robot_id);
        ros::spinOnce();
        loopRate.sleep();
    }
}

void teamStatusCallback(const multi_sync::RobotStatus::ConstPtr& status_msgs){
    //check if message came from monitor
    if(status_msgs->header.frame_id == "monitor"){
        ROS_INFO("Robot %d: Team is ready. Let's move!", robot_id);
        teamReady = true;
    }
}

void move_forward(){
    // Drive forward at a given speed.
    geometry_msgs::Twist cmd_vel;
    cmd_vel.linear.x = FORWARD_SPEED;
    cmd_vel.angular.z = 0.0;

    // Loop at 10Hz, publishing movement commands until we shut down 
    ros::Rate rate(10);

    while (ros::ok()){
        cmd_vel_pub.publish(cmd_vel);
        rate.sleep();
    }

    //Stop the robot
    geometry_msgs::Twist stop_cmd_vel;
    stop_cmd_vel.linear.x = 0.0;
    stop_cmd_vel.angular.z = 0.0;
    cmd_vel_pub.publish(stop_cmd_vel);
    ROS_INFO("robot no. %d stopped", robot_id);
}