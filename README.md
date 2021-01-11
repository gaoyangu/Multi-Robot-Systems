## 89-689 Multi-Robot Systems

## Usage
### 1. Simulating multiple robots in Stage
```bash
$roslaunch stage_multi stage_multi.launch
```

### 2. Robots Synchronization
```bash
$rosrun multi_sync monitor 4
$roslaunch stage_multi stage_multi.launch
```

### 3. Find Robots Position
```bash
$roslaunch tf_multi tf_multi.launch
$rosrun teleop_twist_keyboard teleop_twist_keyboard.py cmd_vel:=robot_0/cmd_vel
```

### 4. Leader-Follower Formation
```bash
$roslaunch tf_multi tf_multi.launch
$rosrun teleop_twist_keyboard teleop_twist_keyboard.py cmd_vel:=robot_0/cmd_vel
```

### 5. Running the Navigation Stack
```bash
$roslaunch navigation_stage move_base_multi_robot.launch
```

## Thanks
- [89-689: Multi-Robot Systems](https://u.cs.biu.ac.il/~yehoshr1/89-689/) (Summer 2015)
- [89-685: Introduction to Robotics](https://u.cs.biu.ac.il/~yehoshr1/89-685/) (Fall 2016)