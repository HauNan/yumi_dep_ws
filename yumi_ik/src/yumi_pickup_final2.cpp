/**
 * 该例程监听tf数据，并计算、发布the command of left-arm via pose control. 
 * 
 */

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Vector3Stamped.h>
//发布海龟速度指令话题所需要的头文件
#include <yumi_ik/YumiGrasp.h>
#include <moveit/move_group_interface/move_group_interface.h>
//cmake need to write

tf::StampedTransform transform;
//tf::Vector3 position_obj_robot, obj_camera_frame;
// moveit::planning_interface::MoveGroupInterface armgroup("left_arm");
//geometry_msgs::Pose target_pose1;

void positionCallback(const geometry_msgs::Vector3Stamped& positionMsg);
void attainPosition(float x, float y, float z);

void positionCallback(const geometry_msgs::Vector3Stamped& positionMsg)
//05.回调函数poseCallback，接收到订阅的消息后，产生中断，回调函数会被激活做处理
//输入参数是对camera_marker position的read only object,base on camera，类型为geometry_msgs::Vector3Stamped&，指针形参名为positionMsg
//常指针可读不可写
//positionMsg对象指针中有x和y这两个成员变量
{
    double x = positionMsg.vector.x;
    double y = positionMsg.vector.y;
    double z = positionMsg.vector.z;

     ROS_INFO("camera_marker position: x:%0.6f, y:%0.6f, z:%0.6f", positionMsg.vector.x, positionMsg.vector.y, positionMsg.vector.z);
    // 将接收到的消息打印出来，指针信息的调用
    //打印目前海龟所在的位置
  //  obj_camera_frame.setZ(z);
  //  obj_camera_frame.setY(y);
  //  obj_camera_frame.setX(x);
	// position_obj_robot = transform * obj_camera_frame;
}

/*
void attainPosition(float x, float y, float z)
{
  // ROS_INFO("The attain position function called");

  // For getting the pose
  geometry_msgs::PoseStamped currPose = armgroup.getCurrentPose();

  geometry_msgs::Pose target_pose1;
  target_pose1.orientation = currPose.pose.orientation;

  // Starting Postion before picking
  target_pose1.position.x = x;
  target_pose1.position.y = y;
  target_pose1.position.z = z;
  armgroup.setPoseTarget(target_pose1);

  armgroup.move();
}
*/

int main(int argc, char** argv)
{
	// 初始化ROS节点
	ros::init(argc, argv, "yumi_pickup_final");

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

// ros::Subscriber position_sub = node.subscribe("/aruco_tracker/position", 10, positionCallback);
    //03.创建一个Subscriber，订阅者名字是position_sub 
    //订阅名为"/aruco_tracker/position"的topic,its msg is: geometry_msgs::Vector3Stamped
    //订阅话题的队列长度是10，订阅者的接受能力不能确定，
    ////因此创建一个长度为10的缓冲区，保证最新10个数据的接受
    //绑定回调函数poseCallback


//topic是力控制相关——————————待修改——————————————
//rostopic pub /yumi/gripper_l_effort_cmd std_msgs/Float64 "data: effort_command"
/*
创建发布turtle2速度控制指令的发布者turtle_vel，话题编程
	ros::Publisher turtle_vel =
 node.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 10);
*/
//topic是力控制相关——————————待修改——————————————

//创建tf的监听器
/*
	tf::TransformListener listener;

		// 获取相机frame与yumi_base frame之间的tf数据
      //创建一个对象，，transform用于保存平移和旋转的关系
		try
		{
			listener.waitForTransform("yumi_base_link",
           "camera_color_optical_frame",ros::Time(0), ros::Duration(3.0));
   //等待变换waitForTransform
   //等待系统中是否存在yumi_base_link到camera_color_optical_frame的坐标变换
   //是，这句话才跳过
   //ros::Time(0)，查询当前时间是否存在变换，查询最新实时数据
   //ros::Duration(3.0)等待时间，等待3秒后提示超时报错
			listener.lookupTransform("yumi_base_link",
           "camera_color_optical_frame",ros::Time(0), transform);
   //查询变换lookupTransform
   //时间是当前的时间
   //结果保存到transform中去，包含两坐标系的平移与旋转（以向量形式保存）
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
		}
//订阅话题：    /aruco_tracker/position
//然后用pose的基座标来改变


// 根据turtle1与turtle2坐标系之间的位置关系，发布turtle2的速度控制指令
//（欧氏距离计算，让龟2动起来）
//  attainPosition(position_obj_robot.getX(), position_obj_robot.getY(), position_obj_robot.getZ() + 0.2);

   ros::WallDuration(1.0).sleep();
   //计算得到角速度angular和线速度linear，
   //0.5 均为时间系数t，希望海龟在2s完成这个距离，v=s/t
   //sqrt(pow(transform.getOrigin().x(), 2) +pow(transform.getOrigin().y(), 2)
    	//得到向量长度s
              //发布速度指令
	*/
	return 0;
};