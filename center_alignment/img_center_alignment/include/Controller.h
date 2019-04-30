/*
 * Controller.h
 * Copyright (C) 2019 theo <theo@not-arch-linux>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <std_msgs/UInt8.h>
#include <img_center_alignment/GatePredictionMessage.h>
#include <img_center_alignment/PIDConfig.h>
#include <math.h>
#include <list>

#include "PID.h"


// TODO: Read from config
#define DETECTION_RATE 10 // 100 / 10 = 10Hz
#define IMG_WIDTH 340
#define IMG_HEIGHT 255
#define NB_WINDOWS 25
#define CROSSING_TIME 5
#define MAX_GATE_HEIGHT 100
#define PREVIOUS_PREDICTIONS_CNT 5

typedef enum {
	LANDED,
	TAKEOFF,
	AIMING,
	REFINING,
	FLYING,
	CROSSING,
	LEAVING,
	LANDING
} State;

typedef int* Vector3Ptr;

using namespace img_center_alignment;

class Controller {
	public:
		Controller(gain_param k_x, gain_param k_y, float z_velocity,
				int filter_window_size);
		~Controller();
		void Run();
	private:
		PID *PIDBoy;
		State state;
		Vector3d current_velocity;
		ros::NodeHandle handle;
		ros::Subscriber subHeightSensor;
		ros::Subscriber subPredictor;
		ros::Subscriber subVelocity;
		ros::Publisher pubVelocity;
		ros::Publisher pubFilteredWindow;
		dynamic_reconfigure::Server<PIDConfig> dynRcfgServer;
		float altitude;
		int gate_region;
		int rate;
		int filter_window_size;
		std::list<int> filter_window;
		void HeightSensorCallback(const Vector3Ptr &msg);
		void GatePredictionCallback(const GatePredictionMessage &msg);
		void CurrentVelocityCallback(geometry_msgs::TwistStampedConstPtr msg);
		void PublishVelocity(Vector3d velocity);
		void PublishVelocity(float yawVelocity);
		void DynamicReconfigureCallback(PIDConfig &cfg, uint32_t level);
		int FilterPrediction(int prediction);
		Vector3d ComputeGateCenter();
};

#endif /* !CONTROLLER_H */
