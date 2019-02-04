package com.robosoc.eurobot2018;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.List;

import org.openimaj.math.geometry.point.Point2d;
import org.openimaj.math.geometry.point.Point2dImpl;
import org.openimaj.util.pair.IndependentPair;
import org.openimaj.video.capture.Device;
import org.openimaj.video.capture.VideoCapture;
import org.openimaj.video.capture.VideoCaptureException;

import Jama.Matrix;

import org.openimaj.image.DisplayUtilities;
import org.openimaj.image.FImage;
import org.openimaj.image.MBFImage;
import org.openimaj.image.analysis.algorithm.HoughCircles;
import org.openimaj.image.analysis.algorithm.HoughCircles.WeightedCircle;
import org.openimaj.image.camera.Camera;
import org.openimaj.image.camera.calibration.CameraCalibrationZhang;
import org.openimaj.image.camera.calibration.ChessboardCornerFinder;
import org.openimaj.image.camera.calibration.ChessboardCornerFinder.Options;
import org.openimaj.image.colour.ColourSpace;
import org.openimaj.image.processing.convolution.FFastGaussianConvolve;
import org.openimaj.image.processing.convolution.CLSobel;
import org.openimaj.image.processing.edges.CannyEdgeDetector;
import org.openimaj.image.processing.resize.ResizeProcessor;

public class AtomDetector {
	
	private static final float COLOUR_TOLERANCE = 25f;

	private static final float CIRCLE_THRESHOLD = 0.5f;

	private static final int MAX_CIRCLES = 10;

	private static final int NUM_ANGLES = 1000;

	private static final int RADIUS_INCREMENT = 1;

	private static final int MAX_RADIUS = 20;

	private static final int MIN_RADIUS = 10;

	public static final int NUM_CALIBRATION_EXPOSURES = 10;

	private static final int CHESSBOARD_WIDTH = 9;
	private static final int CHESSBOARD_HEIGHT = 6;
	private static final double CHESSBOARD_SQUARE_SIDE_LENGTH = 10;
	
	private static final String POINTS_FILENAME = "points.dat";

	private static final int CAMERA_WIDTH = 640;

	private static final int CAMERA_HEIGHT = 480;

	private static final long SLEEP_TIME = 2000;

	private static final float BLUR_SIGMA = 5;
	
	private static final float REDIUM_A = 13, REDIUM_B = 32;
	private static final float BLUEIUM_A = -8, BLUEIUM_B = -30;
	private static final float GREENIUM_A = -53, GREENIUM_B = 46;
	
	private List<List<? extends IndependentPair<? extends Point2d, ? extends Point2d>>> points;

	private List<Point2dImpl> model;
	
	private CameraCalibrationZhang calibration;

	private ChessboardCornerFinder chessboardAnalyser;
	
	private Device videoDevice;

	public AtomDetector(Device device) {
		this.videoDevice = device;
		
		DisplayUtilities.createNamedWindow("camera");
		
		model = buildModel(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT, CHESSBOARD_SQUARE_SIDE_LENGTH);
		
		chessboardAnalyser = new ChessboardCornerFinder(
				CHESSBOARD_WIDTH,
				CHESSBOARD_HEIGHT,
				Options.FILTER_QUADS,
				Options.FAST_CHECK,
				Options.ADAPTIVE_THRESHOLD);
		
		File pointsFile = new File(POINTS_FILENAME);
		if(pointsFile.exists()) {
			try(FileInputStream fileIn = new FileInputStream(pointsFile);
					ObjectInputStream objectIn = new ObjectInputStream(fileIn)) {
				deserialisePoints(objectIn);
				
				calibrate();
				
				return;
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		calibrateIntrinsics();
		
	}
	
	private List<IndependentPair<? extends Point2d, ? extends Point2d>> pointPairsToIndependent(
			List<PointPair> pointPairs) {
		List<IndependentPair<? extends Point2d, ? extends Point2d>> independentPairs = new ArrayList<>();
		
		for(PointPair pair : pointPairs) {
			independentPairs.add(pair.toIndependentPair());
		}
		
		return independentPairs;
	}
	
	private List<PointPair> independentPairsToPoint(
			List<? extends IndependentPair<? extends Point2d, ? extends Point2d>> pairList) {
		List<PointPair> pointPairs = new ArrayList<>();
		
		for(IndependentPair<? extends Point2d, ? extends Point2d> pair : pairList) {
			pointPairs.add(new PointPair(pair));
		}
		
		return pointPairs;
	}
	
	private void serialisePoints(ObjectOutputStream objectOut) throws IOException {
		List<List<PointPair>> serialisablePoints = new ArrayList<>();
		
		for(List<? extends IndependentPair<? extends Point2d, ? extends Point2d>> pairList : points) {
			serialisablePoints.add(independentPairsToPoint(pairList));
		}
		
		objectOut.writeObject(serialisablePoints);
	}
	
	private void deserialisePoints(ObjectInputStream objectIn) throws ClassNotFoundException, IOException {
		points = new ArrayList<>();
		List<List<PointPair>> rawPoints = (List<List<PointPair>>) objectIn.readObject();
		
		for(List<PointPair> pairList : rawPoints) {
			points.add(pointPairsToIndependent(pairList));
		}
	}
	
	public void calibrateIntrinsics() {
		// take k photos, use them to calibrate, overwriting existing points
		
		points = new ArrayList<List<? extends IndependentPair<? extends Point2d, ? extends Point2d>>>();
		
		try(VideoCapture vidCap = new VideoCapture(CAMERA_WIDTH, CAMERA_HEIGHT, 0.5, videoDevice)) {
			
			while(points.size() < NUM_CALIBRATION_EXPOSURES) {
				chessboardFromCurrentFrame(vidCap);
				if(chessboardAnalyser.isFound()) {
					points.add(IndependentPair.pairList(model, chessboardAnalyser.getCorners()));
					System.out.println("Image " + points.size() + " added.");
					Thread.sleep(SLEEP_TIME);
					long time = System.currentTimeMillis();
					while(System.currentTimeMillis() < time + SLEEP_TIME) {
						DisplayUtilities.displayName(vidCap.getNextFrame(), "camera");
					}
				} else {
					System.out.println("Make sure the chessboard is within the frame.");
				}
			}
			
		} catch (VideoCaptureException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		calibrate();
	}

	private void chessboardFromCurrentFrame(VideoCapture vidCap) throws InterruptedException {
		System.out.println("Capturing image...");
		MBFImage frame = vidCap.getNextFrame();
		
		DisplayUtilities.displayName(frame, "camera");
		
		System.out.println("Analysing image...");
		chessboardAnalyser.analyseImage(frame.flatten());
	}
	
	public boolean calibrateExtrinsics() {
		// take 1 photo, overwrite the last pointset with its points and recalibrate

		try(VideoCapture vidCap = new VideoCapture(CAMERA_WIDTH, CAMERA_HEIGHT, videoDevice)) {
			
			chessboardFromCurrentFrame(vidCap);
			
			if(chessboardAnalyser.isFound()) {
				points.set(points.size() - 1, IndependentPair.pairList(model, chessboardAnalyser.getCorners()));
				calibrate();
				System.out.println("Extrinsic parameters calibrated.");
				return true;
			} else {
				System.out.println("Chessboard not found.");
			}

		} catch (VideoCaptureException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return false;
	}
	
	private void calibrate() {
		calibration = new CameraCalibrationZhang(points, CAMERA_WIDTH, CAMERA_HEIGHT);
		
		File pointsFile = new File(POINTS_FILENAME);
		try(FileOutputStream fileOut = new FileOutputStream(pointsFile);
				ObjectOutputStream objectOut = new ObjectOutputStream(fileOut)) {
			serialisePoints(objectOut);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * From CalibrationDemo:
	 * https://github.com/jonhare/COMP3204/blob/master/app/src/main/java/uk/ac/soton/ecs/comp3204/l11/CalibrationDemo.java#L152
	 * @param width Number of squares in the chessboard horizontally
	 * @param height Number of squares in the chessboard vertically
	 * @param d Side length of a square on the chessboard
	 * @return list of points found on corners on the chessboard
	 */
	private List<Point2dImpl> buildModel(int width, int height, double d) {
	    final List<Point2dImpl> pts = new ArrayList<Point2dImpl>();
	    for (int i = 0; i < height; i++)
	        for (int j = 0; j < width; j++)
	            pts.add(new Point2dImpl(j * d, i * d));
	    return pts;
	}
	
	private MBFImage getBirdsEye() throws VideoCaptureException {
		MBFImage frame;
		
		try(VideoCapture vidCap = new VideoCapture(CHESSBOARD_WIDTH, CAMERA_HEIGHT, videoDevice)) {
			frame = vidCap.getNextFrame();
		}
		
		DisplayUtilities.display(frame);
		
		List<Camera> cameras = calibration.getCameras();
		Matrix homography = cameras.get(cameras.size() - 1).computeHomography();
		MBFImage birdsEye = frame.transform(homography.inverse());
		DisplayUtilities.displayName(birdsEye, "camera");
		return birdsEye;
	}
	
	private static List<Point2d> findGoodCircles(FImage img) {
		DisplayUtilities.display(img);
		img.processInplace(new FFastGaussianConvolve(BLUR_SIGMA, 5));
		CLSobel sobel = new CLSobel();
		sobel.analyseImage(img);
		FImage edges = sobel.getMagnitudes();
		DisplayUtilities.display(edges);
		
		HoughCircles houghCircles = new HoughCircles(MIN_RADIUS, MAX_RADIUS, RADIUS_INCREMENT, NUM_ANGLES);
		edges.analyseWith(houghCircles);
		List<WeightedCircle> bestCircles = houghCircles.getBest(MAX_CIRCLES);
		
		List<Point2d> goodCircles = new ArrayList<>();
		for(WeightedCircle circle : bestCircles) {
			if(circle.weight > CIRCLE_THRESHOLD) {
				goodCircles.add(circle.calculateCentroid());
			}
		}
		
		return goodCircles;
	}
	
	public List<Point2d> findRedium() throws VideoCaptureException {
		MBFImage birdsEye = getBirdsEye();
		FImage red = colourThreshold(birdsEye, REDIUM_A, REDIUM_B, COLOUR_TOLERANCE);
		return findGoodCircles(red);
	}
	
	public List<Point2d> findGreenium() throws VideoCaptureException {
		MBFImage birdsEye = getBirdsEye();
		FImage green = colourThreshold(birdsEye, GREENIUM_A, GREENIUM_B, COLOUR_TOLERANCE);
		return findGoodCircles(green);
	}
	
	
	public List<Point2d> findBlueium() throws VideoCaptureException {
		MBFImage birdsEye = getBirdsEye();
		FImage blue = colourThreshold(birdsEye, BLUEIUM_A, BLUEIUM_B, COLOUR_TOLERANCE);
		return findGoodCircles(blue);
	}
	
	private FImage colourThreshold(MBFImage rgb, float a, float b, float tolerance) {
		FImage out = new FImage(rgb.getWidth(), rgb.getHeight());
		
		MBFImage lab = ColourSpace.convert(rgb, ColourSpace.CIE_Lab);
		
		for(int i = 0; i < lab.getWidth(); i++) {
			for(int j = 0; j < lab.getHeight(); j++) {
				
				float dist = (float) Math.sqrt(
						Math.pow(a - lab.getBand(1).pixels[j][i], 2) + 
						Math.pow(b - lab.getBand(2).pixels[j][i], 2));
				
				if(dist <= tolerance) {
					out.pixels[j][i] = 1;
				}
				
			}
		}
		
		return out;
	}
}
