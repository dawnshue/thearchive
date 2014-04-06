import java.text.DecimalFormat;
import java.util.Scanner;

public class Tennis {

	public static void main(String[] args) {
		
		Scanner input = new Scanner(System.in);
		
		int cases = Integer.parseInt(input.next());
		
		int wins;
		double prob;
		double[] outputs = new double[cases];
		for(int a = 0; a<cases; a++) {
			wins = Integer.parseInt(input.next());
			prob = Double.parseDouble(input.next());
			outputs[a] = probability(wins, prob);
		}
		
		DecimalFormat df = new DecimalFormat("#0.00");
		for(int b = 0; b<cases; b++) {
			System.out.println(df.format(outputs[b]));
		}
		
		input.close();
		
	}
	
	
	static double probability(int w, double p) {
		int games = 2*w - 1;
		int games2 = games - 1;
		int wins2 = w - 1;
		double ans = 0;
		for (int a = games2; a>=wins2; a--) {
			ans = ans + choose(a, wins2)*Math.pow(p,wins2)*Math.pow(1-p,a-wins2)*p;
		}
		return ans;
	}
	
	
	static double choose(int n, int k) {
		double temp = 1;
		for(int a = n; a > k; a--) {
			temp = temp*a;
		}
		
		double temp2 = 1;
		for(int b = (n - k); b>1; b--) {
			temp2 = temp2*b;
		}
		
		return temp/(double)temp2;
	}
	/*
	static float choose2(int n, int k) {
		return factorial(n)/(factorial(k)*factorial(n-k));
	}
	
	static long factorial(int x) {
		long ans = 1;
		if (x == 0) {
			return 1;
		}
		for (int a=x; a>1; a--) {
			ans = ans*a;
		}
		return ans;
	}
	*/
}
