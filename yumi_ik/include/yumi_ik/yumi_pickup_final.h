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

#ifndef YUMI_PICKUP_FINAL
#define YUMI_PICKUP_FINAL

#include <ros/ros.h>
//#include <image_transport/image_transport.h>
//#include <sensor_msgs/image_encodings.h>
#include <geometry_msgs/PoseStamped.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
//#include <moveit_visual_tools/moveit_visual_tools.h>
//#include <cv_bridge/cv_bridge.h>
#include <tf/transform_listener.h>
#include <yumi_ik/YumiGrasp.h>

//#include "probot_grasping/vision_manager.h"

class YumiGraspingDemo
{
  private:
	/**
 	 * @brief NodeHandle of the current node
 	 */
	ros::NodeHandle nh_;
	ros::Subscriber position_sub;
	moveit_msgs::OrientationConstraint ocm;
	moveit_msgs::Constraints test_constraints;
	/**
     * @brief target_pose_1 is the pose to moveit!
     */
	geometry_msgs::Pose target_pose1;
	geometry_msgs::Pose home_pose;
	double obj_z;
	geometry_msgs::PoseStamped currPose;
	/**
     * @brief armgroup moveit interface for arm
     */
	moveit::planning_interface::MoveGroupInterface armgroup;
	/**
     * @brief grippegroup moveit interface for gripper
     */
	// moveit::planning_interface::MoveGroupInterface grippergroup;
	/**
     * @brief it_ takes care of message to image conversion
     */
	// image_transport::ImageTransport it_;
	/**
     * @brief image_sub_ subscribes to image/raw topic
     */
	   //image_transport::Subscriber image_sub_;

	yumi_ik::YumiGrasp srv_close;
	yumi_ik::YumiGrasp srv_release;

	ros::ServiceClient close_gripper;
	ros::ServiceClient release_gripper;
	/**
     * @brief boolean to control the grasping movements
     */

	bool grasp_running;
	/**
     * @brief cv_ptr is the pointer to image as received by it_
     */

	// cv_bridge::CvImagePtr cv_ptr;
	/**
     * @brief vMng_ is the instance of the library for object detection
     */

	//VisionManager vMng_;
	/**
	 * @brief camera_to_robot takes care of the transformation from camera to robot frame
	 */

	tf::StampedTransform camera_to_robot_;
	/**
	 * @brief tf_camera_to_robot is an instance of tf_listener
	 */

	tf::TransformListener tf_camera_to_robot;
	/**
	 * @brief obj_camera_frame, obj_robot_frame are instance of tf::Vector
	 */

	tf::Vector3 obj_camera_frame ,obj_robot_frame;
	/**
	 * @brief homePose is StampedPose keeping Home position of arm
	 */

	geometry_msgs::PoseStamped homePose;
	/**
	 * @brief my_plan is an instance of moveit! planning interface
	 */

	moveit::planning_interface::MoveGroupInterface::Plan my_plan;
	/**
	 * @brief pregrasp_x, pregrasp_y and pregrasp_z is the pregrasp position of arm
	 */

	float pregrasp_x, pregrasp_y, pregrasp_z;
	/**
	 * @brief      attainPosition achieved the given position of the arm
	 *
	 * @param[in]  x     x-position of gripping frame	
	 * @param[in]  y     y-position of gripping frame	
	 * @param[in]  z     z-position of gripping frame
	 */

	void movetoPosition(float x, float y);
	void movetoPosition(float x, float y, float z);
	void movetoPosition(float x, float y, float z,geometry_msgs::Quaternion ori);
	void release();
	/**
	 * @brief      attainObject tries to get the gripper next to object
	 */
	void attainObject();
	/**
	 * @brief      grasp executes the grasping action
	 */
	void grasp();
	/**
	 * @brief      lift attempts to lift the object
	 */
	void lift();

  public:
	/**
 	 * @brief      GraspingDemo behaviour Constructor
 	 *
 	 * @param[in]  n_          ros_NodeHandle
 	 * @param[in]  pregrasp_x  Desired PregraspingX
 	 * @param[in]  pregrasp_y  Desired PregraspingY
 	 * @param[in]  pregrasp_z  Desired PregraspingZ
 	 * @param[in]  length      The length of table
 	 * @param[in]  breadth     The breadth of table
 	 */
	YumiGraspingDemo(ros::NodeHandle n_, float pregrasp_x, float pregrasp_y, float pregrasp_z);
	/**
	 * @brief      imageCb is called when a new image is received from the camera
	 *
	 * @param[in]  msg   Image received as a message
	 */
	//void imageCb(const sensor_msgs::ImageConstPtr &msg);

	void positionCallback(const geometry_msgs::Vector3Stamped& positionMsg);
	/**
	 * @brief      initiateGrasping initiates the grasping behaviour
	 */
	void initiateGrasping();
	/**
	 * @brief      Function brings the arm back to home configuration
	 */
	void goHome();

};

#endif