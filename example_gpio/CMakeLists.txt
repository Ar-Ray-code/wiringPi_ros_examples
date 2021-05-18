cmake_minimum_required(VERSION 3.5)
project(example_gpio)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)

set(WIRINGPI
  -lwiringPi
  -lcrypt
  -lpthread
  -lm
  -lrt
)

include_directories(
  include/example_gpio
)

## button_node -------------------------------------------
add_executable(button_node
  src/button_node.cpp
)

target_link_libraries(button_node
  ${WIRINGPI}
)

ament_target_dependencies(button_node
  rclcpp
  example_interfaces
)

## led_node ----------------------------------------------
add_executable(led_node
  src/led_node.cpp
)

target_link_libraries(led_node
  ${WIRINGPI}
)

ament_target_dependencies(led_node
  rclcpp
  example_interfaces
)

## blink_node ----------------------------------------------
add_executable(blink_node
  src/blink_node.cpp
)

ament_target_dependencies(blink_node
  rclcpp
  example_interfaces
)

## -------------------------------------------

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS
  button_node
  led_node
  blink_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch/
  DESTINATION share/${PROJECT_NAME}
)

ament_package()