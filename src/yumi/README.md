# ROS packages for the ABB YuMi (IRB14000) robot



For documentation, and instructions on how to install and employ this package, check [our wiki](https://github.com/kth-ros-pkg/yumi/wiki).


[moveit]
roslaunch yumi_moveit_config demo_online.launch
[关闭夹爪]（gaz/real通用）
rosservice call /yumi/yumi_gripper/do_grasp "gripper_id: 1"
rosservice call /yumi/yumi_gripper/do_grasp "gripper_id: 2"

[仿真A gazebo_traj_pos]	
roslaunch yumi_launch yumi_gazebo_traj_pos_control.launch	
[YAML]
name: yumi/joint_traj_pos_controller_l
[备注]
1.运动组控制


[仿真B gazebo_traj_vel]
roslaunch yumi_launch yumi_gazebo_traj_vel_control.launch
[gazebo-launch-arm_controllers]
joint_traj_vel_controller_l
joint_traj_vel_controller_r
[moveit.config-YAML]
name: yumi/joint_traj_vel_controller_l
name: yumi/joint_traj_vel_controller_r
[问题]
1. 删除了yumi_control/traj_controllers && yumi_control/velocity_controllers里面的gazebo_ros_control/pid_gains才可以用。


[仿真C gazebo_vel]
roslaunch yumi_launch yumi_gazebo_vel_control.launch
[gazebo-launch-arm_controllers]
yumi/joint_vel_controller_1_r
yumi/joint_vel_controller_2_r
......
[moveit.config-YAML]

[仿真D gazebo_pos]
roslaunch yumi_launch yumi_gazebo_pos_control.launch

[rqt_gui]
rosrun rqt_gui rqt_gui 
matplot监视数据：
1.数组监视
/yumi/joint_states/position[2]
2.topic直监
/yumi/joint_pos_controller_1_l/command/data

rosrun rqt_controller_manager rqt_controller_manager --force-discover
可以看现有的控制器（moveit链接控制器，moveit-config配置的是要链接的控制器）


[标定信息发布方法 tf.static_transform_publisher]：
先做手眼标定，标好直接发布整体信息。
static_transform_publisher x y z yaw pitch roll frame_id child_frame_id period_in_ms
eg：
  <node pkg="tf" type="static_transform_publisher" name="realsense_link_broadcaster"
        args="0.05425 -0.024 -0.1258 3.14 1.32 0 /m1n6s200_link_6 /camera_link 10" />

joint_traj

[问题]
Interactive marker 'EE:goal_yumi_link_7_l' contains unnormalized quaternions. This warning will only be output once but may be true for others; enable DEBUG messages for ros.rviz.quaternions to see more details.

打开：moveit.rviz
修改：Interactive Marker Size: 0
变为：Interactive Marker Size: 0.25
不能解决

[问题]
夹爪是如何控制的:
/yumi/gripper_states
/yumi/gripper_r_effort_cmd
/yumi/gripper_r_effort_controller_r/command


