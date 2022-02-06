#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <robot_msgs/motor_srv.h>
#include <pthread.h>
#include <mutex>
using namespace cv;

int line_ = -1;
int sector = 0;
bool mode = false;
VideoCapture cap(6, CAP_V4L);
Mat frame;
std::mutex m;
/*bool count_callback(robot_msgs::capture::Request& req, robot_msgs::capture::Response& res)
{
    line_++;
    sector = 0;
    ROS_INFO("line change %d", line);
    if(req.flag == true)
        mode = true;
    else
        mode = false;
    return true;
}*/

void *threadCB(void *data)
{
    while(ros::ok())
    {
        m.lock();
        cap >> frame;
        m.unlock();
        waitKey(1);
    }
}

bool count_callback(robot_msgs::motor_srv::Request& req, robot_msgs::motor_srv::Response& res)
{
    sector = 0;
    ROS_INFO("line change %d", line);
    if(req.direction == 4){
        mode = true;
        line_++;
    }
    else
        mode = false;
    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "capture");
    ros::NodeHandle nh;
    ros::ServiceServer service = nh.advertiseService("capture", count_callback);
    //VideoCapture cap(0, CAP_INTEL_MFX);
    pthread_t th;
    pthread_create(&th, NULL, threadCB, NULL);
    Mat image;
    if(!cap.isOpened()){
        ROS_INFO("fail");
        return -1;}
    
    ros::Rate r(1);
    
    while(ros::ok()){
        if(mode){
            m.lock();
            frame.copyTo(image);
            m.unlock();
            std::stringstream ss;
            if(sector<10)
                ss<<"/home/jo/image/"<<std::to_string(line_)<<",0"<<std::to_string(sector)<<".jpg";
            else
                ss<<"/home/jo/image/"<<std::to_string(line_)<<","<<std::to_string(sector)<<".jpg";
            imwrite(ss.str(), frame);
            ROS_INFO("save image %d %d", line_, sector);
            sector++;
        r.sleep();
        }
	    else ROS_INFO("mode false");
        ros::spinOnce();
    }
    return 0;
}
