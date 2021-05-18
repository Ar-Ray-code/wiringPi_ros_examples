#!/bin/python3
import launch
import launch_ros.actions

def generate_launch_description():
    buttons = launch_ros.actions.Node(
        package='example_gpio', executable='button_node'
    )
    leds = launch_ros.actions.Node(
        package='example_gpio', executable='led_node'
    )

    return launch.LaunchDescription([
        buttons,
        leds
    ])