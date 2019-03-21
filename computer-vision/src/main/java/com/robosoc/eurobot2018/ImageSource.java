package com.robosoc.eurobot2018;

import org.openimaj.image.MBFImage;

/**
 * Camera interface.
 * @author billy
 *
 */
public interface ImageSource {
	
	/**
	 * Gets an image from the camera.
	 * @return image
	 */
	public MBFImage getImage();
}
