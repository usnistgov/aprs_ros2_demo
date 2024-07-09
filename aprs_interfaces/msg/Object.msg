# Object Type
uint8 KIT_TRAY=1
uint8 PART_TRAY=2
uint8 PART=3

# Object Identifier
uint8 SMALL_GEAR=10
uint8 MEDIUM_GEAR=11
uint8 LARGE_GEAR=12
uint8 SMALL_GEAR_TRAY=13 
uint8 MEDIUM_GEAR_TRAY=14
uint8 LARGE_GEAR_TRAY=15
uint8 M2L1_KIT_TRAY=16
uint8 S2L2_KIT_TRAY=17

string name
uint8 object_type # 1: KitTray, 2: PartTray, 3: Part
uint8 object_identifier # 10: SmallGear, 11: MediumGear, 12: LargeGear, 13: SmallGearTray, 14: MediumGearTray, 15: LargeGearTray, 16: M2L1KitTray, 17: S2L2KitTray  
geometry_msgs/PoseStamped pose_stamped
