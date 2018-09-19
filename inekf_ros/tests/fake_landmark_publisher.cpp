#include "ros/ros.h"
#include "inekf_msgs/LandmarkStamped.h"
#include <random>

using namespace std;

/**
 * Publishes fake landmark data over the /landmarks topic
 */
int main(int argc, char **argv) {
    // Initialize ROS
    ros::init(argc, argv, "fake_landmark_publisher");
    ros::NodeHandle n;
    // Create Fake landmark publisher
    ros::Publisher imu_pub = n.advertise<inekf_msgs::LandmarkStamped>("/landmarks", 1000);

    // Specify publishing frequency
    ros::NodeHandle nh("~");
    float rate = 1;
    nh.getParam("rate", rate);
    cout << "Publishing landmarks at " << rate << " Hz." << endl;
    ros::Rate loop_rate(rate);

    // Initialize random number generator
    default_random_engine generator;
    double landmark_std = 0.1;
    nh.getParam("noise/landmark_std", landmark_std);
    cout << "Landmark noise std: " << landmark_std << endl;
    normal_distribution<double> landmark_noise(0,landmark_std);

    uint32_t seq = 0;
    while (ros::ok()) {
        // Construct landmark message
        inekf_msgs::LandmarkStamped msg;

        msg.header.seq = seq;
        msg.header.stamp = ros::Time::now();
        msg.header.frame_id = "/imu"; 

        msg.landmark.id = 0;
        msg.landmark.position.x = 0 + landmark_noise(generator);
        msg.landmark.position.y = 0 + landmark_noise(generator);
        msg.landmark.position.z = 0 + landmark_noise(generator);

        // Send message
        imu_pub.publish(msg);

        // Spin and sleep
        ros::spinOnce();
        loop_rate.sleep();
        seq++;
    }


    return 0;
}