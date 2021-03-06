#include <thread>

#include <ros/ros.h>

#include "segmapper/segmapper.hpp"
#include "map_msgs/SaveMap.h"
#include <unistd.h> 
#include <iostream>



int main(int argc, char **argv) {
  ros::init(argc, argv, "SegMapper");
  ros::NodeHandle node_handle("~");

  SegMapper mapper(node_handle);

  std::thread publish_map_thread(&SegMapper::publishMapThread, &mapper);
  std::thread publish_tf_thread(&SegMapper::publishTfThread, &mapper);
  std::thread segmatch_thread(&SegMapper::segMatchThread, &mapper);

  // ros::Subscriber control_sub = node_handle.subscribe("control", 10, controlCall);

  try {
    ros::spin();
  }
  catch (const std::exception& e) {
    ROS_ERROR_STREAM("Exception: " << e.what());
    return 1;
  }
  catch (...) {
    ROS_ERROR_STREAM("Unknown Exception");
    return 1;
  }

  publish_map_thread.join();
  publish_tf_thread.join();
  segmatch_thread.join();

  return 0;
}
