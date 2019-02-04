package com.robosoc.eurobot2018;

import java.io.Serializable;

import org.openimaj.math.geometry.point.Point2d;
import org.openimaj.math.geometry.point.Point2dImpl;
import org.openimaj.util.pair.IndependentPair;

class PointPair implements Serializable {
	private float firstX;
	private float firstY;
	private float secondX;
	private float secondY;
	
	public PointPair(IndependentPair<? extends Point2d, ? extends Point2d> pair) {
		firstX = pair.firstObject().getX();
		firstY = pair.firstObject().getY();
		secondX = pair.secondObject().getX();
		secondY = pair.secondObject().getY();
	}
	
	public IndependentPair<? extends Point2d, ? extends Point2d> toIndependentPair() {
		return new IndependentPair<Point2d, Point2d>(
				new Point2dImpl(firstX, firstY), 
				new Point2dImpl(secondX, secondY));
	}
}