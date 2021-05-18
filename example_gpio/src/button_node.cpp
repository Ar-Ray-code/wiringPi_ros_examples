#include <wiringPi.h>
#include <unistd.h>

#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include <example_interfaces/msg/bool.hpp>

class one_btn
{
    private:
        int btn_pin;
        example_interfaces::msg::Bool btn_msg;
    public:
        one_btn(int _btn_pin)
        {
            btn_pin = _btn_pin;
            pinMode(btn_pin,INPUT);
            pullUpDnControl(btn_pin, PUD_UP);
        }
        example_interfaces::msg::Bool read()
        {
            btn_msg.data = digitalRead(btn_pin);
            return btn_msg;
        }
};

class example_btn:public rclcpp::Node
{
    private:
        rclcpp::TimerBase::SharedPtr timer_;

        rclcpp::Publisher<example_interfaces::msg::Bool>::SharedPtr publisher_a;
        rclcpp::Publisher<example_interfaces::msg::Bool>::SharedPtr publisher_b;

        void pub_btn_thread()
        {
            static one_btn target_btn_a(20);
            static one_btn target_btn_b(21);
            
            RCLCPP_INFO(this->get_logger(), "%d, %d", target_btn_a.read().data, target_btn_b.read().data);
            publisher_a->publish(target_btn_a.read());
            publisher_b->publish(target_btn_b.read());
        }

    public:
        example_btn(const std::string name, const rclcpp::NodeOptions & options):Node(name, options)
        {   
            using namespace std::chrono_literals;
            
            publisher_a = this->create_publisher<example_interfaces::msg::Bool>("btna", 1);
            publisher_b = this->create_publisher<example_interfaces::msg::Bool>("btnb", 1);

            timer_ = this->create_wall_timer(10ms, std::bind(&example_btn::pub_btn_thread, this));
        }
};

int main(int argc, char** argv)
{
    if(wiringPiSetupGpio() == -1) return 1;
    
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    auto node = std::make_shared<example_btn>("btn_example",options);
	
    rclcpp::spin(node);
    std::cout << "start reading" << std::endl;
    rclcpp::shutdown();
    return 0;
}
