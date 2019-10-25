// Copyright (c) 2019 AutonomouStuff, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef PACMOD3__PACMOD3_NODE_HPP_
#define PACMOD3__PACMOD3_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <rclcpp_lifecycle/lifecycle_node.hpp>
#include <can_msgs/msg/frame.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/float64.hpp>

#include <memory>
#include <string>

#include "pacmod3/pacmod3_common.hpp"

namespace lc = rclcpp_lifecycle;
using LNI = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface;

namespace AS
{
namespace Drivers
{
namespace PACMod3
{

/// \brief PACMod3Node class which can translate messages
/// being sent to or from a PACMod drive-by-wire system.
class PACMod3Node final
  : public lc::LifecycleNode
{
public:
  /// \brief Default constructor
  explicit PACMod3Node(rclcpp::NodeOptions options);

  /// \brief Callback from transition to "configuring" state.
  /// \param[in] state The current state that the node is in.
  LNI::CallbackReturn on_configure(const lc::State & state) override;

  /// \brief Callback from transition to "activating" state.
  /// \param[in] state The current state that the node is in.
  LNI::CallbackReturn on_activate(const lc::State & state) override;

  /// \brief Callback from transition to "deactivating" state.
  /// \param[in] state The current state that the node is in.
  LNI::CallbackReturn on_deactivate(const lc::State & state) override;
  
  /// \brief Callback from transition to "unconfigured" state.
  /// \param[in] state The current state that the node is in.
  LNI::CallbackReturn on_cleanup(const lc::State & state) override;
  
  /// \brief Callback from transition to "shutdown" state.
  /// \param[in] state The current state that the node is in.
  LNI::CallbackReturn on_shutdown(const lc::State & state) override;

private:
  void callback_can_tx(const can_msgs::msg::Frame::SharedPtr msg);
  void callback_accel_cmd(const pacmod_msgs::msg::SystemCmdFloat::SharedPtr msg);
  void callback_brake_cmd(const pacmod_msgs::msg::SystemCmdFloat::SharedPtr msg);
  void callback_shift_cmd(const pacmod_msgs::msg::SystemCmdInt::SharedPtr msg);
  void callback_steer_cmd(const pacmod_msgs::msg::SteerSystemCmd::SharedPtr msg);
  void callback_turn_cmd(const pacmod_msgs::msg::SystemCmdInt::SharedPtr msg);

  VehicleType vehicle_type_;
  std::string frame_id_;

  std::shared_ptr<lc::LifecyclePublisher<can_msgs::msg::Frame>> pub_can_rx_;
  std::unordered_map<unsigned int, std::shared_ptr<lc::LifecyclePublisherInterface>> can_pubs_;
  std::shared_ptr<lc::LifecyclePublisher<std_msgs::msg::Bool>> pub_enabled_;
  std::shared_ptr<lc::LifecyclePublisher<std_msgs::msg::Float64>> pub_vehicle_speed_ms_;
  std::shared_ptr<lc::LifecyclePublisher<pacmod_msgs::msg::AllSystemStatuses>> pub_all_system_statuses_;

  std::shared_ptr<rclcpp::Subscription<can_msgs::msg::Frame>> sub_can_tx_;
  std::unordered_map<unsigned int, std::shared_ptr<rclcpp::SubscriptionBase>> can_subs_;
};

}  // namespace PACMod3
}  // namespace Drivers
}  // namespace AS

#endif  // PACMOD3__PACMOD3_NODE_HPP_
