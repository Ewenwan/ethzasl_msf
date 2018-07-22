/* 姿态 传感器 pose_sensor
 */
#include "pose_sensormanager.h"

int main(int argc, char** argv) {
  ros::init(argc, argv, "msf_pose_sensor");

  msf_pose_sensor::PoseSensorManager manager;

  ros::spin();

  return 0;
}
