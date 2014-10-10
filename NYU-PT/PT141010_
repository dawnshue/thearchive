package progteam141010;

import java.math.BigInteger;
import java.util.Scanner;
public class FibonacciFreeze {
    //Determines the fibonacci number up to the 5000th number in the sequence
    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        int input;
        BigInteger[] allfib = getFibonacci(5000);
        BigInteger answer;
        while(scan.hasNext()) {
            input = scan.nextInt();
            answer = allfib[input];
            System.out.println("The Fibonacci number for "+input+" is "+answer);
        }
    }
    public static BigInteger[] getFibonacci(int input) {
        BigInteger[] allfib = new BigInteger[input+1];
        allfib[0] = BigInteger.valueOf(0);
        allfib[1] = BigInteger.valueOf(1);
        
        for (int i=2; i<=input; i++) {
            allfib[i] = allfib[i-1].add(allfib[i-2]);
        }
        
        return allfib;
    }
}
