#include <memory.h>
#include <ros/ros.h>
#include <signal.h>
#include <std_msgs/Char.h>
#include <termios.h>
#include <unistd.h>

const char KEYCODE_R = 0x43;
const char KEYCODE_L = 0x44;
const char KEYCODE_U = 0x41;
const char KEYCODE_D = 0x42;

int kfd = 0;
struct termios cooked, raw;

void key_loop(ros::Publisher &key_pub)
{
    // get the console in raw mode
    tcgetattr(kfd, &cooked);
    memcpy(&raw, &cooked, sizeof(struct termios));
    raw.c_lflag &= ~(ICANON | ECHO);
    // Setting a new line, then end of file
    raw.c_cc[VEOL] = 1;
    raw.c_cc[VEOF] = 2;
    tcsetattr(kfd, TCSANOW, &raw);

    while (1)
    {
        std_msgs::Char c;
        if (::read(kfd, &c.data, 1) < 0)
        {
            ROS_WARN("read():");
            exit(-1);
        }
        switch (c.data)
        {
        case KEYCODE_R:
            key_pub.publish(c);
            ROS_INFO("R");
            break;
        case KEYCODE_L:
            ROS_INFO("L");
            key_pub.publish(c);
            break;
        case KEYCODE_U:
            ROS_INFO("U");
            key_pub.publish(c);
            break;
        case KEYCODE_D:
            ROS_INFO("D");
            key_pub.publish(c);
            break;
        }
    }
}

void quit(int sig)
{
    (void)sig;
    tcsetattr(kfd, TCSANOW, &cooked);
    ros::shutdown();
    exit(0);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "key_publisher");
    ros::NodeHandle nh_;
    ros::Publisher key_pub = nh_.advertise<std_msgs::Char>("/key_signal", 1);
    signal(SIGINT, quit);
    key_loop(key_pub);
    return 0;
}
