package com.robosoc.eurobot2018;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.List;
import java.util.Scanner;

import org.openimaj.math.geometry.point.Point2d;
import org.openimaj.video.capture.VideoCapture;

/**
 * Connection to the main python program.
 * @author billy
 *
 */
public class Connection {
	
	/**
	 * Sole constructor. Blocks execution until connection drops, but this shouldn't
	 * be a problem since there will only need to be one connection at a time.
	 * @param socket Socket for communication with python
	 * @param device Raspberry Pi camera if -1, otherwise specifies a usb webcam
	 */
	public Connection(Socket socket, int device) {
		ImageSource source;
		if(device == -1) {
			source = new PiCam();
		} else {
			source = new WebCam(VideoCapture.getVideoDevices().get(device));
		}
		
		AtomDetector atoms = new AtomDetector(source);
		
		try(Scanner sc = new Scanner(socket.getInputStream());
				PrintWriter out = new PrintWriter(socket.getOutputStream())) {
			while(true) {
				String message = sc.nextLine();
				
				switch(message) {
				case "calibrateIntrinsics":
					atoms.calibrateIntrinsics();
					break;
				case "calibrateExtrinsics":
					if(atoms.calibrateExtrinsics()) {
						out.println("success");
					} else {
						out.println("failed");
					}
					out.flush();
					break;
				case "findRedium":
					sendPoints(out, atoms.findRedium());
					break;
				case "findGreenium":
					sendPoints(out, atoms.findGreenium());
					break;
				case "findBlueium":
					sendPoints(out, atoms.findBlueium());
					break;
				}
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * Send a list of points as comma separated values.
	 * @param out PrintWriter to write to
	 * @param points Points to send
	 */
	private static void sendPoints(PrintWriter out, List<Point2d> points) {
		for(Point2d point : points) {
			out.println(point.getX() + "," + point.getY());
		}
		out.println();
		out.flush();
	}

}
