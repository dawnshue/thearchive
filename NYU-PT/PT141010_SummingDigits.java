package progteam141010;

import java.util.Scanner;
public class SummingDigits {
    // Recursively sums the digits of a given input value until the sum is a single digit
    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        
        int input;
        while(scan.hasNext()) {
            input = scan.nextInt();
            if (input==0) {
                break;
            }
            System.out.println(getSingleDigit(input));
        }
        
    }
    public static int getSingleDigit(int input) {
        if(input>=10) {
            int sum = 0;
            while(input>=10) {
                sum = sum + input%10;
                input = input/10;
            }
            sum = sum+input;
            return getSingleDigit(sum);
        } else {
            return input;
        }
    }
    
}
