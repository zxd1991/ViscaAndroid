package androidjni.example.com.viscaandroid;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, View.OnLongClickListener, RadioGroup.OnCheckedChangeListener, View.OnTouchListener {

    private static final String TAG = MainActivity.class.getSimpleName();

    private static final String CAMERA_IP = "10.1.11.151";

    private static final int CAMERA_PORT = 9000;
    private EditText pantiltSpeed;
    private Button pantiltSpeedSave;
    private Button up;
    private Button down;
    private Button left;
    private Button right;
    private Button home;
    private Button zoomWide;
    private EditText zoomWideSpeed;
    private Button zoomTele;
    private EditText zoomTeleSpeed;
    private EditText prePosEdit;
    private Button prePosRecall;
    private Button prePosSet;
    private static final int TEACHER = 1;
    private static final int STUDENT = 2;
    private static final int CAM_LEFT = 0;
    private static final int CAM_UP = 1;
    private static final int CAM_RIGHT = 2;
    private static final int CAM_DOWN = 3;
    private static final int ZOOM_FAR = 4;
    private static final int ZOOM_NEAR = 5;

    private RadioGroup mRadioGroup;

    private int mRole = TEACHER;

    private EditText mServerIp;

    private EditText mServerPort;

    private Button mConnect;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        //ViscaJni.getInstance().setTraceCameraAddress(CAMERA_IP, CAMERA_PORT);
    }

    private void initView() {

        mRadioGroup = findViewById(R.id.radio_group);
        mRadioGroup.setOnCheckedChangeListener(this);

        pantiltSpeed = (EditText) findViewById(R.id.pantilt_speed);
        pantiltSpeedSave = (Button) findViewById(R.id.pantilt_speed_save);

        up = (Button) findViewById(R.id.up);
        down = (Button) findViewById(R.id.down);
        left = (Button) findViewById(R.id.left);
        right = (Button) findViewById(R.id.right);
        home = (Button) findViewById(R.id.reset);

        mServerIp = findViewById(R.id.server_ip);
        mServerPort = findViewById(R.id.server_port);
        mConnect = findViewById(R.id.connect);
        mConnect.setOnClickListener(this);

        up.setOnClickListener(this);
        down.setOnClickListener(this);
        left.setOnClickListener(this);
        right.setOnClickListener(this);
        home.setOnClickListener(this);

        up.setOnLongClickListener(this);
        down.setOnLongClickListener(this);
        left.setOnLongClickListener(this);
        right.setOnLongClickListener(this);

        up.setOnTouchListener(this);
        down.setOnTouchListener(this);
        left.setOnTouchListener(this);
        right.setOnTouchListener(this);

        zoomWide = (Button) findViewById(R.id.zoom_wide);
        zoomWideSpeed = (EditText) findViewById(R.id.zoom_wide_speed);
        zoomTele = (Button) findViewById(R.id.zoom_tele);
        zoomTeleSpeed = (EditText) findViewById(R.id.zoom_tele_speed);
        prePosEdit = (EditText) findViewById(R.id.pre_pos_edit);
        prePosRecall = (Button) findViewById(R.id.pre_pos_recall);
        prePosSet = (Button) findViewById(R.id.pre_pos_set);
        pantiltSpeedSave.setOnClickListener(this);

        zoomWide.setOnClickListener(this);
        zoomTele.setOnClickListener(this);
        zoomWide.setOnLongClickListener(this);
        zoomTele.setOnLongClickListener(this);
        zoomWide.setOnTouchListener(this);
        zoomTele.setOnTouchListener(this);
        prePosRecall.setOnClickListener(this);
        prePosSet.setOnClickListener(this);
    }

    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            int what = msg.what;
            switch (what) {
                case CAM_LEFT:
                    ViscaJni.getInstance().setPantiltPos(mRole, CAM_LEFT, true);
                    sendEmptyMessageDelayed(CAM_LEFT, 50);
                    break;
                case CAM_RIGHT:
                    ViscaJni.getInstance().setPantiltPos(mRole, CAM_RIGHT, true);
                    sendEmptyMessageDelayed(CAM_RIGHT, 50);
                    break;
                case CAM_UP:
                    ViscaJni.getInstance().setPantiltPos(mRole, CAM_UP, true);
                    sendEmptyMessageDelayed(CAM_UP, 50);
                    break;
                case CAM_DOWN:
                    ViscaJni.getInstance().setPantiltPos(mRole, CAM_DOWN, true);
                    sendEmptyMessageDelayed(CAM_DOWN, 50);
                    break;
                case ZOOM_FAR:
                    ViscaJni.getInstance().setZoomPosition(mRole, true);
                    sendEmptyMessageDelayed(ZOOM_FAR, 50);
                    break;
                case ZOOM_NEAR:
                    ViscaJni.getInstance().setZoomPosition(mRole, false);
                    sendEmptyMessageDelayed(ZOOM_NEAR, 50);
                    break;
            }
        }
    };

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.left:
                Log.d(TAG, " left: " + ViscaJni.getInstance().setPantiltPos(mRole, CAM_LEFT, false));
                break;
            case R.id.right:
                Log.d(TAG, " right: " + ViscaJni.getInstance().setPantiltPos(mRole, CAM_RIGHT, false));
                break;
            case R.id.up:
                Log.d(TAG, " up: " + ViscaJni.getInstance().setPantiltPos(mRole, CAM_UP, false));
                break;
            case R.id.down:
                Log.d(TAG, " down: " + ViscaJni.getInstance().setPantiltPos(mRole, CAM_DOWN, false));
                break;
            case R.id.pantilt_speed_save:
                String valueStr = pantiltSpeed.getText().toString();
                Log.d(TAG, " setSpeed: " + ViscaJni.getInstance().setPantileSpeed(mRole, Integer.valueOf(valueStr)));
                break;
            case R.id.reset: {
                Log.d(TAG, " setHome: " + ViscaJni.getInstance().setPantiltHome(mRole));
            }
            break;
            case R.id.zoom_wide: {
                Log.d(TAG, " zoom_wide: " + ViscaJni.getInstance().setZoomPosition(mRole, true));
            }
            break;
            case R.id.zoom_tele: {
                Log.d(TAG, " zoom_wide: " + ViscaJni.getInstance().setZoomPosition(mRole, false));
            }
            break;
            case R.id.pre_pos_set: {
                String valueStr1 = prePosEdit.getText().toString();
                Log.d(TAG, " pre_pos_set: " + ViscaJni.getInstance().setCamMemory(mRole, Integer.valueOf(valueStr1)));
            }
            break;
            case R.id.pre_pos_recall: {
                String valueStr2 = prePosEdit.getText().toString();
                Log.d(TAG, " pre_pos_recall: " + ViscaJni.getInstance().reCallCamMemory(mRole, Integer.valueOf(valueStr2)));
            }
            break;

            case R.id.connect:
                connect();
                break;
            default:
                break;
        }
    }

    @Override
    public boolean onLongClick(View v) {
        switch (v.getId()) {
            case R.id.left:
                mHandler.sendEmptyMessageDelayed(CAM_LEFT, 0);
                break;
            case R.id.right:
                mHandler.sendEmptyMessageDelayed(CAM_RIGHT, 0);
                break;
            case R.id.up:
                mHandler.sendEmptyMessageDelayed(CAM_UP, 0);
                break;
            case R.id.down:
                mHandler.sendEmptyMessageDelayed(CAM_DOWN, 0);
                break;
            case R.id.zoom_wide:
                mHandler.sendEmptyMessageDelayed(ZOOM_FAR, 0);
                break;
            case R.id.zoom_tele:
                mHandler.sendEmptyMessageDelayed(ZOOM_NEAR, 0);
                break;
        }
        return false;
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_MENU) {
            mHandler.removeCallbacksAndMessages(null);
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public void onCheckedChanged(RadioGroup group, int checkedId) {
        if (checkedId == R.id.teacher) {
            mRole = TEACHER;
        } else {
            mRole = STUDENT;
        }
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_UP) {
            mHandler.removeCallbacksAndMessages(null);
        }
        return false;
    }

    private void connect() {
        try {
            String ip = mServerIp.getText().toString();
            String port = mServerPort.getText().toString();
            Log.d(TAG, " connect " + ip + " port: " + port);
            int connectResult = ViscaJni.getInstance().setTraceCameraAddress(ip, Integer.valueOf(port));
            if (connectResult >= 0) {
                Toast.makeText(this, " connect success ", Toast.LENGTH_LONG).show();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}
