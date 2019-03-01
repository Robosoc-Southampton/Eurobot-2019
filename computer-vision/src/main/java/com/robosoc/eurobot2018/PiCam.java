package com.robosoc.eurobot2018;

import org.openimaj.image.ImageUtilities;
import org.openimaj.image.MBFImage;

import uk.co.caprica.picam.ByteArrayPictureCaptureHandler;
import uk.co.caprica.picam.Camera;
import uk.co.caprica.picam.CameraConfiguration;
import static uk.co.caprica.picam.CameraConfiguration.cameraConfiguration;

import java.io.ByteArrayInputStream;


public class PiCam implements ImageSource {

	@Override
	public MBFImage getImage() {
		CameraConfiguration config = cameraConfiguration();
		
		try(Camera camera = new Camera(config)) {
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
