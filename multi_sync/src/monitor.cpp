// ros includes
#include <ros/ros.h>
#include "multi_sync/RobotStatus.h"

#define MAX_ROBOTS_NUM 20

unsigned int teamSize;
unsigned int robotsCount = 0;
bool robotsReady[MAX_ROBOTS_NUM];

ros::Subscriber team_status_sub;
ros::Publisher team_status_pub;

void teamStatusCallback(const multi_sync::RobotStatus::ConstPtr& status_msg);

int main(int argc, char **argv)
{
    if(argc < 2){
        ROS_ERROR("You must specify team size.");
        return -1;
    }
    char *teamSizeStr = argv[1];
    teamSize = atoi(teamSizeStr);

    //check that robot id is between 0 and MAX_ROBOTS_NUM
    if(teamSize > MAX_ROBOTS_NUM || teamSize < 0){
        ROS_ERROR("The team size must be an integer number between 1 and %d", MAX_ROBOTS_NUM);
        return -1;
    }

    ros::init(argc, argv, "monitor");
    ros::NodeHandle nh;
    team_status_pub = nh.advertise<multi_sync::RobotStatus>("team_status", 10);
    team_status_sub = nh.subscribe("team_status", 1, &teamStatusCallback);

    ROS_INFO("wait for robots to connect...");
    ros::spin();
}

void teamStatusCallback(const multi_sync::RobotStatus::ConstPtr& status_msg){
    int robot_id = status_msg->robot_id;

    //if (!robotsReady[robot_id])
    if (status_msg->is_ready){
        ROS_INFO("Robot %d id ready!\n", robot_id);
        robotsReady[robot_id] = true;
        robotsCount++;
        if(robotsCount == teamSize){
            ROS_INFO("All robots Go!");
            multi_sync::RobotStatus status_msg;
            status_msg.header.stamp = ros::Time::now();
            status_msg.header.frame_id = "monitor";
            team_status_pub.publish(status_msg);
        }
    }
}