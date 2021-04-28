/**
 * 该例程监听tf数据，并计算、发布the command of left-arm via pose control. 
 * 
 */

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
//发布海龟速度指令话题所需要的头文件
#include <yumi_ik/YumiGrasp.h>

int main(int argc, char** argv)
{
	// 初始化ROS节点
	ros::init(argc, argv, "yumi_pickup");

    // 创建节点句柄
	ros::NodeHandle node;

//发布/yumi/yumi_gripper/do_grasp请求关闭夹子，服务编程

	ros::service::waitForService("/yumi/yumi_gripper/do_grasp");
 //查询是否没有/yumi/yumi_gripper/do_grasp这个服务，如果没有就一直等待

 	ros::service::waitForService("/yumi/yumi_gripper/release_grasp");
 //查询是否没有/yumi/yumi_gripper/release_grasp这个服务，如果没有就一直等待
 
	ros::ServiceClient close_gripper = 
node.serviceClient<yumi_ik::YumiGrasp>("/yumi/yumi_gripper/do_grasp");
	yumi_ik::YumiGrasp srv_close;
//用ros::ServiceClient类，创建客户端close_gripper，
//用节点句柄node创建，句柄成员为serviceClient
//发送请求的数据类型是<yumi_ik::YumiGrasp>

    ros::ServiceClient release_gripper = 
node.serviceClient<yumi_ik::YumiGrasp>("/yumi/yumi_gripper/release_grasp");
	yumi_ik::YumiGrasp srv_release;
//用ros::ServiceClient类，创建客户端release_gripper，
//用节点句柄node创建，句柄成员为serviceClient
//发送请求的数据类型是<yumi_ik::YumiGrasp>

   srv_close.request.gripper_id = yumi_ik::YumiGrasp::Request::LEFT_GRIPPER;
   srv_release.request.gripper_id = yumi_ik::YumiGrasp::Request::LEFT_GRIPPER;

  //	close_gripper.call(srv_close);
//客户端close_gripper,向服务名字为"/yumi/yumi_gripper/do_grasp"服务发送请求
  //	release_gripper.call(srv_release);
//客户端release_gripper,向服务名字为"/yumi/yumi_gripper/release_grasp"服务发送请求


//topic是力控制相关——————————待修改——————————————
//rostopic pub /yumi/gripper_l_effort_cmd std_msgs/Float64 "data: effort_command"
/*
创建发布turtle2速度控制指令的发布者turtle_vel，话题编程
	ros::Publisher turtle_vel =
 node.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 10);
 */
//topic是力控制相关——————————待修改——————————————

//创建tf的监听器

	ros::Rate rate(100.0);
 //rate为while循环里的频率
	while (node.ok())
	{
		release_gripper.call(srv_release);
		rate.sleep();
		close_gripper.call(srv_close);
	}
	return 0;
};