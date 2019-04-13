package com.robosoc.eurobot2018;

import org.openimaj.image.ImageUtilities;
import org.openimaj.image.MBFImage;

import uk.co.caprica.picam.ByteArrayPictureCaptureHandler;
import uk.co.caprica.picam.Camera;
import uk.co.caprica.picam.CameraConfiguration;
import static uk.co.caprica.picam.CameraConfiguration.cameraConfiguration;

import java.io.ByteArrayInputStream;

/**
 * Interface to raspberry pi camera.
 * @author billy
 *
 */
public class PiCam implements ImageSource {
	
	// pi camera instance
	private Camera camera;
	
	/**
	 * Sole constructor.
	 */
	public PiCam() {
		CameraConfiguration config = cameraConfiguration()
				.width(640)
				.height(480);
		
		camera = new Camera(config);
	}

	@Override
	public MBFImage getImage() {
		
		
		try {
			ByteArrayPictureCaptureHandler handler = new ByteArrayPictureCaptureHandler();
			camera.takePicture(handler);
			
			return ImageUtilities.readMBF(new ByteArrayInputStream(handler.result().toByteArray()));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return null;
	}

}
