# ethzasl_msf [ROS] 多传感器卡尔曼融合框架 Ethzasl MSF Framework 编译与使用


[API文档](http://ethz-asl.github.io/ethzasl_msf)


    多传感器融合是机器人导航上面一个非常基本的问题，
    通常在一个稳定可用的机器人系统中，会使用视觉（RGB或RGBD）、激光、IMU、马盘等，
    一系列传感器的数据来最终输出一个稳定和不易丢失的姿态。
    Ethzasl MSF Framework 是一个机器人上面的多传感器融合框架，
    它使用了扩展卡尔曼的原理对多传感器进行融合。
    同时内部使用了很多工程上的 trick 解决多传感器的刷新率同步等问题，
    API 封装也相对简单，非常适合新手使用。



# 源码分析
## 命名空间
    eigen_conversions	
    Ngeometry_msgs	            Defines the start row and col for the covariance entries in
    Nmsf_core	
    Nmsf_pose_pressure_sensor	
    Nmsf_pose_sensor	
    Nmsf_position_sensor	
    Nmsf_spherical_position	
    Nmsf_tmp	
    Nmsf_updates	

## 

# 编译安装
## 0、系统环境：

    Ubuntu 14.04.3
    ROS indigo

## 1、新建 MSF 工作目录：

    在命令行执行如下命令：
      mkdir -p MSF/src
      cd ./MSF/src            进入src下
      catkin_init_workspace   初始ros工作空间

    退出 src 目录到 PATH_TO_MSF 根目录下，编译生成 ROS 工程文件：
      cd ..
      catkin_make  编译原始工作空间

    后面我们将这个新建的 MSF 路径称为 PATH_TO_MSF。

## 2、下载依赖库和 Ethzasl MSF Framework：
    进入 PATH_TO_MSF/src 目录。
      cd ./src

    在此目录下载所有依赖库和 MSF 框架。

    1）下载 glog 日志分析 段错误追踪：
      git clone https://github.com/ethz-asl/glog_catkin.git

    2）下载 catkin_simple：
      git clone https://github.com/catkin/catkin_simple.git

    3）下载 asctec_mav_framework：
      git clone https://github.com/ethz-asl/asctec_mav_framework.git

    4）最后下载 Ethzasl MSF Framework 框架源代码：
      git clone https://github.com/ethz-asl/ethzasl_msf.git
## 3、编译 ROS 工程：
    1）设置环境：
      退出 src 目录，进入 PATH_TO_MSF 目录。
      在命令行中使用如下命令设置当前窗口的编译环境：	
      cd ..
      source devel/setup.bash

    2）再次编译整个工程：
      catkin_make
      中间可能会出现很多 warning，不过最后如果能够 100% 完成 built target 就成功了。

## 4、运行例子： MSF Viconpos Sensor Framework（使用 ROS Bag）：
    官方的例子使用了 Vicon 的设备进行 6ROF 的姿态估计，这个传感器很专业，但是我们一般没有。
    这里面我们使用官方提供的一个 bag (日志回放)文件来进行模拟。

    1）首先从 ros 网站下载 Vicon 的数据集：

        这个数据包有 3.8 MB 左右，如果速度慢的可以下载我百度网盘的文件：
        http://pan.baidu.com/s/1eShq7lg

        我这里将其放置在 PATH_TO_MSF/data 目录下面。

    2）修改 src/ethzasl_msf/msf_updates/viconpos_sensor_fix.yaml 文件：
        将其中所有的 话题名称：
        /pose_sensor/pose_sensor/

        替换为：
        /msf_viconpos_sensor/pose_sensor/

        找到：
        /pose_sensor/core/data_playback: false 

        修改成：
        /pose_sensor/core/data_playback: true   日志回放

    3）修改 src/ethzasl_msf/msf_updates/launch/viconpos_sensor.launch 文件：
        找到：
          <rosparam file="$(find msf_updates)/viconpos_sensor_fix.yaml"/>

          在这一行的前面加入两行 remap 操作，将传感器的 topic 与引擎的 topic 对应上：
          <remap from="/msf_core/imu_state_input" to="/auk/fcu/imu"  />
          <remap from="msf_updates/transform_input" to="/vicon/auk/auk" />

        找到：
          </node>

          在其之后添加（这一步是初始化卡尔曼滤波器的，非常重要）：

          <node pkg="rosservice" type="rosservice" name="initialize" args="call --wait /msf_viconpos_sensor/pose_sensor/initialize_msf_scale 1"/>

    4）启动 ros 内核：
        在一个窗口打开　roscore：
        roscore


    5）启动 MSF pose_sensor 节点：
        快捷键 Ctrl + Alt + T 新建窗口，在 PATH_TO_MSF 目录下执行如下命令打开　pose_sensor 节点：
        source devel/setup.bash 
        roslaunch msf_updates viconpos_sensor.launch

    6）打开动态配置参数功能（可选）：
        快捷键 Ctrl + Alt + T 新建窗口，执行如下命令打开动态配置功能：
        rosrun rqt_reconfigure rqt_reconfigure

        可以看到一个窗口，在窗口中选中 msf_viconpos_sensor 下面菜单（可以动态调整参数）：
    7）播放 vicon 的 bag 文件：
        快捷键 Ctrl + Alt + T 新建窗口，在 PATH_TO_MSF 目录下执行如下命令：
        rosbag play data/dataset.bag --pause -s 25
        这一行命令是暂停并从第 25s 后开始播放 bag 文件，文档中说这是为了等待 MAV 硬件系统站稳并处于非观察模式（不理解）。

        总之，如果你准备好运行了，就可以开始点击空格键进行数据播放了，播放的数据大约剩余 86s 左右。
![](http://wiki.ros.org/ethzasl_sensor_fusion/Tutorials/Introductory%20Tutorial%20for%20Multi-Sensor%20Fusion%20Framework?action=AttachFile&do=get&target=rosgraph.png)


## 5、数据模拟：
    刚才跑成功了数据融合节点，但是并没有任何可视化的输出可以给我们看到。
    ethzasl msf 提供了一些脚本来进行数据模拟的功能，可以让我们更直观地看到结果。

    1）修改 src/ethzasl_msf/msf_core/scripts/plot_relevant 文件：
        找到：
        rxplot msf_core/state_out/data[0]:data[1]:data[2] msf_core/state_out/data[3]:data[4]:data[5] -b $T -t "position & velocity" -l px,py,pz,vx,vy,vz &
        rxplot msf_core/state_out/data[13]:data[14]:data[15] msf_core/state_out/data[16] -b $T -t "acc bias & scale" -l x,y,z,L 

        修改成：
        rqt_plot msf_core/state_out/data[0]:data[1]:data[2]
        # rxplot msf_core/state_out/data[0]:data[1]:data[2] msf_core/state_out/data[3]:data[4]:data[5] -b $T -t "position & velocity" -l px,py,pz,vx,vy,vz &
        # rxplot msf_core/state_out/data[13]:data[14]:data[15] msf_core/state_out/data[16] -b $T -t "acc bias & scale" -l x,y,z,L 

    2）启动 plot_relevant 脚本：
        快捷键 Ctrl + Alt + T 新建窗口，在 PATH_TO_MSF 目录下执行如下命令打开　plot_relevant 脚本：
        source devel/setup.bash 
        rosrun msf_core plot_relevant

        另外也可以直接在命令行运行：
        rqt_plot msf_core/state_out/data[0]:data[1]:data[2]

        如果一切正常，即可看到如下曲线绘制，这样就表示成功运行起来了。
