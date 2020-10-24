package blog.claybailey.seniorproject;

import android.media.midi.MidiDeviceService;
import android.media.midi.MidiReceiver;

import java.io.IOException;

public class AppMidiDeviceService extends MidiDeviceService {
    private MidiReceiver mVirtualDevice = new VirtualDevice();

    @Override
    public MidiReceiver[] onGetInputPortReceivers() {
        return new MidiReceiver[]{ mVirtualDevice };
    }

    class VirtualDevice extends MidiReceiver {
        @Override
        public void onSend(byte[] msg, int offset, int count, long timestamp) throws IOException {
            //Todo: what goes here? how to connect to Amidi asap?
        }
    }
}
