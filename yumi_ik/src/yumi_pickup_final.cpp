/***********************************************************************
Copyright 2019 Wuhan PS-Micro Technology Co., Itd.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***********************************************************************/

#include "yumi_ik/yumi_pickup_final.h"

//init the gripper service
//listen the tf infomation
//move arm above the obj
YumiGraspingDemo::YumiGraspingDemo(ros::NodeHandle n_, float pregrasp_x, float pregrasp_y, float pregrasp_z) :
    armgroup("left_arm")
{
  this->nh_ = n_;
  try
  {
    this->tf_camera_to_robot.waitForTransform("/yumi_base_link", "/camera_color_optical_frame", ros::Time(0), ros::Duration(50.0));
  }
  catch (tf::TransformException &ex)
  {
    ROS_ERROR("[adventure_tf]: (wait) %s", ex.what());
    ros::Duration(1.0).sleep();
  }

  try
  {
    this->tf_camera_to_robot.lookupTransform("/yumi_base_link", "/camera_color_optical_frame", ros::Time(0), (this->camera_to_robot_));
  }

  catch (tf::TransformException &ex)
  {
    ROS_ERROR("[adventure_tf]: (lookup) %s", ex.what());
  }
  
  this->pregrasp_x = pregrasp_x;
  this->pregrasp_y = pregrasp_y;
  this->pregrasp_z = pregrasp_z;

  ros::AsyncSpinner spinner(1);
  spinner.start();
  ros::WallDuration(3.0).sleep();
  ROS_INFO_STREAM("Getting into the Grasping home Position....");
  //this->home_pose = armgroup.getCurrentPose().pose;
    this->home_pose.orientation.x = -0.98;
    this->home_pose.orientation.y = 0.20;
    this->home_pose.orientation.z = 0.02;
    this->home_pose.orientation.w = 0.01;

    this->home_pose.position.x = 0.521;
    this->home_pose.position.y = 0.216;
    this->home_pose.position.z = 0.323;
    movetoPosition(home_pose.position.x, home_pose.position.y, home_pose.position.z,home_pose.orientation);

  ros::WallDuration(1.0).sleep();
  // Subscribe to input video feed and publish object location
  this->position_sub = nh_.subscribe("/aruco_tracker/position", 1, &YumiGraspingDemo::positionCallback,this);
  //image_sub_ = it_.subscribe("/probot_anno/camera/image_raw", 1, &GraspingDemo::imageCb, this);
}

//located the obj location 
//transform the position of obj base on robot
void YumiGraspingDemo::positionCallback(const geometry_msgs::Vector3Stamped& positionMsg)
{
  if (!this->grasp_running)
  {
    ROS_INFO_STREAM("Processing the Image to locate the Object...");
  
    double x = positionMsg.vector.x;
    double y = positionMsg.vector.y;
    double z = positionMsg.vector.z;

    this->obj_z = positionMsg.vector.z;

    std::cout<< " OBJ in X-Co-ordinate of Camera Frame :" << x << std::endl;
    std::cout<< " OBJ in Y-Co-ordinate of Camera Frame :" << y << std::endl;
    std::cout<< " OBJ in Z-Co-ordinate of Camera Frame :" << z << std::endl;
    this->obj_camera_frame.setZ(z);
    this->obj_camera_frame.setY(y);
    this->obj_camera_frame.setX(x);
    
    ROS_INFO_STREAM("Processing the transform...");
    this->obj_robot_frame = this->camera_to_robot_ * this->obj_camera_frame;
    ROS_INFO_STREAM("The transform is ok.");

    // double x1 = obj_robot_frame.getX();
    std::cout<< " OBJ in x-Co-ordinate of robot Frame :" << obj_robot_frame.getX()<< std::endl;
    std::cout<< " OBJ in y-Co-ordinate of robot Frame :" << obj_robot_frame.getY()<< std::endl;
    std::cout<< " OBJ in z-Co-ordinate of robot Frame :" << obj_robot_frame.getZ()<< std::endl;

    this->grasp_running = true;
  }
}

//move arm to a positon
void YumiGraspingDemo::movetoPosition(float x, float y, float z)
{
  // ROS_INFO("The attain position function called");

  // For getting the pose
  this->currPose = armgroup.getCurrentPose();

  this->target_pose1.orientation = currPose.pose.orientation;
  /*
  this->target_pose1.orientation.x = -0.98;
  this->target_pose1.orientation.y = 0.20;
  this->target_pose1.orientation.z = 0.02;
  this->target_pose1.orientation.w = 0.01;
  */

  this->target_pose1.position.x = x;
  this->target_pose1.position.y = y;
  this->target_pose1.position.z = z;
  armgroup.setPoseTarget(this->target_pose1);
  armgroup.move();
}

void YumiGraspingDemo::movetoPosition(float x, float y, float z, geometry_msgs::Quaternion ori)
{
  // ROS_INFO("The attain position function called");

  // For getting the pose
  this->currPose = armgroup.getCurrentPose();

  this->target_pose1.orientation = ori;
  /*
  this->target_pose1.orientation.x = -0.98;
  this->target_pose1.orientation.y = 0.20;
  this->target_pose1.orientation.z = 0.02;
  this->target_pose1.orientation.w = 0.01;
  */

  this->target_pose1.position.x = x;
  this->target_pose1.position.y = y;
  this->target_pose1.position.z = z;
  armgroup.setPoseTarget(this->target_pose1);
  armgroup.move();
}

//move with out depth axis
void YumiGraspingDemo::movetoPosition(float x, float y)
{
  // ROS_INFO("The attain position function called");

  // For getting the pose
  this->currPose = armgroup.getCurrentPose();

  this->target_pose1.orientation = currPose.pose.orientation;
  /*
  this->target_pose1.orientation.x = -0.98;
  this->target_pose1.orientation.y = 0.20;
  this->target_pose1.orientation.z = 0.02;
  this->target_pose1.orientation.w = 0.01;
  */

  this->target_pose1.position.x = x;
  this->target_pose1.position.y = y;
  this->target_pose1.position.z = currPose.pose.position.z;
  armgroup.setPoseTarget(this->target_pose1);
  armgroup.move();
}

//move arm go down in order to attain the obj
//gripper close
void YumiGraspingDemo::attainObject()
{
    // Open Gripper
  ros::WallDuration(1.0).sleep();
  this->release_gripper.call(this->srv_release);
  ros::WallDuration(2.0).sleep();
  ROS_INFO("move to above place");
  this->movetoPosition(obj_robot_frame.getX()+0.05, obj_robot_frame.getY());

  ROS_INFO("The arm is ready to go Down.");
  ros::WallDuration(2.0).sleep();
  //add an ocm.
      this->ocm.link_name = armgroup.getEndEffectorLink();
      this->ocm.header.frame_id = "/yumi_base_link";
      this->ocm.orientation = this->home_pose.orientation;
      this->ocm.absolute_x_axis_tolerance = 1;
      this->ocm.absolute_y_axis_tolerance = 1;
      this->ocm.absolute_z_axis_tolerance = 1;
      this->ocm.weight = 0.8;
      this->test_constraints.orientation_constraints.push_back(ocm);
      armgroup.setPathConstraints(test_constraints);
      this->movetoPosition(obj_robot_frame.getX()+0.05, obj_robot_frame.getY(), obj_robot_frame.getZ()+0.22);
      this->armgroup.clearPathConstraints();
}

void YumiGraspingDemo::grasp()
{
  ros::WallDuration(1.0).sleep();
  ROS_INFO("The Grasping function called");
  this->close_gripper.call(this->srv_close);
  ros::WallDuration(1.0).sleep();
}

//pick it into home
void YumiGraspingDemo::lift()
{
  // ROS_INFO("The lift function called");

  // For getting the pose
  this->currPose = armgroup.getCurrentPose();

  //this->target_pose1.orientation.x = -0.98;
  //this->target_pose1.orientation.y = 0.20;
  //this->target_pose1.orientation.z = 0.02;
  //this->target_pose1.orientation.w = 0.01;

  this->target_pose1.orientation = currPose.pose.orientation;
  this->target_pose1.position.x = currPose.pose.position.x;
  this->target_pose1.position.y = currPose.pose.position.y;
  this->target_pose1.position.z = home_pose.position.z;

  // Starting Postion after picking
  //target_pose1.position.z = target_pose1.position.z + 0.06;
  
  armgroup.setPoseTarget(target_pose1);
  armgroup.move();

  // Open Gripper
  ros::WallDuration(2.0).sleep();

  this->target_pose1.position.x = home_pose.position.x;
  this->target_pose1.position.y = home_pose.position.y;

  armgroup.setPoseTarget(target_pose1);
  armgroup.move();
}

//down and release the gripper
void YumiGraspingDemo::release()
{
  this->target_pose1.position.z = this->home_pose.position.z -0.1;
  armgroup.setPoseTarget(target_pose1);
  armgroup.move();
  ros::WallDuration(2.0).sleep();
  this->release_gripper.call(this->srv_release);
}

void YumiGraspingDemo::goHome()
{
  this->currPose = armgroup.getCurrentPose();
  // Go to Home Position
  this->movetoPosition(this->home_pose.position.x, this->home_pose.position.y, this->home_pose.position.z);
  ros::WallDuration(2.0).sleep();
}

void YumiGraspingDemo::initiateGrasping()
{
  ros::AsyncSpinner spinner(1);
  spinner.start();
  ros::WallDuration(1.0).sleep();

  ros::service::waitForService("/yumi/yumi_gripper/do_grasp");
 //查询是否没有/yumi/yumi_gripper/do_grasp这个服务，如果没有就一直等待

 	ros::service::waitForService("/yumi/yumi_gripper/release_grasp");
 //查询是否没有/yumi/yumi_gripper/release_grasp这个服务，如果没有就一直等待
 
	this->close_gripper = nh_.serviceClient<yumi_ik::YumiGrasp>("/yumi/yumi_gripper/do_grasp");
	
  //用ros::ServiceClient类，创建客户端close_gripper，
  //用节点句柄node创建，句柄成员为serviceClient
  //发送请求的数据类型是<yumi_ik::YumiGrasp>

  this->release_gripper = nh_.serviceClient<yumi_ik::YumiGrasp>("/yumi/yumi_gripper/release_grasp");
	  
  //用ros::ServiceClient类，创建客户端release_gripper，
  //用节点句柄node创建，句柄成员为serviceClient
  //发送请求的数据类型是<yumi_ik::YumiGrasp>

  this->srv_close.request.gripper_id = yumi_ik::YumiGrasp::Request::LEFT_GRIPPER;
  this->srv_release.request.gripper_id = yumi_ik::YumiGrasp::Request::LEFT_GRIPPER;
  //	close_gripper.call(srv_close);
  //客户端close_gripper,向服务名字为"/yumi/yumi_gripper/do_grasp"服务发送请求
  //	release_gripper.call(srv_release);
  //客户端release_gripper,向服务名字为"/yumi/yumi_gripper/release_grasp"服务发送请求
  this->grasp_running = false;
  
  ROS_INFO_STREAM("Approaching the Object....");
  this->attainObject();

  ROS_INFO_STREAM("Attempting to Grasp the Object now..");
  this->grasp();

  ROS_INFO_STREAM("Lifting the Object....");
  this->lift();

  ROS_INFO_STREAM("Lifting the Object....");
  this->release();

  ROS_INFO_STREAM("Going back to home position....");
  this->goHome();

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "simple_grasping");
  float pregrasp_x= 0.521;
  float pregrasp_y= 0.216; 
  float pregrasp_z= 0.323;
  ros::NodeHandle n;

  YumiGraspingDemo simGrasp(n, pregrasp_x, pregrasp_y, pregrasp_z);
  ROS_INFO_STREAM("Waiting for five seconds..");
  simGrasp.initiateGrasping();

  /*
  ros::WallDuration(5.0).sleep();
  while (ros::ok())
  {
    // Process image callback
    ros::spinOnce();

    simGrasp.initiateGrasping();
  }
  return 0;
  */
}
