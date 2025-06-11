#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
// tf chage https://zhuanlan.zhihu.com/p/340016739

// 功能​​：监听map到camera_init的TF变换，提取2D信息后广播新的map到camera_init_2d的TF变换
// ​​输入​​：TF变换(map→camera_init)
// ​​输出​​：新的TF变换(map→camera_init_2d)
// ​​特点​​：

//     使用TF系统
//     运行频率1000Hz
//     只保留XY位置和Z轴旋转
//     与文件1结构相似但处理不同坐标系


int main(int argc, char** argv){
  ros::init(argc, argv, "Trans_TF_2d_odom");

  ros::NodeHandle node;

  tf::TransformListener listener;
  tf::TransformBroadcaster broadcaster;
  tf::Transform transform_broadcaster;

  ros::Rate rate(1000);
  while (node.ok()){
    tf::StampedTransform transform_listener;
    
    try{
      listener.lookupTransform("map", "camera_init",  
                               ros::Time(0), transform_listener);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }
    float robot_pose_x=transform_listener.getOrigin().x();
    float robot_pose_y=transform_listener.getOrigin().y();
    float robot_pose_z=0;
    float robot_oriation_x=transform_listener.getRotation().getX();
    float robot_oriation_y=transform_listener.getRotation().getY();
    float robot_oriation_z=transform_listener.getRotation().getZ();
    float robot_oriation_w=transform_listener.getRotation().getW();

    transform_broadcaster.setOrigin( tf::Vector3(robot_pose_x, robot_pose_y, 0.0) );
    transform_broadcaster.setRotation( tf::Quaternion(0, 0, robot_oriation_z, robot_oriation_w) );
    broadcaster.sendTransform(tf::StampedTransform(transform_broadcaster, ros::Time::now(), "map", "camera_init_2d"));
    

    rate.sleep();
  }
  return 0;
};