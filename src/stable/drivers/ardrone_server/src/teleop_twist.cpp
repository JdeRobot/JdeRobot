/**
Software License Agreement (BSD)
\file      teleop_twist.cpp
\authors   Mani Monajjemi <mmonajje@sfu.ca>
\copyright Copyright (c) 2012, Autonomy Lab (Simon Fraser University), All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Autonomy Lab nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

modified by: Alberto Martín Florido <almartinflorido@gmail.com>
*/

#include "teleop_twist.h"
#include "utils/ardrone_date.h"

inline float max(float a, float b) { return a > b ? a : b; }
inline float min(float a, float b) { return a < b ? a : b; }

bool needs_takeoff = false;
bool needs_land = false;
bool needs_reset = false;

float old_left_right = -10.0;
float old_front_back = -10.0;
float old_up_down = -10.0;
float old_turn = -10.0;

int cam_state = DEFAULT_CAM_STATE; // 0 for forward and 1 for vertical, change to enum later
int set_navdata_demo_value = DEFAULT_NAVDATA_DEMO; 
int32_t detect_enemy_color = ARDRONE_DETECTION_COLOR_ORANGE_YELLOW;
int32_t detect_dtype = CAD_TYPE_MULTIPLE_DETECTION_MODE;
int32_t detect_hori_type = TAG_TYPE_MASK(TAG_TYPE_SHELL_TAG_V2);
int32_t detect_vert_type = TAG_TYPE_MASK(TAG_TYPE_BLACK_ROUNDEL);
int32_t detect_indoor_hull = 0;
int32_t detect_disable_placeholder = 0;
int32_t detect_enable_placeholder = 1;

jderobot::CMDVelDataPtr cmd_vel=new jderobot::CMDVelData();

const LED_ANIMATION_IDS ledAnimMap[14] = {
	BLINK_GREEN_RED, BLINK_GREEN, BLINK_RED, BLINK_ORANGE,
	SNAKE_GREEN_RED, FIRE, STANDARD, RED, GREEN, RED_SNAKE,BLANK,
	LEFT_GREEN_RIGHT_RED, LEFT_RED_RIGHT_GREEN, BLINK_STANDARD};


bool drone_toggleCam()
{
	const int _modes = 2;
	cam_state = (cam_state + 1) % _modes;
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT (video_channel, &cam_state, NULL);
	fprintf(stderr, "Setting camera channel to : %d.\n", cam_state);
	return true;
}


bool record_video(bool record)
{
	char record_command[ARDRONE_DATE_MAXSIZE + 64];
	int32_t new_codec;
	if(record) {
		char date[ARDRONE_DATE_MAXSIZE];
		time_t t = time(NULL);
		// For some reason the linker can't find this, so we'll just do it manually, cutting and pasting
		//    ardrone_time2date(t, ARDRONE_FILE_DATE_FORMAT, date);
		strftime(date, ARDRONE_DATE_MAXSIZE, ARDRONE_FILE_DATE_FORMAT, localtime(&t));
		snprintf(record_command, sizeof(record_command), "%d,%s", USERBOX_CMD_START, date);
		new_codec = MP4_360P_H264_720P_CODEC;
	} else {	
		snprintf(record_command, sizeof(record_command), "%d", USERBOX_CMD_STOP );
		new_codec = H264_360P_CODEC;
	}

	vp_os_mutex_lock(&twist_lock);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT (video_codec, &new_codec, NULL );
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT (userbox_cmd, record_command, NULL );
	vp_os_mutex_unlock(&twist_lock);

	return true;
}    

bool setLedAnimation(int type, float duration, float freq)
{
	LED_ANIMATION_IDS anim_id = ledAnimMap[type % 14]; // Don't trick me
	vp_os_mutex_lock(&twist_lock);
	ardrone_at_set_led_animation(anim_id, (float) fabs(freq), (uint32_t) abs(duration));
	vp_os_mutex_unlock(&twist_lock);

	return true;
}

bool setFlightAnimation(int type, float duration)
{
	char param[20];
	const int anim_type = type % ARDRONE_NB_ANIM_MAYDAY;
	const int anim_duration = (duration > 0) ? duration : MAYDAY_TIMEOUT[anim_type];
	snprintf(param, sizeof (param), "%d,%d", anim_type, anim_duration);
	vp_os_mutex_lock(&twist_lock);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(flight_anim, param, NULL);
	vp_os_mutex_unlock(&twist_lock);
	
	return true;
}

bool drone_flatTrim()
{
	vp_os_mutex_lock(&twist_lock);
	ardrone_at_set_flat_trim();
	vp_os_mutex_unlock(&twist_lock);
	fprintf(stderr, "\nFlat Trim Set.\n");
}

void setCMDVel(jderobot::CMDVelDataPtr cmd)
{
	vp_os_mutex_lock(&twist_lock);
		// Main 4DOF
		cmd_vel->linearX  = max(min(-cmd->linearX, 1.0), -1.0);
		cmd_vel->linearY  = max(min(-cmd->linearY, 1.0), -1.0);
		cmd_vel->linearZ  = max(min(cmd->linearZ, 1.0), -1.0);
		cmd_vel->angularZ = max(min(-cmd->angularZ, 1.0), -1.0);
		// These 2DOF just change the auto hover behaviour
		// No bound() required
		cmd_vel->angularX = cmd->angularX;
		cmd_vel->angularY = cmd->angularY; 		
	vp_os_mutex_unlock(&twist_lock);
}

void drone_land()
{
	vp_os_mutex_lock(&twist_lock);
	needs_land = true;
	vp_os_mutex_unlock(&twist_lock);
}

void drone_reset()
{	
	vp_os_mutex_lock(&twist_lock);	
	needs_reset = true;
	vp_os_mutex_unlock(&twist_lock);
}

void drone_takeoff()
{
	vp_os_mutex_lock(&twist_lock);
	needs_takeoff = true;
	vp_os_mutex_unlock(&twist_lock);
}
C_RESULT open_teleop(void)
{
	return C_OK;
}

C_RESULT update_teleop(void)
{
	// This function *toggles* the emergency state, so we only want to toggle the emergency
	// state when we are in the emergency state (because we want to get out of it).
	vp_os_mutex_lock(&twist_lock);
	if (needs_reset)
	{
		ardrone_tool_set_ui_pad_select(1);
		needs_reset = false;
	}
	else if (needs_takeoff)
	{
		ardrone_tool_set_ui_pad_start(1);
		needs_takeoff = false;
	}
	else if (needs_land)
	{
		ardrone_tool_set_ui_pad_start(0);
		needs_land = false;
	}
	else
	{

		float left_right = static_cast<float>(cmd_vel->linearY);
		if(std::fabs(left_right) == 0) //for negatives values closed to 0
			left_right = 0;
		float front_back = static_cast<float>(cmd_vel->linearX);
		if(std::fabs(front_back) == 0) //for negatives values closed to 0
			front_back = 0;		
		float up_down = static_cast<float>(cmd_vel->linearZ);
		if(std::fabs(up_down) == 0) //for negatives values closed to 0
			up_down = 0;		
		float turn = static_cast<float>(cmd_vel->angularZ);
		if(std::fabs(turn) == 0) //for negatives values closed to 0
			turn = 0;		

		bool is_changed = !(
			(fabs(left_right - old_left_right) < _EPS) && 
			(fabs(front_back - old_front_back) < _EPS) && 
			(fabs(up_down - old_up_down) < _EPS) && 
			(fabs(turn - old_turn) < _EPS)
			);

		// These lines are for testing, they should be moved to configurations
		// Bit 0 of control_flag == 0: should we hover?
		// Bit 1 of control_flag == 1: should we use combined yaw mode?

		int32_t control_flag = 0x00;
		int32_t combined_yaw = 0x00;

		// Auto hover detection based on ~0 values for 4DOF cmd_vel
		int32_t hover = (int32_t)
			(
			(fabs(left_right) < _EPS) && 
			(fabs(front_back) < _EPS) && 
			(fabs(up_down) < _EPS) && 
			(fabs(turn) < _EPS) &&
			// Set angular.x or angular.y to a non-zero value to disable entering hover
			// even when 4DOF control command is ~0
			(fabs(cmd_vel->angularX) < _EPS) &&
			(fabs(cmd_vel->angularY) < _EPS)
			);

		control_flag |= ((1 - hover) << 0);
		control_flag |= (combined_yaw << 1);
		//ROS_INFO (">>> Control Flag: %d", control_flag);

		old_left_right = left_right;
		old_front_back = front_back;
		old_up_down = up_down;
		old_turn = turn;
		//is_changed = true;
		if ((is_changed) || (hover))
		{
		   ardrone_tool_set_progressive_cmd(control_flag, left_right, front_back, up_down, turn, 0.0, 0.0);
		}

	}
	vp_os_mutex_unlock(&twist_lock);
	return C_OK;
}

C_RESULT close_teleop(void)
{
	return C_OK;
}

input_device_t teleop = {
	"Teleop",
	open_teleop,
	update_teleop,
	close_teleop
};


