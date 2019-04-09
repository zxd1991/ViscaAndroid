#include <string>
#include "visca.h"
#include "visca_data.h"
#include <stdlib.h>

#define LOG_TAG "VISCA"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define    VISCA_MEMORY                        0x3F
#define    VISCA_COMMAND                      0x01
#define    VISCA_INQUIRY                      0x09
#define    VISCA_CATEGORY                     0x04
#define    VISCA_PT_HOME                       0x04
#define    VISCA_TERMINATOR                   0xFF
#define    VISCA_MEMORY_SET                    0x01
#define    VISCA_MEMORY_RECALL                 0x02
#define    VISCA_ZOOM_VALUE                    0x47
#define    VISCA_PT_POSITION_INQ               0x12
#define    VISCA_RESPONSE_COMPLETED             0x50
#define    VISCA_PANTILT_SPEED_MAX              0x14
#define    VISCA_PANTILT_SPEED_MIN              0x01
#define    VISCA_CATEGORY_PAN_TILTER            0x06
#define    VISCA_PT_ABSOLUTE_POSITION           0x02
const static uint8_t COMMAND_RESPONSE_LEN = 6;
const static uint8_t COMMAND_RESPONSE_INDEX = 4;
const static uint8_t INQUIRY_ZOOM_POS_RESP_LEN = 7;
const static uint8_t INQUIRY_PANTILT_RESP_LEN = 11;
const static uint8_t PAN_TILT_DEGREE_DUR = 20;
const static uint8_t ZOOM_POS_CHANGE_DUR = 24;
const static uint16_t PAN_TILT_DEGREE_HORIZ_MAX = 0x09a4;
const static uint16_t PAN_TILT_DEGREE_VER_MAX = 0x570;
const static uint16_t ZOOM_POS_MAX = 0x0900;
const static uint16_t ZOOM_POS_MIN = 0x0500;
static uint8_t pan_tilt_speed_teac = 1;
static uint8_t pan_tilt_speed_stud = 1;

#include <assert.h>

#define INIT_PRIV(viscaManagerImpl) \
    ViscaManagerImpl *viscaManagerImpl = NULL; \
    if (m_priv) { \
        viscaManagerImpl = static_cast<ViscaManagerImpl *>(m_priv); \
    } \
    assert(viscaManagerImpl); \


class ViscaManagerImpl {
public:
    ViscaManagerImpl();

    ~ViscaManagerImpl();

    struct ViscaPacket {
        unsigned char bytes[32];
        uint32_t length;
    };

    uint16_t getZoomValue(uint8_t camera_id);

    uint32_t getPantiltPosition(uint8_t camera_id);

    void appendPantiltPosPacket(ViscaPacket *packet, uint8_t camera_id, uint16_t horizpos,
                                uint16_t verpos);

    int checkPanTiltPos(uint16_t pan_pos_horiz, uint16_t pan_pos_ver, uint8_t dir);

    char *sendViscaPacket(uint8_t camera_id, ViscaPacket *packet);

    void viscaPacketInit(ViscaPacket *packet);

    void viscaAppendByte(ViscaPacket *packet, unsigned char byte);

    int getCommandResult (char * result);
};

ViscaManager::ViscaManager()
        : m_priv(new ViscaManagerImpl()) {}

ViscaManager::~ViscaManager() {
    if (m_priv) {
        ViscaManagerImpl *d = static_cast<ViscaManagerImpl *>(m_priv);
        if (d) {
            delete d;
            m_priv = NULL;
        }
    }
}

ViscaManagerImpl::ViscaManagerImpl() {}

ViscaManagerImpl::~ViscaManagerImpl() {}

int ViscaManager::connectHostCam(const char *ip, uint16_t port) {
    return connectMyServer(ip, port);
}

int ViscaManager::setPantiltPos(uint8_t camera_id, uint8_t dir) {
    INIT_PRIV(viscaManagerImpl);
    uint32_t pan_pos = viscaManagerImpl->getPantiltPosition(camera_id);
    u_int16_t pan_pos_horiz = pan_pos >> 16;
    u_int16_t pan_pos_ver = (pan_pos << 16) >> 16;
    if (viscaManagerImpl->checkPanTiltPos(pan_pos_horiz, pan_pos_ver, dir) != VISCA_SUCCESS) {
        LOGE("exceed the max");
        return VISCA_FAILURE;
    }
    LOGD("pan_pos_horiz:%d,pan_pos_ver:%d", pan_pos_horiz, pan_pos_ver);
    ViscaManagerImpl::ViscaPacket packet;
    viscaManagerImpl->viscaPacketInit(&packet);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_COMMAND);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_CATEGORY_PAN_TILTER);

    switch (dir) {
        case LEFT:
            pan_pos_horiz -= PAN_TILT_DEGREE_DUR;
            break;
        case UP:
            pan_pos_ver -= PAN_TILT_DEGREE_DUR;
            break;
        case RIGHT:
            pan_pos_horiz += PAN_TILT_DEGREE_DUR;
            break;
        case DOWN:
            pan_pos_ver += PAN_TILT_DEGREE_DUR;
            break;
        case HOME:
            viscaManagerImpl->viscaAppendByte(&packet, VISCA_PT_HOME);
            break;
    }
    if (dir != HOME) {
        viscaManagerImpl->viscaAppendByte(&packet, VISCA_PT_ABSOLUTE_POSITION);
        viscaManagerImpl->appendPantiltPosPacket(&packet, camera_id, pan_pos_horiz, pan_pos_ver);
    }
    char *result = viscaManagerImpl->sendViscaPacket(camera_id, &packet);
    if (viscaManagerImpl->getCommandResult(result) != VISCA_SUCCESS) {
        return VISCA_FAILURE;
    }
    return VISCA_SUCCESS;
}

int ViscaManager::setPantiltHome(uint8_t camera_id) {
    INIT_PRIV(viscaManagerImpl);
    ViscaManagerImpl::ViscaPacket packet;
    viscaManagerImpl->viscaPacketInit(&packet);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_COMMAND);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_CATEGORY_PAN_TILTER);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_PT_HOME);
    char *result = viscaManagerImpl->sendViscaPacket(camera_id, &packet);
    return viscaManagerImpl->getCommandResult(result);
}

int ViscaManagerImpl::checkPanTiltPos(uint16_t pan_pos_horiz, uint16_t pan_pos_ver, uint8_t dir) {
    switch (dir) {
        case ViscaManager::LEFT:
            if (pan_pos_horiz <= PAN_TILT_DEGREE_DUR) {
                return VISCA_FAILURE;
            }
            break;
        case ViscaManager::UP:
            if (pan_pos_ver <= PAN_TILT_DEGREE_DUR) {
                return VISCA_FAILURE;
            }
            break;
        case ViscaManager::RIGHT:
            if (pan_pos_horiz >= PAN_TILT_DEGREE_HORIZ_MAX) {
                return VISCA_FAILURE;
            }
            break;
        case ViscaManager::DOWN:
            if (pan_pos_ver >= PAN_TILT_DEGREE_VER_MAX) {
                return VISCA_FAILURE;
            }
            break;
        default:
            break;
    }

    return VISCA_SUCCESS;
}


void ViscaManagerImpl::appendPantiltPosPacket
        (ViscaPacket *packet, uint8_t camera_Id, u_int16_t horizpos,
         uint16_t verpos) {
    LOGD("appendPantiltPosPacket hori: %d,ver:%d", horizpos, verpos);
    if (camera_Id == ViscaManager::TEACHER) {
        viscaAppendByte(packet, pan_tilt_speed_teac);
        viscaAppendByte(packet, pan_tilt_speed_teac);
    } else {
        viscaAppendByte(packet, pan_tilt_speed_stud);
        viscaAppendByte(packet, pan_tilt_speed_stud);
    }
    viscaAppendByte(packet, (horizpos & 0xF000) >> 12);
    viscaAppendByte(packet, (horizpos & 0x0F00) >> 8);
    viscaAppendByte(packet, (horizpos & 0x00F0) >> 4);
    viscaAppendByte(packet, (horizpos & 0x000F));

    viscaAppendByte(packet, (verpos & 0xF000) >> 12);
    viscaAppendByte(packet, (verpos & 0x0F00) >> 8);
    viscaAppendByte(packet, (verpos & 0x00F0) >> 4);
    viscaAppendByte(packet, (verpos & 0x000F));
}


int ViscaManager::setPantiltSpeed(uint8_t camera_id, uint8_t speed) {
    if (speed > VISCA_PANTILT_SPEED_MAX || speed < VISCA_PANTILT_SPEED_MIN) {
        return VISCA_FAILURE;
    }
    if (camera_id == TEACHER) {
        pan_tilt_speed_teac = speed;
    } else if (camera_id == STUDENT) {
        pan_tilt_speed_stud = speed;
    }
    return VISCA_SUCCESS;
}

int ViscaManager::setCamMemory(uint8_t camera_id, uint8_t channel) {
    INIT_PRIV(viscaManagerImpl);
    ViscaManagerImpl::ViscaPacket packet;
    viscaManagerImpl->viscaPacketInit(&packet);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_COMMAND);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_CATEGORY);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_MEMORY);

    viscaManagerImpl->viscaAppendByte(&packet, VISCA_MEMORY_SET);
    viscaManagerImpl->viscaAppendByte(&packet, channel);
    char *result = viscaManagerImpl->sendViscaPacket(camera_id, &packet);
    return viscaManagerImpl->getCommandResult(result);
}

int ViscaManager::recallCamMemory(uint8_t camera_id, uint8_t channel) {
    INIT_PRIV(viscaManagerImpl);
    ViscaManagerImpl::ViscaPacket packet;
    viscaManagerImpl->viscaPacketInit(&packet);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_COMMAND);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_CATEGORY);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_MEMORY);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_MEMORY_RECALL);
    viscaManagerImpl->viscaAppendByte(&packet, channel);
    char *result = viscaManagerImpl->sendViscaPacket(camera_id, &packet);
    return viscaManagerImpl->getCommandResult(result);
}

int ViscaManager::setZoomPosition(uint8_t camera_id, bool far) {
    INIT_PRIV(viscaManagerImpl);
    uint16_t zoomPos = viscaManagerImpl->getZoomValue(camera_id);
    LOGD("zoomPos:%d", zoomPos);
    if ((far && zoomPos >= ZOOM_POS_MAX) || (!far && zoomPos <= ZOOM_POS_MIN)) {
        return VISCA_FAILURE;
    }
    if (far) {
        zoomPos += ZOOM_POS_CHANGE_DUR;
    } else {
        zoomPos -= ZOOM_POS_CHANGE_DUR;
    }
    ViscaManagerImpl::ViscaPacket packet;
    viscaManagerImpl->viscaPacketInit(&packet);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_COMMAND);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_CATEGORY);
    viscaManagerImpl->viscaAppendByte(&packet, VISCA_ZOOM_VALUE);
    viscaManagerImpl->viscaAppendByte(&packet, ((zoomPos & 0xF000) >> 12));
    viscaManagerImpl->viscaAppendByte(&packet, ((zoomPos & 0x0F00) >> 8));
    viscaManagerImpl->viscaAppendByte(&packet, ((zoomPos & 0x00F0) >> 4));
    viscaManagerImpl->viscaAppendByte(&packet, ((zoomPos & 0x00F)));
    char *result = viscaManagerImpl->sendViscaPacket(camera_id, &packet);
    return viscaManagerImpl->getCommandResult(result);
}

uint16_t ViscaManagerImpl::getZoomValue(uint8_t camera_id) {
    ViscaPacket packet;
    viscaPacketInit(&packet);
    viscaAppendByte(&packet, VISCA_INQUIRY);
    viscaAppendByte(&packet, VISCA_CATEGORY);
    viscaAppendByte(&packet, VISCA_ZOOM_VALUE);
    char *result = sendViscaPacket(camera_id, &packet);
    unsigned char temp[INQUIRY_ZOOM_POS_RESP_LEN];
    memset(temp, 0, INQUIRY_ZOOM_POS_RESP_LEN);
    memcpy(temp, result, INQUIRY_ZOOM_POS_RESP_LEN);
    delete []result;
    uint16_t zoomValue =
            ((temp[2] & 0x0F) << 12) + ((temp[3] & 0x0F) << 8) + ((temp[4] & 0x0F) << 4) +
            ((temp[5] & 0x0F));
    return zoomValue;
}

uint32_t ViscaManagerImpl::getPantiltPosition(uint8_t camera_id) {
    ViscaPacket packet;
    viscaPacketInit(&packet);
    viscaAppendByte(&packet, VISCA_INQUIRY);
    viscaAppendByte(&packet, VISCA_CATEGORY_PAN_TILTER);
    viscaAppendByte(&packet, VISCA_PT_POSITION_INQ);
    char *result = sendViscaPacket(camera_id, &packet);
    unsigned char temp[INQUIRY_PANTILT_RESP_LEN];
    memset(temp, 0, INQUIRY_PANTILT_RESP_LEN);
    memcpy(temp, result, INQUIRY_PANTILT_RESP_LEN);
    delete []result;
    uint32_t pantile_pos = 0;
    pantile_pos = (temp[2] << 28) + (temp[3] << 24) + (temp[4] << 20) + (temp[5] << 16) +
                  (temp[6] << 12) + (temp[7]
            << 8) + (temp[8] << 4) + temp[9];
    return pantile_pos;
}

int ViscaManagerImpl::getCommandResult(char *result) {
    unsigned char temp[COMMAND_RESPONSE_LEN];
    memset(temp, 0, COMMAND_RESPONSE_LEN);
    memcpy(temp, result, COMMAND_RESPONSE_LEN);
    int command_resp = result[COMMAND_RESPONSE_INDEX];
    command_resp = command_resp & 0xF0;
    delete []result;
    if (command_resp == VISCA_RESPONSE_COMPLETED) {
        return VISCA_SUCCESS;
    }
    return VISCA_FAILURE;
}


void ViscaManagerImpl::viscaPacketInit(ViscaPacket *packet) {
    packet->length = 1;
}

 char *ViscaManagerImpl::sendViscaPacket(uint8_t cam_id, ViscaPacket *packet) {
    packet->bytes[0] = 0x80;
    packet->bytes[0] |= cam_id;
    viscaAppendByte(packet, VISCA_TERMINATOR);
    return sendData(packet->bytes,packet->length);
}

void ViscaManagerImpl::viscaAppendByte(ViscaPacket *packet, unsigned char byte) {
    packet->bytes[packet->length] = byte;
    (packet->length)++;
}


