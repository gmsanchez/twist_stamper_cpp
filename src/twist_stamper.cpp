#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>

class TwistStamper : public rclcpp::Node
{

public:
    TwistStamper() : Node("twist_stamper_cpp")
    {
        
        this->declare_parameter("frame_id", "base_link");

        cmd_vel_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "cmd_vel_in", rclcpp::SystemDefaultsQoS(),
            std::bind(&TwistStamper::topic_callback, this, std::placeholders::_1)
        );
        
        cmd_vel_stamped_pub_ = this->create_publisher<geometry_msgs::msg::TwistStamped>(
            "cmd_vel_out", rclcpp::QoS(rclcpp::KeepLast(1))
        );

    }

    void topic_callback(const geometry_msgs::msg::Twist::ConstSharedPtr &twist)
    {
        geometry_msgs::msg::TwistStamped empty_twist_stamped;

        empty_twist_stamped.header.stamp = this->get_clock()->now();
        empty_twist_stamped.header.frame_id = this->get_parameter("frame_id").as_string();
        empty_twist_stamped.twist = *twist;
        cmd_vel_stamped_pub_->publish(empty_twist_stamped);

    }

private:
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub_;
    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr cmd_vel_stamped_pub_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TwistStamper>());
    rclcpp::shutdown();
    return 0;
}