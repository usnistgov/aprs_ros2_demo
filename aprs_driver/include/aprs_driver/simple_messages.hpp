#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cstring>

#include <arpa/inet.h>

namespace simple_message {

  /*
  ==============================================================================
  STATUS MESSAGE
  ==============================================================================
  */
  class Status{
    int comm_type;
    int reply_code;
    int drives_powered;
    int e_stopped;
    int error_code;
    int in_error;
    int in_motion;
    int mode;
    int motion_possible;
    
  public:
    bool update(char* input);
    std::string output();

    static const int PACKET_LENGTH = 40;
    const int msg_type = 13;
  };

  /*
  ==============================================================================
  JOINT FEEDBACK MESSAGE
  ==============================================================================
  */
  class JointFeedback{
    int comm_type;
    int reply_code;
    int robot_id;
    int valid_fields;
    float time;
    std::vector<float> positions;
    std::vector<float> velocities;
    std::vector<float> accelerations;

  public:
    bool update(char* input);
    std::string output();
    std::vector<float> get_joint_positions() {return positions;};
    std::vector<float> get_joint_velocities() {return velocities;};
    std::vector<float> get_joint_accelerations() {return accelerations;};

    static const int PACKET_LENGTH = 144;
    int msg_type = 15;
  };

  /*
  ==============================================================================
  JOINT POSITION MESSAGE
  ==============================================================================
  */
  class JointPosition{
    int comm_type;
    int reply_code;
    int sequence;
    std::vector<float> joint_data;
  
  public:
    bool update(char* input);
    std::string output();
    std::vector<float> get_joint_data() {return joint_data;};

    static const int PACKET_LENGTH = 56;
    int msg_type = 10;
  };

  /*
  ==============================================================================
  WRITE IO REPLY
  ==============================================================================
  */
  class WriteIOReply{
    bool init(char* input);
    std::string output();

    int packet_length = 16;

    int msg_type = 2006;
    int comm_type;
    int reply_code;
    int result;
  };

  /*
  ==============================================================================
  READ IO REPLY
  ==============================================================================
  */
  class ReadIOReply{
    public:
      bool init(char* input);
      std::string output();
      int get_value(){return value;};
    
    private:
      int packet_length = 20;
      int msg_type = 2004;

      int comm_type;
      int reply_code;
      int value;
      int result;
  };

  /*
  ==============================================================================
  MOTO MOTION REPLY
  ==============================================================================
  */
  class MotoMotionReply{
    public:
      bool update(char* input);
      std::string output();
      bool is_successful(){return result == 0;};
      int get_result_num(){return result;};
      std::string get_result(){return result_codes[result];};
      std::string get_subcode(){return subcodes[subcode];};
    private:
      int packet_length = 72;
      int msg_type = 2002;

      int comm_type;
      int reply_code;
      int robot_id;
      int sequence;
      int command;
      int result;
      int subcode;
      std::vector<float> data;

      std::map<int, std::string> command_codes = {
        {14, "CHECK_MOTION_READY"},
        {200101, "CHECK_MOTION_READY"}, 
        {200102, "CHECK_QUEUE_CNT"}, 
        {200111, "STOP_MOTION"}, 
        {200121, "START_TRAJ_MODE"},
        {200122, "STOP_TRAJ_MODE"}
      };

      std::map<int, std::string> result_codes = {
        {0, "SUCCESS/TRUE"},
        {1, "BUSY"}, 
        {2, "FAILURE/FALSE"}, 
        {3, "INVALID"}, 
        {4, "ALARM"},
        {5, "NOT_READY"},
        {6, "MP_FAILURE"}
      };

      std::map<int, std::string> subcodes = {
        {0, "ROS_RESULT_SUCCESS/ROS_RESULT_TRUE"},
        {1, "ROS_RESULT_BUSY"},
        {2, "ROS_RESULT_FAILURE/ROS_RESULT_FALSE"},
        {3, "ROS_RESULT_INVALID"},
        {4, "ROS_RESULT_ALARM"},
        {5, "ROS_RESULT_NOT_READY"},
        {6, "ROS_RESULT_MP_FAILURE"},
        {3000, "ROS_RESULT_INVALID_UNSPECIFIED"},
        {3001, "ROS_RESULT_INVALID_MSGSIZE"},
        {3002, "ROS_RESULT_INVALID_MSGHEADER"},
        {3003, "ROS_RESULT_INVALID_MSGTYPE"},
        {3004, "ROS_RESULT_INVALID_GROUPNO"},
        {3005, "ROS_RESULT_INVALID_SEQUENCE"},
        {3006, "ROS_RESULT_INVALID_COMMAND"},
        {3010, "ROS_RESULT_INVALID_DATA"},
        {3011, "ROS_RESULT_INVALID_DATA_START_POS"},
        {3012, "ROS_RESULT_INVALID_DATA_POSITION"},
        {3013, "ROS_RESULT_INVALID_DATA_SPEED"},
        {3014, "ROS_RESULT_INVALID_DATA_ACCEL"},
        {3015, "ROS_RESULT_INVALID_DATA_INSUFFICIENT"},
        {5000, "ROS_RESULT_NOT_READY_UNSPECIFIED"},
        {5001, "ROS_RESULT_NOT_READY_ALARM"},
        {5002, "ROS_RESULT_NOT_READY_ERROR"},
        {5003, "ROS_RESULT_NOT_READY_ESTOP"},
        {5004, "ROS_RESULT_NOT_READY_NOT_PLAY"},
        {5005, "ROS_RESULT_NOT_READY_NOT_REMOTE"},
        {5006, "ROS_RESULT_NOT_READY_SERVO_OFF"},
        {5007, "ROS_RESULT_NOT_READY_HOLD"},
        {5008, "ROS_RESULT_NOT_READY_NOT_STARTED"},
        {5009, "ROS_RESULT_NOT_READY_WAITING_ROS"},
        {11120, "IO_FEEDBACK_WAITING_MP_INCMOVE"},
        {11121, "IO_FEEDBACK_MP_INCMOVE_DONE"}, 
        {11122, "IO_FEEDBACK_MP_INITIALIZATION_DONE"}, 
        {11123, "IO_FEEDBACK_CONNECTSERVERRUNNING"}, 
        {11124, "IO_FEEDBACK_MOTIONSERVERCONNECTED"},
        {11125, "IO_FEEDBACK_STATESERVERCONNECTED"},
        {11127, "IO_FEEDBACK_FAILURE"},
        {11127, "IO_FEEDBACK_FAILURE"}
      };                                             
  };


  /*
  ==============================================================================
  JOINT TRAJ PT REQUEST
  ==============================================================================
  */
  class JointTrajPt{
  public:
    JointTrajPt(int _seq, std::vector<float> _pos, float _vel, float _dur);
    std::vector<uint8_t> to_bytes();
  
  private:
    int length = 64;
    int msg_type = 11;
    int comm_type = 2;
    int reply_code = 0;
    
    int sequence;
    std::vector<float> joint_data;
    float velocity;
    float duration;
  };

  /*
  ==============================================================================
  JOINT TRAJ PT FULL REQUEST
  ==============================================================================
  */
  class JointTrajPtFull{
    public:
      JointTrajPtFull(int _seq, float _time, std::vector<float> _pos, std::vector<float> _vel, std::vector<float> _acc);
      std::vector<uint8_t> to_bytes();
    
    private:
      int length = 136;
      int msg_type = 14;
      int comm_type = 2;
      int reply_code = 0;
      int robot_id = 0;
      int valid_fields = 7;
      
      int sequence;
      float time;
      std::vector<float> positions;
      std::vector<float> velocities;
      std::vector<float> accelerations;
  };

  /*
  ==============================================================================
  MOTO MOTION CONTROL REQUEST
  ==============================================================================
  */
  class MotoMotionCtrl{
    public:
      MotoMotionCtrl(std::string);
      std::vector<uint8_t> to_bytes();
    private:
      int length = 64;
      int msg_type = 2001;
      int comm_type = 2;
      int reply_code = 0;
      int robot_id = 0;
      int seq = 0;

      int command;

      std::map<std::string, int> commands = { 
        {"CHECK_MOTION_READY", 200101}, 
        {"CHECK_QUEUE_CNT", 200102}, 
        {"STOP_MOTION", 200111}, 
        {"START_TRAJ_MODE", 200121},
        {"STOP_TRAJ_MODE", 200122}
      };
  };

  /*
  ==============================================================================
  READ IO REQUEST
  ==============================================================================
  */
  class ReadIORequest{
    public:
      ReadIORequest(int _address);
      std::vector<uint8_t> to_bytes();
    
    private:
      int length = 16;
      int msg_type = 2003;
      int comm_type = 2;
      int reply_code = 0;

      int address;
  };

  /*
  ==============================================================================
  WRITE IO REQUEST
  ==============================================================================
  */
  class WriteIORequest{
    public:
      WriteIORequest(int _address, int data);
      std::vector<uint8_t> to_bytes();
      
    private:
      int length = 25;
      int msg_type = 2005;
      int comm_type = 2;
      int reply_code = 0;

      int address;
      int data;
  };
}