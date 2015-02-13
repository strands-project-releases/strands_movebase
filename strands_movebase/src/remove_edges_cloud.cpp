#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>

ros::Publisher pub; // publishes pointcloud with removed edges
double cutoff; // how many pixels to cut off in the depth image
double cutoff_z; // how much to cut off along the z axis, meters

void callback(const sensor_msgs::PointCloud2::ConstPtr& msg)
{
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::fromROSMsg(*msg, cloud);
	
	Eigen::Matrix3f K; // camera matrix of short range primesense
	K << 570.34f, 0.0f, 314.5f, 0.0f, 570.34f, 235.5f, 0.0f, 0.0f, 1.0f;
	
	pcl::PointCloud<pcl::PointXYZ> new_cloud;
	new_cloud.resize(cloud.size());
	Eigen::Vector3f p;
	int counter = 0;
	for (int i = 0; i < cloud.size(); ++i) {
	    // have a threshold for closeness as well
	    if (cloud.points[i].z < cutoff_z) {
	        continue;
	    }
	    // transform points to image plane and make sure they are within bounds
		p = K*cloud.points[i].getVector3fMap();
		p = p / p(2); // we don't have any points at z = 0
		if (p(0) > 0.0f + cutoff && p(0) < 629.0f - cutoff &&
			p(1) > 0.0f + cutoff && p(1) < 471.0f - cutoff) {
			new_cloud.points[counter] = cloud.points[i];
			++counter;
		}
	}
	new_cloud.resize(counter);

	sensor_msgs::PointCloud2 msg_cloud;
    pcl::toROSMsg(new_cloud, msg_cloud);
	msg_cloud.header = msg->header;
	pub.publish(msg_cloud);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "remove_edges_cloud");
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

    // how many pixels to cut off in the depth image
	if (!pn.hasParam("cutoff")) {
        ROS_ERROR("Could not find parameter cutoff.");
        return -1;
    }
    pn.getParam("cutoff", cutoff);
    
    if (!pn.hasParam("cutoff_z")) {
        ROS_ERROR("Could not find parameter cutoff_z.");
        return -1;
    }
    pn.getParam("cutoff_z", cutoff_z);
    
	ros::Subscriber sub = n.subscribe(input, 1, callback);
    pub = n.advertise<sensor_msgs::PointCloud2>(output, 1);
    
    ros::spin();
	
	return 0;
}
