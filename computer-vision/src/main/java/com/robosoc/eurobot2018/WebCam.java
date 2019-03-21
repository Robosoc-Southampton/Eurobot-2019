package com.robosoc.eurobot2018;

import org.openimaj.image.MBFImage;
import org.openimaj.video.capture.Device;
import org.openimaj.video.capture.VideoCapture;
import org.openimaj.video.capture.VideoCaptureException;

/**
 * Interface to webcam.
 * @author billy
 *
 */
public class WebCam implements ImageSource {
	
	// camera parameters
	private static final int CAMERA_WIDTH = 640;
	private static final int CAMERA_HEIGHT = 480;
	
	// webcam device
	private Device videoDevice;
	
	/**
	 * Sole constructor
	 * @param videoDevice webcam device to use
	 */
	public WebCam(Device videoDevice) {
		this.videoDevice = videoDevice;
	}

	@Override
	public MBFImage getImage() {
		MBFImage frame = null;
		
		try(VideoCapture vidCap = new VideoCapture(CAMERA_WIDTH, CAMERA_HEIGHT, videoDevice)) {
			frame = vidCap.getNextFrame();
		} catch (VideoCaptureException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return frame;
	}

}
