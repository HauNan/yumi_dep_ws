#include <string>
#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "yumi_ik_demo");
    ros::AsyncSpinner spinner(1);
    spinner.start();

    static const std::string PLANNING_GROUP = "left_arm";

    moveit::planning_interface::MoveGroupInterface arm(PLANNING_GROUP);


    const robot_state::JointModelGroup* joint_model_group =
      arm.getCurrentState()->getJointModelGroup(PLANNING_GROUP);

    //获取终端link的名称
    std::string end_effector_link = arm.getEndEffectorLink();

    //设置目标位置所使用的参考坐标系
    std::string reference_frame = "/yumi_base_link";
    arm.setPoseReferenceFrame(reference_frame);

    //当运动规划失败后，允许重新规划
    arm.allowReplanning(true);

    //设置位置(单位：米)和姿态（单位：弧度）的允许误差
    arm.setGoalPositionTolerance(0.1);
    arm.setGoalOrientationTolerance(0.1);

    //设置允许的最大速度和加速度
    arm.setMaxAccelerationScalingFactor(0.2);
    arm.setMaxVelocityScalingFactor(0.2);

    // 控制机械臂先回到初始化位置
    //arm.setNamedTarget("home");
    //arm.move();
    sleep(1);

    moveit_msgs::OrientationConstraint ocm;
      ocm.link_name = "gripper_l_base";
      ocm.header.frame_id = "/yumi_base_link";
      ocm.orientation.w = 0.1;
      ocm.absolute_x_axis_tolerance = 1;
      ocm.absolute_y_axis_tolerance = 1;
      ocm.absolute_z_axis_tolerance = 1;
      ocm.weight = 1.0;

    moveit_msgs::Constraints test_constraints;
        test_constraints.orientation_constraints.push_back(ocm);
        arm.setPathConstraints(test_constraints);

    robot_state::RobotState start_state(*arm.getCurrentState());

    geometry_msgs::Pose target_pose;
    target_pose.orientation.x = 0.005;
    target_pose.orientation.y = 0.002;
    target_pose.orientation.z = 0.004;
    target_pose.orientation.w = 1;

    target_pose.position.x = 0.305;
    target_pose.position.y = 0.400;
    target_pose.position.z = 0.034;

        start_state.setFromIK(joint_model_group, target_pose);
        arm.setStartState(start_state);

    // 设置机器臂当前的状态作为运动初始状态
    arm.setStartStateToCurrentState();

    arm.setPoseTarget(target_pose);

    arm.setPlanningTime(10.0);

    // 进行运动规划，计算机器人移动到目标的运动轨迹，此时只是计算出轨迹，并不会控制机械臂运动
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    moveit::planning_interface::MoveItErrorCode success = arm.plan(plan);

    ROS_INFO("Plan (pose goal) %s",success?"":"FAILED");   

    //让机械臂按照规划的轨迹开始运动。
    if(success)
      arm.execute(plan);
    sleep(1);

    // 控制机械臂先回到初始化位置
    //arm.setNamedTarget("home");
    //arm.move();
    sleep(1);

    ros::shutdown(); 

    return 0;
}
