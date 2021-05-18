#include <wiringPi.h>
#include <unistd.h>

#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include <example_interfaces/msg/bool.hpp>

class one_led
{
    private:
        int led_pin;
        example_interfaces::msg::Bool led_msg;
    public:
        one_led(int _led_pin)
        {
            led_pin = _led_pin;
            pinMode(led_pin, OUTPUT);
        }
        void write(example_interfaces::msg::Bool::SharedPtr msg)
        {
            digitalWrite(led_pin, msg->data);
        }
};

class example_led:public rclcpp::Node
{
    private:
        rclcpp::TimerBase::SharedPtr timer_;

        rclcpp::Subscription<example_interfaces::msg::Bool>::SharedPtr subscriber_a;
        rclcpp::Subscription<example_interfaces::msg::Bool>::SharedPtr subscriber_b;

        void sub_button_a(const example_interfaces::msg::Bool::SharedPtr msg)
        {
            static one_led target_led_a(19);
            target_led_a.write(msg);
        }

        void sub_button_b(const example_interfaces::msg::Bool::SharedPtr msg)
        {
            static one_led target_led_b(26);
            target_led_b.write(msg);
        }

    public:
        example_led(const std::string name, const rclcpp::NodeOptions & options):Node(name, options)
        {   
            using namespace std::chrono_literals;
            
            subscriber_a = this->create_subscription<example_interfaces::msg::Bool>("btna", 1, std::bind(&example_led::sub_button_a, this, std::placeholders::_1));
            subscriber_b = this->create_subscription<example_interfaces::msg::Bool>("btnb", 1, std::bind(&example_led::sub_button_b, this, std::placeholders::_1));
        }
};

int main(int argc, char** argv)
{
    if(wiringPiSetupGpio() == -1) return 1;
    
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    auto node = std::make_shared<example_led>("led_example",options);
	
    rclcpp::spin(node);
    std::cout << "start reading" << std::endl;
    rclcpp::shutdown();
    return 0;
}
