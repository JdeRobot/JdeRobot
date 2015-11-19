//=================================================================================================
// Copyright (c) 2012, Johannes Meyer, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Flight Systems and Automatic Control group,
//       TU Darmstadt, nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================

/*
* quadrotor motion controller:
*
* This software is a fragment of original source
*
* Created on: Oct 22, 2012
* Author: Hongrong huang
*
* Adapted on: Nov 15, 2015
* Author: Victor Arribas
*
*/


#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H


#include <sdf/sdf.hh>


namespace quadrotor{

class PIDController {
public:
  PIDController();
  virtual ~PIDController();
  virtual void Load(sdf::ElementPtr _sdf, const std::string& prefix = "");

  double gain_p;
  double gain_i;
  double gain_d;
  double time_constant;
  double limit;

  double input;
  double dinput;
  double output;
  double p, i, d;

  double update(double input, double x, double dx, double dt);
  void reset();
};


typedef
struct Controllers {
  PIDController roll;
  PIDController pitch;
  PIDController yaw;
  PIDController velocity_x;
  PIDController velocity_y;
  PIDController velocity_z;
} Controllers_t;

}//NS

#endif // PIDCONTROLLER_H
