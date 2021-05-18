#!/bin/python3
import launch
import launch_ros.actions

def generate_launch_description():
    blink = launch_ros.actions.Node(
        package='example_gpio', executable='blink_node'
    )
    leds = launch_ros.actions.Node(
        package='example_gpio', executable='led_node'
    )

    return launch.LaunchDescription([
        blink,
        leds
    ])