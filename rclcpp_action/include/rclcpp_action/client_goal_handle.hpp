// Copyright 2018 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RCLCPP_ACTION__CLIENT_GOAL_HANDLE_HPP_
#define RCLCPP_ACTION__CLIENT_GOAL_HANDLE_HPP_

#include <rcl_action/action_client.h>

#include <rclcpp/macros.hpp>

#include <functional>
#include <future>
#include <memory>
#include <mutex>

#include "rclcpp_action/types.hpp"
#include "rclcpp_action/visibility_control.hpp"

namespace rclcpp_action
{
// Forward declarations
template<typename ACTION>
class Client;

template<typename ACTION>
class ClientGoalHandle
{
public:
  RCLCPP_SMART_PTR_DEFINITIONS_NOT_COPYABLE(ClientGoalHandle)

  using Result = typename ACTION::Result;
  using Feedback = typename ACTION::Feedback;
  using FeedbackCallback = std::function<void (
    ClientGoalHandle::SharedPtr, const Feedback &)>;

  virtual ~ClientGoalHandle();

  const GoalID & get_goal_id() const;

  std::shared_future<Result>
  async_result();

  FeedbackCallback
  get_feedback_callback();

  int8_t
  get_status();

  bool
  is_feedback_aware();

  bool
  is_result_aware();

private:
  // The templated Client creates goal handles
  friend Client<ACTION>;

  ClientGoalHandle(const GoalInfo & info, FeedbackCallback callback);

  void
  set_feedback_callback(FeedbackCallback callback);

  void
  set_result_awareness(bool awareness);

  void
  set_status(int8_t status);

  void
  set_result(const Result & result);

  void
  set_result(const typename Result::SharedPtr & result)
  {
    return set_result(*(result.get()));
  }

  void
  invalidate();

  GoalInfo info_;

  bool is_result_aware_{false};
  std::promise<Result> result_promise_;
  std::shared_future<Result> result_future_;

  FeedbackCallback feedback_callback_{nullptr};
  int8_t status_{GoalStatus::STATUS_ACCEPTED};

  std::mutex handle_mutex_;
};
}  // namespace rclcpp_action

#include <rclcpp_action/client_goal_handle_impl.hpp>  // NOLINT(build/include_order)
#endif  // RCLCPP_ACTION__CLIENT_GOAL_HANDLE_HPP_