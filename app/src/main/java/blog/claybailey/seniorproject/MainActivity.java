package blog.claybailey.seniorproject;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.pm.ActivityInfo;
import android.media.AudioManager;
import android.media.midi.MidiDevice;
import android.media.midi.MidiDeviceInfo;
import android.media.midi.MidiInputPort;
import android.media.midi.MidiManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import java.util.logging.Level;
import java.util.logging.Logger;

public class MainActivity extends AppCompatActivity {
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    //declare native functions HERE
    private static long mEngineHandle = 0;

    private native long startEngine();
    private native void stopEngine(long engineHandle);
    private native void resumeEngine(long engineHandle);
    private native void tapKick(long engineHandle);
    private native void tapSteelDrum(long engineHandle, double frequency);

    private static native void native_setDefaultStreamValues(int sampleRate, int framesPerBurst);

    private double kSteelDrumFreq1 = 260;
    private double kSteelDrumFreq2 = 330;
    private double kSteelDrumFreq3 = 392;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    //Display content and configure audio when app is started
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        setContentView(R.layout.activity_main);
        mEngineHandle = startEngine();

        Button kickButton = (Button)findViewById(R.id.button_kick);
        if(kickButton != null) {
            kickButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    tapKick(mEngineHandle);
                }
            });
        } else {
            Logger.getAnonymousLogger().log(Level.FINER, "Kick button not found");
        }

        Button steelDrumButton1 = (Button)findViewById(R.id.button_steel_drum_1);
        if(steelDrumButton1 != null) {
            steelDrumButton1.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    tapSteelDrum(mEngineHandle, kSteelDrumFreq1);
                }
            });
        }

        Button steelDrumButton2 = (Button)findViewById(R.id.button_steel_drum_2);
        if(steelDrumButton2 != null) {
            steelDrumButton2.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    tapSteelDrum(mEngineHandle, kSteelDrumFreq2);
                }
            });
        }

        Button steelDrumButton3 = (Button)findViewById(R.id.button_steel_drum_3);
        if(steelDrumButton3 != null) {
            steelDrumButton3.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    tapSteelDrum(mEngineHandle, kSteelDrumFreq3);
                }
            });
        }

        //Configure app to hardware-specific audio output settings
        setDefaultStreamValues(this);
    }

    //Start the audio engine when app in focus
    @Override
    protected void onResume() {
        if (mEngineHandle != 0) resumeEngine(mEngineHandle);
        super.onResume();
    }

    //Stop the audio when app is out of focus
    @Override
    protected void onPause() {
        stopEngine(mEngineHandle);
        super.onPause();
    }

    //Asks hardware for audio stream settings
    //code block from MegaDrone Sample
    static void setDefaultStreamValues(Context context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1){
            AudioManager myAudioMgr = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
            String sampleRateStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
            int defaultSampleRate = Integer.parseInt(sampleRateStr);
            String framesPerBurstStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
            int defaultFramesPerBurst = Integer.parseInt(framesPerBurstStr);

            native_setDefaultStreamValues(defaultSampleRate, defaultFramesPerBurst);
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    private void setupMidi() {
        // Setup MIDI
        MidiManager midiManager = (MidiManager) getSystemService(MIDI_SERVICE);
        final MidiDeviceInfo compDeviceInfo = findDevice(midiManager, "ClayBailey",
                "CompositionDevice");
        int portIndex = 0;

        final int destinationPortIndex = 0;

        midiManager.openDevice(compDeviceInfo,
                new MidiManager.OnDeviceOpenedListener() {
                    @Override
                    public void onDeviceOpened(MidiDevice compDevice) {
                        MidiInputPort destinationInputPort = compDevice
                                .openInputPort(destinationPortIndex);
                    }
                }, null);
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    public static MidiDeviceInfo findDevice(MidiManager midiManager,
                                            String manufacturer, String product) {
        for (MidiDeviceInfo info : midiManager.getDevices()) {
            String deviceManufacturer = info.getProperties()
                    .getString(MidiDeviceInfo.PROPERTY_MANUFACTURER);
            if ((manufacturer != null)
                    && manufacturer.equals(deviceManufacturer)) {
                String deviceProduct = info.getProperties()
                        .getString(MidiDeviceInfo.PROPERTY_PRODUCT);
                if ((product != null) && product.equals(deviceProduct)) {
                    return info;
                }
            }
        }
        return null;
    }
}
