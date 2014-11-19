#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <boost/thread/thread.hpp>
#include "strands_movebase/noise_approximate_voxel_grid.h"

ros::Publisher pub;

void callback(const sensor_msgs::PointCloud2::ConstPtr& msg)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::fromROSMsg(*msg, *cloud);
	pcl::PointCloud<pcl::PointXYZ> voxel_cloud;
	
    noise_approximate_voxel_grid sor(5, 20); // a bit faster than nvg, not as accurate
	sor.setInputCloud(cloud);
    sor.setLeafSize(0.05f, 0.05f, 0.05f);
	sor.filter(voxel_cloud);

	/*pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor; // another possibility, slow
	sor.setInputCloud(new_cloud);
	sor.setMeanK(20);
	sor.setStddevMulThresh(1.0);
	sor.filter(voxel_cloud);*/

	sensor_msgs::PointCloud2 msg_cloud;
    pcl::toROSMsg(voxel_cloud, msg_cloud);
	msg_cloud.header = msg->header;

	pub.publish(msg_cloud);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "subsample_cloud");
	ros::NodeHandle n;
	
	ros::NodeHandle pn("~");
    // topic of input cloud
    if (!pn.hasParam("input")) {
        ROS_ERROR("Could not find parameter input.");
        return -1;
    }
    std::string input;
    pn.getParam("input", input);
    
    // topic of output cloud
    if (!pn.hasParam("output")) {
        ROS_ERROR("Could not find parameter output.");
        return -1;
    }
    std::string output;
    pn.getParam("output", output);
    
	ros::Subscriber sub = n.subscribe(input, 1, callback);
    pub = n.advertise<sensor_msgs::PointCloud2>(output, 1);
    
    ros::Rate rate(5); // updating at 5 hz, slightly faster than move_base
    while (n.ok()) {
        rate.sleep();
        ros::spinOnce();
    }
	
	return 0;
}
