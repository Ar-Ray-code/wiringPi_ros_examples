#include <unistd.h>
#include <chrono>

#include <rclcpp/rclcpp.hpp>
#include <example_interfaces/msg/bool.hpp>


class example_btn:public rclcpp::Node
{
    private:
        rclcpp::TimerBase::SharedPtr timer_;

        rclcpp::Publisher<example_interfaces::msg::Bool>::SharedPtr publisher_a;
        rclcpp::Publisher<example_interfaces::msg::Bool>::SharedPtr publisher_b;

        example_interfaces::msg::Bool data_a;
        example_interfaces::msg::Bool data_b;

        void pub_btn_thread()
        {
            data_a.data = !data_a.data;
            data_b.data = !data_b.data;

            publisher_a->publish(data_a);
            publisher_b->publish(data_b);
        }

    public:
        example_btn(const std::string name, const rclcpp::NodeOptions & options):Node(name, options)
        {   
            using namespace std::chrono_literals;
            
            publisher_a = this->create_publisher<example_interfaces::msg::Bool>("btna", 1);
            publisher_b = this->create_publisher<example_interfaces::msg::Bool>("btnb", 1);

            timer_ = this->create_wall_timer(500ms, std::bind(&example_btn::pub_btn_thread, this));
        }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    auto node = std::make_shared<example_btn>("blink_example",options);
	
    rclcpp::spin(node);
    std::cout << "start reading" << std::endl;
    rclcpp::shutdown();
    return 0;
}
