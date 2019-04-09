package androidjni.example.com.viscaandroid;

/**
 * Created by zhangxd on 2019/3/11.
 */

public class ViscaJni {

    static {
        System.loadLibrary("native-lib");
    }

    private static ViscaJni instance;

    public static ViscaJni getInstance() {
        if (instance == null) {
            instance = new ViscaJni();
        }
        return instance;
    }

    /**
     *  connect the camera
     * @param ip camera server ip
     * @param port camera server port
     * @return 0 success,otherwise failure.
     */
    public native int setTraceCameraAddress(String ip,int port);

    /**
     * 控制云台方向位置
     * @param camId 0 means teacher camera,1 means student camera
     * @param dir include left,up,right,bottom
     * @param continuous true means change the pantilt pos continuously
     * @return 0 success,otherwise failure.
     */
    public native int setPantiltPos(int camId, int dir,boolean continuous);

    /**
     *  设置云台方向回到Home位置
     * @param camId int 值，1 为老师，2为学生
     * @return 0 success,otherwise failure.
     */
    public native int setPantiltHome(int camId);

    /**
     * 设置云台速度
     * @param camId int 值，1 为老师，2为学生
     * @param speed
     * @return 0 success,otherwise failure.
     */
    public native int setPantileSpeed(int camId, int speed);

    /**
     * 设置预制位
     * @param camId int 值，1 为老师，2为学生
     * @param channel 预制位
     * @return 0 success,otherwise failure.
     */
    public native int setCamMemory(int camId, int channel);

    /**
     *  调用预制位
     * @param camId int 值，1 为老师，2为学生
     * @param channel 预制位
     * @return  0 success,otherwise failure.
     */
    public native int reCallCamMemory(int camId, int channel);

    /**
     *  设置焦距
     * @param camId int 值，1 为老师，2为学生
     * @param far true 远 false 近
     * @return 0 success,otherwise failure.
     */
    public native int setZoomPosition(int camId,boolean far);

}
