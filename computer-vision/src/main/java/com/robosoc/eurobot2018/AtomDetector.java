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

import org.openimaj.math.geometry.line.Line2d;
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
import org.openimaj.image.processing.convolution.FSobel;
import org.openimaj.image.processing.edges.CannyEdgeDetector;
import org.openimaj.image.processing.resize.ResizeProcessor;

/**
 * Computer vision system for locating atoms.
 * @author billy
 *
 */
public class AtomDetector {
	
	// max allowable distance from hard-coded AB values when colour thresholding
	private static final float COLOUR_TOLERANCE = 25f;

	// minimum circle weight to count as a circle
	private static final float CIRCLE_THRESHOLD = 0.05f;

	// number of best circles to get from the Hough transform
	private static final int MAX_CIRCLES = 10;

	// number of angles to use from each point in the Hough transform
	private static final int NUM_ANGLES = 1000;

	// radius increment to use in Hough transform
	private static final int RADIUS_INCREMENT = 1;

	// maximum circle radius in Hough transform
	private static final int MAX_RADIUS = 37;

	// minimum circle radius in Hough transform
	private static final int MIN_RADIUS = 33;

	// number of chessboard exposures to calibrate camera instrinsic parameters
	public static final int NUM_CALIBRATION_EXPOSURES = 10;

	// chessboard parameters for calibration
	private static final int CHESSBOARD_WIDTH = 9;
	private static final int CHESSBOARD_HEIGHT = 6;
	private static final double CHESSBOARD_SQUARE_SIDE_LENGTH = 22.25;
	
	// file where calibration data is stored
	private static final String POINTS_FILENAME = "points.dat";

	// camera parameters
	private static final int CAMERA_WIDTH = 640;
	private static final int CAMERA_HEIGHT = 480;

	// time between image captures when calibrating intrinsic parameters
	private static final long SLEEP_TIME = 2000;

	// sigma for the Gaussian blur before edge detection
	private static final float BLUR_SIGMA = 1;
	
	/*
	 *  LAB colour space values for the colours of atoms. 
	 *  L value is not used, because it's the lightness. This changes lots with lighting,
	 *  exposure, etc.
	 */
	private static final float REDIUM_A = 49, REDIUM_B = 30;
	private static final float BLUEIUM_A = 2, BLUEIUM_B = -41;
	private static final float GREENIUM_A = -18, GREENIUM_B = 42;
	
	
	// Calibration points
	private List<List<? extends IndependentPair<? extends Point2d, ? extends Point2d>>> points;

	// positions of chessboard corners
	private List<Point2dImpl> model;
	
	// calibration itself
	private CameraCalibrationZhang calibration;

	// finds and analyses chessboards in images
	private ChessboardCornerFinder chessboardAnalyser;
	
	// either a webcam or the pi camera
	private ImageSource source;

	/**
	 * Sole constructor.
	 * @param source Camera interface.
	 */
	public AtomDetector(ImageSource source) {
		this.source = source;
		
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
	
	/**
	 * Takes a list of serialisable PointPairs and converts it to
	 * a list of OpenImaj IndependentPairs.
	 * @param pointPairs PointPairs
	 * @return IndependentPairs
	 */
	private List<IndependentPair<? extends Point2d, ? extends Point2d>> pointPairsToIndependent(
			List<PointPair> pointPairs) {
		List<IndependentPair<? extends Point2d, ? extends Point2d>> independentPairs = new ArrayList<>();
		
		for(PointPair pair : pointPairs) {
			independentPairs.add(pair.toIndependentPair());
		}
		
		return independentPairs;
	}
	
	/**
	 * Takes a list of OpenImaj IndependentPairs and converts it to
	 * a list of serialisable PointPairs.
	 * @param pairList
	 * @return
	 */
	private List<PointPair> independentPairsToPoint(
			List<? extends IndependentPair<? extends Point2d, ? extends Point2d>> pairList) {
		List<PointPair> pointPairs = new ArrayList<>();
		
		for(IndependentPair<? extends Point2d, ? extends Point2d> pair : pairList) {
			pointPairs.add(new PointPair(pair));
		}
		
		return pointPairs;
	}
	
	/**
	 * Serialise points and feed them to the given stream.
	 * @param objectOut Output stream to feed the points to.
	 * @throws IOException
	 */
	private void serialisePoints(ObjectOutputStream objectOut) throws IOException {
		List<List<PointPair>> serialisablePoints = new ArrayList<>();
		
		for(List<? extends IndependentPair<? extends Point2d, ? extends Point2d>> pairList : points) {
			serialisablePoints.add(independentPairsToPoint(pairList));
		}
		
		objectOut.writeObject(serialisablePoints);
	}
	
	/**
	 * Deserialise points from an object input stream.
	 * @param objectIn Input to read the points from.
	 * @throws ClassNotFoundException
	 * @throws IOException
	 */
	private void deserialisePoints(ObjectInputStream objectIn) throws ClassNotFoundException, IOException {
		points = new ArrayList<>();
		List<List<PointPair>> rawPoints = (List<List<PointPair>>) objectIn.readObject();
		
		for(List<PointPair> pairList : rawPoints) {
			points.add(pointPairsToIndependent(pairList));
		}
	}
	
	/**
	 * Calibrate the camera parameters from scratch, including intrinsic parameters.
	 * The final set of calibration points is used to calibrate the intrinsics.
	 */
	public void calibrateIntrinsics() {
		// take k photos, use them to calibrate, overwriting existing points
		
		points = new ArrayList<List<? extends IndependentPair<? extends Point2d, ? extends Point2d>>>();
		
		try {
			
			while(points.size() < NUM_CALIBRATION_EXPOSURES) {
				chessboardFromCurrentFrame();
				if(chessboardAnalyser.isFound()) {
					points.add(IndependentPair.pairList(model, chessboardAnalyser.getCorners()));
					System.out.println("Image " + points.size() + " added.");
					Thread.sleep(SLEEP_TIME);
					long time = System.currentTimeMillis();
					while(System.currentTimeMillis() < time + SLEEP_TIME) {
						DisplayUtilities.displayName(source.getImage(), "camera");
					}
				} else {
					System.out.println("Make sure the chessboard is within the frame.");
				}
			}
			
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		calibrate();
	}

	/**
	 * Gets a new image and analyses it for a chessboard.
	 */
	private void chessboardFromCurrentFrame() {
		System.out.println("Capturing image...");
		MBFImage frame = source.getImage();
		
		DisplayUtilities.displayName(frame, "camera");
		
		System.out.println("Analysing image...");
		chessboardAnalyser.analyseImage(frame.flatten());
	}
	
	/**
	 * Takes a single new image and if there's a chessboard in it,
	 * uses it to calibrate the extrinsic parameters of the camera.
	 * @return True if there is a chessboard found, otherwise false.
	 */
	public boolean calibrateExtrinsics() {
		// take 1 photo, overwrite the last pointset with its points and recalibrate

		chessboardFromCurrentFrame();

		if(chessboardAnalyser.isFound()) {
			points.set(points.size() - 1, IndependentPair.pairList(model, chessboardAnalyser.getCorners()));
			calibrate();
			System.out.println("Extrinsic parameters calibrated.");
			return true;
		} else {
			System.out.println("Chessboard not found.");
		}


		return false;
	}
	
	/**
	 * Calibrate the camera's intrinsic and extrinsic parameters from
	 * existing calibration information.
	 */
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
	
	/**
	 * Get a view from the camera, warped so that it appears to be
	 * taken from above, and atoms appear circular.
	 * @return Warped view.
	 */
	private MBFImage getBirdsEye() {
		MBFImage frame = source.getImage();
		
		DisplayUtilities.display(frame);
		
		List<Camera> cameras = calibration.getCameras();
		Matrix homography = cameras.get(cameras.size() - 1).computeHomography();
		MBFImage birdsEye = frame.transform(homography.inverse());
		DisplayUtilities.displayName(birdsEye, "camera");
		return birdsEye;
	}
	
	/**
	 * Find the good circles in the image.
	 * First Gaussian blur to remove noise.
	 * Then differentiate image with Sobel.
	 * Then calculate edge magnitudes from result.
	 * Then threshold on the magnitudes to find edges.
	 * Then run Hough transform on edges to find circles.
	 * Filter out the bad circles with low weight.
	 * Finally, eliminate overlapping circles.
	 * @param img Image to find circles in.
	 * @return Centroids of circles.
	 */
	private static List<Point2d> findGoodCircles(FImage img) {
		DisplayUtilities.display(img);
		img.processInplace(new FFastGaussianConvolve(BLUR_SIGMA, 5));
		FSobel sobel = new FSobel();
		sobel.analyseImage(img);
		FImage edges = new FImage(sobel.dx.getWidth(), sobel.dx.getHeight());
		
		for(int i = 0; i < edges.getHeight(); i++) {
			for(int j = 0; j < edges.getWidth(); j++) {
				edges.pixels[i][j] = (float) Math.sqrt(
						Math.pow(sobel.dx.pixels[i][j], 2)
						+ Math.pow(sobel.dy.pixels[i][j], 2));
			}
		}
		
		edges.threshold(2.5f);
		
		DisplayUtilities.display(edges);
		
		HoughCircles houghCircles = new HoughCircles(MIN_RADIUS, MAX_RADIUS, RADIUS_INCREMENT, NUM_ANGLES);
		edges.analyseWith(houghCircles);
		List<WeightedCircle> bestCircles = houghCircles.getBest(MAX_CIRCLES);
		
		System.out.println("filtering circles");
		
		List<Point2d> goodCircles = new ArrayList<>();
		for(WeightedCircle circle : bestCircles) {
			System.out.println(circle.weight);
			if(circle.weight > CIRCLE_THRESHOLD) {
				goodCircles.add(circle.calculateCentroid());
			}
		}
		
		goodCircles = eliminateDuplicates(goodCircles);
		
		return goodCircles;
	}
	
	/**
	 * Eliminate circles that overlap with better ones.
	 * @param goodCircles Circles.
	 * @return Best non-overlapping circles.
	 */
	private static List<Point2d> eliminateDuplicates(List<Point2d> goodCircles) {
		
		ArrayList<Point2d> unique = new ArrayList<>();
		
		for(Point2d circle : goodCircles) {
			boolean exists = false;
			for(Point2d existing : unique) {
				if(Line2d.distance(existing, circle) < MIN_RADIUS) {
					exists = true;
					break;
				}
			}
			
			if(!exists) {
				unique.add(circle);
			}
		}
		
		return unique;
	}

	/**
	 * Find redium coordinates.
	 * @return coordinates of redium.
	 */
	public List<Point2d> findRedium() {
		MBFImage birdsEye = getBirdsEye();
		FImage red = colourThreshold(birdsEye, REDIUM_A, REDIUM_B, COLOUR_TOLERANCE);
		return findGoodCircles(red);
	}

	/**
	 * Find greenium coordinates.
	 * @return coordinates of greenium.
	 */
	public List<Point2d> findGreenium() {
		MBFImage birdsEye = getBirdsEye();
		FImage green = colourThreshold(birdsEye, GREENIUM_A, GREENIUM_B, COLOUR_TOLERANCE);
		return findGoodCircles(green);
	}
	
	/**
	 * Find blueium coordinates.
	 * @return coordinates of blueium.
	 */
	public List<Point2d> findBlueium() {
		MBFImage birdsEye = getBirdsEye();
		FImage blue = colourThreshold(birdsEye, BLUEIUM_A, BLUEIUM_B, COLOUR_TOLERANCE);
		return findGoodCircles(blue);
	}
	
	/**
	 * Threshold LAB image based on colour.
	 * Pixels are 1 if less than tolerance from specified A,B values.
	 * @param rgb Image in RGB.
	 * @param a A value of wanted colour.
	 * @param b B value of wanted colour.
	 * @param tolerance Maxiumum allowable distance from wanted colour.
	 * @return Thresholded image.
	 */
	private FImage colourThreshold(MBFImage rgb, float a, float b, float tolerance) {
		FImage out = new FImage(rgb.getWidth(), rgb.getHeight());
		
		MBFImage lab = ColourSpace.convert(rgb, ColourSpace.CIE_Lab);
		DisplayUtilities.display(lab);
		
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
