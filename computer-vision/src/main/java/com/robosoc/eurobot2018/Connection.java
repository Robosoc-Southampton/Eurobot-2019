package com.robosoc.eurobot2018;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.List;
import java.util.Scanner;

import org.openimaj.math.geometry.point.Point2d;
import org.openimaj.video.capture.VideoCapture;

public class Connection {
	
	public Connection(Socket socket, int device) {
		AtomDetector atoms = new AtomDetector(VideoCapture.getVideoDevices().get(device));
		
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
	
	private static void sendPoints(PrintWriter out, List<Point2d> points) {
		for(Point2d point : points) {
			out.println(point.getX() + "," + point.getY());
		}
		out.println();
		out.flush();
	}

}
