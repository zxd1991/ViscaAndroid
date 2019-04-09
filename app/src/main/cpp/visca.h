//
// Created by zhangxd on 2019/3/11.
//

#define    VISCA_SUCCESS                       0
#define    VISCA_FAILURE                      -1

class ViscaManager {

public:
    //
    enum CAM_ID {
        TEACHER = 1,
        STUDENT
    };

    //
    enum CAM_DIR {
        LEFT = 0,
        UP,
        RIGHT,
        DOWN,
        HOME
    };

    ViscaManager();

    ~ViscaManager();

    //@brief connect the host camera,@ipAddress represents the host ip,@port represents the server's port
    //@return 0 means successful result, or  -1 means failure result
    int connectHostCam(const char *ipAddress, uint16_t port);

    //@brief set the pantilt pos according to direction
    //@param camera_id @ see CAM_ROLE#TEACHER represents the id of teacher camera,@ see CAM_ROLE#STUDENT
    // represents the id of student camera
    //@param dir @see CAM_DIR,@see CAM_DIR#LEFT,CAM_DIR#UP,CAM_DIR#RIGHT,CAM_DIR#DOWN,CAM_DI#HOME
    //@return 0 means successful result, or  -1 means failure result
    int setPantiltPos(uint8_t camera_id, uint8_t dir);

    //@brief set the dev return home position
    //@param camera_id @ see CAM_ROLE#TEACHER represents the id of teacher camera,@ see CAM_ROLE#STUDENT
    //@return 0 is suc, or -1 failed
    int setPantiltHome(uint8_t camera_id);

    //@brief set the speed of pantilt
    //@param camera_id @ see CAM_ROLE#TEACHER represents the id of teacher camera,@ see CAM_ROLE#STUDENT
    //@speed the speed of moving the pantilt,from minimum 0x01 to maximum 0x14
    //@return 0 is suc, or -1 failed
    int setPantiltSpeed(uint8_t camera_id, uint8_t speed);

    //@brief set the preset of camera,including stats of the direction,zoom..
    //@param camera_id @ see CAM_ROLE#TEACHER represents the id of teacher camera,@ see CAM_ROLE#STUDENT
    //@channel the preset of states of camera,from 0x00 to 0xFF .
    //@return 0 is suc, or -1 failed
    int setCamMemory(uint8_t camera_id, uint8_t channel);

    //@brief set the recall of camera preset,including the states of the direction,zoom.
    //@param camera_id @ see CAM_ROLE#TEACHER represents the id of teacher camera,@ see CAM_ROLE#STUDENT
    //@channel the preset of states of camera,from 0x00 to 0xFF .
    //@return 0 is suc, or -1 failed
    int recallCamMemory(uint8_t camera_id, uint8_t channel);

    //@brief set the recall of camera preset,including stats of the direction,zoom..
    //@param camera_id @ see CAM_ROLE#TEACHER represents the id of teacher camera,@ see CAM_ROLE#STUDENT
    //@wide means the far zoom position,or near zoom position
    //@return 0 is suc, or -1 failed
    int setZoomPosition(uint8_t camera_id, bool wide);

private:
    void *m_priv;
};

