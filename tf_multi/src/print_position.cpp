// ros includes 
#include <ros/ros.h>
#include <tf/transform_listener.h>

//boost includes
#include <boost/lexical_cast.hpp>

// std includes
#include <string>
using namespace std;

string tf_prefix;
//pair<double, double> getRobotPosition();
pair<double, double> getRobotPosition(int robot_no);

int main(int argc, char** argv)
{
    ros::init(argc, argv, "print_position");
    ros::NodeHandle nh;

    //get tf_prefix from the parameter server
    nh.getParam("tf_prefix", tf_prefix);

    pair<double, double> currPosition;
    ros::Rate loopRate(1);

    int team_size = 4;

    while (ros::ok()){
        for(int i = 0; i < team_size; i++){
            currPosition = getRobotPosition(i);
            ROS_INFO("Robot %d position:(%.3f, %.3f)", i, currPosition.first, currPosition.second);
        }
        ROS_INFO("----------------");
        loopRate.sleep();
    }
    return 0;
}

pair<double, double> getRobotPosition(int robot_no)
{
    tf::TransformListener listener;
    tf::StampedTransform transform;
    pair<double, double> currPosition;
    try{
        string robot_str = "/robot_";
        robot_str += boost::lexical_cast<string>(robot_no);
        string base_footprint_frame = tf::resolve(robot_str, "base_footprint");
        listener.waitForTransform("/map", base_footprint_frame, ros::Time(0), ros::Duration(10.0));
        listener.lookupTransform("/map", base_footprint_frame, ros::Time(0), transform);
        currPosition.first = transform.getOrigin().x();
        currPosition.second = transform.getOrigin().y();
    }
    catch(tf::TransformException &ex){
        ROS_ERROR("%s", ex.what());
    }
    return currPosition;
}

/*pair<double, double> getRobotPosition()
{
    tf::TransformListener listener;
    tf::StampedTransform transform;
    pair<double, double> currPosition;

    try{
        string base_footprint_frame = tf::resolve(tf_prefix, "base_footprint");
        listener.waitForTransform("/map", base_footprint_frame, ros::Time(0), ros::Duration(10.0));
        listener.lookupTransform("/map", base_footprint_frame, ros::Time(0), transform);

        currPosition.first = transform.getOrigin().x();
        currPosition.second = transform.getOrigin().y();
    }catch(tf::TransformException &ex){
        ROS_ERROR("%s", ex.what());
    }
    return currPosition;
}*/