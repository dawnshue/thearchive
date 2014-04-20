package nyuprog.cs480.homework11;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Scanner;

/*
 * Solution to NYU CS480 HW11 Problem A - Sunny Mountains
 * http://acm.hust.edu.cn/vjudge/contest/view.action?cid=44404#problem/A
*/

public class SunnyMountains {
	
	public static void main (String[] args) {
		
		ArrayList<Double> answer = new ArrayList<Double>();
		Scanner input = new Scanner(System.in);
		
		int testCases = input.nextInt();
		for (int it_case = 0; it_case < testCases; it_case++) {
			int totalCoordinates = input.nextInt();
			ArrayList<Point> coordinates = new ArrayList<Point>();
			int x, y;
			for (int it_coord = 0; it_coord < totalCoordinates; it_coord++) {
				x = input.nextInt();
				y = input.nextInt();
				coordinates.add(new Point(x, y));
			}
			printCoords(coordinates);
			Collections.sort(coordinates, new Comparator<Point>() {
				public int compare(Point a, Point b) {
					if (a.X < b.X) {
						return -1;
					} else if (a.X == b.X) {
						return 0;
					} else {
						return 1;
					}
				}
			});
			printCoords(coordinates);
			Collections.reverse(coordinates);
			printCoords(coordinates);
			
			answer.add((double)findSunLength(coordinates));
		}
		
		for (int a = 0; a < answer.size(); a++) {
			System.out.println(new DecimalFormat("#.##").format(answer.get(a)));
		}
	}
	public static double findSunLength(ArrayList<Point> coordinates) {
		double sum = 0;
		int tallest = 0;
		int size = coordinates.size();
		size = size - size%2;
		for (int it = 0; it < size; it = it + 2) {
			if (coordinates.get(it+1).Y > tallest) {
				sum = sum + Point.getSun(coordinates.get(it), coordinates.get(it+1), tallest);
				tallest = coordinates.get(it+1).Y;
			}
		}
		return sum;
	}
	public static void printCoords(ArrayList<Point> coordinates) {
		System.out.print("Coordinates: ");
		for (int a = 0; a < coordinates.size(); a++) {
			System.out.print("("+coordinates.get(a).X+","+coordinates.get(a).Y+") ");
		}
		System.out.println();
	}

}

class Point {
	int X;
	int Y;
	public Point(int x, int y) {
		X = x;
		Y = y;
	}
	public static double getSun(Point bot, Point top, int  height) {
		System.out.println("getSun("+bot.X+","+bot.Y+")("+top.X+","+top.Y+")"+" height: "+height);
		double ans = Math.pow(top.X - bot.X, 2) + Math.pow(top.Y - bot.Y, 2);
		System.out.println("ans: "+ans);
		ans = Math.sqrt(ans);
		System.out.println("ans: "+ans);
			System.out.println("return: "+(ans * (top.Y - height)/top.Y));
			return (ans * (top.Y - height)/top.Y);
	}
}
