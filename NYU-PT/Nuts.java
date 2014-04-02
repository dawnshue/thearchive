import java.util.Scanner;

/**
 * NYU Programming Team Entry Problems 5 - Nuts
 * @author Vangie
 */
public class Nuts {

    
    public static void main(String[] args) {
        
        Scanner scan = new Scanner(System.in);
        int in_K = scan.nextInt(); // Sections PER Box
        int in_A = scan.nextInt(); // Total Nuts
        int in_B = scan.nextInt(); // Total Divisors
        int in_V = scan.nextInt(); // Max Nuts per Section
        
        
        int totNuts = in_A;
        int sections = in_K;
        int divisors = in_B;
        int nuts = in_V;
        
        int boxes = 0;
        while (totNuts > 0) {
            if(divisors == 0 || sections > (divisors+1)) {
                totNuts = totNuts - (divisors+1)*nuts;
                divisors = 0;
            } else {
                totNuts = totNuts - (sections)*nuts;
                divisors = divisors - (sections-1);
            }
            boxes++;
        }
        
        System.out.print(boxes);
        
    }
    
}
